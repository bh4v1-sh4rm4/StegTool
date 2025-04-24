#ifndef STEG_UTILS_H
#define STEG_UTILS_H

#include <string>

bool encodeMessageToImage(const std::string& inputImage, const std::string& messageFile, const std::string& outputImage, const std::string& magicString);
bool decodeMessageFromImage(const std::string& inputImage, const std::string& outputMessageFile, const std::string& magicString);

#endif // STEG_UTILS_H
