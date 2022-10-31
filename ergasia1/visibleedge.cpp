#include <iostream>
#include "visibleedge.h"

VisibleEdge::VisibleEdge(std::vector<Point> points)
{
    this->points = points;
    Create_Convex_Hull();
}

VisibleEdge::~VisibleEdge()
{
}

void VisibleEdge::Add_Point(Point p1)
{
    polygon.push_back(p1);
}
// Initializes convex_hull_points and adds them to the polygon class
void VisibleEdge::Create_Convex_Hull()
{
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(convex_hull_points));
    std::cout << convex_hull_points.size() << " points on the convex hull" << std::endl;
    for (Point point : convex_hull_points)
    {
        polygon.push_back(point);
    }
    for (int i = 0; i < convex_hull_points.size() - 1; i++)
    {
        edges.push_back(Segment(convex_hull_points[i], convex_hull_points[i + 1]));
    }
    edges.push_back(Segment(convex_hull_points.back(), convex_hull_points.front()));
    Print_Edges();
}

void VisibleEdge::Print_Edges()
{
    std::cout << "Printing Edges\n";
    for (Segment seg : edges)
    {

        std::cout << "(" << seg[0] << ")"
                  << "->"
                  << "(" << seg[1] << ")" << std::endl;
    }
}

bool VisibleEdge::Is_Point_Included_In_Polygonal_Chain(Point p)
{

    for (int i = 0; i < polygon.size(); i++)
    {
        if (polygon[i].x() == p.x() && polygon[i].y() == p.y())
        {
            return 1;
        }
    }
    return 0;
}

void VisibleEdge::Create_Polygon()
{
    points_not_in_chain.clear();
    for (Point p : points)
    {
        if (!Is_Point_Included_In_Polygonal_Chain(p))
        {
            points_not_in_chain.push_back(p);
        }
    }
    
    while (points_not_in_chain.size() > 0)
    {
        for (Segment edge : polygon.edges())
        {
            cout << "Current edge" << edge << std::endl;
            Point temp = Find_Nearest_Point_To_Segment(edge);
            cout << "TEMP IS : " << temp << std::endl;
            // Insert point between points of edge
            PointIterator position_to_insert = find(polygon.vertices().begin(), polygon.vertices().end(), temp);

            int index = position_to_insert - polygon.vertices().begin();
            cout << "Position to insert is :" << index << std::endl;
            //EDO TROEI SEGMENTATION
            cout << "Polygon Size is: "<< polygon.size()<<std::endl;
            polygon.insert(polygon.begin() + index, temp);

            // remove from points_not_in chain

            points_not_in_chain.erase(points_not_in_chain.begin() + Find_Index_Of_Point_In_Vector(temp, points_not_in_chain));
            for (Point p : points_not_in_chain)
            {
                cout << p << std::endl;
            }
            if (points_not_in_chain.size() == 0)
            {
                break;
            }
            cout << "PNIC SIZE IS :" << points_not_in_chain.size() << std::endl;
        }
    }

    cout << "ALL DONE" << std::endl;
}

Point VisibleEdge::Find_Nearest_Point_To_Segment(Segment s)
{
    Point temp = points_not_in_chain[0];
    FT min_dist = CGAL::squared_distance(s, points_not_in_chain[0]);
    for (Point p : points_not_in_chain)
    {
        FT dist = CGAL::squared_distance(s, points_not_in_chain[0]);
        if (dist < min_dist)
        {
            min_dist = dist;
            temp = p;
        }
    }
    return temp;
}

int VisibleEdge::Find_Index_Of_Point_In_Vector(Point p, vector<Point> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i].x() == p.x() && v[i].y() == p.y())
        {
            return i;
        }
    }
    return -1;
}