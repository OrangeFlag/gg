/* -*-mode:c++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "ycloud.hh"

#include <stdexcept>
#include <utility>
#include <string>

using namespace std;

const static string YCLOUD_FOLDER_ENV{"YCLOUD_FOLDER"};
const static string YCLOUD_IAM_TOKEN_ENV{"YCLOUD_IAM_TOKEN"};
const static string YCLOUD_API_KEY_ENV{"YCLOUD_API_KEY"};

YCloudCredentials::YCloudCredentials() : YCloudCredentials(safe_getenv(YCLOUD_FOLDER_ENV),
                                                           safe_getenv(YCLOUD_IAM_TOKEN_ENV),
                                                           safe_getenv(YCLOUD_API_KEY_ENV)) {}

YCloudCredentials::YCloudCredentials(const string &folder, const string &iam_token,
                                     const string &api_key) :
        folder_(folder),
        iam_token_(iam_token.length() > 0, iam_token),
        api_key_(api_key.length() > 0, api_key) {
    if (iam_token_->empty() && api_key_->empty()) {
        throw std::runtime_error("Missing Yandex Cloud iam token or api key");
    }
}
