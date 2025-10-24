#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstring>

struct TGAHeader {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
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
        header = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 24, 0}; // Initialize with default values
    }
    explicit TGAImage(const std::string& filename);
    

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