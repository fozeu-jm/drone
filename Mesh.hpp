//
// Created by kwd on 23/01/2020.
//

#ifndef DRONE_PROJECT_MESH_HPP
#define DRONE_PROJECT_MESH_HPP
#include <vector>
#include <list>

#include "Triangle.hpp"

class Mesh{
public:
    std::vector<Vector2D> vertices;
    std::vector<Triangle> triangles_;


    Mesh(const float tab_vect[][2], int size_tab_vect, const int tab_tri[][3], int size_tab_tri)
    {
        for (int i = 0; i < size_tab_vect; i++)
        {
            Vector2D vertex = Vector2D{tab_vect[i][0], tab_vect[i][1]};
            vertices.push_back(vertex);
        }

        for (int i = 0; i < size_tab_tri; i++)
        {
            Vector2D* first_vertex = &vertices[tab_tri[i][0]];
            Vector2D* second_vertex = &vertices[tab_tri[i][1]];
            Vector2D* third_vertex = &vertices[tab_tri[i][2]];

            Triangle triangle = Triangle(first_vertex, second_vertex, third_vertex);
            triangles_.push_back(triangle);
        }

        check_delaunay();
    }

    void onMouseMove(const Vector2D& pos)
    {
        for (auto& triangle: triangles_)
        {
            triangle.onMouseMove(pos);
        }
    }

    void draw()
    {
        for (auto& triangle: triangles_)
        {
            triangle.draw();
        }

        for (auto& triangle: triangles_)
        {
            triangle.drawCircle();
        }
    }

    void onMouseDown(const Vector2D& pos)
    {
        Triangle *triangle = get_triangle(pos);

//        if (triangle)
//        {
//            cout << "clicked" << triangle->whoami(vertices) << endl;
//            if (!triangle->is_delaunay_)
//            {
//                Triangle* triangle_neighbor = neighbor_inside(triangle);
//
//                if (triangle_neighbor)
//                {
//                    cout << "Neighbor:" << triangle_neighbor->whoami(vertices) << endl;
//
//                    flip(triangle, triangle_neighbor);
//                    check_delaunay();
//                }
//            }
//        }
    }

    Triangle* get_triangle(const Vector2D& pos)
    {
        auto triangle_iterator = triangles_.begin();

        while (triangle_iterator != triangles_.end() && !triangle_iterator->is_inside(pos))
        {
            triangle_iterator++;
        }

        return &(*triangle_iterator);
    }

    void check_delaunay()
    {
        auto t = triangles_.begin();

        while (t != triangles_.end())
        {
            t->check_delaunay(vertices);
            t++;
        }
    }

};
#endif //DRONE_PROJECT_MESH_HPP
