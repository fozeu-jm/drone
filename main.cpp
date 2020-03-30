#include <array>
#include <glutWindow.h>
#include <fstream>
#include "Mesh.hpp"
#include "server.hpp"
#include "MyPolygon.hpp"
#include "Triangle.hpp"
#include "drone.hpp"

using namespace std;

class PolygonDraw: public GlutWindow {
private:
    Mesh* mesh;
    vector<server> servers;
    MyPolygon* S1;
    vector<MyPolygon*> polygons;
    vector<drone *> drones;
    string filepath;

public:

    PolygonDraw(const string &title,int argc,char **argv):GlutWindow(argc,argv,title,1200,1000,FIXED){
        filepath = "../servers.txt";
        cout << "argc=" << argc << endl;
        int i=0;
        while (i<argc) {
            cout << "argv[" << i << "]=" << argv[i] << endl;
            if (strncmp(argv[i],"-h",1)==0){
                cout << "Geometric Algorithmic Project Drone and Server" << endl;
                cout << "-h\tFor help" << endl;
                cout << "-c filename \tTo specify the file to read" << endl;
                cout << "Tape d key to add a drone" << endl;
            }
            if (strncmp(argv[i],"-c",2)==0) {
                i++;
                if (i<argc) {
                    filepath=argv[i++];
                }
            }
            i++;
        }
    }

    void onStart() override;
    void onDraw() override;
    void onQuit() override;
    void onMouseMove(double cx,double cy) override;
    void onMouseDown(int button,double cx,double cy) override;
    void onKeyPressed(unsigned char c,double x,double y) override;
    void loadServers();

    void addDrone();
};

void PolygonDraw::onStart() {
    loadServers();
    std::vector<Vector2D> q50;
    for(auto serv : servers){
        q50.push_back(serv.coordinate.multiply2(1.1));
    }

    S1 = new MyPolygon(q50);

    glClearColor(1.0,1.0,1.0,1.0); // background color
}
void PolygonDraw::loadServers() {
//    server Paris(221,128, RED2,"Paris");
//    int lx,ly;
//    servers.push_back(Paris);
//
//
//    server Rome(141,652, BLUE2 ,"Rome");
//    servers.push_back(Rome);
//
//    server London(414,406,PINK2,"London");
//    servers.push_back(London);
//
//    server Berlin(876,569,YELLOW2,"Berlin");
//    servers.push_back(Berlin);
//
//    server Beijing(532,756,GREEN2,"Beijing");
//    servers.push_back(Beijing);
//
//    server SanFrancisco(690,210,CYAN2,"San Francisco");
//    servers.push_back(SanFrancisco);

    try {
        ifstream fin(filepath);
        string name, color;
        Vector2D position;

        servers.clear();

        if (fin.is_open()) {
            char line[256];
            string strLine,vectorStr;
            int posBeg,posEnd;
            while (!fin.eof()) {
                fin.getline(line,256);
                strLine=line;
                if (strLine.size()>1) {
                    posEnd = strLine.find(';');
                    name = strLine.substr(0, posEnd);
                    //cout << "name=" << name << endl;
                    posBeg = posEnd + 1;
                    posEnd = strLine.find(';', posBeg);
                    vectorStr = strLine.substr(posBeg, posEnd - posBeg);
                    //cout << "vectorStr=" << vectorStr << endl;
                    color = strLine.substr(posEnd + 1);
                    //cout << "color=" << color << endl;
                    posBeg = vectorStr.find('(');
                    posEnd = vectorStr.find(',');
                    position.x = stof(vectorStr.substr(posBeg + 1, posEnd - posBeg - 1));
                    posBeg = posEnd + 1;
                    posEnd = vectorStr.find(')', posBeg);
                    position.y = stof(vectorStr.substr(posBeg, posEnd - posBeg));
                    //cout << "position=" << position << endl;
                    float fcolor[3];

//                    cout<<"KEMOKO COLOR :  ----- "<<color;

                    if(color == "RED"){
                        server Fserver(position.x,position.y,RED2,name);
                        servers.push_back(Fserver);
                    }else if(color == "GREEN"){
                        server Fserver(position.x,position.y,GREEN2,name);
                        servers.push_back(Fserver);
                    }else if(color == "ORANGE"){
                        server Fserver(position.x,position.y,ORANGE2,name);
                        servers.push_back(Fserver);
                    }else if(color == "YELLOW"){
                        server Fserver(position.x,position.y,YELLOW2,name);
                        servers.push_back(Fserver);
                    }else if(color == "CYAN"){
                        server Fserver(position.x,position.y,CYAN2,name);
                        servers.push_back(Fserver);
                    }else if(color == "BLUE"){
                        server Fserver(position.x,position.y,BLUE2,name);
                        servers.push_back(Fserver);
                    }else if(color == "PINK"){
                        server Fserver(position.x,position.y,PINK2,name);
                        servers.push_back(Fserver);
                    }else if(color == "PURPLE"){
                        server Fserver(position.x,position.y,PURPLE2,name);
                        servers.push_back(Fserver);
                    }else if(color == "MAGENTA"){
                        server Fserver(position.x,position.y,MAGENTA2,name);
                        servers.push_back(Fserver);
                    }
                    else if(color == "GREY"){
                        server Fserver(position.x,position.y,GRAY2,name);
                        servers.push_back(Fserver);
                    }else if(color == "BROWN"){
                        server Fserver(position.x,position.y,BROWN2,name);
                        servers.push_back(Fserver);
                    }
                }
            }
        }
        fin.close();
    } catch (ifstream::failure e) {
        cerr << "Exception opening/reading/closing file" << endl;
    }
}

