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
        int polygon_size = polygon.size();
        for (int i = 0; i < polygon_size - 1; i++)
        {
            Point nearest_point = Find_Nearest_Point_To_Segment(Segment(polygon[i], polygon[i + 1]));
            // Insert point between the vertices of the edge.Meaning after i
            polygon.insert(polygon.begin() + i, nearest_point);
            // remove from points_not_in chain

            points_not_in_chain.erase(points_not_in_chain.begin() + Find_Index_Of_Point_In_Vector(nearest_point, points_not_in_chain));
            if (points_not_in_chain.size() == 0)
            {
                break;
            }
        }
    }
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

int VisibleEdge::Find_Index_In_Polygon(Point p)
{
    for (int i = 0; i < polygon.size(); i++)
    {
        if (polygon[i].x() == p.x() && polygon[i].y() == p.y())
        {
            cout << "INDEX : " << i << std::endl;
            return i;
        }
    }
    return -1;
}

FT VisibleEdge::Get_Polygon_Area()
{
    return polygon.area();
}