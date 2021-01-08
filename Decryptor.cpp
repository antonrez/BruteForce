#include "Decryptor.h"
#include "openssl/evp.h"
#include "openssl/aes.h"
#include "openssl/sha.h"
#include <fstream>
#include <iostream>
#include <vector>
unsigned char key[EVP_MAX_KEY_LENGTH];
unsigned char iv[EVP_MAX_IV_LENGTH];

void Decryptor::WriteFile(const std::string& filePath, const std::vector<unsigned char>& buf)
{
    std::basic_ofstream<unsigned char> fileStream(filePath, std::ios::binary);
    fileStream.write(&buf[0], buf.size());
    fileStream.close();
}

void Decryptor::PasswordToKey(std::string& password)
{
    const EVP_MD* dgst = EVP_get_digestbyname("md5");
    if (!dgst)
    {
        throw std::runtime_error("no such digest");
    }

    const unsigned char* salt = NULL;
    if (!EVP_BytesToKey(EVP_aes_128_cbc(), EVP_md5(), salt,
        reinterpret_cast<unsigned char*>(&password[0]),
        password.size(), 1, key, iv))
    {
        throw std::runtime_error("EVP_BytesToKey failed");
    }
};

std::vector<unsigned char> Decryptor::DecryptAes(const std::vector<unsigned char> chipherText /*, std::vector<unsigned char> plainText*/)
{
    std::vector<unsigned char> plainText;
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    
    std::vector<unsigned char> plainTextBuf(chipherText.size() + AES_BLOCK_SIZE);
    int plainTextSize = 0;
    EVP_DecryptUpdate(ctx, &plainTextBuf[0], &plainTextSize, &chipherText[0], chipherText.size() - 32);
    int lastPartLen = 0;
    EVP_DecryptFinal_ex(ctx, &plainTextBuf[0] + plainTextSize, &lastPartLen);
    
    plainTextSize += lastPartLen;
    plainTextBuf.erase(plainTextBuf.begin() + plainTextSize, plainTextBuf.end());

    plainText.swap(plainTextBuf);

    EVP_CIPHER_CTX_free(ctx);
    return plainText;
}

void Decryptor::CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash)
{
    std::vector<unsigned char> hashTmp(SHA256_DIGEST_LENGTH);

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, &data[0], data.size());
    SHA256_Final(&hashTmp[0], &sha256);

    hash.swap(hashTmp);
};