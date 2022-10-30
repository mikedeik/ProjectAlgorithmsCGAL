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
    cout << "New point: " << points[point_position] << "\n";
    cout << "---- Red Edges ----\n";
    for (Segment edge : red_edges)
    {
        cout << edge << "\n";
    }

    find_visible_edges(points[point_position]);
    red_edges.clear();
    ch_polygon.clear();
    convex_hull.clear();
    // sto telos ftiaxnw to neo convex hull kai paw na prosthesw to epomeno simeio
    point_position++;
    CGAL::convex_hull_2(points.begin(), points.begin() + point_position, std::back_inserter(convex_hull));
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
    }
    cout << "--------- Convex Hull edges -----------\n";
    for (Segment edge : ch_polygon.edges())
    {
        cout << edge << "\n";
    }

    cout << "point position is " << position_to_start << "\n";
    // ksekinaw apo to proigoumeno simeio kai paw pros ta mprosta

    for (auto edge_it = ch_polygon.edges().begin() + position_to_start; edge_it != ch_polygon.edges().end(); ++edge_it)
    {
        bool intersects = 0;

        // ftiaxnw trigwno me tin kainourgia akmi
        Triangle t((*edge_it).source(), p, (*edge_it).target());

        // gia kathe epomeni akmi tsekarw an kanoun intersect

        // const Polygon::Edges& ch_edges = ch_polygon.edges();
        // check_intersection_BFS(ch_edges, )

        // gia kathe epomeni tou akmi koitaw an tin kovei (pou simainei pws den einai orati)
        for (auto intersect_it = edge_it + 1; intersect_it != ch_polygon.edges().end(); ++intersect_it)
        {
            // tha doume an to result einai apla ena simeio (diladi to )
            const Segment to_test = *intersect_it;

            if (check_intersection(t, to_test))
            {
                cout << "intersects\n";
                intersects = 1;
                break;
            }
        }
        // opote paw stin epomeni
        if (intersects)
        {
            break;
        }

        // paw twra pros tin antitheti kateuthinsi kai kanw to idio
        for (auto intersect_it = edge_it - 1; intersect_it != ch_polygon.edges().begin(); --intersect_it)
        {
            const Segment to_test = *intersect_it;
            if (check_intersection(t, to_test))
            {
                cout << "intersects\n";
                intersects = 1;
                break;
            }
        }

        if (intersects)
        {
            break;
        }
        // an den kovei kamia akmi tote einai kokkini
        red_edges.push_back(*edge_it);
    }

    // ksekinaw apo to proigoumeno simeio kai paw pros ta pisw
    // edw kathe fora pou vazw ena kainourio red edge thelw na mpainei prin apo auta pou exw prosthesei gia na einai sorted

    for (auto edge_it = ch_polygon.edges().begin() + position_to_start - 1; edge_it != ch_polygon.edges().begin(); --edge_it)
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
            if (check_intersection(t, to_test))
            {
                cout << "intersects\n";
                intersects = 1;
                break;
            }
        }
        if (intersects)
        {
            break;
        }
        // to idio kai gia tis proigoumenes
        for (auto intersect_it = edge_it - 1; intersect_it != ch_polygon.edges().begin(); --intersect_it)
        {
            const Segment to_test = *intersect_it;
            if (check_intersection(t, to_test))
            {
                cout << "intersects\n";
                intersects = 1;
                break;
            }
        }

        if (!intersects)
        {
            red_edges.insert(red_edges.begin(), *edge_it);
        }
    }
}

const void Incrementing::find_visible_edges(Point p)
{
    Point starting_point = red_edges[0].source();
    Point end_point = red_edges[red_edges.size() - 1].target();
    cout << "Starting Point: " << starting_point << "\n";
    cout << "Ending Point: " << end_point << "\n";

    vector<Segment> edges_to_check;
    for (auto it = polygon.vertices().begin(); it != polygon.vertices().end(); it++)
    {
        if (*it == end_point)
        {
            break;
        }

        if (*it == starting_point)
        {
            edges_to_check.push_back(Segment(*it, *(it + 1)));
            starting_point = *(it + 1);
        }
    }

    cout << "edges to check\n";

    for (Segment e : edges_to_check)
    {
        cout << e << "\n";
    }
    // TODO find the visible edges and pick one at random
}