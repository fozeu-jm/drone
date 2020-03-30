//
// Created by kwd on 23/01/2020.
//
#include "Vector2D.hpp"

Vector2D::Vector2D(){
    x = 0;
    y = 0;
}

Vector2D::Vector2D(const Vector2D &clone) {
    x = clone.x;
    y= clone.y;
}

Vector2D::Vector2D(float p_x,float p_y) {
    x = p_x;
    y= p_y;
}

Vector2D Vector2D::operator-(const Vector2D &op2) const {
    return Vector2D(x - op2.x, y - op2.y);
}

Vector2D Vector2D::operator+(const Vector2D &op2) const {
    return Vector2D(x + op2.x, y + op2.y);
}

float Vector2D::operator*(const Vector2D &op2) const {
    return x*op2.x+y*op2.y;
}

void Vector2D::set(float p_x,float p_y) {
    x=p_x;
    y=p_y;
}

void Vector2D::normalize() {
    float norm= std::sqrt(x*x+y*y);
    x/=norm;
    y/=norm;
}

double Vector2D::norm() {
    return std::sqrt(x*x+y*y);
}

Vector2D Vector2D::ortho() {
    return Vector2D(-y,x);
}

Vector2D Vector2D::multiply(float a){
    return Vector2D(a*x,a*y);
}

Vector2D Vector2D::multiply2(float a){
    return Vector2D(round(a*x),round(a*y));
}

Vector2D Vector2D::divide(float a){
    return Vector2D(x/a,y/a);
}



