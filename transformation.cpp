#include "transformation.h"

extern int width, height;

//角度转弧度
float transf::radians(float angle)
{
	return PI / 180 * angle;
}

//罗德里格斯旋转
mat4& transf::rotate(mat4& m, Vec3f axis, float angle)
{
	auto I = mat<3,3,float>::identity();
	axis = axis.normalize();
	angle = radians(angle);
	mat3 NM;
	NM[0] = Vec3f(0.0f, -axis.z, axis.y);
	NM[1] = Vec3f(axis.z, 0.0f, -axis.x);
	NM[2] =	Vec3f(-axis.y, axis.x, 0.0f);
	mat3 NNT;
	NNT[0] = Vec3f( axis.x * axis.x, axis.x * axis.y, axis.x * axis.z );
	NNT[1] = Vec3f( axis.y * axis.x, axis.y * axis.y, axis.y * axis.z );
	NNT[2] = Vec3f( axis.z * axis.x, axis.z * axis.y, axis.z * axis.z );
	
	auto M3 = I * std::cos(angle)  +  NNT * (1 - std::cos(angle))  +   NM * std::sin(angle);
	auto M4 = mat4::identity();
	M4.embedmat(M3);
	m = M4 * m;

	
	return m;
}

mat4& transf::translate(mat4& m, Vec3f axis)
{
	mat4 translate = mat4::identity();
	for (int i = 0; i < 3; i++) {
		translate[3][i] = axis[i];
	}
	m = translate * m;
	
	return m;
}

mat4& transf::scale(mat4& m, Vec3f axis)
{
	mat4 rotate = mat4::identity();
	for (int i = 0; i < 3; i++) {
		rotate[i][i] = axis[i];
	}
	m = rotate * m;
	return m;
}


mat4 transf::viewMatrix(Vec3f position, Vec3f lookPoistion, Vec3f worldUp)
{
	mat4 trans = mat4::identity();
	for (int i = 0; i < 3; i++) {
		trans[i][3] = -position[i];
	}
	Vec3f front = (lookPoistion - position).normalize();
	
	Vec3f right = cross(front , worldUp).normalize();
	Vec3f up = cross(right , front).normalize();
	mat3 rot = mat3::identity();
	for (int i = 0; i < 3; i++) {
		rot[0][i] = right[i];
		rot[1][i] = up[i];
		rot[2][i] = -front[i];
	}
	/*std::cout << "rot:\n" << rot << std::endl;
	system("pause");*/
	mat4 rotate = mat4::identity();
	rotate.embedmat(rot);
	mat4 result = rotate * trans;
	
	return result;
}

mat4 transf::perspective(float fov, float aspect, float near, float far)
{

	//注意near，far代表近、远平面与视点距离，near_view, far_view代表在view空间近远平面z轴坐标
	float near_view = -near;
	float far_view = -far;
	float theta = radians(fov / 2);
	float top = near * tan(theta);
	float bottom = -top;
	float right = top * aspect;
	float left = -right;


	mat4 orth = orthoProjection(left, right, top, bottom, near, far);
	//std::cout << "ortho:\n" << orth << std::endl;
	mat4 per2orth ;
	per2orth[0][0] = near_view;
	per2orth[1][1] = near_view;
	per2orth[2][2] = near_view + far_view;
	per2orth[2][3] = -near_view * far_view;
	per2orth[3][2] = 1.0f;
	
	//std::cout << "per2ortho:\n" << per2orth << std::endl;

	return orth * per2orth;
}

mat4 transf::orthoProjection(float left, float right, float top, float bottom, float near, float far)
{

	near = -near;
	far = -far;
	mat4 translate = mat4::identity();
	translate[0][3] = -((left + right) / 2.0f);
	translate[1][3] = -((top + bottom) / 2.0f);
	translate[2][3] = -((near + far) / 2.0f);

	mat4 scale = mat4::identity();
	scale[0][0] = 2 / (right - left);
	scale[1][1] = 2 / (top - bottom);
	scale[2][2] = 2 / (near - far);

	return scale * translate;

}

mat4 transf::getViewPortM()
{
	mat4 viewportM = mat4::identity();
	viewportM[0] = Vec4f(width / 2.0f, 0.0f, 0.0f, width / 2.0f);
	viewportM[1] = Vec4f(0.0f, height / 2.0f, 0.0f, height / 2.0f);
	viewportM[2] = Vec4f(0.0f, 0.0f, 1.0f / 2.0f, 1.0f);
	viewportM[3] = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	return viewportM;
}


