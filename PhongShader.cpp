#include "PhongShader.h"


extern Model* currentModel;
extern Shader* currentShader;
extern Vec3f lightDir;
extern Vec3f cameraPosition;


Vec4f PhongShader::vertexShader(int iface, int nthvert)
{
    varying_position[nthvert] = currentModel->vert(iface, nthvert);
    Vec4f aPosition = embed<4>(varying_position[nthvert]);
    TBN = currentModel->caculateTBN(iface, this->Model);
    //normals[nthvert] = (normalMatrix *  currentModel->getnormal(iface, nthvert)).normalize();
    varying_uvs[nthvert] = currentModel->uv(iface, nthvert);

    return Projection * View * Model * aPosition;
}

TGAColor PhongShader::fragmentShader(Vec3f bar)
{
    //Vec3f normal_inter = (normals[0] * bar[0] + normals[1] * bar[1] + normals[2] * bar[2]).normalize();
   //插值uv，实际上在opengl中会为我们插值好，但是这里我们还是自己做吧
    Vec3f fragPos = varying_position[0] * bar[0] + varying_position[1] * bar[1] + varying_position[2] * bar[2];
    Vec2f uv_inter = varying_uvs[0] * bar[0] + varying_uvs[1] * bar[1] + varying_uvs[2] * bar[2];
    //由插值纹理坐标采样得到法线
    Vec3f normal_frag =  currentModel->sampler_normal(uv_inter);
    //采样得到镜面反射系数
    material.specular = currentModel->sampler_specular(uv_inter);
    //采样得到反照率
    material.diffuse = currentModel->sampler_diffuse(uv_inter);
    //计算漫反射
    TGAColor diffuse_color =  material.diffuse * std::max(0.0f, (normal_frag * lightDir)) * 0.8f;
    Vec3f diff_color = Vec3f(diffuse_color.r, diffuse_color.g, diffuse_color.b);
    //caculate specular
    Vec3f half = ((cameraPosition- fragPos) + lightDir).normalize();
    Vec3f specular_color_ = light.light_specular * material.specular * std::pow(std::max(0.0f, half * normal_frag), material.shinny) * 255.0f;
    Vec3f result = diff_color + specular_color_;
    //std::cout << result.r << "  " << result.g << "  " << result.b << std::endl;
    return TGAColor(result.x, result.y, result.z, 255);
}

void PhongShader::setNormalMatrix(const mat3& nM)
{
    this->normalMatrix = nM;
}

void PhongShader::setLight(Light& l)
{
    this->light = l;
}

