//
// Created by rxwyun on 2022/7/26.
//

#ifndef ARM_IOT_ENCRYPT_HPP
#define ARM_IOT_ENCRYPT_HPP

#include <sstream>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/include/cryptopp/aes.h"
#include "cryptopp/include/cryptopp/cryptlib.h"
#include "cryptopp/include/cryptopp/modes.h"
#include "cryptopp/include/cryptopp/hex.h"
#include "cryptopp/include/cryptopp/md5.h"
#include "Base64.hpp"

using CryptoPP::Exception;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::AES;
using CryptoPP::CBC_Mode;
using namespace std;


namespace IOT {
    namespace UTIL {
        class Encrypt {
        public:
            string static aesCBCEncrypt(const string &plain, const string &key) {
                string cipher;
                string iv = key.substr(0, 16);

                CBC_Mode<AES>::Encryption e;
                e.SetKeyWithIV(reinterpret_cast<const CryptoPP::byte *>(key.c_str()), key.size(),
                               reinterpret_cast<const CryptoPP::byte *>(iv.c_str()));

                // The StreamTransformationFilter removes
                //  padding as required.
                StringSource s(plain, true,
                               new StreamTransformationFilter(e,
                                                              new StringSink(cipher)
                               ) // StreamTransformationFilter
                ); // StringSource

                // Pretty print
                return Base64::encode(cipher);
            }

            string static aesCBCDecrypt(const string &plain, const string &key) {
                string iv = key.substr(0, 16);

                string encodeByte = Base64::decode(plain);

                string recovered;
                CBC_Mode<AES>::Decryption d;
                d.SetKeyWithIV(reinterpret_cast<const CryptoPP::byte *>(key.c_str()), key.size(),
                               reinterpret_cast<const CryptoPP::byte *>(iv.c_str()));

                // The StreamTransformationFilter removes
                //  padding as required.
                StringSource s(encodeByte, true,
                               new StreamTransformationFilter(d,
                                                              new StringSink(recovered)
                               ) // StreamTransformationFilter
                ); // StringSource

                return recovered;
            }

            string static md5(const string &src) {
                string dst;
                CryptoPP::Weak::MD5 md5;
                StringSource(src, true, new CryptoPP::HashFilter(md5, new CryptoPP::HexEncoder(new StringSink(dst))));

                for (char &i: dst) {
                    i = tolower(i);
                }

                return dst;
            }
        };
    }
}

#endif //ARM_IOT_ENCRYPT_HPP
