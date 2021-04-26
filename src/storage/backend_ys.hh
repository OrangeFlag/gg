/* -*-mode:c++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef STORAGE_BACKEND_YS_HH
#define STORAGE_BACKEND_YS_HH

#include <net/ycloud_storage.hh>
#include "backend.hh"
#include "net/s3.hh"
#include "backend_ys.hh"

class YandexStorageBackend : public StorageBackend {
private:
    S3Client client_;
    std::string bucket_;

public:
    YandexStorageBackend(const YandexStorageCredentials &credentials,
                         const std::string &bucket);

    void put(const std::vector<storage::PutRequest> &requests,
             const PutCallback &success_callback = [](const storage::PutRequest &) {}) override;

    void get(const std::vector<storage::GetRequest> &requests,
             const GetCallback &success_callback = [](const storage::GetRequest &) {}) override;

};

#endif /* STORAGE_BACKEND_YS_HH */