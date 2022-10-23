#include <iostream>
// #include <CGAL/Exact_predicates_exact_constructions_kernel.h>
// #include <CGAL/Polygon_2.h>
#include <sstream>
#include <fstream>
#include <queue>
#include "incrementing.h"
#include <boost/optional/optional_io.hpp>

using std::cout;
using std::priority_queue;
using std::vector;

typedef Kernel::Triangle_2 Triangle;

Incrementing::Incrementing(vector<Point> inc_points, Sorter inc_sorter) : points(inc_points),
                                                                          sorter(inc_sorter),
                                                                          top_red_edge_index(-1),
                                                                          bottom_red_edge_index(-1)
{
    sort_points(&points, sorter);
    int index = 0;
    while (index < 3)
    {
        polygon.push_back(points[index]);
        if (!index)
        {
            index++;
            continue;
        }

        edges.push_back(Segment(points[index - 1], points[index]));
        index++;
    }
    edges.push_back(Segment(points[index - 1], points[0]));
    point_position = 3;

    set_red_edges(1, 2);
}

Incrementing::~Incrementing()
{
}

const void Incrementing::Print_Points()
{
    for (Point point : points)
    {
        cout << point << "\n";
    }
}

const void Incrementing::Print_Edges()
{
    for (Segment segment : edges)
    {
        cout << segment << "\n";
    }
}

void Incrementing::Add_Point(Point p1)
{
    polygon.push_back(p1);
}

const bool Incrementing::Simple()
{
    return polygon.is_simple();
}

const void Incrementing::Create_Polygon_Line()
{

    // while (point_position <= points.size())
    // {

    Point t_source = edges[bottom_red_edge_index].source();
    Point t_target = edges[bottom_red_edge_index].target();
    Triangle triangle(t_source, points[point_position], t_target);

    if (point_position == 6)
    {
        cout << "My triangle:\n";
        cout << triangle << "\n";
        cout << "and this is the edge: \n";
        cout << edges[top_red_edge_index] << "\n";
        getchar();
    }

    const auto result = CGAL::intersection(triangle, edges[top_red_edge_index]);

    cout << "and this is the result: \n";
    cout << result << "\n";

    if (result)
    {
        if (const Segment *s = boost::get<Segment>(&*result))
        {
            cout << "this is the segment: \n";
            cout << *s << "\n";
            cout << "they do intersect\n";
            t_source = edges[top_red_edge_index].source();
            t_target = edges[top_red_edge_index].target();

            edges.insert(edges.begin() + top_red_edge_index, Segment(points[point_position], t_target));
            edges.insert(edges.begin() + top_red_edge_index, Segment(t_source, points[point_position]));

            edges.erase(edges.begin() + top_red_edge_index + 2);
            set_red_edges(top_red_edge_index, top_red_edge_index + 1);
            Print_Edges();
        }
        else
        {
            edges.insert(edges.begin() + bottom_red_edge_index, Segment(points[point_position], t_target));
            edges.insert(edges.begin() + bottom_red_edge_index, Segment(t_source, points[point_position]));

            edges.erase(edges.begin() + bottom_red_edge_index + 2);
            set_red_edges(bottom_red_edge_index, bottom_red_edge_index + 1);

            Print_Edges();
        }
    }
    point_position++;
    // if (CGAL::compare_y(points[point_position], points[point_position - 1]) > 0)
    // {
    //     Point temp_source = edges[top_red_edge_index].source();
    //     Point temp_target = edges[top_red_edge_index].target();

    //     edges.insert(edges.begin() + top_red_edge_index, Segment(points[point_position], temp_target));
    //     edges.insert(edges.begin() + top_red_edge_index, Segment(temp_source, points[point_position]));

    //     edges.erase(edges.begin() + top_red_edge_index + 2);
    //     Print_Edges();
    //     set_red_edges(top_red_edge_index, top_red_edge_index + 1);
    //     point_position++;
    // }
    // else
    // {
    //     Point temp_source = edges[bottom_red_edge_index].source();
    //     Point temp_target = edges[bottom_red_edge_index].target();

    //     edges.insert(edges.begin() + bottom_red_edge_index, Segment(points[point_position], temp_target));
    //     edges.insert(edges.begin() + bottom_red_edge_index, Segment(temp_source, points[point_position]));

    //     edges.erase(edges.begin() + bottom_red_edge_index + 2);
    //     Print_Edges();
    //     set_red_edges(bottom_red_edge_index, bottom_red_edge_index + 1);
    //     point_position++;
    // }
    // // }
}

const void Incrementing::testing()
{

    cout << "top is :" << top_red_edge_index << "\n";
    cout << "bottom is :" << bottom_red_edge_index << "\n";
}

void Incrementing::set_red_edges(int s1_index, int s2_index)
{

    bottom_red_edge_index = s1_index;
    top_red_edge_index = s2_index;
    // int compare;

    // switch (sorter)
    // {
    // case X_ASCENDING:

    //     compare = CGAL::compare_y(edges[s1_index].source(), edges[s2_index].target());
    //     if (compare < 0)
    //     {
    //         if (CGAL::compare_y(edges[s1_index].target(), edges[s2_index].target()) > 0)
    //         {
    //             top_red_edge_index = s2_index;
    //             bottom_red_edge_index = s1_index;
    //             return;
    //         }

    //         top_red_edge_index = s1_index;
    //         bottom_red_edge_index = s2_index;
    //         return;
    //     }
    //     if (compare > 0)
    //     {

    //         if (CGAL::compare_y(edges[s1_index].target(), edges[s2_index].target()) < 0)
    //         {
    //             top_red_edge_index = s2_index;
    //             bottom_red_edge_index = s1_index;
    //             return;
    //         }
    //         top_red_edge_index = s1_index;
    //         bottom_red_edge_index = s2_index;
    //         return;
    //     }

    //     if (CGAL::compare_y(edges[s1_index].target(), edges[s2_index].target()) > 0)
    //     {
    //         top_red_edge_index = s2_index;
    //         bottom_red_edge_index = s1_index;
    //         return;
    //     }
    //     top_red_edge_index = s1_index;
    //     bottom_red_edge_index = s2_index;
    //     return;

    // case X_DESCENDING:

    //     compare = CGAL::compare_y(edges[s1_index].source(), edges[s2_index].target());
    //     if (compare < 0)
    //     {
    //         top_red_edge_index = s2_index;
    //         bottom_red_edge_index = s1_index;
    //         return;
    //     }
    //     if (compare)
    //     {
    //         top_red_edge_index = s1_index;
    //         bottom_red_edge_index = s2_index;
    //         return;
    //     }

    //     if (CGAL::compare_y(edges[s1_index].target(), edges[s2_index].target()))
    //     {
    //         top_red_edge_index = s2_index;
    //         bottom_red_edge_index = s1_index;
    //         return;
    //     }
    //     top_red_edge_index = s1_index;
    //     bottom_red_edge_index = s2_index;
    //     return;

    // case Y_ASCENDING:
    //     break;
    // case Y_DESCENDING:
    //     break;
    // default:
    //     break;
    // }
}