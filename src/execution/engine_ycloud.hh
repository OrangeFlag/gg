/* -*-mode:c++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef ENGINE_YANDEX_HH
#define ENGINE_YANDEX_HH

#include <unordered_map>
#include <chrono>

#include "engine.hh"
#include "thunk/thunk.hh"
#include "net/http_request.hh"
#include "util/uri.hh"
#include "net/ycloud.hh"

class YandexExecutionEngine : public ExecutionEngine {
private:
    size_t running_jobs_{0};
    std::map<uint64_t, std::chrono::steady_clock::time_point> start_times_{};

    HTTPRequest generate_request(const gg::thunk::Thunk &thunk);
public:
    YandexExecutionEngine(const size_t max_jobs, const YCloudCredentials &credentials);

    void init(ExecutionLoop &loop) override;

    void force_thunk(const gg::thunk::Thunk &thunk,
                     ExecutionLoop &exec_loop) override;

    bool is_remote() const override { return true; }

    bool can_execute(const gg::thunk::Thunk &thunk) const override;

    size_t job_count() const override;

    std::string label() const override { return "ycloud"; }
};

#endif //GG_ENGINE_YANDEX_HH
