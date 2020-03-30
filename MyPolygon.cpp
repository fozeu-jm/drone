#include "MyPolygon.hpp"


MyPolygon::MyPolygon(int p_max)
        : tab_pts_{new Vector2D[p_max]},
          n_max_{p_max},
          current_n_{0}
{
    set_color(YELLOW);
}

MyPolygon::MyPolygon(std::vector<Vector2D> &points)
{
    set_color(YELLOW);

//    assert(points.size() > 3);
    points_to_build_polygon_ = points;

    auto p = points.begin();
    auto pymin = points.begin();

    // find point with minimal y and swap with first point
    while (p != points.end())
    {
        if (p->y < pymin->y)
        {
            pymin = p;
        }
        p++;
    }

    // Swap
    if (pymin != points.begin())
        iter_swap(points.begin(), pymin);

    Vector2D origin{points.begin()->x, points.begin()->y};

    // Copy points in a set of points relative to points[0]
    vector<Vector2D> pointsRelative;
    for (auto pOrig: points)
        pointsRelative.push_back(Vector2D{pOrig.x - origin.x, pOrig.y - origin.y});

    // Sorting point with angular criteria.
    sort(pointsRelative.begin() + 1, pointsRelative.end(), polarComparison);

    std::stack<Vector2D*> CHstack;
    Vector2D *top_1, *top;
    CHstack.push(&pointsRelative[0]);
    CHstack.push(&pointsRelative[1]);
    CHstack.push(&pointsRelative[2]);

//    std::cout << CHstack.size() << std::endl;
    vector<Vector2D>::iterator pi = pointsRelative.begin() + 3;
    while (pi != pointsRelative.end())
    {
        top = CHstack.top();
        CHstack.pop();
        top_1 = CHstack.top();
        CHstack.push(top);

        while (!is_on_the_left(&(*pi), top_1, top))
        {
            CHstack.pop();

            // Update values of top and top_1.
            top = CHstack.top();
            CHstack.pop();
            top_1 = CHstack.top();
            CHstack.push(top);
        }
        CHstack.push(&(*pi));
        pi++;
    }

    // Get stack points to create current polygon
    current_n_ = CHstack.size();
    n_max_ = current_n_;
    tab_pts_ = new Vector2D[n_max_ + 1];



    int i = current_n_ - 1;
    while (!CHstack.empty())
    {
        tab_pts_[i--] = *(CHstack.top()) + origin;
        CHstack.pop();
    }

    tab_pts_[current_n_] = tab_pts_[0];
}

MyPolygon::~MyPolygon()
{
    delete [] tab_pts_;
}

bool MyPolygon::polarComparison(Vector2D p1, Vector2D p2)
{
    double a1 = asin(p1.y / sqrt(p1.x * p1.x + p1.y * p1.y));
    if (p1.x < 0.0)
        a1 = M_PI - a1;

    double a2 = asin(p2.y / sqrt(p2.x * p2.x + p2.y * p2.y));
    if (p2.x < 0.0)
        a2 = M_PI - a2;

    return a1 < a2;
}

bool MyPolygon::is_on_the_left(const Vector2D& p, int i)
{
    Vector2D ab = tab_pts_[i + 1] - tab_pts_[i],
            ap = p - tab_pts_[i];

    return cross_product(ab, ap) >= 0;
}

bool MyPolygon::is_on_the_left(const Vector2D* p, const Vector2D* p1, const Vector2D* p2)
{
    Vector2D ab = *p2 - *p1,
            ap = *p - *p1;

    return cross_product(ab, ap) >= 0;
//    (ab.x_ * ap.y_ - ab.y_ * ap.x_) >= 0;
}

bool MyPolygon::is_convex()
{
    uint i = 0;

    while (i < current_n_ && is_on_the_left(tab_pts_[(i + 2) % current_n_], i))
        i++;

    return i == current_n_;
}

bool MyPolygon::add_vertex(const Vector2D &p)
{
//    if (n_max_ == current_n_ - 2)
//        return false;

    if(current_n_ == n_max_)
        return false;

    tab_pts_[current_n_++] = p;
    tab_pts_[current_n_] = tab_pts_[0];

    return true;
}

// Complexity is N because for each edge and there is N edges.
bool MyPolygon::is_inside(const Vector2D& p)
{
    int i = 0;
    while (i < current_n_ && is_on_the_left(p, i))
        i++;

    return i == current_n_;
}

