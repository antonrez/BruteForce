#include <vector>
#include <string>
#include <iostream>
#include "GeneretPass.h"

void GeneretPass(std::vector<char> Chars, HANDLE h, std::vector<std::string>& crackBuf, std::mutex& g_mutex, bool& generet_if)
{
    int n = Chars.size();
    int countPassToVector = 250000;
    int i = 0;
    while (true)
    {
        ++i;
        int N = 1;
        for (int j = 0; j < i; j++)N *= n;

        for (int j = 0; j < N; j++)
        {
            if (!generet_if) return;

            int K = 1;
            std::string crack = "";
            for (int k = 0; k < i; k++)
            {
                crack += Chars[j / K % n];
                K *= n;
            }

            g_mutex.lock();
            crackBuf.emplace_back(crack);
            g_mutex.unlock();

            if (crackBuf.size()== countPassToVector)
            {
                ReleaseSemaphore(h, 1, 0);
            }
            
        }
    }
}
