#include "perlinnoise.h"

inline double PerlinNoise::findnoise2(double x, double y) {
    int n=(int)x+(int)y*57;
    n=(n<<13)^n;
    int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
    return 1.0-((double)nn/1073741824.0);
}

inline double PerlinNoise::interpolate1(double a, double b, double x) {
    double ft=x * 3.1415927;
    double f=(1.0-cos(ft))* 0.5;
    return a*(1.0-f)+b*f;
}

double PerlinNoise::noise2(double x, double y) {
    double floorx=(double)((int)x); //This is kinda a cheap way to floor a double integer.
    double floory=(double)((int)y);
    double s,t,u,v; //Integer declaration
    s=findnoise2(floorx,floory);
    t=findnoise2(floorx+1,floory);
    u=findnoise2(floorx,floory+1); //Get the surrounding pixels to calculate the transition.
    v=findnoise2(floorx+1,floory+1);
    double int1=interpolate1(s,t,x-floorx); //Interpolate between the values.
    double int2=interpolate1(u,v,x-floorx); //Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
    return interpolate1(int1,int2,y-floory); //Here we use y-floory, to get the 2nd dimension.
}

void PerlinNoise::draw(QPainter *painter, int octaves, double p, double zoom, int width, int height) {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            double getnoise =0;
            for(int a = 0; a < octaves - 1; a++) {
                double frequency = qPow(2,a); //This increases the frequency with every loop of the octave.
                double amplitude = qPow(p,a); //This decreases the amplitude with every loop of the octave.
                getnoise += PerlinNoise::noise2(((double)x)*frequency/zoom,((double)y)/zoom*frequency)*amplitude; //This uses our perlin noise functions. It calculates all our zoom and frequency and amplitude. It gives a decimal value, you know, between the pixels. Like 4.2 or 5.1
            }
            int color= (int)((getnoise*128.0)+128.0); //Convert to 0-256 values.
            if(color > 255)   color = 255;
            if(color < 0)     color = 0;
            painter->setPen(QColor(color,color,color));
            painter->drawPoint(x,y);
        }
    }
}
