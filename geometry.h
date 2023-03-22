#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__
#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>
#include <string>
#include "tgaimage.h"



template<size_t DimCols,size_t DimRows,typename T> class mat;

template <size_t DIM, typename T> struct vec {
    vec() { for (size_t i=DIM; i--; data_[i] = T()); }
          T& operator[](const size_t i)       { assert(i<DIM); return data_[i]; }
    const T& operator[](const size_t i) const { assert(i<DIM); return data_[i]; }
private:
    T data_[DIM];
};

/////////////////////////////////////////////////////////////////////////////////

template <typename T> struct vec<2,T> {
    vec() : x(T()), y(T()) {}
    vec(T X, T Y) : x(X), y(Y) {}
    template <class U> vec<2,T>(const vec<2,U> &v);
          T& operator[](const size_t i)       { assert(i<2); return i<=0 ? x : y; }
    const T& operator[](const size_t i) const { assert(i<2); return i<=0 ? x : y; }

    T x,y;
};

/////////////////////////////////////////////////////////////////////////////////

template <typename T> struct vec<3,T> {
    vec() : x(T()), y(T()), z(T()) {}
    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    template <class U> vec<3,T>(const vec<3,U> &v);
    vec(const TGAColor& color) : x(color.r), y(color.g), z(color.b) {}
          T& operator[](const size_t i)       { assert(i<3); return i<=0 ? x : (1==i ? y : z); }
    const T& operator[](const size_t i) const { assert(i<3); return i<=0 ? x : (1==i ? y : z); }
    float norm() { return std::sqrt(x*x+y*y+z*z); }
    vec<3,T> & normalize(T l=1) { *this = (*this)*(l/norm()); return *this; }

    T x,y,z;
};


template <typename T> struct vec<4,T> {
    vec() : x(T()), y(T()), z(T()), w(T()){}
    vec(vec<4, T>& v) :x(v.x), y(v.y), z(v.z), w(v.w) {};

    vec(T _x, T _y, T _z, T _w) :x(_x), y(_y), z(_z), w(_w) {}
    vec(vec<3, T>& v, T _w) :x(v.x), y(v.y), z(v.z), w(_w) {}
    T& operator[](const size_t i) { assert(i < 4); return i <= 0 ? x : (1 == i ? y : ( i == 2 ? z : w)); }
    const T& operator[](const size_t i) const { assert(i < 4); return i <= 0 ? x : (1 == i ? y : (i == 2 ? z : w)); }


    T x, y, z, w;
};


TGAColor operator*(TGAColor& color, float f);

/////////////////////////////////////////////////////////////////////////////////

template<size_t DIM,typename T> T operator*(const vec<DIM,T>& lhs, const vec<DIM,T>& rhs) {
    T ret = T();
    for (size_t i=DIM; i--; ret+=lhs[i]*rhs[i]);
    return ret;
}


template<size_t DIM,typename T>vec<DIM,T> operator+(vec<DIM,T> lhs, const vec<DIM,T>& rhs) {
    for (size_t i=DIM; i--; lhs[i]+=rhs[i]);
    return lhs;
}

template<size_t DIM,typename T>vec<DIM,T> operator-(vec<DIM,T> lhs, const vec<DIM,T>& rhs) {
    for (size_t i=DIM; i--; lhs[i]-=rhs[i]);
    return lhs;
}

template<size_t DIM, typename T>vec<DIM, T> operator-(vec<DIM, T> lhs, const float& rhs) {
    for (size_t i = DIM; i--; lhs[i] -= rhs);
    return lhs;
}



template<size_t DIM,typename T,typename U> vec<DIM,T> operator*(vec<DIM,T> lhs, const U& rhs) {
    for (size_t i=DIM; i--; lhs[i]*=rhs);
    return lhs;
}

template<size_t DIM,typename T,typename U> vec<DIM,T> operator/(vec<DIM,T> lhs, const U& rhs) {
    for (size_t i=DIM; i--; lhs[i]/=rhs);
    return lhs;
}

/// 定义了一个模板函数embed，将低维度的向量嵌入到高维度空间中，并填充默认值fill 
/// @tparam LEN 目标向量的维度
/// @tparam DIM 源向量的维度
/// @tparam T 向量元素的类型
/// @param v 源向量
/// @param fill 填充值，默认为1
/// @return 返回一个长度为LEN类型为T的新向量ret，其前DIM个元素与v相同，其余元素为fill 
template<size_t LEN,size_t DIM,typename T> vec<LEN,T> embed(const vec<DIM,T> &v, T fill=1) {
    vec<LEN,T> ret;
    for (size_t i=LEN; i--; ret[i]=(i<DIM?v[i]:fill));
    return ret;
}


template<size_t LEN,size_t DIM, typename T> vec<LEN,T> proj(const vec<DIM,T> &v) {
    vec<LEN,T> ret;
    for (size_t i=LEN; i--; ret[i]=v[i]);
    return ret;
}

