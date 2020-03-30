//
// Created by kwd on 23/01/2020.
//

#ifndef DRONE_PROJECT_SERVER_HPP
#define DRONE_PROJECT_SERVER_HPP

#include <string>

class server{
public:
    Vector2D coordinate;
    GLuint texture;
    float color [3];
    std::string name;
    vector<class drone*> dronees;
    int max = 5;

public:
    server(float x, float y, const float pcolor [], std::string pname ){
        coordinate.set(x,y);
        name = pname;
        for(int i = 0; i<3;i++){
            color[i] = pcolor[i];
        }
        int lx,ly;
        texture = GlutWindow::loadTGATexture("../Wifi-router-free-icon-9.tga",lx,ly);
        assert(texture!=0);
        glClearColor(1.0,1.0,1.0,1.0); // background color
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    server( Vector2D pcoor ){
        coordinate.x = pcoor.x;
        coordinate.y = pcoor.y;
    }

    const Vector2D &getCoordinate() const {
        return coordinate;
    }

    void setCoordinate(const Vector2D &coordinate) {
        server::coordinate = coordinate;
    }



    const std::string &getName() const {
        return name;
    }

    void setName(const std::string &name) {
        server::name = name;
    }

    void attributeDrone(class drone* target){
        dronees.push_back(target);
    }

};
#endif //DRONE_PROJECT_SERVER_HPP