bool MyPolygon::isInside(Vector2D p) {
    int index =0;
    while (index < current_n_ && is_on_the_left(p,index)){
        index++;
    }
    return  (index==current_n_);
}


bool MyPolygon::is_inside_triangle(const Vector2D& p)
{
    auto triangle = arr_triangle_.begin();

    while (triangle != arr_triangle_.end() && !(*triangle).is_inside(p))
        triangle++;

    return triangle != arr_triangle_.end();
}

void MyPolygon::set_color(const float t_color[4])
{
    memcpy(color, t_color, 4 * sizeof(float));
}

void MyPolygon::draw()
{

    // Draw the border
    glColor3fv(BLACK);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < current_n_; i++)
    {
        glVertex2f(tab_pts_[i].x, tab_pts_[i].y);

    }
    glEnd();

    //TRIANGULATION STEPS
    if (arr_triangle_.empty()){
        triangulation();
        check_delaunay();
        check_delaunay();
    }

    for (auto& triangle: arr_triangle_)
    {
        triangle.draw();
    }

    for (auto& triangle: arr_triangle_)
    {
        triangle.drawCircle();
    }
    // Draw the interior.
    /*glColor3fv(BLACK);

    glBegin(GL_LINE_LOOP);
    for (auto t: arr_triangle_)
    {
        glVertex2f(t.ptr[0]->x, t.ptr[0]->y);
        glVertex2f(t.ptr[1]->x, t.ptr[1]->y);
        glVertex2f(t.ptr[2]->x, t.ptr[2]->y);
    }
    glEnd();*/



    // Draw the number of points.
   /* glLineWidth(4);
    for (int i = 0; i < current_n_; i++)
    {
        glBegin(GL_LINES);
        glVertex2f(tab_pts_[i].x - 10, tab_pts_[i].y - 10);
        glVertex2f(tab_pts_[i].x + 10, tab_pts_[i].y + 10);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(tab_pts_[i].x + 10, tab_pts_[i].y - 10);
        glVertex2f(tab_pts_[i].x - 10, tab_pts_[i].y + 10);
        glEnd();

        GlutWindow::drawText(tab_pts_[i].x - 10, tab_pts_[i].y, to_string(i), GlutWindow::ALIGN_RIGHT);
    }*/

   //LOOP RED DOT POINSTS ON POLYGONE
    for (Vector2D points: points_to_build_polygon_)
    {
        glColor3fv(RED);
        GlutWindow::fillEllipse(points.x, points.y, 6, 6);
    }
    glColor3fv(BLUE);
    GlutWindow::fillEllipse(0 , 362.36, 9, 9);
    GlutWindow::fillEllipse(485.04 , 0, 9, 9);
    GlutWindow::fillEllipse(455.063, 171.497, 9, 9);
    GlutWindow::fillEllipse(147.661 , 384.91, 9, 9);
    GlutWindow::fillEllipse(0 , 0, 9, 9);


}

void MyPolygon::draw2(server servers){
    // Draw the interior of the polygon.
    glColor3ub(servers.color[0],servers.color[1],servers.color[2]);

    glBegin(GL_POLYGON);
    for (int i = 0; i < current_n_; i++){
        Vector2D pt(tab_pts_[i].x, tab_pts_[i].y);
        glVertex2f(tab_pts_[i].x, tab_pts_[i].y);
    }
    glEnd();
//
    // Draw the borders of the polygon.
    glColor3b(0, 0, 255);

    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < current_n_; i++)
    {
        glVertex2f(tab_pts_[i].x, tab_pts_[i].y);
    }
    glEnd();




//    // Draw the interior.
//    //glColor3ub(servers.color[0],servers.color[1],servers.color[2]);
//    glColor3fv(BLUE);
//
//    glBegin(GL_TRIANGLES);
//    for (auto t: arr_triangle_)
//    {
//        glVertex2f(t.ptr[0]->x, t.ptr[0]->y);
//        glVertex2f(t.ptr[1]->x, t.ptr[1]->y);
//        glVertex2f(t.ptr[2]->x, t.ptr[2]->y);
//    }
//    glEnd();
//
//    // Draw the border
//    glColor3fv(BLACK);
//    glLineWidth(3);
//
//    glBegin(GL_LINE_LOOP);
//    for (auto t: arr_triangle_)
//    {
//        glVertex2f(t.ptr_[0]->x_, t.ptr_[0]->y_);
//        glVertex2f(t.ptr_[1]->x_, t.ptr_[1]->y_);
//        glVertex2f(t.ptr_[2]->x_, t.ptr_[2]->y_);
//    }
    for (int i = 0; i < current_n_; i++)
    {
        glVertex2f(tab_pts_[i].x, tab_pts_[i].y);
    }
    glEnd();

    // Draw the interior.


    // Draw the border
