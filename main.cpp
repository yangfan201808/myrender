#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "core.h"
//����Щ��ɫ
TGAColor WHITE(255, 255, 255, 255);
TGAColor RED(255, 0, 0, 255);
TGAColor BLUE(0, 0, 255, 255);
TGAColor GREEN(0, 255, 0, 255);

//���������ͷ����ֱ�ӳ�������ߵأ�����ᷢ��ŷ���������������ӽǷ�������緽�����Ϲ��ߣ����Ϊ0���Ҳ����ҷ�����תʧЧ��
Vec3f camraPosition(0.0f, 0.0f, 4.0f);

int width = 800, height = 800;
Vec3f lightDir = Vec3f(0.0f, 0.0f, -1.0f);





int main(int argc, char** argv) {

	TGAImage img(width, height, TGAImage::RGB);


	//Ӳ����һ������������
	/*Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
	Vec2i t1[3] = { Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180) };
	Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };*/
	//Vec2i pts[3] = { t0, t1, t2 };
	// ����������
	//triangle_barycentric(t0, img, RED);
	//triangle_barycentric(t1, img, WHITE);
	//triangle_barycentric(t2, img, GREEN);

	//����ģ��
	std::string modelstr = "obj/african_head.obj";
	Model model(modelstr.c_str());
	draw(model, img);

	//drawLinePixelStep(13, 20, 80, 40, img, WHITE);
	//drawLinePixelStep(20, 13, 40, 80, img, RED);
	//drawLinePixelStep(80, 40, 13, 20, img, RED);
	img.flip_vertically();
	img.write_tga_file("out_put.tga");
	return 0;
}