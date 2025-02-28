#ifndef FACTORI_PERLINNOISEGENERATOR_H
#define FACTORI_PERLINNOISEGENERATOR_H

#include <Qt>
#include <QtGlobal>
#include <QtWidgets>
#include <QOpenGLTexture>

class PerlinNoiseGenerator {
public:
    PerlinNoiseGenerator(int chunkX, int chunkY, int width, int height, unsigned int seed = 0);
    QOpenGLTexture* generateTexture();

private:
    int chunkX, chunkY;
    int width;
    int height;
    std::vector<int> permutation;
    float noise(float x, float y) const;
    float fade(float t) const;
    float lerp(float a, float b, float t) const;
    float grad(int hash, float x, float y) const;
    void generatePermutation(unsigned int seed);
};


#endif //FACTORI_PERLINNOISEGENERATOR_H