//    glColor3fv(BLACK);
//    glLineWidth(4);
//
//    glBegin(GL_LINE_LOOP);
//    for (auto t: arr_triangle_)
//    {
//        glVertex2f(t.ptr_[0]->x_, t.ptr_[0]->y_);
//        glVertex2f(t.ptr_[1]->x_, t.ptr_[1]->y_);
//        glVertex2f(t.ptr_[2]->x_, t.ptr_[2]->y_);
//    }
//    for (int i = 0; i < current_n_; i++){
//        glVertex2f(tab_pts_[i].x, tab_pts_[i].y);
//    }
//    glEnd();

    // Draw the number of points.

}

vector<float> MyPolygon::intersect(const Vector2D& a, const Vector2D& u, const Vector2D& p, const Vector2D& q)
{
    std::vector<float> k;

    Vector2D pq = q - p;
    Vector2D ap = p - a;

    float det = cross_product(u, pq);

    if (det == 0.0)
        return k;

    k.push_back(cross_product(ap, pq) / det); // k1: am = k1 x u
    k.push_back(cross_product(ap, u) / det); // k2: pm = k2 x pq

    return k;
}

std::vector<float> MyPolygon::intersections(const Vector2D& a, const Vector2D& u)
{
    std::vector<float> k1k2;
    std::vector<float> kres;

    for (int i = 0; i < current_n_; ++i)
    {
        k1k2 = intersect(a, u, tab_pts_[i], tab_pts_[i + 1]);
//        std::cout << "Intersection " << i << ":" << k1k2[0] << ";" << k1k2[1] << std::endl;

        if (k1k2.size() == 2 && k1k2[1] >= 0.0 && k1k2[1] <= 1.0)
        {
            auto it = kres.begin();
            auto prev_it = kres.begin();

            while (it != kres.end() && (*it) < k1k2[0])
            {
                prev_it = it;
                it++;
            }

            kres.insert(prev_it, k1k2[0]);
        }
    }

    for (auto p: kres)
//        std::cout << p << std::endl;

    return kres;
}

