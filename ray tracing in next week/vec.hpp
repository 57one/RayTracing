#ifndef __VEC_H
#define __VEC_H

#include<cassert>
#include<iostream>
#include<cmath>

template<int DimRow, typename T> class Vec {
public:
    Vec() { for(int i = DimRow-1; i>=0; i--) data[i]=T(); }
    T& operator[](const int i) { assert(i<DimRow && i>=0); return data[i]; }
    const T& operator[](const int i) const { assert(i<DimRow && i>=0); return data[i]; }

    double norm() { 
        double res = 0.0;
        for(int i = 0; i<DimRow ;i++) res += data[i] * data[i];
        return std::sqrt(res); 
    }
    Vec<DimRow, T> normalize() { *this = (*this)*(1/norm()); return *this; }
private:
    T data[DimRow];
};

template<typename T> class Vec<3, T> {
public:
    Vec(): x(T()), y(T()), z(T()) { }
    Vec(T _x, T _y, T _z): x(_x), y(_y), z(_z) { }
    // template<typename U> Vec<3, T>(const Vec<3, U> &v);

    Vec<3, T> operator-() const { return Vec<3, T>(-x, -y, -z); }
    T& operator[](const int i) { assert(i<3 && i>=0); return (i==0? x : ( i==1? y : z ));}
    const T& operator[](const int i) const { assert(i<3 && i>=0); return (i==0? x : ( i==1? y : z ));}

    Vec<3, T>& operator+=(const Vec<3, T> &v){
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec<3, T>& operator*=(const T t){
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    Vec<3, T>& operator/=(const T t){
        return *this *= 1 / t;
    }

    inline static Vec<3, T> random() {
        return Vec<3, T>(random_double(), random_double(), random_double());
    }

    inline static Vec<3, T> random(double min, double max) {
        return Vec<3, T>(random_double(min,max), random_double(min,max), random_double(min,max));
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }

    double norm() const { return std::sqrt(x*x + y*y + z*z); }
    double length_squared() const { return x*x + y*y + z*z; }
    Vec<3, T> normalize() { *this = (*this)*(1/norm()); return *this; }
    T x, y, z;
};

template<int DimRow, typename T> std::ostream& operator<<(std::ostream& out, const Vec<DimRow, T> &v){
    for(int i = 0; i<DimRow; i++)
        out << v[i] << " ";
    out << std::endl;
    return out;
}

template<int DimRow, typename T> Vec<DimRow, T> operator+(Vec<DimRow, T> lhs, const Vec<DimRow, T> rhs){
    for(int i = DimRow-1; i>=0; i--)
        lhs[i] += rhs[i];
    return lhs;
}

template<int DimRow, typename T> Vec<DimRow, T> operator-(Vec<DimRow, T> lhs, const Vec<DimRow, T> rhs){
    for(int i = DimRow-1; i>=0; i--)
        lhs[i] -= rhs[i];
    return lhs;
}

//this method is not dot product
template<int DimRow, typename T> Vec<DimRow, T> operator*(Vec<DimRow, T> lhs, Vec<DimRow, T> rhs){
    for(int i = DimRow-1; i>=0; i--)
        lhs[i] = lhs[i] * rhs[i];
    return lhs;
}

template<int DimRow, typename T, typename U> Vec<DimRow, T> operator*(Vec<DimRow, T> lhs, const U rhs){
    for(int i = DimRow-1; i>=0; i--)
        lhs[i] *= rhs;
    return lhs;
}

template<int DimRow, typename T, typename U> Vec<DimRow, T> operator*(const U lhs, Vec<DimRow, T> rhs){
    return rhs * lhs;
}

template<int DimRow, typename T> Vec<DimRow, T> operator/(Vec<DimRow, T> lhs, const Vec<DimRow, T> rhs){
    for(int i = DimRow-1; i>=0; i--)
        lhs[i] /= rhs[i];
    return lhs;
}

template<int DimRow, typename T, typename U> Vec<DimRow, T> operator/(Vec<DimRow, T> lhs, U rhs){
    for(int i = DimRow-1; i>=0; i--)
        lhs[i] /= rhs;
    return lhs;
}

template<int DimRow, typename T> T dot(const Vec<DimRow, T> lhs, const Vec<DimRow, T> rhs){
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

template<typename T> Vec<3, T> cross(const Vec<3, T> &v1, const Vec<3, T> &v2){
    return Vec<3, T>(v1.y*v2.z-v2.y*v1.z, v1.z*v2.x-v1.x*v2.z, v1.x*v2.y-v1.y*v2.x);
}

template<int DimRow, typename T> Vec<DimRow, T> unit_vector(Vec<DimRow, T> v){
    return v / v.norm();
}

template<typename T> Vec<3, T> random_in_unit_sphere(){
    while (true) {
        auto p = Vec<3, T>::random(-1,1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

template<typename T> Vec<3, T> random_unit_vector() {
    return unit_vector(random_in_unit_sphere<T>());
}

template<typename T> Vec<3, T> random_in_hemisphere(const Vec<3, T>& normal) {
    Vec<3, T> in_unit_sphere = random_in_unit_sphere<T>();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

template<typename T> Vec<3,T> reflect(const Vec<3,T>& v, const Vec<3,T>  &n) {
    return v - 2*dot(v,n)*n;
}

template<typename T> Vec<3, T> refract(const Vec<3, T> &uv, const Vec<3, T> &n, double etai_over_etat){
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vec<3, T> r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    Vec<3, T> r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

template<typename T> Vec<3, T> random_in_unit_disk() {
    while (true) {
        auto p = Vec<3, double>(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }   
}

typedef Vec<3, double> Vec3d;
using Point3d = Vec3d;
using Color3d = Vec3d;

#endif