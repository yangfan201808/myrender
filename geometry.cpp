#include "geometry.h"

template <> template <> vec<3,int>  ::vec(const vec<3,float> &v) : x(int(v.x+.5f)),y(int(v.y+.5f)),z(int(v.z+.5f)) {}

template <> template <> vec<3,float>::vec(const vec<3,int> &v)   : x(v.x),y(v.y),z(v.z) {}
template <> template <> vec<2,int>  ::vec(const vec<2,float> &v) : x(int(v.x+.5f)),y(int(v.y+.5f)) {}
template <> template <> vec<2,float>::vec(const vec<2,int> &v)   : x(v.x),y(v.y) {}





TGAColor operator*(TGAColor& color, float f) {
    return TGAColor((float)color.r * f, (float)color.g * f, (float)color.b * f, 255);
};

TGAColor operator*(TGAColor& color1, TGAColor& color2) {
    return TGAColor((float)color1.r * color2.r, (float)color1.g * color2.g, (float)color1.b * color2.b, 255);
};

TGAColor operator+(TGAColor& color1, TGAColor& color2) {
    return TGAColor((float)color1.r * color2.r, (float)color1.g * color2.g, (float)color1.b * color2.b, 255);
};
TGAColor operator/(TGAColor& color1, float f) {
    return TGAColor((float)color1.r / f, (float)color1.g / f, (float)color1.b / f, 255);
};