void MyPolygon::triangulation(){
    std::vector<Vector2D*> tmp;

    // copy the list of vertices in the tmp list.
    for (int i = 0; i < current_n_; i++)
    {
        tmp.push_back(&(tab_pts_[i]));
    }

    int n = current_n_;

    // while we can add a triangle to tabTriangle.
    while (n > 2)
    {
        int i = 0;
        auto p = tmp.begin();
        bool test;
        // create a triangle using p, p+1 and p+2 as vertices
        Triangle T(*p, *(p + 1), *(p + 2));

        // Search a triangle without another points inside
        do {
            test = !T.isEmpty(tmp, i + 3);
            if (test) {
                i++;
                p++;
                T = Triangle(*p, *(p + 1), *(p + 2));
            }
        } while (i < n - 2 && test);

        assert(i < n - 2);

        // Add T to tabTriangles.
        arr_triangle_.push_back(T);
        tmp.erase(p + 1); // remove point(p + 1) from tmp.
        n--; // or n = tmp.size().
    }


    std::vector <Vector2D*> list;
    for(auto point : points_to_build_polygon_){
        list.push_back(&point);
    }


    std::vector<Triangle> filled_triangles;
    std::vector <Vector2D> interiorP;
    int f = 0;
    for(Triangle tri : arr_triangle_){
        for(auto point : points_to_build_polygon_){
            if(tri.is_inside2(point)){

                //cout<<tri.ptr[0]->x<<" "<<tri.ptr[0]->y<<" - "<<tri.ptr[1]->x<<" "<<tri.ptr[1]->y<<" - "<<tri.ptr[2]->x<<" "<<tri.ptr[2]->y<<endl;
                filled_triangles.push_back(tri);
                interiorP.push_back(point);
                arr_triangle_.erase(arr_triangle_.begin() + 1);
                f++;
            }
        }
    }
    int i = 0;
    for(auto point : interiorP){

        Vector2D* interior =  new Vector2D(point.x,point.y);

        Triangle A1 = Triangle(filled_triangles[i].ptr[2], interior, filled_triangles[i].ptr[1] );

        Triangle A2  = Triangle(filled_triangles[i].ptr[1], interior, filled_triangles[i].ptr[0]);
        Triangle A3  = Triangle (filled_triangles[i].ptr[0], interior,filled_triangles[i].ptr[2]);

//        cout<<A1.ptr[0]->x<<" "<<A1.ptr[0]->y<<" - "<<A1.ptr[1]->x<<" "<<A1.ptr[1]->y<<" - "<<A1.ptr[2]->x<<" "<<A1.ptr[2]->y<<endl;
//
//        cout<<A2.ptr[0]->x<<" "<<A2.ptr[0]->y<<" - "<<A2.ptr[1]->x<<" "<<A2.ptr[1]->y<<" - "<<A2.ptr[2]->x<<" "<<A2.ptr[2]->y<<endl;
//
//        cout<<A3.ptr[0]->x<<" "<<A3.ptr[0]->y<<" - "<<A3.ptr[1]->x<<" "<<A3.ptr[1]->y<<" - "<<A3.ptr[2]->x<<" "<<A3.ptr[2]->y<<endl;

        arr_triangle_.push_back(A1);
        arr_triangle_.push_back(A2);
        arr_triangle_.push_back(A3);

        i++;
    }
    solveDelaunay();
    check_delaunay();
    //voronoid();
    //drawPolygons();
}

float MyPolygon::cross_product(const Vector2D& u, const Vector2D& v)
{
    return (u.x * v.y - u.y * v.x);
}

void MyPolygon::check_delaunay(){
    auto t = arr_triangle_.begin();

    while (t != arr_triangle_.end()){
        t->check_delaunay(points_to_build_polygon_);
        t++;
    }
}

void MyPolygon::onMouseMove(const Vector2D& pos)
{
    for (auto& triangle: arr_triangle_ )
    {
        triangle.onMouseMove(pos);
    }
}

Triangle* MyPolygon::neighborInside(Triangle* currentTriangle){
    vector<Triangle> neighbour;
    std::vector<Triangle> tab;

    for(const auto& tri : arr_triangle_){
        tab.push_back(tri);
    }

    int f =0;

    for(auto tri : tab){
        if(tri == *currentTriangle){
            tab.erase(tab.begin() + f);
            break;
        }
        f++;
    }


    int test = 0;
    for(const auto& tri : tab){
       for(auto pnt : tri.ptr){
           if(currentTriangle->is_inside3(*pnt)){
               test++;
           }
       }

       if(test == 2){
           neighbour.push_back(tri);
       }
       test=0;
    }

    /*if(!neighbour.empty()){
        cout<<"Humm "<<endl;
    }*/
    //cout<<"Tab :"<<tab.size();


    if(!neighbour.empty()){
        int i =0;
        for(auto pnt : neighbour[0].ptr ){
            if(currentTriangle->is_inside_circle(*pnt)){
                i++;
            }
        }

        if(i>0){
           // cout<<"Photograph : "<<neighbour[0].ptr[0]->x<<" "<<neighbour[0].ptr[0]->y<<" - "<<neighbour[0].ptr[1]->x<<" "<<neighbour[0].ptr[1]->y<<" - "<<neighbour[0].ptr[2]->x<<" "<<neighbour[0].ptr[2]->y<<endl;
            auto* tr = new Triangle(neighbour[0].ptr[0],neighbour[0].ptr[1],neighbour[0].ptr[2]);
            return tr;
        }else{
            return nullptr;
        }
    }else{
        return nullptr;
    }

 //return nullptr;

}


