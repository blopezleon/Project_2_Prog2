#include "TGAImage.h"
#include <cmath>

TGAImage::TGAImage(const std::string& filename) {
    load(filename);
}

void TGAImage::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    // Read header
    file.read(reinterpret_cast<char*>(&header), sizeof(TGAHeader));
    if (!file) {
        throw std::runtime_error("Error reading header from file: " + filename);
    }

    // Skip image ID field
    if (header.idLength > 0) {
        file.seekg(header.idLength, std::ios::cur);
    }

    // Skip color map if present
    if (header.colorMapType > 0) {
        int mapSize = header.colorMapLength * (header.colorMapDepth + 7) / 8;
        file.seekg(mapSize, std::ios::cur);
    }

    // Read pixel data
    int size = header.width * header.height * (header.bitsPerPixel / 8);
    pixelData.resize(size);
    file.read(reinterpret_cast<char*>(pixelData.data()), size);
    if (!file) {
        throw std::runtime_error("Error reading pixel data from file: " + filename);
    }

    // If not RGB format, convert to RGB
    if (header.bitsPerPixel != 24) {
        
        throw std::runtime_error("Unsupported bits per pixel: " + std::to_string(header.bitsPerPixel));
    }
}

void TGAImage::save(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot create file: " + filename);
    }

    
    file.write(reinterpret_cast<char*>(&header), sizeof(TGAHeader));
    
    
    file.write(reinterpret_cast<char*>(pixelData.data()), pixelData.size());
}

unsigned char TGAImage::clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return static_cast<unsigned char>(value);
}

float TGAImage::normalize(unsigned char value) {
    return static_cast<float>(value) / 255.0f;
}

unsigned char TGAImage::denormalize(float value) {
    return clamp(static_cast<int>(value * 255.0f + 0.5f));
}

unsigned char TGAImage::getBlue(int x, int y) const {

    int flipped_y = header.height - 1 - y;
    int index = (flipped_y * header.width + x) * 3;
    return pixelData[index];
}

unsigned char TGAImage::getGreen(int x, int y) const {

    int flipped_y = header.height - 1 - y;
    int index = (flipped_y * header.width + x) * 3;
    return pixelData[index + 1];
}

unsigned char TGAImage::getRed(int x, int y) const {
 
    int flipped_y = header.height - 1 - y;
    int index = (flipped_y * header.width + x) * 3;
    return pixelData[index + 2];
}

void TGAImage::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {

    int flipped_y = header.height - 1 - y;
    int index = (flipped_y * header.width + x) * 3;
    pixelData[index] = b;     
    pixelData[index + 1] = g; 
    pixelData[index + 2] = r; 
}

TGAImage TGAImage::multiply(const TGAImage& top, const TGAImage& bottom) {
    TGAImage result = bottom; 
    
    for (int y = 0; y < result.getHeight(); y++) {
        for (int x = 0; x < result.getWidth(); x++) {
            float r1 = normalize(top.getRed(x, y));
            float g1 = normalize(top.getGreen(x, y));
            float b1 = normalize(top.getBlue(x, y));
            
            float r2 = normalize(bottom.getRed(x, y));
            float g2 = normalize(bottom.getGreen(x, y));
            float b2 = normalize(bottom.getBlue(x, y));
            
            result.setPixel(x, y,
                denormalize(r1 * r2),
                denormalize(g1 * g2),
                denormalize(b1 * b2));
        }
    }
    return result;
}

TGAImage TGAImage::screen(const TGAImage& top, const TGAImage& bottom) {
    TGAImage result = bottom;
    
    for (int y = 0; y < result.getHeight(); y++) {
        for (int x = 0; x < result.getWidth(); x++) {
            float r1 = normalize(top.getRed(x, y));
            float g1 = normalize(top.getGreen(x, y));
            float b1 = normalize(top.getBlue(x, y));
            
            float r2 = normalize(bottom.getRed(x, y));
            float g2 = normalize(bottom.getGreen(x, y));
            float b2 = normalize(bottom.getBlue(x, y));
            
            result.setPixel(x, y,
                denormalize(1.0f - (1.0f - r1) * (1.0f - r2)),
                denormalize(1.0f - (1.0f - g1) * (1.0f - g2)),
                denormalize(1.0f - (1.0f - b1) * (1.0f - b2)));
        }
    }
    return result;
}

TGAImage TGAImage::subtract(const TGAImage& top, const TGAImage& bottom) {
    TGAImage result = bottom;
    
    for (int y = 0; y < result.getHeight(); y++) {
        for (int x = 0; x < result.getWidth(); x++) {
            result.setPixel(x, y,
                clamp(static_cast<int>(bottom.getRed(x, y)) - static_cast<int>(top.getRed(x, y))),
                clamp(static_cast<int>(bottom.getGreen(x, y)) - static_cast<int>(top.getGreen(x, y))),
                clamp(static_cast<int>(bottom.getBlue(x, y)) - static_cast<int>(top.getBlue(x, y))));
        }
    }
    return result;
}

