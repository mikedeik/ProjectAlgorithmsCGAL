#include <iostream>
#include <sstream>
#include <fstream>
#include <queue>
#include "incrementing.h"
#include <boost/optional/optional_io.hpp>
#include <chrono>
#include <CGAL/ch_selected_extreme_points_2.h>

using std::cout;
using std::priority_queue;
using std::vector;
typedef std::vector<Point>::const_iterator PointIterator;
using namespace std::chrono;

Incrementing::Incrementing(vector<Point> inc_points, Sorter inc_sorter) : points(inc_points),
                                                                          sorter(inc_sorter)
{
    sort_points(&points, sorter);
    cout << "INIT\n";

    polygon.push_back(points[2]);
    polygon.push_back(points[0]);
    polygon.push_back(points[1]);

    CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convex_hull));

    // TODO arxika suneutheiaka simeia

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
    auto start = high_resolution_clock::now();
    while (point_position < points.size())
    {
        find_red_edges(points[point_position]);
        cout << "New point: " << points[point_position] << "\n";
        cout << "---- Red Edges ----\n";
        for (Segment edge : red_edges)
        {
            cout << edge << "\n";
        }

        find_visible_edges(points[point_position]);
        int random_edge = random() % visible_edges.size();
        cout << "edge to break " << visible_edges[random_edge];

        PointIterator position_to_insert = find(polygon.vertices().begin(), polygon.vertices().end(), visible_edges[random_edge].target());
        int index = position_to_insert - polygon.vertices().begin();

        if (!index)
        {
            polygon.push_back(points[point_position]);
        }
        else
        {
            polygon.insert(polygon.begin() + index, points[point_position]);
        }
        if (!polygon.is_simple())
        {
            cout << "---- Polygon Edges ----\n";
            for (Segment edge : polygon.edges())
            {
                cout << edge << "\n";
            }
            // getchar();
        }

        visible_edges.clear();
        red_edges.clear();
        ch_polygon.clear();
        convex_hull.clear();
        // sto telos ftiaxnw to neo convex hull kai paw na prosthesw to epomeno simeio
        point_position++;
        CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convex_hull));
    }

    auto stop = high_resolution_clock::now();

    // Get duration. Substart timepoints to
    // get duration. To cast it to proper unit
    // use duration cast method
    auto duration = duration_cast<seconds>(stop - start);

    cout << "Time taken by function: "
         << duration.count() << " seconds"
         << "\n";
}

const void Incrementing::testing()
{
}

const void Incrementing::find_red_edges(Point p)
{
    // kathe fora pou prospathw na valw ena simeio to proigoumeno tou tha einai sigoura koryfi tou CH

    int position_to_start;

    for (auto it = convex_hull.begin(); it != convex_hull.end(); ++it)
    {

        // tha kratiwsw ti thesi tou proigoumenou simeiou sto convex hull
        ch_polygon.push_back(*it);
    }

    PointIterator position_pointer = find(ch_polygon.begin(), ch_polygon.end(), points[point_position - 1]);
    position_to_start = position_pointer - ch_polygon.begin();

    cout << "--------- Convex Hull edges -----------\n";
    for (Segment edge : ch_polygon.edges())
    {
        cout << edge << "\n";
    }

    // ksekinaw apo to proigoumeno simeio kai paw pros ta mprosta

    traverse_ccw(ch_polygon, position_to_start, p, &red_edges, sorter);

    if (red_edges.size() == ch_polygon.size())
    {
        return;
    }

    // ksekinaw apo to proigoumeno simeio kai paw pros ta pisw
    // edw kathe fora pou vazw ena kainourio red edge thelw na mpainei prin apo auta pou exw prosthesei gia na einai sorted

    traverse_cw(ch_polygon, position_to_start, p, &red_edges, sorter);

    if (red_edges.size() == 0)
    {
        cout << p;

        for (Segment edge : ch_polygon.edges())
        {
            cout << edge << "\n";
        }
    }
}

const void Incrementing::find_visible_edges(Point p)
{

    cout << "we get in visible edges\n";

    Point starting_point = red_edges[0].source();
    Point end_point = red_edges[red_edges.size() - 1].target();
    cout << "Starting Point: " << starting_point << "\n";
    cout << "Ending Point: " << end_point << "\n";
    int first, last;

    // vriskw tis theseis twn simeiwn pou tha prepei na ksekinisw kai na stamatisw apo tis kokkines akmes

    cout << "---- Polygon Edges ----\n";
    for (Segment edge : polygon.edges())
    {
        cout << edge << "\n";
    }

    for (auto it = polygon.edges_begin(); it != polygon.edges_end(); ++it)
    {
        if ((*it).target() == end_point)
        {
            last = it - polygon.edges_begin();
            cout << "last is : " << last << "\n";
        }

        if ((*it).source() == starting_point)
        {
            first = it - polygon.edges_begin();
            cout << "first is : " << first << "\n";
        }

        if (first == last)
        {
            visible_edges.push_back(*it);
            return;
        }
    }

    int go_to_start = 0;
    int go_to_next = 0;

    for (auto itt = polygon.edges_begin() + first; itt != polygon.edges_begin() + last; ++itt)
    {
        if (itt == polygon.edges_end() - 1)
        {
            cout << "reached end";
            go_to_start = 1;
            break;
        }

        cout << "edge : " << *itt << "\n";

        bool intersects = 0;

        for (auto intersect_it = polygon.edges_begin() + first; intersect_it != polygon.edges_begin() + last; ++intersect_it)
        {
            if (*itt == *intersect_it)
            {
                continue;
            }
            if (check_intersection(Triangle((*itt).source(), p, (*itt).target()), *intersect_it))
            {
                cout << "intersects\n";
                intersects = 1;
                break;
            }
        }
        if (!intersects)
        {
            visible_edges.push_back(*itt);
        }
    }

    if (go_to_start)
    {

        for (auto itt = polygon.edges_begin(); itt != polygon.edges_begin() + last; ++itt)
        {
            cout << "in here";
            bool intersects = 0;

            for (auto intersect_it = polygon.edges_begin() + first; intersect_it != polygon.edges_begin() + last; ++intersect_it)
            {
                if (*itt == *intersect_it)
                {
                    continue;
                }
                if (check_intersection(Triangle((*itt).source(), p, (*itt).target()), *intersect_it))
                {
                    cout << "intersects\n";
                    intersects = 1;
                    break;
                }
            }
            if (!intersects)
            {
                visible_edges.push_back(*itt);
            }
        }
    }
    cout << "---- Visible Edges -----\n";
    for (Segment edge : visible_edges)
    {
        cout << edge << "\n";
    }
    getchar();
    if (visible_edges.size() == 0)
    {
        cout << "no visible edge";
        getchar();
    }
}