void MyPolygon::flip(Triangle &ptr1, Triangle &ptr2){

    Vector2D *P = ptr1.getVertexNotIn(ptr2);
    Vector2D *Q = ptr2.getVertexNotIn(ptr1);

    Vector2D *R = ptr1.getNextVertex(P);
    Vector2D *S = ptr2.getNextVertex(Q);

    Vector2D *G = nullptr;

    if(R->x == S->x && R->y == S->y ){
        for(int i =0; i<3;i++){
            if(ptr2.ptr[i]->x != Q->x && ptr2.ptr[i]->y != Q->y  &&  ptr2.ptr[i]->x != S->x && ptr2.ptr[i]->y != S->y){
                G = ptr2.ptr[i];
//                cout<<"G ehh : "<<G->x<<" "<<G->y<<endl;
            }
        }
    }

    if(G == nullptr){
        ptr1.updateVertices(P,R,Q);
        ptr2.updateVertices(Q,S,P);
    }else{
        ptr1.updateVertices(P,G,Q);
        ptr2.updateVertices(Q,S,P);
    }


}

void MyPolygon::solveDelaunay() {

    list<Triangle*> processList;

    auto t = arr_triangle_.begin(); // copy tabTriangles in a list
    while (t!=arr_triangle_.end()) {
        processList.push_back(&(*t));
        t++;
    }


    while (processList.size()>1) { // while a triangle is in the list
        Triangle* current = processList.front(); // pop current
        processList.pop_front();
        if (!current->is_delaunay_) { // if current is not Delaunay

            Triangle *Tneighbor = neighborInside(current);

            if (Tneighbor!=nullptr) { // and if a neighbor is available

//                cout<<"Current : "<<current->ptr[0]->x<<" "<<current->ptr[0]->y<<" - "<<current->ptr[1]->x<<" "<<current->ptr[1]->y<<" - "<<current->ptr[2]->x<<" "<<current->ptr[2]->y<<endl;

                Triangle copy(Tneighbor->ptr[0],Tneighbor->ptr[1],Tneighbor->ptr[2]);

//                cout<<"Neighbour Element : "<<copy.ptr[0]->x<<" "<<copy.ptr[0]->y<<" - "<<copy.ptr[1]->x<<" "<<copy.ptr[1]->y<<" - "<<copy.ptr[2]->x<<" "<<copy.ptr[2]->y<<endl;

                for(int i = 0; i<arr_triangle_.size();i++){
                    if(arr_triangle_[i] == copy){
//                        cout<<"Verification Element : "<<arr_triangle_[i].ptr[0]->x<<" "<<arr_triangle_[i].ptr[0]->y<<" - "<<arr_triangle_[i].ptr[1]->x<<" "<<arr_triangle_[i].ptr[1]->y<<" - "<<arr_triangle_[i].ptr[2]->x<<" "<<arr_triangle_[i].ptr[2]->y<<endl;
                        flip(*current,arr_triangle_[i]);
                        Tneighbor = &arr_triangle_[i];
                    }
                }

//                cout<<"AF-Neighbour : "<<Tneighbor->ptr[0]->x<<" "<<Tneighbor->ptr[0]->y<<" - "<<Tneighbor->ptr[1]->x<<" "<<Tneighbor->ptr[1]->y<<" - "<<Tneighbor->ptr[2]->x<<" "<<Tneighbor->ptr[2]->y<<endl;
//                cout<<"AF-CURRENT : "<<current->ptr[0]->x<<" "<<current->ptr[0]->y<<" - "<<current->ptr[1]->x<<" "<<current->ptr[1]->y<<" - "<<current->ptr[2]->x<<" "<<current->ptr[2]->y<<endl;

                //cout<<"Neighbour : "<<copy.ptr[0]->x<<" "<<copy.ptr[0]->y<<" - "<<copy.ptr[1]->x<<" "<<copy.ptr[1]->y<<" - "<<copy.ptr[2]->x<<" "<<copy.ptr[2]->y<<endl;
                 // flip the common edge



                // remove Tneighbor form the list
                auto tr = processList.begin();
//                cout<<"before "<<processList.size()<<endl;
//                cout<<"Copy Element : "<<copy.ptr[0]->x<<" "<<copy.ptr[0]->y<<" - "<<copy.ptr[1]->x<<" "<<copy.ptr[1]->y<<" - "<<copy.ptr[2]->x<<" "<<copy.ptr[2]->y<<endl;
                for(const Triangle* tri : processList){
//                    cout<<"list Element : "<<tri->ptr[0]->x<<" "<<tri->ptr[0]->y<<" - "<<tri->ptr[1]->x<<" "<<tri->ptr[1]->y<<" - "<<tri->ptr[2]->x<<" "<<tri->ptr[2]->y<<endl;
                }
                for(const Triangle* tri : processList){
                    if(*Tneighbor == *tri){
//                        cout<<"Delete Element : "<<tri->ptr[0]->x<<" "<<tri->ptr[0]->y<<" - "<<tri->ptr[1]->x<<" "<<tri->ptr[1]->y<<" - "<<tri->ptr[2]->x<<" "<<tri->ptr[2]->y<<endl<<endl;
                        processList.erase(tr);
                        break;
                    }
                    tr++;
                }
                for(const Triangle* tri : processList){
//                    cout<<"after list Element : "<<tri->ptr[0]->x<<" "<<tri->ptr[0]->y<<" - "<<tri->ptr[1]->x<<" "<<tri->ptr[1]->y<<" - "<<tri->ptr[2]->x<<" "<<tri->ptr[2]->y<<endl;
                }
//                cout<<"After "<<processList.size()<<endl;

            } else {
                processList.push_back(current); // postpone the treatment
               // cout<<"test size : "<<current->ptr[0]->y<<endl;

            }
        }
    }

//    cout<<"Triangles size :"<<arr_triangle_.size()<<endl;

    for(const auto& tri : arr_triangle_){
//        cout<<"Final Element : "<<tri.ptr[0]->x<<" "<<tri.ptr[0]->y<<" - "<<tri.ptr[1]->x<<" "<<tri.ptr[1]->y<<" - "<<tri.ptr[2]->x<<" "<<tri.ptr[2]->y<<endl;
    }
}

