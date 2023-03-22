#ifndef __SHADER__
#define __SHADER__


#include "core.h"



class PhongShader :
    public Shader
{
    struct Light {
        Vec3f light_diffuse = Vec3f(0.8f, 0.8f, 0.8f);
        Vec3f light_specular = Vec3f(0.4f, 0.4f, 0.4f);
        Vec3f light_ambient = Vec3f(0.2f, 0.2f, 0.2f);
        
    } light;
    
    struct Material {
        TGAColor diffuse;
        float specular;
        float shinny = 32.f;
    } material;

    Vec3f varying_position[3];
    Vec2f varying_uvs[3];
    Vec3f varying_normals[3];
    mat3 normalMatrix;
    mat3 TBN;
public:
    Vec4f vertexShader(int iface, int nthvert);
    TGAColor fragmentShader(Vec3f bar);
    void setNormalMatrix(const mat3& nM);
    void setLight(Light& l);
};



#endif // !__SHADER__



