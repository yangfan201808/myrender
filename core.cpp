#include "core.h"


extern Vec3f camraPosition;
extern int width, height;
extern Vec3f lightDir;
extern Model* currentModel;
extern Shader* currentShader;






//画线算法
void drawLineOptionStep(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color) {
	for (float t = 0.0f; t < 1.0f; t += 0.01f) {
		int x = x0 + (x1 - x0) * t;
		int y = y0 + (y1 - y0) * t;
		image.set(x, y, color);
	}
}
void drawLinePixelStep(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color) {
	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 + (y1 - y0) * t;
		image.set(x, y, color);
	}
}
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color) {
	bool steep = false;
	//判断谁的变化率更小，选取他作为参考轴
	if (abs(x1 - x0) < abs(y1 - y0)) {
		steep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	//保证在参考轴方向上x0 > x1
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	float distant = (float)(x1 - x0);
	float derror = abs((y1 - y0) / (float)(x1 - x0));
	float error = 0.0f;
	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / distant;
		int y = y0 + t * (y1 - y0);
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error += derror;
		if (error > 0.5f) {
			y += (y1 > y0 ? 1 : -1);
			error -= 1.;
		}
	}
}
void line(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor& color) {
	bool steep = false;
	int x0 = t0.x, x1 = t1.x;
	int y0 = t0.y, y1 = t1.y;
	if (abs(x1 - x0) < abs(y1 - y0)) {
		steep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	float distant = (float)(x1 - x0);
	float derror = abs((y1 - y0) / (float)(x1 - x0));
	float error = 0.0f;
	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / distant;
		int y = y0 + t * (y1 - y0);
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error += derror;
		if (error > 0.5f) {
			y += (y1 > y0 ? 1 : -1);
			error -= 1.;
		}
	}
}
void bresenhamLine(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor& color) {
	bool steep = false;
	int x0 = t0.x, x1 = t1.x;
	int y0 = t0.y, y1 = t1.y;
	if (abs(x1 - x0) < abs(y1 - y0)) {
		steep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror = 2 * abs(dy);
	int error = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error += derror;
		if (error > dx) {
			y += (y1 > y0 ? 1 : -1);
			error -= 2 * dx;
		}
	}
}

//该方法为线扫描画线法，不常用,因为单线程，并不好设计并行
//该函数待优化，代码重复
void triangle_line(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);
	int totall_height = t2.y - t0.y;
	int segment_height = t1.y - t0.y;
	for (int y = t0.y; y <= t1.y; y++) {
		float totallinter = (y - t0.y) / (float)(totall_height);
		float segmentinter = (y - t0.y) / (float)(segment_height);
		Vec2i x0 = t0 + (t2 - t0) * totallinter;
		Vec2i x1 = t0 + (t1 - t0) * segmentinter;
		/*	image.set(x0.x, x0.y, RED);
			image.set(x1.x, x1.y, GREEN);*/
			/*line(x0, x1, image, WHITE);*/
		if (x0.x > x1.x) std::swap(x0, x1);
		for (int i = x0.x; i <= x1.x; i++) {
			image.set(i, y, color);
		}
	}
	int segment_upperheight = t2.y - t1.y;
	for (int y = t1.y; y <= t2.y; y++) {
		float alpha = (y - t1.y) / (float)segment_upperheight;
		float beta = (y - t0.y) / (float)totall_height;
		Vec2i A = t1 + (t2 - t1) * alpha;
		Vec2i B = t0 + (t2 - t0) * beta;
		if (A.x > B.x) std::swap(A, B);
		for (int i = A.x; i <= B.x; i++) {
			image.set(i, y, color);
		}
	}

	/*bresenhamLine(t0, t1, image, GREEN);
	bresenhamLine(t1, t2, image, GREEN);
	bresenhamLine(t2, t0, image, RED);*/
}

//向量叉乘算法
void triangle_crossproduct() {}


//实际计算机会用的算法：重心插值判断 ,其实这样单纯用中心插值来填充三角形实在有点大材小用，但是，我们获取到的重心坐标可不仅仅会用来判断点是否在三角形内
	if (std::abs(v.z) < 1) return Vec3f(-1, 1, 1);
	return Vec3f(1.0f - (v.x + v.y) / (float)v.z, v.x / (float)v.z, v.y / (float)v.z);
}

	//计算bounding box
	Vec2i bbox_min = Vec2i(width - 1, height - 1);
	Vec2i bbox_max = Vec2i(0, 0);
	for (int i = 0; i < 3; i++) {
		try {
			if (bbox_min.x < 0 || bbox_min.x > width - 1 || bbox_min.y < 0 || bbox_min.y > height - 1 || bbox_max.x < 0 || bbox_max.x > width - 1 || bbox_max.y < 0 || bbox_max.y > height - 1) {
				throw "bbox error!";
			}
		}
		catch (std::string s) {
			std::cout << s << std::endl;
		}

	}

	//遍历包围盒，判定在三角内的被着色
	for (int i = bbox_min.x; i <= bbox_max.x; i++) {
		for (int j = bbox_min.y; j <= bbox_max.y; j++) {
			if (baryc.x >= 0 && baryc.y >= 0 && baryc.z >= 0) {
				if (depth > zBuffer[i + j * width - 1]) {
					auto pixColor = model.diffuse(pixeluv);
					image.set(i, j, pixColor);
					zBuffer[i + j * width - 1] = depth;
				}
			}
		}
	}


}

//画一个模型
void draw(TGAImage& img, std::vector<float>& zBuffer) {

void draw(Model& model, TGAImage& img) {
	std::vector<int> zBuffer(width * height, INT32_MIN);
	mat4 modelM = mat4::identity();
	mat4 viewM = transf::viewMatrix(camraPosition, Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
	mat4 projectionM = transf::perspective(45.0, 1.0f, 1.0, 100);
	auto transform_matrix = projectionM * viewM * modelM;
	mat4 viewportM = transf::getViewPortM();

			}
		}


		}

void Shader::setView(mat4& v)
{
	for (int i = 0; i < 4; i++) {
		View[i] = v[i];
	}
}

void Shader::setProjection(mat4& p)
{
	for (int i = 0; i < 4; i++) {
		Projection[i] = p[i];
	}
	}

void Shader::setNormalMatrix(const mat3& nM)
{
	this->normalMatrix = nM;
}


