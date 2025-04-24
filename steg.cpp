#include <iostream>
#include <string>
#include "steg_utils.h"

void encodeMessage() {
    std::string inputImage, messageFile, outputImage, magicString;

    std::cout << "Enter the input BMP image path: ";
    std::getline(std::cin, inputImage);

    std::cout << "Enter the message file path: ";
    std::getline(std::cin, messageFile);

    std::cout << "Enter the output BMP image path: ";
    std::getline(std::cin, outputImage);

    std::cout << "Enter a magic string for identification: ";
    std::getline(std::cin, magicString);

    if (encodeMessageToImage(inputImage, messageFile, outputImage, magicString)) {
        std::cout << "Message encoded successfully!\n";
    } else {
        std::cout << "Failed to encode the message.\n";
    }
}

void decodeMessage() {
    std::string inputImage, magicString, outputMessageFile;

    std::cout << "Enter the BMP image path to decode: ";
    std::getline(std::cin, inputImage);

    std::cout << "Enter the magic string for identification: ";
    std::getline(std::cin, magicString);

    std::cout << "Enter the output file path to save the extracted message: ";
    std::getline(std::cin, outputMessageFile);

    if (decodeMessageFromImage(inputImage, outputMessageFile, magicString)) {
        std::cout << "Message decoded successfully!\n";
    } else {
        std::cout << "Failed to decode the message.\n";
    }
}

int main() {
    std::string action;

    while (true) {
        std::cout << "\nEnter 'encode' to encode a message or 'decode' to decode: ";
        std::getline(std::cin, action);

        if (action == "encode") {
            encodeMessage();
        } else if (action == "decode") {
            decodeMessage();
        } else {
            std::cout << "Invalid option. Please enter 'encode' or 'decode'.\n";
        }

        std::string continueAction;
        std::cout << "\nDo you want to perform another operation? (y/n): ";
        std::getline(std::cin, continueAction);

        if (continueAction != "y") {
            std::cout << "Exiting the program.\n";
            break;
        }
    }

    return 0;
}
