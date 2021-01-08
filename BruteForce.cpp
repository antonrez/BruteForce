#include <vector>
#include <fstream>
#include <exception>
#include <iostream>
#include <thread>
#include "openssl/evp.h"
#include "openssl/aes.h"
#include "openssl/sha.h"
#include "GeneretPass.h"
#include "Decryptor.h"
#include "SimpleTimer.h"

HANDLE h = INVALID_HANDLE_VALUE;
std::vector<std::string> crackBuf;
bool generet_if = true;
int index = 0;
std::mutex g_mutex;

void ReadFile(const std::string& filePath, std::vector<unsigned char>& buf)
{
    
   
    std::basic_fstream<unsigned char> fileStream(filePath, std::ios::binary | std::fstream::in);
    if (!fileStream.is_open())
    {
        throw std::runtime_error("Can not open file " + filePath);
    }

    buf.clear();
    buf.insert(buf.begin(), std::istreambuf_iterator<unsigned char>(fileStream), std::istreambuf_iterator<unsigned char>());

    fileStream.close();
   
}

void CrackThread( std::vector<unsigned char> chipherText, std::vector<unsigned char> chipherhash, std::string plainFile)
{

    std::vector<std::string> crackB;
    DWORD res = WaitForSingleObject(h, INFINITE);
    g_mutex.lock();
    crackB = std::move(crackBuf);
    g_mutex.unlock();

    for (int i = 0; i < crackB.size(); ++i)
    {  
        if (!generet_if) return;

        Decryptor Decry;

        Decry.PasswordToKey(crackB[i]);

        std::vector<unsigned char> plainText = Decry.DecryptAes(chipherText);
        if (plainText.size() > 0) {
            std::vector<unsigned char> m_hash;
            Decry.CalculateHash(plainText, m_hash);
            if (chipherhash == m_hash) {
                Decry.WriteFile(plainFile, plainText);
                std::cout << "Cracked password: " << crackB[i] << std::endl;
                generet_if = false;
                ReleaseSemaphore(h, 10, 0);
                return;
            }
        }
    }
}


int main()
{
    MySimpleTimer timerprocces;
    OpenSSL_add_all_algorithms();
    std::string chiferFile = "chipher_text_brute_force";
    std::string plainFile = "plain_text_brute_force";
    std::vector<unsigned char> chipherText;

    ReadFile(chiferFile, chipherText);
    std::vector<unsigned char> chipherhash(chipherText.begin() + chipherText.size() - 32, chipherText.end());
    std::vector<char> Chars;    //סטלגמכû ןמהבמנא

    for (char c = '0'; c <= 'z'; c++) {
        if (islower(c) || isdigit(c))Chars.push_back(c);
    }
    try
    {    
         h = CreateSemaphoreW(NULL, 0, 10, L"GeneratePassData");
        
         std::thread Generet(GeneretPass, Chars, h, std::ref(crackBuf), std::ref(g_mutex), std::ref(generet_if));
         std::thread tr1(CrackThread, chipherText, chipherhash, plainFile);
         std::thread tr2(CrackThread, chipherText, chipherhash, plainFile);
         std::thread tr3(CrackThread, chipherText, chipherhash, plainFile);
         std::thread tr4(CrackThread, chipherText, chipherhash, plainFile);
         std::thread tr5(CrackThread, chipherText, chipherhash, plainFile);
         std::thread tr6(CrackThread, chipherText, chipherhash, plainFile);
         std::thread tr7(CrackThread, chipherText, chipherhash, plainFile);
         std::thread tr8(CrackThread, chipherText, chipherhash, plainFile);
         
         
         tr1.join();
         tr2.join();
         tr3.join();
         tr4.join();
         tr5.join();
         tr6.join();
         tr7.join();
         tr8.join();
         Generet.join();
    }
    catch (const std::runtime_error& ex)
    {
        std::cerr << ex.what();
    }
    CloseHandle(h);
    return 0;
}