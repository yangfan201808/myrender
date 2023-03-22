#ifndef __TRANSFORMATION__
#define __TRANSFORMATION__
#define PI 3.14159265

#include "geometry.h"
#include <vector>

namespace transf {
	
	

	float radians(float angle);


	//罗德里格斯旋转公式
	mat4& rotate(mat4& m, Vec3f axis, float angle);

	mat4& translate(mat4& m, Vec3f axis);
	mat4& scale(mat4& m, Vec3f axis);

	mat4 viewMatrix(Vec3f position, Vec3f lookPoistion, Vec3f worldUp);

	mat4 perspective(float fov, float aspect, float near, float far);

	mat4 orthoProjection(float left, float right, float top, float bottom, float near, float far);


	mat4 getViewPortM();
		

}


#endif // !__TRANSFORMATION__