void MyPolygon::voronoid(vector<MyPolygon*>& polygons) {
//    cout<<endl<<endl<<endl<<endl<<endl;
    for(auto pnt : points_to_build_polygon_ ){
        MyPolygon* P1 = new MyPolygon(50); //create a new polygon
        vector<Triangle> subset;
        vector<Vector2D> fPoints;
        for(auto tri : arr_triangle_){
            if(tri.is_inside3(pnt)){
                subset.push_back(tri);
            }
        }
//        cout<<"Point :"<<pnt.x<<"  "<<pnt.y<<endl;
        for(const Triangle& tri : subset){
//            cout<<"Subset Element : "<<tri.ptr[0]->x<<" "<<tri.ptr[0]->y<<" - "<<tri.ptr[1]->x<<" "<<tri.ptr[1]->y<<" - "<<tri.ptr[2]->x<<" "<<tri.ptr[2]->y<<endl;
        }

        for(Triangle& tri : subset){
            Vector2D* Next = tri.getNextVertex(&pnt);
            Vector2D* prev = tri.getPrevVertex(&pnt);
            vector<Vector2D*> nexPrev{Next,prev};

//            cout<<"The triangle(excluded) : "<<tri.ptr[0]->x<<" "<<tri.ptr[0]->y<<" - "<<tri.ptr[1]->x<<" "<<tri.ptr[1]->y<<" - "<<tri.ptr[2]->x<<" "<<tri.ptr[2]->y<<endl;
//            cout<<"The next is :"<<Next->x<<" "<<Next->y<<endl;
//            cout<<"The prev is :"<<prev->x<<" "<<prev->y<<endl;

            if(isOpened(subset,*Next,tri)){
//                cout<<"The Next is opened"<<endl;
                Vector2D E = *Next - pnt ;
                Vector2D H = (pnt + *Next).divide(2.0);
                Vector2D U(E.y,-(E.x));
                Vector2D Q = intersectionWithBorders(H,U,0,0,1200,1000);
                fPoints.push_back(Q);
                P1->add_vertex(Q);
//                cout<<"PointO :"<<pnt.x<<"  "<<pnt.y<<endl;
//                cout<<"PrevO :"<<Next->x<<" "<<Next->y<<endl;
//                cout<<"ResultO :"<<(pnt.x + Next->x)<<" "<<(pnt.y + Next->y)<<endl;
//                cout<<"E-O :"<<E.x<<" "<<E.y<<endl;
//                cout<<"The first points is :"<<Q.x<<" "<<Q.y<<endl;
            }else{
              //  cout<<"The Next is Not opened"<<endl;
            }
            if(isOpened(subset,*prev,tri)){
               // cout<<"The Prev is opened"<<endl;
                Vector2D E =  pnt - *prev  ;
                Vector2D H = (pnt + *prev).divide(2.0);
                Vector2D U(E.y,-(E.x));
                Vector2D Q = intersectionWithBorders(H,U,0,0,1200,1000);
                fPoints.push_back(Q);
                P1->add_vertex(Q);
              /*  cout<<"PointO :"<<pnt.x<<"  "<<pnt.y<<endl;
                cout<<"PrevO :"<<prev->x<<" "<<prev->y<<endl;
                cout<<"ResultO :"<<(pnt.x + Next->x)<<" "<<(pnt.y + prev->y)<<endl;
                cout<<"E-O :"<<E.x<<" "<<E.y<<endl;
                cout<<"The first points is :"<<Q.x<<" "<<Q.y<<endl;*/
            }else{
               // cout<<"The prev is Not opened"<<endl;
            }
        }
        for(auto hex : subset){
            hex.calculateCircle();
            fPoints.push_back(hex.circum_center_);
            P1->add_vertex(hex.circum_center_);
//            cout<<"The circumcenter added : "<<hex.circum_center_.x<<" , "<<hex.circum_center_.y<<endl;
        }
        float x = 15;
        float y=15;
        for(auto pts : fPoints){
            if(pts.y == 0 || pts.y == 1000){
                y = pts.y;
            }

            if(pts.x == 0 || pts.x == 1200){
                x = pts.x;
            }
        }
        if(x == 15 && y == 15){

        }else if(x == 15 && y != 15){

        }else if(x != 15 && y == 15){

        }else{
//            cout<<"Last Point :"<<x<<" , "<<y<<endl;
            fPoints.push_back(Vector2D(x,y));
            P1->add_vertex(Vector2D(x,y));
        }
//        cout<<"Number of points added "<<fPoints.size()<<endl;

        //polygons.push_back(P1);

        MyPolygon* P2 = new MyPolygon(fPoints); //create a new polygon
        polygons.push_back(P2);

       // drawPolygons();
    }

}

