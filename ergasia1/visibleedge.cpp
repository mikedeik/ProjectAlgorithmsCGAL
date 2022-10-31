#include <iostream>
#include "visibleedge.h"

VisibleEdge::VisibleEdge(std::vector<Point> points)
{
    this->points = points;
    for (Point point : points)
    {
        Add_Point(point);
    }
    Create_Convex_Hull();
}

VisibleEdge::~VisibleEdge()
{
}

void VisibleEdge::Add_Point(Point p1)
{
    polygon.push_back(p1);
}
//Initializes convex_hull_points and adds them to the polygon class
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