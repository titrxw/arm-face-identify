//
// Created by rxwyun on 2022/8/6.
//

#ifndef ARM_IOT_BASE64_HPP
#define ARM_IOT_BASE64_HPP

#include "sstream"
#include "cryptopp/include/cryptopp/base64.h"

using CryptoPP::Base64Encoder;
using CryptoPP::Base64Decoder;
using CryptoPP::StringSink;
using CryptoPP::StringSource;

using namespace std;


namespace IOT {
    namespace UTIL {

        class Base64 {
        public:
            string static encode(string data) {
                string encoded;
                StringSource(data, true,
                             new Base64Encoder(
                                     new StringSink(encoded)
                             )
                );

                return encoded;
            }

            string static decode(string data) {
                string encoded;
                StringSource(data, true, new Base64Decoder(
                        new StringSink(encoded)
                ));

                return encoded;
            }
        };
    }
}

#endif //ARM_IOT_BASE64_HPP