template <typename T> vec<3,T> cross(vec<3,T> v1, vec<3,T> v2) {
    return vec<3,T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

template <size_t DIM, typename T> std::ostream& operator<<(std::ostream& out, vec<DIM,T>& v) {
    for(unsigned int i=0; i<DIM; i++) {
        out << v[i] << " " ;
    }
    return out ;
}

/////////////////////////////////////////////////////////////////////////////////

template<size_t DIM,typename T> struct dt {
    static T det(const mat<DIM,DIM,T>& src) {
        T ret=0;
        for (size_t i=DIM; i--; ret += src[0][i]*src.cofactor(0,i));
        return ret;
    }
};

template<typename T> struct dt<1,T> {
    static T det(const mat<1,1,T>& src) {
        return src[0][0];
    }
};



///////////////////////////////////定义矩阵MAT//////////////////////////////////////////////

template<size_t DimRows,size_t DimCols,typename T> class mat {
    vec<DimCols,T> rows[DimRows];
public:
    mat() {}


  

    vec<DimCols,T>& operator[] (const size_t idx) {
        assert(idx<DimRows);
        return rows[idx];
    }

    const vec<DimCols,T>& operator[] (const size_t idx) const {
        assert(idx<DimRows);
        return rows[idx];
    }

    vec<DimRows,T> col(const size_t idx) const {
        assert(idx<DimCols);
        vec<DimRows,T> ret;
        for (size_t i=DimRows; i--; ret[i]=rows[i][idx]);
        return ret;
    }

    void set_col(size_t idx, vec<DimRows,T> v) {
        assert(idx<DimCols);
        for (size_t i=DimRows; i--; rows[i][idx]=v[i]);
    }

    static mat<DimRows,DimCols,T> identity() {
        mat<DimRows,DimCols,T> ret;
        for (size_t i=DimRows; i--; )
            for (size_t j=DimCols;j--; ret[i][j]=(i==j));
        return ret;
    }

    template <size_t r, size_t c>
    void embedmat(mat<r, c, T>& m ) {
        try {
            if (r > DimRows || c > DimCols) {
                throw "erro when doing matembed!!";
            }
        }
        catch (const std::string a) {
            std::cout << a << std::endl;
        }

        for (size_t i = 0; i < r; i++) {
            for (size_t j = 0; j < c; j++) {
                this->rows[i][j] = m[i][j];
            }
        }
    }







    T det() const {
        return dt<DimCols,T>::det(*this);
    }
    ////矩阵的降维
    //mat<DimRows - 1, DimCols - 1, T> get_little() const {
    //    mat<DimRows - 1, DimCols - 1, T> ret;
    //    for (size_t i = DimRows - 1; i--; )
    //        for (size_t j = DimCols - 1; j--; ret[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1]);
    //    return ret;
    //}
    //求(row, col)的余子式
    mat<DimRows-1,DimCols-1,T> get_minor(size_t row, size_t col) const {
        mat<DimRows-1,DimCols-1,T> ret;
        for (size_t i=DimRows-1; i--; )
            for (size_t j=DimCols-1;j--; ret[i][j]=rows[i<row?i:i+1][j<col?j:j+1]);
        return ret;
    }

    //求(row,col)的代数余子式
    T cofactor(size_t row, size_t col) const {
        return get_minor(row,col).det()*((row+col)%2 ? -1 : 1);
    }

    //求伴随矩阵的转置
    mat<DimRows,DimCols,T> adjugate() const {
        mat<DimRows,DimCols,T> ret;
        for (size_t i=DimRows; i--; )
            for (size_t j=DimCols; j--; ret[i][j]=cofactor(i,j));
        return ret;
    }

    //求矩阵的逆转置
    mat<DimRows,DimCols,T> invert_transpose() {
        mat<DimRows,DimCols,T> ret = adjugate();
        T tmp = ret[0]*rows[0];
        return ret/tmp;
    }
};

/////////////////////////////////////////////////////////////////////////////////


template<size_t DimRows, size_t DimCols, typename T> mat<DimRows, DimCols, T> operator+(const mat<DimRows, DimCols, T>& lhs, const mat<DimRows, DimCols, T>& rhs) {
    mat<DimRows, DimCols, T> result;
    for (int i = 0; i < DimRows; i++) {
        result[i] = lhs[i] + rhs[i];
    }
    return result;
}



template<size_t DimRows,size_t DimCols,typename T> vec<DimRows,T> operator*(const mat<DimRows,DimCols,T>& lhs, const vec<DimCols,T>& rhs) {
    vec<DimRows,T> ret;
    for (size_t i=DimRows; i--; ret[i]=lhs[i]*rhs);
    return ret;
}


template<size_t R1,size_t C1,size_t C2,typename T>mat<R1,C2,T> operator*(const mat<R1,C1,T>& lhs, const mat<C1,C2,T>& rhs) {
    mat<R1,C2,T> result;
    for (size_t i=R1; i--; )
        for (size_t j=C2; j--; result[i][j]=lhs[i]*rhs.col(j));
    return result;
}

template<size_t DimRows, size_t DimCols, typename T>mat<DimRows, DimCols, T> operator*(const mat<DimRows, DimCols, T>& lhs, const T& rhs) {
    mat<DimRows, DimCols, T> result;
    for (int i = 0; i < DimRows; i++) {
        result[i] = lhs[i] * rhs;
    }
    return result;
}


template<size_t DimRows,size_t DimCols,typename T>mat<DimCols,DimRows,T> operator/(mat<DimRows,DimCols,T> lhs, const T& rhs) {
    for (size_t i=DimRows; i--; lhs[i]=lhs[i]/rhs);
    return lhs;
}

template <size_t DimRows,size_t DimCols,class T> std::ostream& operator<<(std::ostream& out, mat<DimRows,DimCols,T>& m) {
    for (size_t i=0; i<DimRows; i++) out << m[i] << std::endl;
    return out;
}

TGAColor operator*(TGAColor& color, float f);
TGAColor operator+(TGAColor& color1, TGAColor& color2);

/////////////////////////////////////////////////////////////////////////////////

typedef vec<2,  float> Vec2f;
typedef vec<2,  int>   Vec2i;
typedef vec<3,  float> Vec3f;
typedef vec<3,  int>   Vec3i;
typedef vec<4,  float> Vec4f;
typedef mat<4,4,float> mat4;
typedef mat<3, 3, float> mat3;



#endif //__GEOMETRY_H__

