//
//  CryptoUtil.h
//  cie-pkcs11
//
//  Created by ugo chirico on 07/01/2019.
//  SPDX-License-Identifier: BSD-3-Clause
//

#ifndef CryptoUtil_h
#define CryptoUtil_h

#include "../Cryptopp/modes.h"
#include "../Cryptopp/aes.h"
#include "../Cryptopp/filters.h"
#include "../keys.h"
#include "../Cryptopp/sha.h"
#include <string>

using namespace CryptoPP;

int encrypt(std::string& message, std::string& ciphertext)
{
    CryptoPP::byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );
    
    std::string enckey = ENCRYPTION_KEY;
    
    CryptoPP::byte digest[SHA1::DIGESTSIZE];
    CryptoPP::SHA1().CalculateDigest(digest, (CryptoPP::byte*)enckey.c_str(), enckey.length());
    memcpy(key, digest, CryptoPP::AES::DEFAULT_KEYLENGTH );
    //
    // Create Cipher Text
    //
    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );
    
    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( ciphertext ) );
    stfEncryptor.Put( reinterpret_cast<const unsigned char*>( message.c_str() ), message.length() + 1 );
    stfEncryptor.MessageEnd();
    
    return 0;
};

int decrypt(std::string& ciphertext, std::string& message)
{
    CryptoPP::byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );
    
    std::string enckey = ENCRYPTION_KEY;
    
    CryptoPP::byte digest[SHA1::DIGESTSIZE];
    CryptoPP::SHA1().CalculateDigest(digest, (CryptoPP::byte*)enckey.c_str(), enckey.length());
    memcpy(key, digest, CryptoPP::AES::DEFAULT_KEYLENGTH );
    
    //
    // Decrypt
    //
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );
    
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( message ) );
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
    stfDecryptor.MessageEnd();
    
    return 0;
};

#endif /* CryptoUtil_h */
