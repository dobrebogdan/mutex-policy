#ifndef MTXPOL_COMMON_UTILS_HPP_
#define MTXPOL_COMMON_UTILS_HPP_

#include <string>

namespace mtxpol {

std::string home();

std::string mtxpolHome();

std::string mtxpolInDir();

std::string mtxpolOutDir();

void writeMessage(int fd, void* message, int messageSize);

int readMessage(int fd, void* message, int messageSize, int maxReads = -1);

int openOrDie(const char* filePath, int mode);

}  // namespace mtxpol

#endif  // MTXPOL_COMMON_UTILS_HPP_