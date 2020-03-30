//
// Created by kwd on 23/01/2020.
//

#ifndef DRONE_PROJECT_VECTOR2D_HPP
#define DRONE_PROJECT_VECTOR2D_HPP

#include <cmath>
class Vector2D{
public:
    float x,y;
    Vector2D();

    Vector2D(const Vector2D &clone);

    Vector2D(float p_x,float p_y);

    Vector2D operator-(const Vector2D &op2) const;

    Vector2D operator+(const Vector2D &op2) const;

    float operator*(const Vector2D &op2) const;

    void set(float p_x,float p_y);

    void normalize();

    double norm();

    Vector2D multiply2(float a);

    Vector2D ortho();
    Vector2D multiply(float a);
    Vector2D divide(float a);

};

#endif //DRONE_PROJECT_VECTOR2D_HPP
