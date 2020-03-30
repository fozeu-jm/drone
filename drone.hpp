//y
// Created by kwd on 26/01/2020.
//

#ifndef DRONE_PROJECT_DRONE_HPP
#define DRONE_PROJECT_DRONE_HPP

#include <glutWindow.h>
#include "Vector2D.hpp"
#include "server.hpp"
#include "Triangle.hpp"

class drone{
public:
    Vector2D* speed;
    Vector2D* location;
    Vector2D* acceleration;
    Vector2D* start;
    Vector2D* server;
    float weight;
    float radius;
    float maxSpeed;
    double dmax;
    double r;
    float thrustForce;
    float collisionForceStrength;
    float smoothDamping;
    GLuint texture;

    drone() {
        //Drone draw
        int lx, ly;
        texture = GlutWindow::loadTGATexture("../dronie.tga",lx,ly);
        assert(texture!=0);
        glClearColor(1.0,1.0,1.0,1.0); // background color
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        location = new Vector2D(50, 50);
        speed = new Vector2D();
        acceleration = new Vector2D();
        start = new Vector2D();
        server = new Vector2D();

        weight = 0.8;
        radius = 25;
        maxSpeed = 44.704;
        dmax = 0 + 53;
        r = radius + 48;
        thrustForce = 0.2;
        collisionForceStrength = 0.006;
        smoothDamping = 0.9;
    }


    void currentServer( vector<MyPolygon*> &polygons,vector<class server> &servers){
        for(auto poly : polygons){
            if(poly->is_inside(Vector2D(location->x, location->y))){
                cout<<"Found Polygon";
                for(auto serv : servers){
                    if(poly->is_inside(serv.coordinate)){
                        cout<<"Je survole le serveur de "<<serv.name<<endl;
                        glColor3fv(BLACK);
                        glPushMatrix();
                        glLineWidth(2);
                        glBegin(GL_LINES);
                        glVertex2f(location->x,location->y);
                        glVertex2f(serv.coordinate.x,serv.coordinate.y);
                        glEnd();
                        glPopMatrix();
                    }else{
                        cout<<"Nanana"<<endl;
                    }
                }
            }else{
                cout<<"Not found !"<<endl;
            }
        }
    }



    void draw(vector<drone*> drones) {
        float width = 1000;
        float height = 1000;

        updateSpeed(drones);

        location->x += speed->x;
        location->y += speed->y;

        float a = (float) (atan(speed->y / speed->x) + M_PI_2);

        if (speed->x > 0) {
            a += M_PI;
        }
        glEnable(GL_TEXTURE_2D);
        //glColor3f(0.0f,0.0f,0.0f);

        glBindTexture(GL_TEXTURE_2D, texture);
        glPushMatrix();
        glTranslatef(location->x - radius, location->y - radius, 1.0);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex2f(0.0, 0.0);

        glTexCoord2f(1.0, 0.0);
        glVertex2f(radius * 2, 0.0);

        glTexCoord2f(1.0, 1.0);
        glVertex2f(radius * 2, radius * 2);

        glTexCoord2f(0.0, 1.0);
        glVertex2f(0.0, radius * 2);

        glEnd();
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);

        glColor3fv(BLACK);
        glPushMatrix();

        float centerx = location->x;
        float centery = location->y;
        glTranslatef(centerx, centery, 0);
        glLineWidth(1);
        glLineStipple(1, 0x00FF);
        glEnable(GL_LINE_STIPPLE);
        glBegin(GL_LINE_LOOP);
        float rad = dmax;
        for (int i = 0; i < 21; i++) {
            glVertex2f(rad * cos(a), rad * sin(a));
            a += (float) (M_PI / 20.0);
        }
        glEnd();
        glDisable(GL_LINE_STIPPLE);
//        glLineWidth(1);
        glPopMatrix();

        glColor3fv(BLACK);
        glPushMatrix();
        glLineWidth(2);

//        glBegin(GL_LINES);

        glEnable(GL_LINE_STIPPLE);
        glBegin(GL_LINE_LOOP);

        glVertex2f(centerx, centery);
        glVertex2f(centerx + (speed->x * 25), centery + (speed->y * 25));

        glEnd();
        glDisable(GL_LINE_STIPPLE);

//        glEnd();
        glPopMatrix();
    }

    void updateServer(class server* newServer) {
        server = new  Vector2D(newServer->coordinate.x, newServer->coordinate.y);
        start = new Vector2D(location->x, location->y);
    }

    void updateSpeed(vector<drone*> drones) {
        Vector2D dragForce = Vector2D();
        Vector2D weightForce = Vector2D();
        Vector2D liftForce = Vector2D();
        Vector2D distance = (*server - *this->location);
        float thrustStrength;
        if (distance.norm() < 10) {
            thrustStrength = 0;
        } else {
            thrustStrength = thrustForce;
        }
        Vector2D thrustForce = Vector2D(distance);
        thrustForce.normalize();
        thrustForce =  thrustForce.multiply(thrustStrength);

        Vector2D force = weightForce + liftForce + dragForce + thrustForce;

        for (auto drone:drones) {
            if (drone->location->x != this->location->x && drone->location->y != this->location->y) {
                Vector2D ab = *this->location - *drone->location;
                double dab = ab.norm();
                ab.normalize();
                ab =  ab.multiply(collisionForceStrength);
                float coef;
                if (dab < dmax) {
                    if (dab < r) {
                        coef = 1;
                    } else if (dab < dmax) {
                        coef = (dab - dmax) / (r - dmax);
                    }
                    Vector2D collisionForce = ab.multiply(coef);
                    force = collisionForce;
                }
            }
        }

        Vector2D tmpAcc = force.multiply((1 / weight));
        acceleration->x = tmpAcc.x;
        acceleration->y = tmpAcc.y;

        Vector2D tmpSpeed = (*speed + *acceleration).multiply(smoothDamping);


        speed->x = tmpSpeed.x;
        speed->y = tmpSpeed.y;

    }

    Vector2D getPosition() {
        return *location;
    }



};
#endif //DRONE_PROJECT_DRONE_HPP
