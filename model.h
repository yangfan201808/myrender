#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"
#include "tgaimage.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<Vec3i>> faces_; // attention, this Vec3i means vertex/uv/normal
	std::vector<Vec3f> norms_;
	std::vector<Vec2f> uv_;
	TGAImage diffusemap_;
	TGAImage specularmap_;
	TGAImage normalmap_;
	void load_texture(std::string filename, const char *suffix, TGAImage &img);
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	Vec2f uv(int iface, int nvert);
	Vec3f getnormal(int iface, int nvert);
	TGAColor sampler_diffuse(Vec2f uv);
	float sampler_specular(Vec2f uv);
	Vec3f sampler_normal(Vec2f uv);
	std::vector<int> face(int idx);
	Vec3f Model::vert(int iface, int nthvert);
	mat3 caculateTBN(int iface, mat4& modeView);
};

#endif //__MODEL_H__
