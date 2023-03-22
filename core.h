#ifndef __CORE__
#define __CORE__

#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "transformation.h"

using namespace transf;

//�����㷨
void drawLineOptionStep(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
void drawLinePixelStep(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);

//bresenham�㷨
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
void line(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor& color);
void bresenhamLine(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor& color);


//��ɨ�軭������
void triangle_line(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color);
//�����Ĳ�ֵ�㷨
Vec3f barycentric(Vec3i* pts, Vec2i P);
//���Ĳ�ֵ����������
void triangle_barycentric(Vec3i* pts, Vec2i* uvs, std::vector<int>& zBuffer, TGAImage& image, Model& model, const TGAColor& color);

void draw(Model& model, TGAImage& img);



//class Shader {
//private:
//	mat4 Model;
//	mat4 View;
//	mat4 projection;
//
//
//};


#endif // !__CORE__
