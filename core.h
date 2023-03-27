#ifndef __CORE__
#define __CORE__

#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "transformation.h"

using namespace transf;

//void clearZbuffer();

//画线算法
void drawLineOptionStep(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
void drawLinePixelStep(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);

//bresenham算法
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
void line(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor& color);
void bresenhamLine(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor& color);


//先扫描画三角形
void triangle_line(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color);
//求重心插值算法
Vec3f barycentric(Vec4f* pts, Vec2f P);
//重心插值法画三角形
void triangle_barycentric(Vec4f* pts, std::vector<float>& zBuffer, TGAImage& image);

void draw( TGAImage& img, std::vector<float>& zBuffer);



class Shader {
protected:
	mat4 Model;
	mat4 View;
	mat4 Projection;
	/*Vec2f Varying_texCoord;
	Vec3f varying_normal;*/
	mat3 normalMatrix;

public:
	virtual ~Shader() {};
	virtual Vec4f vertexShader(int iface, int nthvert) = 0;
	virtual TGAColor fragmentShader(Vec3f bar) = 0;
	void setModel(mat4& m);
	void setView(mat4& v);
	void setProjection(mat4& p);
	void setNormalMatrix(const mat3& nM);
};



//class Shader {
//private:
//	mat4 Model;
//	mat4 View;
//	mat4 projection;
//
//
//};


#endif // !__CORE__