bool MyPolygon::isOpened( std::vector<Triangle> &subset, Vector2D &point,  Triangle &excluded){
    bool open = true;
    for( Triangle& tri : subset){
        if(excluded != tri){
            if(tri.is_inside3(point)){
                open = false;
            }
        }
    }
    return open;
}

Vector2D MyPolygon::intersectionWithBorders(const Vector2D& a, Vector2D u, float x0, float y0, float x1, float y1){
//    cout<<"A "<<a.x<<"  "<<a.y<<endl;
//    cout<<"U "<<u.x<<"  "<<u.y<<endl;

    float k0 =(x0 - a.x)/u.x;
    float k1 =(x1 - a.x)/u.x;
    float k2 =(y0 - a.y)/u.y;
    float k3 =(y1 - a.y)/u.y;
    float min = 10000000.000f;
    vector<float> floats{k0,k1,k2,k3};

    for(auto k : floats){
//        cout<<k<<"  ";
        if(k<min && k>0){
            min = k;
        }
    }
//    cout<<"--- min K :"<<min<<endl;
//    cout<<"results "<<(u.multiply(min)).x<<"  "<<(u.multiply(min)).y<<endl;
    Vector2D result = (u.multiply(min));

//    cout<<"With K 0 : "<<(a+u.multiply(k0)).x<<"  "<<(a+u.multiply(k0)).y<<endl;
//    cout<<"With K 1 : "<<(a+u.multiply(k1)).x<<"  "<<(a+u.multiply(k1)).y<<endl;
//    cout<<"With K 2 : "<<(a+u.multiply(k2)).x<<"  "<<(a+u.multiply(k2)).y<<endl;
//    cout<<"With K'3 : "<<(a+u.multiply(k3)).x<<"  "<<(a+u.multiply(k3)).y<<endl;

    float ax = a.x;
    float rx = u.multiply(min).x;

    float ay = a.y;
    float ry = u.multiply(min).y;

    float t = ax+ rx;

//    cout<<"Addition test : "<<ax<<" + "<<rx<<" = "<<floorf((ax + rx)*100)/100<<endl;

    float x = floorf((ax + rx)*100)/100;
    float y = floorf((ay + ry)*100)/100;

//    cout<<"X-es :"<<x<<endl;
    Vector2D P(x,y);
    return P ;
}