void PolygonDraw::onDraw() {
    // draw the referential
    glPushMatrix();
    glTranslatef(10,10,0);
    glColor3fv(RED);
    glBegin(GL_QUADS);
    glVertex2f(0.0,-2.0);
    glVertex2f(100.0,-2.0);
    glVertex2f(100.0,2.0);
    glVertex2f(0.0,2.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(110.0,0.0);
    glVertex2f(90.0,-10.0);
    glVertex2f(90.0,10.0);
    glEnd();

    glColor3fv(GREEN);
    glBegin(GL_QUADS);
    glVertex2f(-2.0,0.0);
    glVertex2f(2.0,0.0);
    glVertex2f(2.0,100.0);
    glVertex2f(-2.0,100.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(0.0,110.0);
    glVertex2f(-10.0,90.0);
    glVertex2f(10.0,90.0);
    glEnd();

    glPopMatrix();
    S1->draw();
    S1->voronoid(polygons);
//    cout<<"Polygons number :"<<polygons.size();

    for(auto poly : polygons){
        for(auto serv : servers){
            if(poly->is_inside(serv.coordinate)){
                poly->draw2(serv);
            }
        }

    }
    polygons.clear();


    for(auto server : servers){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,server.texture);
        glPushMatrix();
        glTranslatef(server.coordinate.x,server.coordinate.y,1.0);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex2f(0.0,0.0);
        glTexCoord2f(1.0,0.0);
        glVertex2f(60,0.0);
        glTexCoord2f(1.0,1.0);
        glVertex2f(60,60.0);
        glTexCoord2f(0.0,1.0);
        glVertex2f(0.0,60.0);
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        glColor3ub(0,0,0);
        drawText(server.coordinate.x+35,server.coordinate.y-20,server.name,ALIGN_CENTER,GLUT_BITMAP_HELVETICA_12);
        drawText(server.coordinate.x+35,server.coordinate.y-35,"( "+to_string(server.dronees.size())+" )",ALIGN_CENTER,GLUT_BITMAP_HELVETICA_12);
    }

    for(int i = 0; i<drones.size();i++){
        drones[i]->draw(drones);
        if(i == drones.size()-1){

           drones[i]->currentServer(polygons,servers);
        }
    }

//    for (auto t:tris) {
//        t->draw();
//    }
//    for (auto t:tris) {
//        t->drawCircle();
//    }


}

void PolygonDraw::onQuit() {

}

void PolygonDraw::onMouseMove(double cx, double cy) {
    Vector2D vertex((float) cx,(float) cy);
//    for (auto t:tris) {
//        t->onMouseMove(v);
//    }

    S1->onMouseMove(vertex);
}

void PolygonDraw::onMouseDown(int button, double cx, double cy){

}



void PolygonDraw::onKeyPressed(unsigned char c, double x, double y){
    switch (c){
        case 'd' :
            addDrone();
            break;
        default:
            break;
    }
}

void PolygonDraw::addDrone() {
    int min = 1000;
    for(auto item1 : servers){
        if(item1.dronees.size() < min ){
            min = item1.dronees.size();
        }
    }
    //int j = 0;
//    for(auto item2 : servers){
//        j++;
//        if(item2.dronees.size() == min ){
//            item2.get
//            break;
//        }
//    }

    for(int i = 0; i < servers.size(); i++){
        if(servers[i].dronees.size() == min ){
            drone *dr = new drone();
            drones.push_back(dr);
            dr->updateServer(&servers[i]);
            drones.push_back(dr);
            servers[i].attributeDrone(dr);
           // cout<<"Number hahaha   :"<<i<<endl;
            break;
        }
    }






//    srand (time(NULL));
//    int n = rand() % servers.size();

}


int main(int argc,char **argv) {
    PolygonDraw window("Triangulation", argc,argv);
    window.start();


    return 0;
}