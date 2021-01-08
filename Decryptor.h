#include <iostream>
#include <vector>
#include <Windows.h>
class Decryptor
{
public:
    void PasswordToKey(std::string& password);
    void CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash);
    std::vector<unsigned char> DecryptAes(const std::vector<unsigned char> chipherText/*, std::vector<unsigned char> plainText*/);
    void WriteFile(const std::string& filePath, const std::vector<unsigned char>& buf);
};

