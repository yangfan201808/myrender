#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "core.h"
#include "PhongShader.h"

//定义些颜色
TGAColor WHITE(255, 255, 255, 255);
TGAColor RED(255, 0, 0, 255);
TGAColor BLUE(0, 0, 255, 255);
TGAColor GREEN(0, 255, 0, 255);


Model* currentModel = nullptr;
Shader* currentShader = nullptr;
//这里相机镜头不能直接冲着天或者地！否则会发生欧拉角死锁，这样视角方向和世界方向向上共线，叉乘为0，找不到右方向，旋转失效。
Vec3f cameraPosition(2.0f, 0.5f, 2.5f);
int width = 800, height = 800;
Vec3f lightDir = Vec3f(0.0f, 0.0f, 1.0f);
Vec3f lightColor = Vec3f(255, 255, 255);




class GouraudShader : public Shader {


private:
	Vec3f color[3];
	Vec3f fragColor;; // written by vertex shader, read by fragment shader

public:
	Vec4f vertexShader(int iface, int nthvert)
	{

		float intensity =  std::max(0.f, (normalMatrix * currentModel->getnormal(iface, nthvert)) * lightDir); // get diffuse lighting intensity
		TGAColor vcolor = currentModel->sampler_diffuse(currentModel->uv(iface, nthvert));
		color[nthvert] = Vec3f( intensity * vcolor.r, intensity * vcolor.g, intensity * vcolor.b);
		Vec4f gl_Vertex = embed<4>(currentModel->vert(iface, nthvert)); // read the vertex from .obj file
		return  Projection * View * Model * gl_Vertex; // transform it to screen coordinates;
	}

	TGAColor fragmentShader(Vec3f bar)
	{
		fragColor = color[0] * bar[0] + color[1] * bar[1] + color[2] * bar[2];
		return TGAColor(fragColor.x , fragColor.y , fragColor.z , 255);
		//return TGAColor(255, 255, 255, 255);
	}

int width = 800, height = 800;
Vec3f lightDir = Vec3f(0.0f, 0.0f, -1.0f);

};




int main(int argc, char** argv) {

	TGAImage img(width, height, TGAImage::RGB);


	//硬编码一个三个三角形
	/*Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
	Vec2i t1[3] = { Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180) };
	Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };*/
	//Vec2i pts[3] = { t0, t1, t2 };
	// 绘制三角形
	//triangle_barycentric(t0, img, RED);
	//triangle_barycentric(t1, img, WHITE);
	//triangle_barycentric(t2, img, GREEN);

	//加载模型
	std::string modelstr = "obj/african_head.obj";
	Model model(modelstr.c_str());
	currentModel = &model;

	//着色器设置
	GouraudShader gShader;
	mat4 modelM = mat4::identity();
	mat4 viewM = transf::lookat(cameraPosition, Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
	mat4 project = transf::perspective(45.0f, 1.0f, 1.0f, 100.0f);
	mat4 normalM_ = (viewM * modelM).invert_transpose();
	mat3 normalM = normalM_.get_minor(4, 4);

	gShader.setModel(modelM);
	gShader.setView(viewM);
	gShader.setProjection(project);
	gShader.setNormalMatrix(normalM);
	std::vector<float> zbuffer(width*height, FLT_MIN);
	currentShader = &gShader;
	draw(img, zbuffer);
	img.flip_vertically();
	img.write_tga_file("out_GouraudShading.tga");
	
	//clearZbuffer();

	PhongShader phongShader;
	currentShader = &phongShader;
	phongShader.setModel(modelM);
	phongShader.setView(viewM);
	phongShader.setProjection(project);
	
	phongShader.setNormalMatrix(normalM);
	
	std::vector<float> zbuffer2(width * height, FLT_MIN);
	TGAImage img2(width, height, TGAImage::RGB);
	draw(img2, zbuffer2);
	img2.flip_vertically();
	img2.write_tga_file("out_phongShading.tga");


	//system("pause");
	return 0;
}