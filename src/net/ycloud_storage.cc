/* -*-mode:c++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "ycloud_storage.hh"

#include <stdexcept>

#include "util/util.hh"

using namespace std;

const static string YCLOUD_ACCESS_KEY_ENV { "YCLOUD_ACCESS_KEY" };
const static string YCLOUD_SECRET_KEY_ENV { "YCLOUD_SECRET_KEY" };

YandexStorageCredentials::YandexStorageCredentials()
        : YandexStorageCredentials( safe_getenv( YCLOUD_ACCESS_KEY_ENV ),
                                    safe_getenv( YCLOUD_SECRET_KEY_ENV ) )
{}

YandexStorageCredentials::YandexStorageCredentials( const string & access_key,
                                                    const string & secret_key )
        : access_key_( access_key ), secret_key_( secret_key )
{
    if ( access_key_.length() == 0 or secret_key_.length() == 0 )
    {
        throw runtime_error( "Missing Yandex Cloud access key and/or secret key" );
    }
}
