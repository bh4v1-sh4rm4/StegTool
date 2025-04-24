// bmp_utils.h
#ifndef BMP_UTILS_H
#define BMP_UTILS_H

#include <vector>
#include <string>
#include <cstdint>

class BMPImage
{
public:
    bool load(const std::string &filename);
    bool save(const std::string &filename);
    bool getPixelBit(size_t byteIndex, size_t bitIndex) const;
    void setPixelBit(size_t pixelIndex, size_t colorChannel, bool bit);
    int width() const { return width_; }
    int height() const { return height_; }
    int bitsPerPixel() const { return bitsPerPixel_; }
    std::vector<uint8_t> &data() { return pixelData; }
    const std::vector<uint8_t> &data() const { return pixelData; }

private:
    int32_t width_ = 0;
    int32_t height_ = 0;
    int16_t bitsPerPixel_ = 0;
    uint32_t dataOffset_ = 0;
    std::vector<uint8_t> pixelData;
};

#endif // BMP_UTILS_H
