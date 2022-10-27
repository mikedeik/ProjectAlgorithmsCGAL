#include <iostream>
#include "convexhull.h"
ConvexHull::ConvexHull(std::vector<Point> points)
{
    this->points = points;
    for (Point point : points)
    {
        Add_Point(point);
    }
    Create_ConvexHull();
}

ConvexHull::~ConvexHull()
{
}

void ConvexHull::Add_Point(Point p1)
{
    polygon.push_back(p1);
}

void ConvexHull::Create_ConvexHull()
{
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(result));
    std::cout << result.size() << " points on the convex hull" << std::endl;
    for (Point point : result)
    {
        cout << point << std::endl;
    }
}