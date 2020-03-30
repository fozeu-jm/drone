//
// Created by kwd on 23/01/2020.
//
#include "Triangle.hpp"

Triangle::Triangle(Vector2D *ptr1, Vector2D *ptr2,Vector2D *ptr3){
    ptr[0] = ptr1;
    ptr[1] = ptr2;
    ptr[2] = ptr3;
    is_highlighted_ = false;
    calculateCircle();
}

void Triangle::updateVertices(Vector2D *ptr1,Vector2D *ptr2,Vector2D *ptr3){
    ptr[0] = ptr1;
    ptr[1] = ptr2;
    ptr[2] = ptr3;
    is_highlighted_ = false;
    calculateCircle();
}

void Triangle::calculateCircle(){
    // calculate circle parameters
    // AB = B - A
    Vector2D AB = *ptr[1] - *ptr[0];
    Vector2D AC = *ptr[2] - *ptr[0];
    Vector2D VAC = AC.ortho();

    float k = (AB * AB - AC * AB) / (2 * ((VAC * AB )));

    circum_center_ = AC.multiply(0.5f)  + (VAC.multiply(k)) + *ptr[0]; // O + 0.5AC + K * VAC + A
    circum_radius_ = (circum_center_ - *ptr[0]).norm(); // R = AO = BO = CO
}

void Triangle::onMouseMove(const Vector2D& pos)
{
    is_highlighted_ = is_inside(pos);
}

bool Triangle::is_on_the_left(const Vector2D *P0, const Vector2D *P1, const Vector2D *P2){
    Vector2D AB = *P2-*P1, AP = *P0 - *P1;

    return (AB.x*AP.y - AB.y*AP.x) >= 0;
}

bool Triangle::isEmpty(const std::vector<Vector2D*> &tmp,int n){
    auto p = tmp.begin() + n;

    while (p!=tmp.end() && (!is_on_the_left(*p, ptr[0], ptr[1]) || !is_on_the_left(*p, ptr[1], ptr[2]) || !is_on_the_left( *p, ptr[2] , ptr[0]))) {
        p++;
    }

    return p == tmp.end();
}

bool Triangle::is_inside(const Vector2D &P){
    return is_on_the_left(&P, ptr[0], ptr[1]) && is_on_the_left(&P, ptr[1], ptr[2]) && is_on_the_left(&P, ptr[2], ptr[0]);
}

bool Triangle::is_inside_circle(const Vector2D &P){
    Matrix33 mat33;

    mat33.m[0][0] = ptr[0]->x - P.x;
    mat33.m[0][1] = ptr[0]->y - P.y;
    mat33.m[0][2] = ((ptr[0]->x * ptr[0]->x) - (P.x * P.x)) + ((ptr[0])->y * ptr[0]->y) - (P.y * P.y);

    mat33.m[1][0] = ptr[1]->x - P.x;
    mat33.m[1][1] = ptr[1]->y - P.y;
    mat33.m[1][2] = ((ptr[1]->x * ptr[1]->x) - (P.x * P.x)) + ((ptr[1]->y * ptr[1]->y) - (P.y * P.y));

    mat33.m[2][0] = ptr[2]->x - P.x;
    mat33.m[2][1] = ptr[2]->y - P.y;
    mat33.m[2][2] = ((ptr[2]->x * ptr[2]->x) - (P.x * P.x)) + ((ptr[2]->y * ptr[2]->y) - (P.y * P.y));

    return mat33.determinant() > 0;
}

bool Triangle::check_delaunay(const vector<Vector2D> &points) {
    auto p = points.begin();

    while(p != points.end() && !is_inside_circle(*p))
    {
        p++;
    }

    is_delaunay_ = p == points.end();
    return is_delaunay_;
}

void Triangle::draw() {
    glColor3fv(is_delaunay_ ? (is_highlighted_ ? GREEN : GREY) : (is_highlighted_ ? ORANGE : YELLOW));

    // Draw the triangle.
    glBegin(GL_TRIANGLES);
    glVertex2f(ptr[0]->x,ptr[0]->y);
    glVertex2f(ptr[1]->x,ptr[1]->y);
    glVertex2f(ptr[2]->x,ptr[2]->y);
    glEnd();

    // Draw the borders.
    glColor3fv(BLACK);
    glBegin(GL_LINE_LOOP);
    glVertex2f(ptr[0]->x,ptr[0]->y);
    glVertex2f(ptr[1]->x,ptr[1]->y);
    glVertex2f(ptr[2]->x,ptr[2]->y);
    glEnd();
}

