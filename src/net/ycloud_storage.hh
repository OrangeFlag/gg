/* -*-mode:c++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef YCLOUD_HH
#define YCLOUD_HH

#include <string>

class YandexStorageCredentials
{
private:
    std::string access_key_;
    std::string secret_key_;

public:
    YandexStorageCredentials();
    YandexStorageCredentials( const std::string & access_key,
                              const std::string & secret_key );

    const std::string & access_key() const { return access_key_; }
    const std::string & secret_key() const { return secret_key_; }
};

#endif /* YCLOUD_HH */
