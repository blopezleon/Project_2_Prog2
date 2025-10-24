#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstring>

struct TGAHeader {
    unsigned char idLength;
    unsigned char colorMapType;
    unsigned char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    unsigned char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    unsigned char bitsPerPixel;
    unsigned char imageDescriptor;
};

class TGAImage {
private:
    TGAHeader header;
    std::vector<unsigned char> pixelData;
    static unsigned char clamp(int value);
    static float normalize(unsigned char value);
    static unsigned char denormalize(float value);

public:
    TGAImage() {
        memset(&header, 0, sizeof(TGAHeader));
        header.dataTypeCode = 2;     // Uncompressed RGB
        header.bitsPerPixel = 24;    // 24-bit RGB
        header.imageDescriptor = 32;  // Upper-left origin, 8 bits alpha
    }
    explicit TGAImage(const std::string& filename);
    TGAImage(const TGAImage& other) : header(other.header), pixelData(other.pixelData) {}
    TGAImage& operator=(const TGAImage& other) {
        if (this != &other) {
            header = other.header;
            pixelData = other.pixelData;
        }
        return *this;
    }
    

    void load(const std::string& filename);
    void save(const std::string& filename);
    

    int getWidth() const { return header.width; }
    int getHeight() const { return header.height; }
    

    unsigned char getBlue(int x, int y) const;
    unsigned char getGreen(int x, int y) const;
    unsigned char getRed(int x, int y) const;

    void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    
    static TGAImage multiply(const TGAImage& top, const TGAImage& bottom);
    static TGAImage screen(const TGAImage& top, const TGAImage& bottom);
    static TGAImage subtract(const TGAImage& top, const TGAImage& bottom);
    static TGAImage add(const TGAImage& top, const TGAImage& bottom);
    static TGAImage overlay(const TGAImage& top, const TGAImage& bottom);
    

    void addToChannel(char channel, unsigned char value);
    void multiplyChannel(char channel, float factor);
    TGAImage extractChannel(char channel) const;
    void setChannel(char channel, const TGAImage& source);
    

    TGAImage rotate180() const;
};