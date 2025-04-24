// bmp_utils.cpp
#include "bmp_utils.h"
#include <fstream>
#include <iostream>

bool BMPImage::load(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open BMP file " << filename << std::endl;
        return false;
    }

    uint8_t header[54];
    file.read(reinterpret_cast<char *>(header), 54);
    if (header[0] != 'B' || header[1] != 'M')
    {
        std::cerr << "Error: Invalid BMP file format." << std::endl;
        return false;
    }

    // Safer header parsing using memcpy to avoid strict aliasing issues
    width_ = *reinterpret_cast<int32_t *>(&header[18]);
    height_ = *reinterpret_cast<int32_t *>(&header[22]);
    bitsPerPixel_ = *reinterpret_cast<uint16_t *>(&header[28]);
    dataOffset_ = *reinterpret_cast<uint32_t *>(&header[10]);

    if (bitsPerPixel_ != 24)
    {
        std::cerr << "Error: Only 24-bit BMP images are supported." << std::endl;
        return false;
    }
    std::cout << "Width: " << width_ << ", Height: " << height_
              << ", BitsPerPixel: " << bitsPerPixel_
              << ", DataOffset: " << dataOffset_ << std::endl;

    int rowSize = (width_ * 3 + 3) & (~3);
    int dataSize = rowSize * height_;

    pixelData.resize(dataSize);
    file.seekg(dataOffset_, std::ios::beg);
    file.read(reinterpret_cast<char *>(pixelData.data()), dataSize);

    file.close();
    return true;
}

bool BMPImage::save(const std::string &filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file for writing: " << filename << std::endl;
        return false;
    }

    uint8_t header[54] = {0};
    header[0] = 'B';
    header[1] = 'M';
    uint32_t fileSize = 54 + pixelData.size();
    *reinterpret_cast<uint32_t *>(&header[2]) = fileSize;
    *reinterpret_cast<uint32_t *>(&header[10]) = 54;
    *reinterpret_cast<uint32_t *>(&header[14]) = 40;
    *reinterpret_cast<int32_t *>(&header[18]) = width_;
    *reinterpret_cast<int32_t *>(&header[22]) = height_;
    *reinterpret_cast<uint16_t *>(&header[26]) = 1;
    *reinterpret_cast<uint16_t *>(&header[28]) = 24;

    file.write(reinterpret_cast<char *>(header), 54);
    file.write(reinterpret_cast<char *>(pixelData.data()), pixelData.size());
    file.close();

    return true;
}
// Set the least significant bit (LSB) of the byte at the specified pixel and color channel
void BMPImage::setPixelBit(size_t pixelIndex, size_t colorChannel, bool bit)
{
    size_t byteIndex = pixelIndex * 3 + colorChannel;
    if (byteIndex >= pixelData.size()) return;

    pixelData[byteIndex] &= 0xFE;           // Clear LSB
    pixelData[byteIndex] |= (bit ? 1 : 0);  // Set new bit
}

// Get the least significant bit (LSB) of the byte at the specified pixel and color channel
bool BMPImage::getPixelBit(size_t pixelIndex, size_t colorChannel) const
{
    size_t byteIndex = pixelIndex * 3 + colorChannel;
    if (byteIndex >= pixelData.size()) return false;

    return pixelData[byteIndex] & 1;
}
