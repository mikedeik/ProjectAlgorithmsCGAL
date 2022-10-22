#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <sstream>
#include <fstream>
#include <queue>

// Enums

using std::cout;
using std::priority_queue;
using std::vector;

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Polygon_2<Kernel> Polygon;
typedef Kernel::Point_2 Point;
typedef Kernel::Segment_2 Segment_2;

// sorting vasi x fthinousa
typedef struct Sort_Desc_X
{
    bool operator()(Point p1, Point p2)
    {
        return CGAL::compare_x(p1, p2) < 0;
    }
} Sort_Desc_X;

// sorting vasi x auksousa
typedef struct Sort_Asc_X
{
    bool operator()(Point p1, Point p2)
    {
        return CGAL::compare_x(p1, p2) > 0;
    }
} Sort_Asc_X;

// sorting vasi y fthinousa
typedef struct Sort_Desc_Y
{
    bool operator()(Point p1, Point p2)
    {
        return CGAL::compare_y(p1, p2) < 0;
    }
} Sort_Desc_Y;

// sorting vasi y auksousa
typedef struct Sort_Asc_Y
{
    bool operator()(Point p1, Point p2)
    {
        return CGAL::compare_y(p1, p2) > 0;
    }
} Sort_Asc_Y;

void incrementing_algorithm(vector<Point> points)
{

    priority_queue<Point, vector<Point>, Sort_Desc_Y> sorted_points;

    for (Point point : points)
    {
        sorted_points.push(point);
    }

    while (!sorted_points.empty())
    {

        cout << sorted_points.top() << "\n";
        sorted_points.pop();
    }
}