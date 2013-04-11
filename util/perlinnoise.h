#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <QDebug>
#include <QtCore/qmath.h>
#include <QPainter>


/**
 * @brief Perlin Noise Generator
 *
 * This is based on the excellent code by Pontus on Dream-in-Code.
 * You can find the original code at:
 * http://www.dreamincode.net/forums/topic/66480-perlin-noise
 *
 * The noise returned is the same for a set of parameters (not random).
 *
 */
class PerlinNoise
{
private:
    static inline double findnoise2(double x, double y);
    static inline double interpolate1(double a, double b, double x);
    static double noise2(double x, double y);

public:
    /**
    * @brief Draw noise onto a painter.
    *
    * @param painter: The painter to draw with
    * @param octaves: The number of octaves to smooth out the noise amplitudes
    * @param p: The persistance. Good value is 1/2.
    * @param zoom: Zoom. Good value is 75.
    */
    static void draw(QPainter *painter, int octaves, double p, double zoom, int width, int height);

};

#endif // PERLINNOISE_H
