/* -*-mode:c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef YCLOUD_HH
#define YCLOUD_HH

#include <ctime>
#include <string>
#include <map>

#include "http_request.hh"
#include "util/optional.hh"
#include "util/util.hh"

class YCloudCredentials {
private:
    std::string folder_;
    Optional<std::string> iam_token_;
    Optional<std::string> api_key_;

public:
    YCloudCredentials();

    explicit YCloudCredentials(std::string folder, const std::string &iam_token = {},
                               const std::string &api_key = {});

    const std::string &folder() const { return folder_; }

    const Optional<std::string> &iam_token() const { return iam_token_; }

    const Optional<std::string> &api_key() const { return api_key_; }

};

#endif /* YCLOUD_HH */