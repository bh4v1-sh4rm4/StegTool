#include "steg_utils.h"
#include "bmp_utils.h"
#include <fstream>
#include <iostream>

bool encodeMessageToImage(const std::string &inputImage, const std::string &messageFile, const std::string &outputImage, const std::string &magicString)
{
    std::ifstream messageStream(messageFile);
    if (!messageStream.is_open())
    {
        std::cerr << "Error: Could not open message file.\n";
        return false;
    }

    std::string message((std::istreambuf_iterator<char>(messageStream)), std::istreambuf_iterator<char>());
    messageStream.close();
    message = magicString + message;

    BMPImage bmp;
    std::cout << "Loading image..." << std::endl;
    if (!bmp.load(inputImage))
    {
        std::cerr << "Error: Could not load the input BMP image.\n";
        return false;
    }
    std::cout << "Reading message file..." << std::endl;
    size_t imageCapacity = bmp.width() * bmp.height() * 3;
    int requiredBits = 32 + (int)magicString.size() * 8 + message.size() * 8;
    std::cout << "Available bits: " << imageCapacity << ", Required bits: " << requiredBits << std::endl;

    if (requiredBits > imageCapacity)
    {
        std::cerr << "Error: Message is too large to fit in the image.\n";
        return false;
    }

    size_t bitIndex = 0;
    for (char c : message)
    {
        for (int i = 7; i >= 0; --i)
        {
            size_t pixelIndex = bitIndex / 3;
            size_t colorChannel = bitIndex % 3;

            bool bit = (c >> i) & 1;
            bmp.setPixelBit(pixelIndex, colorChannel, bit);
            bitIndex++;
        }
    }

    if (!bmp.save(outputImage))
    {
        std::cerr << "Error: Could not save the output BMP image.\n";
        return false;
    }

    return true;
}

bool decodeMessageFromImage(const std::string &inputImage, const std::string &outputMessageFile, const std::string &magicString)
{
    BMPImage bmp;
    if (!bmp.load(inputImage))
    {
        std::cerr << "Error: Could not load the input BMP image.\n";
        return false;
    }

    std::string decodedMessage;
    std::string currentBuffer;
    char currentChar = 0;
    int bitsCollected = 0;
    bool magicStringFound = false;

    size_t totalBits = bmp.width() * bmp.height() * 3;

    for (size_t bitIndex = 0; bitIndex < totalBits; ++bitIndex)
    {
        size_t pixelIndex = bitIndex / 3;
        size_t colorChannel = bitIndex % 3;

        bool bit = bmp.getPixelBit(pixelIndex, colorChannel);
        currentChar = (currentChar << 1) | bit;
        ++bitsCollected;

        if (bitsCollected == 8)
        {
            bitsCollected = 0;

            if (!magicStringFound)
            {
                currentBuffer += currentChar;
                if (currentBuffer.size() > magicString.size())
                {
                    currentBuffer.erase(0, 1); // keep last N chars
                }

                if (currentBuffer == magicString)
                {
                    magicStringFound = true;
                }
            }
            else
            {
                decodedMessage += currentChar;

                // Optional stop condition: null-terminator
                if (currentChar == '\0')
                    break;
            }

            currentChar = 0;
        }
    }

    if (magicStringFound)
    {
        std::ofstream outFile(outputMessageFile);
        if (!outFile)
        {
            std::cerr << "Error: Could not open output file.\n";
            return false;
        }

        outFile << decodedMessage;
        outFile.close();
        std::cout << "Message decoded successfully.\n";
        return true;
    }

    std::cerr << "Error: Magic string not found.\n";
    return false;
}
