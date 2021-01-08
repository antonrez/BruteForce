#pragma once

#include <vector>
#include <string>
#include <Windows.h>
#include <mutex>

void GeneretPass(std::vector<char> Chars, HANDLE h, std::vector<std::string>& crackBuf, std::mutex& g_mutex, bool& generet_if);