//
// Created by rxwyun on 2022/7/26.
//

#ifndef ARM_FACE_IDENTIFY_ENCRYPT_H
#define ARM_FACE_IDENTIFY_ENCRYPT_H

#include <sstream>
#include "cryptopp/include/cryptopp/aes.h"
#include "cryptopp/include/cryptopp/cryptlib.h"
#include "cryptopp/include/cryptopp/base64.h"
#include "cryptopp/include/cryptopp/modes.h"

using CryptoPP::Base64Encoder;
using CryptoPP::Base64Decoder;
using CryptoPP::Exception;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::AES;
using CryptoPP::CBC_Mode;
using namespace std;

class Encrypt {
public:
    string static encrypt(string plain, string key) {
        string cipher;
        string iv = key.substr(0, 16);

        CBC_Mode< AES >::Encryption e;
        e.SetKeyWithIV((byte*)key.c_str(), key.size(), (byte*)iv.c_str());

        // The StreamTransformationFilter removes
        //  padding as required.
        StringSource s(plain, true,
                       new StreamTransformationFilter(e,
                                                      new StringSink(cipher)
                       ) // StreamTransformationFilter
        ); // StringSource

        // Pretty print
        string encoded;
        StringSource(cipher, true,
                     new Base64Encoder(
                             new StringSink(encoded)
                     ) // HexEncoder
        ); // StringSource
        return encoded;
    }
    string static decrypt(string plain, string key) {
        string encodeByte;
        string iv = key.substr(0, 16);

        StringSource(plain, true, new Base64Decoder(
                new StringSink(encodeByte)
        ));

        string recovered;
        CBC_Mode<AES>::Decryption d;
        d.SetKeyWithIV((byte *) key.c_str(), key.size(), (byte *) iv.c_str());

        // The StreamTransformationFilter removes
        //  padding as required.
        StringSource s(encodeByte, true,
                       new StreamTransformationFilter(d,
                                                      new StringSink(recovered)
                       ) // StreamTransformationFilter
        ); // StringSource

        return recovered;
    }
};


#endif //ARM_FACE_IDENTIFY_ENCRYPT_H