void Triangle::drawCircle() {
    // draw circle
    if (is_highlighted_) {
        glColor3fv(RED);
        glPushMatrix();
        glTranslatef(circum_center_.x, circum_center_.y, 0);
        glLineWidth(3);
        glBegin(GL_LINE_LOOP);
        float a=0.0;
        for (int i=0; i<40; i++) {
            glVertex2f(circum_radius_ * cos(a), circum_radius_ * sin(a));
            a+=(float)(M_PI/20.0);
        }
        glEnd();
        glLineWidth(1);
        glPopMatrix();
    }


}

float Triangle::area() {
    return abs( (ptr[0]->x * (ptr[1]->y - ptr[2]->y) + ptr[1]->x * (ptr[2]->y - ptr[0]->y) + ptr[2]->x * (ptr[0]->y - ptr[1]->y) ) / 2.0 );
}

bool Triangle::is_inside2(Vector2D &P) {
    int inside = 0;
    for(auto pts : ptr){
        if(pts->x == P.x && pts->y == P.y){
            inside = 1;
        }
    }

    if(inside != 1){
        float A = area();

        Triangle A1(ptr[0],ptr[1], &P);

        Triangle A2(ptr[1],ptr[2],&P);

        Triangle A3(ptr[0],ptr[2], &P);

        float a1 = A1.area();
        float a2 = A2.area();
        float a3 = A3.area();
        //cout<<"Area of A1 + A2 + A3 is "<< a1+a2+a3 <<"And Area of Big A is "<<A;
        return (A == a1+a2+a3);
    }else{
        return false;
    }

}

bool Triangle::is_inside3(Vector2D &P) {
        float A = area();

        Triangle A1(ptr[0],ptr[1], &P);

        Triangle A2(ptr[1],ptr[2],&P);

        Triangle A3(ptr[0],ptr[2], &P);

        float a1 = A1.area();
        float a2 = A2.area();
        float a3 = A3.area();
        //cout<<"Area of A1 + A2 + A3 is "<< a1+a2+a3 <<"And Area of Big A is "<<A;
        return (A == a1+a2+a3);
}

bool Triangle::operator==(const Triangle &tria){
    int test = 0;
    for(auto pnt : tria.ptr){
        if(is_inside3(*pnt)){
            //cout<<"hayaa"<<pnt->x<<endl;
            test++;
        }
    }
    if(test >= 3){
        return true;
    }else{
        return false;
    }
}

bool Triangle::operator != (const Triangle &tria){
    int test = 0;
        for(auto pnt : tria.ptr){
            if(is_inside3(*pnt)){
                //cout<<"hayaa"<<pnt->x<<endl;
                test++;
            }
        }
        if(test < 3){
            return true;
        }else{
            return false;
        }
}

Vector2D* Triangle::getVertexNotIn(Triangle triangle){
    bool a,b,c,d,e,f;

    for(int i = 0; i<3; i++){
        a = ptr[i]->x != triangle.ptr[0]->x;
        b = ptr[i]->y != triangle.ptr[0]->y;

        c = ptr[i]->x != triangle.ptr[1]->x;
        d = ptr[i]->y != triangle.ptr[1]->y;

        e = ptr[i]->x != triangle.ptr[2]->x;
        f = ptr[i]->y != triangle.ptr[2]->y;

        if(a && b && c && d && e && f){
            return ptr[i];
        }
    }

}

Vector2D* Triangle::getNextVertex(Vector2D *vector ){
    for(int i = 0; i<3; i++){
        bool a = ptr[i]->x == vector->x && ptr[i]->y == vector->y;
        if(a){
            int j = i+1;
            return (ptr[j%3]);
        }
    }
}

Vector2D* Triangle::getPrevVertex(Vector2D *vector ){
    for(int i = 0; i<3; i++){
        bool a = ptr[i]->x == vector->x && ptr[i]->y == vector->y;
        if(a){
            int j = i-1;
            if(j == -1){
                return ptr[2];
            }else{
                return ptr[j];
            }
        }
    }
}