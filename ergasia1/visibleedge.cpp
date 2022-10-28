#include <iostream>
#include "visibleedge.h"

VisibleEdge::VisibleEdge(std::vector<Point> points)
{
    this->points = points;
    for (Point point : points)
    {
        Add_Point(point);
    }
    Create_ConvexHull();
}

VisibleEdge::~VisibleEdge()
{
}

void VisibleEdge::Add_Point(Point p1)
{
    polygon.push_back(p1);
}

void VisibleEdge::Create_ConvexHull()
{
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(result));
    std::cout << result.size() << " points on the convex hull" << std::endl;
    for (Point point : result)
    {
        cout << point << std::endl;
    }
}