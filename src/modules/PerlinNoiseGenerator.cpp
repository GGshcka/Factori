#include "PerlinNoiseGenerator.h"
#include <QImage>
#include <QOpenGLTexture>
#include <random>

PerlinNoiseGenerator::PerlinNoiseGenerator(int chunkX, int chunkY, int width, int height, unsigned int seed)
        : chunkX(chunkX), chunkY(chunkY), width(width), height(height) {
    generatePermutation(seed);
}

void PerlinNoiseGenerator::generatePermutation(unsigned int seed) {
    permutation.resize(256);
    for (int i = 0; i < 256; ++i) permutation[i] = i;
    std::mt19937 gen(seed);
    std::shuffle(permutation.begin(), permutation.end(), gen);
    permutation.insert(permutation.end(), permutation.begin(), permutation.end());
}

float PerlinNoiseGenerator::fade(float t) const {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoiseGenerator::lerp(float a, float b, float t) const {
    return a + t * (b - a);
}

float PerlinNoiseGenerator::grad(int hash, float x, float y) const {
    int h = hash & 15;
    float u = h < 4 ? x : y;
    float v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0 * v : 2.0 * v);
}

float PerlinNoiseGenerator::noise(float x, float y) const {
    x += chunkX * ((float)width / 100.0);
    y += chunkY * ((float)height / 100.0);

    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;
    x -= std::floor(x);
    y -= std::floor(y);
    float u = fade(x);
    float v = fade(y);
    int A = permutation[X] + Y;
    int B = permutation[X + 1] + Y;

    return lerp(lerp(grad(permutation[A], x, y), grad(permutation[B], x - 1, y), u),
                lerp(grad(permutation[A + 1], x, y - 1), grad(permutation[B + 1], x - 1, y - 1), u),
                v);
}


QOpenGLTexture* PerlinNoiseGenerator::generateTexture() {
    QImage image(width * 32, height * 32, QImage::Format_RGBA8888);
    QImage textureGrass(":/map/grass");
    QImage textureSand(":/map/sand");
    QImage textureWater(":/map/water");

    QPainter painter(&image);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float value = noise(x * 0.01f, y * 0.01f) * 0.75f + 0.75f;
            QImage* curTexture;

            if (value < 0.2f) {
                curTexture = &textureWater;
            } else if (value < 0.3f) {
                curTexture = &textureSand;
            } else {
                curTexture = &textureGrass;
            }

            QRect sourceRect(0, 0, 32, 32);  // Берём верхний левый кусок текстуры (можно сделать рандомный)
            QRect targetRect(x * 32, y * 32, 32, 32);
            painter.drawImage(targetRect, curTexture->mirrored(), sourceRect);
        }
    }

    painter.end();

    QOpenGLTexture* texture = new QOpenGLTexture(image);
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Nearest);
    return texture;
}

