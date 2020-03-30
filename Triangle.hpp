//
// Created by kwd on 23/01/2020.
//

#ifndef DRONE_PROJECT_TRIANGLE_HPP
#define DRONE_PROJECT_TRIANGLE_HPP

#include "determinant.h"
#include "Vector2D.hpp"
#include <glutWindow.h>
#include <vector>

const float BLACK[4]={0.0, 0.0, 0.0, 1.0f};
const float GREY[4]={0.75f, 0.75f, 0.75f, 1.0f};
const float RED[4]={1.0f, 0.0, 0.0, 1.0f};
const float ORANGE[4]={1.0f, 0.27f, 0.0, 1.0f};
const float YELLOW[4]={1.0f, 1.0f, 0.0, 1.0f};
const float GREEN[4]={0.0, 1.0f, 0.0, 1.0f};
const float BLUE[4]={0.0, 0.0, 1.0f, 1.0f};
const float CYAN2[3]={79, 244, 255 };
const float PINK2[3]={243, 131, 173 };
const float BLUE2[3]={17, 102, 233 };
const float GREEN2[3]={49, 254, 8 };
const float ORANGE2[3]={255,127,80};
const float YELLOW2[3]={248, 252, 9};
const float RED2[3]={255, 66, 15  };
const float PURPLE2[3]={128,0,128};
const float MAGENTA2[3]={202, 31, 123};
const float GRAY2[3]={192,192,192};
const float BROWN2[3]={150,75,0};

class Triangle {
public:
    Vector2D *ptr[3];
    bool is_highlighted_;
    Vector2D circum_center_;
    float circum_radius_;
    bool is_delaunay_;

    Triangle(Vector2D *ptr1,Vector2D *ptr2,Vector2D *ptr3);

    void updateVertices(Vector2D *ptr1,Vector2D *ptr2,Vector2D *ptr3);

    void calculateCircle();

    void onMouseMove(const Vector2D& pos);

    bool is_on_the_left(const Vector2D *P0, const Vector2D *P1, const Vector2D *P2);

    bool isEmpty(const std::vector<Vector2D*> &tmp,int n);

    bool is_inside(const Vector2D &P);

    bool is_inside2( Vector2D &P);

    bool is_inside3( Vector2D &P);

    float  area();

    bool is_inside_circle(const Vector2D &P);

    bool check_delaunay(const vector<Vector2D> &points);

    void draw();

    void drawCircle();

    bool operator!=(const Triangle &tria) ;

    Vector2D* getVertexNotIn(Triangle triangle);

    Vector2D* getNextVertex(Vector2D *vector);

    Vector2D* getPrevVertex(Vector2D *vector);

    bool operator==(const Triangle &tria);
};
#endif //DRONE_PROJECT_TRIANGLE_HPP
