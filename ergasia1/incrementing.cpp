#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <sstream>
#include <fstream>
#include <queue>
#include "incrementing.h"

// Enums

using std::cout;
using std::priority_queue;
using std::vector;

// typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
// typedef CGAL::Polygon_2<Kernel> Polygon;
// typedef Kernel::Point_2 Point;
// typedef Kernel::Segment_2 Segment_2;

Incrementing::Incrementing(vector<Point> inc_points, Sorter inc_sorter) : points(inc_points),
                                                                          sorter(inc_sorter)
{
    sort_points(&points, sorter);
}

Incrementing::~Incrementing()
{
}

void Incrementing::Sort_Points()
{
    sort_points(&points, sorter);
}

const void Incrementing::Print_Points()
{
    for (Point point : points)
    {
        cout << point << "\n";
    }
}

// void incrementing_algorithm(vector<Point> points)
// {

//     priority_queue<Point, vector<Point>, Sort_Desc_Y> sorted_points;

//     for (Point point : points)
//     {
//         sorted_points.push(point);
//     }

//     while (!sorted_points.empty())
//     {

//         cout << sorted_points.top() << "\n";
//         sorted_points.pop();
//     }
// }