TGAImage TGAImage::add(const TGAImage& top, const TGAImage& bottom) {
    TGAImage result = bottom;
    
    for (int y = 0; y < result.getHeight(); y++) {
        for (int x = 0; x < result.getWidth(); x++) {
            result.setPixel(x, y,
                clamp(static_cast<int>(top.getRed(x, y)) + static_cast<int>(bottom.getRed(x, y))),
                clamp(static_cast<int>(top.getGreen(x, y)) + static_cast<int>(bottom.getGreen(x, y))),
                clamp(static_cast<int>(top.getBlue(x, y)) + static_cast<int>(bottom.getBlue(x, y))));
        }
    }
    return result;
}

TGAImage TGAImage::overlay(const TGAImage& top, const TGAImage& bottom) {
    TGAImage result = bottom;
    
    for (int y = 0; y < result.getHeight(); y++) {
        for (int x = 0; x < result.getWidth(); x++) {
            float r1 = normalize(top.getRed(x, y));
            float g1 = normalize(top.getGreen(x, y));
            float b1 = normalize(top.getBlue(x, y));
            
            float r2 = normalize(bottom.getRed(x, y));
            float g2 = normalize(bottom.getGreen(x, y));
            float b2 = normalize(bottom.getBlue(x, y));
            
            auto overlayChannel = [](float c1, float c2) {
                if (c2 <= 0.5f) {
                    return 2.0f * c1 * c2;
                } else {
                    return 1.0f - 2.0f * (1.0f - c1) * (1.0f - c2);
                }
            };
            
            result.setPixel(x, y,
                denormalize(overlayChannel(r1, r2)),
                denormalize(overlayChannel(g1, g2)),
                denormalize(overlayChannel(b1, b2)));
        }
    }
    return result;
}

void TGAImage::addToChannel(char channel, unsigned char value) {
    size_t offset = (channel == 'R') ? 2 : (channel == 'G' ? 1 : 0);
    
    for (size_t i = offset; i < pixelData.size(); i += 3) {
        pixelData[i] = clamp(static_cast<int>(pixelData[i]) + value);
    }
}

void TGAImage::multiplyChannel(char channel, float factor) {
    size_t offset = (channel == 'R') ? 2 : (channel == 'G' ? 1 : 0);
    
    for (size_t i = offset; i < pixelData.size(); i += 3) {
        pixelData[i] = clamp(static_cast<int>(pixelData[i] * factor));
    }
}

TGAImage TGAImage::extractChannel(char channel) const {
    TGAImage result = *this;
    
    for (int y = 0; y < header.height; y++) {
        for (int x = 0; x < header.width; x++) {
            unsigned char value = 0;
            switch (channel) {
                case 'R': value = getRed(x, y); break;
                case 'G': value = getGreen(x, y); break;
                case 'B': value = getBlue(x, y); break;
            }
            result.setPixel(x, y, value, value, value);
        }
    }
    return result;
}

void TGAImage::setChannel(char channel, const TGAImage& source) {
    int offset = (channel == 'R') ? 2 : (channel == 'G' ? 1 : 0);
    
    for (int y = 0; y < header.height; y++) {
        for (int x = 0; x < header.width; x++) {
            int index = (y * header.width + x) * 3 + offset;
            pixelData[index] = source.getRed(x, y); 
        }
    }
}

TGAImage TGAImage::rotate180() const {
    TGAImage result = *this;
    
    for (int y = 0; y < header.height / 2; y++) {
        for (int x = 0; x < header.width; x++) {
            int opposite_y = header.height - 1 - y;
            int opposite_x = header.width - 1 - x;
            
            unsigned char r1 = getRed(x, y);
            unsigned char g1 = getGreen(x, y);
            unsigned char b1 = getBlue(x, y);
            
            unsigned char r2 = getRed(opposite_x, opposite_y);
            unsigned char g2 = getGreen(opposite_x, opposite_y);
            unsigned char b2 = getBlue(opposite_x, opposite_y);
            
            result.setPixel(x, y, r2, g2, b2);
            result.setPixel(opposite_x, opposite_y, r1, g1, b1);
        }
    }
    
    
    if (header.height % 2 == 1) {
        int y = header.height / 2;
        for (int x = 0; x < header.width / 2; x++) {
            int opposite_x = header.width - 1 - x;
            
            unsigned char r1 = getRed(x, y);
            unsigned char g1 = getGreen(x, y);
            unsigned char b1 = getBlue(x, y);
            
            unsigned char r2 = getRed(opposite_x, y);
            unsigned char g2 = getGreen(opposite_x, y);
            unsigned char b2 = getBlue(opposite_x, y);
            
            result.setPixel(x, y, r2, g2, b2);
            result.setPixel(opposite_x, y, r1, g1, b1);
        }
    }
    
    return result;
}