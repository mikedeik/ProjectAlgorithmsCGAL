#include <iostream>
#include <sstream>
#include <fstream>
#include <queue>
#include "incrementing.h"
#include <boost/optional/optional_io.hpp>
#include <chrono>

using std::cout;
using std::priority_queue;
using std::vector;
using namespace std::chrono;

Incrementing::Incrementing(vector<Point> inc_points, Sorter inc_sorter) : points(inc_points),
                                                                          sorter(inc_sorter)
{

    CGAL::convex_hull_2(points.begin(), points.begin() + 3, std::back_inserter(convex_hull));

    for (Point p : convex_hull)
    {
        cout << p << "\n";
    }

    // TODO arxika suneutheiaka simeia

    for (auto it = points.begin(); it < points.begin() + 3; ++it)
        polygon.push_back(*it);

    point_position = 3;
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
    find_red_edges(points[point_position]);

    for (Segment edge : red_edges)
    {
        cout << edge << "\n";
    }
}

const void Incrementing::testing()
{
}

const void Incrementing::find_red_edges(Point p)
{
    // kathe fora pou prospathw na valw ena simeio to proigoumeno tou tha einai sigoura koryfi tou CH
    int position = 0;
    int position_to_start = 0;

    for (auto it = convex_hull.begin(); it != convex_hull.end(); ++it)
    {
        // create the convex hull polygon
        ch_polygon.push_back(*it);

        // tha kratiwsw ti thesi tou proigoumenou simeiou sto convex hull
        if (!CGAL::compare_yx(points[point_position - 1], *it))
        {
            position_to_start = position;
        }
        position++;
        cout << "point position is " << position_to_start << "\n";
    }
    // ksekinaw apo to proigoumeno simeio kai paw pros ta pisw

    for (auto edge_it = ch_polygon.edges().begin() + position_to_start; edge_it != ch_polygon.edges().end(); ++edge_it)
    {
        bool intersects = 0;

        Triangle t((*edge_it).source(), p, (*edge_it).target());

        // gia kathe epomeni akmi tsekarw an kanoun intersect

        // const Polygon::Edges& ch_edges = ch_polygon.edges();
        // check_intersection_BFS(ch_edges, )

        for (auto intersect_it = edge_it + 1; intersect_it != ch_polygon.edges().end(); ++intersect_it)
        {
            // tha doume an to result einai apla ena simeio (diladi to )
            const Segment to_test = *intersect_it;
            const auto result = CGAL::intersection(t, to_test);

            if (result)
            {
                if (const Segment *s = boost::get<Segment>(&*result))
                {
                    cout << "intersects\n";
                    intersects = 1;
                    break;
                }
            }
        }
        if (intersects)
        {
            break;
        }

        for (auto intersect_it = edge_it - 1; intersect_it != ch_polygon.edges().begin(); --intersect_it)
        {
            const Segment to_test = *intersect_it;
            const auto result = CGAL::intersection(t, to_test);
            if (result)
            {
                if (const Segment *s = boost::get<Segment>(&*result))
                {
                    cout << "intersects\n";
                    intersects = 1;
                    break;
                }
            }
        }

        if (!intersects)
        {
            red_edges.push_back(*edge_it);
        }
    }

    // ksekinaw apo to proigoumeno simeio kai paw pros ta pisw

    for (auto edge_it = ch_polygon.edges().begin() + position_to_start; edge_it != ch_polygon.edges().begin(); --edge_it)
    {
        bool intersects = 0;

        Triangle t((*edge_it).source(), p, (*edge_it).target());

        // gia kathe epomeni akmi tsekarw an kanoun intersect

        // const Polygon::Edges& ch_edges = ch_polygon.edges();
        // check_intersection_BFS(ch_edges, )

        for (auto intersect_it = edge_it + 1; intersect_it != ch_polygon.edges().end(); ++intersect_it)
        {
            // tha doume an to result einai apla ena simeio (diladi to )
            const Segment to_test = *intersect_it;
            const auto result = CGAL::intersection(t, to_test);

            if (result)
            {
                if (const Segment *s = boost::get<Segment>(&*result))
                {
                    cout << "intersects\n";
                    intersects = 1;
                    break;
                }
            }
        }
        if (intersects)
        {
            break;
        }

        for (auto intersect_it = edge_it - 1; intersect_it != ch_polygon.edges().begin(); --intersect_it)
        {
            const Segment to_test = *intersect_it;
            const auto result = CGAL::intersection(t, to_test);
            if (result)
            {
                if (const Segment *s = boost::get<Segment>(&*result))
                {
                    cout << "intersects\n";
                    intersects = 1;
                    break;
                }
            }
        }

        if (!intersects)
        {
            red_edges.push_back(*edge_it);
        }
    }

    // for (auto itt = ch_polygon.vertices().begin() + position_to_start; itt < ch_polygon.vertices().end(); ++itt)
    // {
    //     Triangle triangle(*itt, p, *(itt + 1));
    //     const auto result = CGAL::intersection(triangle, Segment(*itt , *(itt + 1)));
    //     if (CGAL::intersection())
    //     {
    //         /* code */
    //     }

    // }
}