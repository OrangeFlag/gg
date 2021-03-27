/* -*-mode:c++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "engine_ycloud.hh"

#include <stdexcept>
#include <cmath>

#include "response.hh"
#include "thunk/ggutils.hh"
#include "net/http_response.hh"
#include "net/nb_secure_socket.hh"
#include "util/base64.hh"
#include "util/system_runner.hh"
#include "util/units.hh"


using namespace std;
using namespace gg;
using namespace gg::thunk;


HTTPRequest YandexExecutionEngine::generate_request(const Thunk &thunk) {
    //TODO
}

void YandexExecutionEngine::force_thunk(const Thunk &thunk, ExecutionLoop &exec_loop) {
    HTTPRequest request = generate_request(thunk);

    const Address address_ = {};
    uint64_t connection_id = exec_loop.make_http_request<SSLConnection>(
            thunk.hash(),
            address_,
            request,
            [this](const uint64_t id, const string &thunk_hash, const HTTPResponse &http_response) -> bool {
                running_jobs_--;

                if (http_response.status_code() != "200") {
                    cerr << "======== HTTP Response ========" << endl;
                    cerr << http_response.str() << endl;
                    cerr << "===============================" << endl;
                    failure_callback_(thunk_hash, JobStatus::ExecutionFailure);
                    return false;
                }

                ExecutionResponse response = ExecutionResponse::parse_message(http_response.body());

                /* print the output, if there's any */
                if (response.stdout.length()) {
                    cerr << response.stdout << endl;
                }

                switch (response.status) {
                    case JobStatus::Success: {
                        if (response.thunk_hash != thunk_hash) {
                            cerr << http_response.str() << endl;
                            throw runtime_error("expected output for " +
                                                thunk_hash + ", got output for " +
                                                response.thunk_hash);
                        }

                        for (const auto &output : response.outputs) {
                            gg::cache::insert(gg::hash::for_output(response.thunk_hash, output.tag), output.hash);

                            if (output.data.length()) {
                                roost::atomic_create(base64::decode(output.data),
                                                     gg::paths::blob(output.hash));
                            }
                        }

                        gg::cache::insert(response.thunk_hash, response.outputs.at(0).hash);

                        vector<ThunkOutput> thunk_outputs;
                        for (auto &output : response.outputs) {
                            thunk_outputs.emplace_back(move(output.hash), move(output.tag));
                        }

                        success_callback_(response.thunk_hash, move(thunk_outputs),
                                          compute_cost(start_times_.at(id)));

                        start_times_.erase(id);
                        break;
                    }

                    default: /* in case of any other failure */
                        failure_callback_(thunk_hash, response.status);
                }

                return false;
            },
            [this](const uint64_t id, const string &thunk_hash) {
                start_times_.erase(id);
                failure_callback_(thunk_hash, JobStatus::SocketFailure);
            }
    );

    start_times_.insert({connection_id, chrono::steady_clock::now()});

    running_jobs_++;
}

bool YandexExecutionEngine::can_execute(const Thunk &thunk) const {
    return thunk.infiles_size() < 128_MiB;
}

size_t YandexExecutionEngine::job_count() const {
    return running_jobs_;
}

float YandexExecutionEngine::compute_cost(const chrono::steady_clock::time_point &,
                                          const chrono::steady_clock::time_point &) {
    return 0.f;
}
