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

using namespace std::chrono;

/* O constructor pairnei san orisma to array apo points kai ton Sorter*/
Incrementing::Incrementing(vector<Point> inc_points, Sorter inc_sorter, string output) : points(inc_points),
                                                                                         sorter(inc_sorter),
                                                                                         output_file(output)
{
    // kanei stort ta points analoga me ton sorter
    sort_points(&points, sorter);

    // dhmiourgei to Convex Hull
    CGAL::convex_hull_2(points.begin(), points.begin() + 3, std::back_inserter(convex_hull));

    // kai analogws arxikopoiei tin arxiki polygwnikh grammh
    for (Point point : convex_hull)
    {
        polygon.push_back(point);
    }
    // thetei point_position se 3 gia na ksekinisei apo auto to simeio
    point_position = 3;
    time=0;
}

// adeiazw ola ta vectors gia dealocate memmory
Incrementing::~Incrementing()
{
    polygon.clear();
    ch_polygon.clear();
    points.clear();
    convex_hull.clear();
    visible_edges.clear();
    red_edges.clear();
}

const void Incrementing::Print_Points()
{
    cout << "==== Polygon Points ====\n";
    for (Point point : polygon.vertices())
    {
        cout << point << "\n";
    }
}

const void Incrementing::Print_Edges()
{
    cout << "==== Polygon Edges ====\n";
    for (Segment edge : polygon.edges())
    {
        cout << edge << "\n";
    }
}

const bool Incrementing::Simple()
{
    return polygon.is_simple();
}

/* Dhmiourgia polygwnikis grammis */
const void Incrementing::Create_Polygon_Line()
{
    auto start = high_resolution_clock::now();
    while (point_position < points.size())
    {
        find_red_edges(points[point_position]);

        find_visible_edges(points[point_position]);
        int random_edge = random() % visible_edges.size();

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

        visible_edges.clear();
        red_edges.clear();
        ch_polygon.clear();
        convex_hull.clear();
        // sto telos ftiaxnw to neo convex hull kai paw na prosthesw to epomeno simeio
        point_position++;
        CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convex_hull));
    }

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<seconds>(stop - start);
    time=duration.count();
    cout << "Time taken by function: "
         << duration.count() << " seconds"
         << "\n";
}

const void Incrementing::find_red_edges(Point p)
{
    // kathe fora pou prospathw na valw ena simeio to proigoumeno tou tha einai sigoura koryfi tou CH

    int position_to_start = 0;

    for (auto it = convex_hull.begin(); it != convex_hull.end(); ++it)
    {

        // tha kratiwsw ti thesi tou proigoumenou simeiou sto convex hull
        ch_polygon.push_back(*it);
        // if (*it == points[point_position - 1])
        // {
        //     position_to_start = it - ch_polygon.begin();
        // }
    }

    PointIterator position_pointer = find(ch_polygon.begin(), ch_polygon.end(), points[point_position - 1]);
    position_to_start = position_pointer - ch_polygon.begin();

    // cout << "--------- Convex Hull edges -----------\n";
    // for (Segment edge : ch_polygon.edges())
    // {
    //     cout << edge << "\n";
    // }
    // getchar();
    // ksekinaw apo to proigoumeno simeio kai paw pros ta mprosta

    traverse_ccw(ch_polygon, position_to_start, p, &red_edges);

    if (red_edges.size() == ch_polygon.size())
    {
        return;
    }

    // ksekinaw apo to proigoumeno simeio kai paw pros ta pisw
    // edw kathe fora pou vazw ena kainourio red edge thelw na mpainei prin apo auta pou exw prosthesei gia na einai sorted

    traverse_cw(ch_polygon, position_to_start, p, &red_edges);
}

const void Incrementing::find_visible_edges(Point p)
{

    // cout << "we get in visible edges\n";

    Point starting_point = red_edges[0].source();
    Point end_point = red_edges[red_edges.size() - 1].target();
    // cout << "Starting Point: " << starting_point << "\n";
    // cout << "Ending Point: " << end_point << "\n";
    int first, last;

    // cout << "---- Polygon Edges ----\n";
    // for (Segment edge : polygon.edges())
    // {
    //     cout << edge << "\n";
    // }
    // vriskw tis theseis twn simeiwn pou tha prepei na ksekinisw kai na stamatisw apo tis kokkines akmes
    for (auto it = polygon.edges_begin(); it != polygon.edges_end(); ++it)
    {

        if ((*it).target() == end_point)
        {
            last = it - polygon.edges_begin();
            // cout << "last is : " << last << "\n";
        }

        if ((*it).source() == starting_point)
        {
            first = it - polygon.edges_begin();
            // cout << "first is : " << first << "\n";
        }

        // an yparxei mono ena edge den xreiazetai na kanoume kanenan elegxo
        if (first == last)
        {
            visible_edges.push_back(*it);
            return;
        }
    }

    int go_to_start = 0;
    int go_to_next = 0;

    // ksekinaw apo to prwto mov simeio kai paw mexri to deutero
    for (auto itt = polygon.edges_begin() + first; itt != polygon.edges_begin() + last + 1; ++itt)
    {

        bool intersects = 0;
        int count = first;
        for (auto intersect_it = polygon.edges_begin() + first; intersect_it != polygon.edges_begin() + last + 1; ++intersect_it)
        {

            if (*itt == *intersect_it)
            {
                if (intersect_it == polygon.edges_end() - 1)
                {
                    go_to_next = 1;
                    break;
                }
                continue;
            }

            if (check_intersection(Triangle((*itt).source(), p, (*itt).target()), *intersect_it))
            {
                intersects = 1;
                break;
            }

            if (intersect_it == polygon.edges_end() - 1)
            {

                go_to_next = 1;
                break;
            }
        }

        if (go_to_next)
        {
            for (auto intersect_it = polygon.edges_begin(); intersect_it != polygon.edges_begin() + last + 1; ++intersect_it)
            {
                if (*itt == *intersect_it)
                {
                    if (intersect_it == polygon.edges_end() - 1)
                    {
                        go_to_next = 1;
                        break;
                    }
                    continue;
                }

                if (check_intersection(Triangle((*itt).source(), p, (*itt).target()), *intersect_it))
                {
                    // cout << "intersects\n";
                    intersects = 1;
                    break;
                }
            }
        }

        // an den kovw kamia akmi epestrepse
        if (!intersects)
        {
            visible_edges.push_back(*itt);
        }

        if (itt == polygon.edges_end() - 1)
        {
            go_to_start = 1;
            break;
        }
    }

    if (go_to_start)
    {
        // cout << "we are in this case\n";
        // cout << "last is " << last << "\n";

        // getchar();

        for (auto itt = polygon.edges_begin(); itt != polygon.edges_begin() + last + 1; ++itt)
        {
            // cout << "in here";
            bool intersects = 0;

            for (auto intersect_it = polygon.edges_begin() + first; intersect_it != polygon.edges_begin() + last + 1; ++intersect_it)
            {
                if (*itt == *intersect_it)
                {
                    if (intersect_it == polygon.edges_end() - 1)
                    {
                        go_to_next = 1;
                        break;
                    }
                    continue;
                }

                if (check_intersection(Triangle((*itt).source(), p, (*itt).target()), *intersect_it))
                {
                    // cout << "intersects\n";
                    intersects = 1;
                    break;
                }

                if (intersect_it == polygon.edges_end() - 1)
                {
                    go_to_next = 1;
                    break;
                }
            }

            if (go_to_next)
            {
                for (auto intersect_it = polygon.edges_begin(); intersect_it != polygon.edges_begin() + last + 1; ++intersect_it)
                {
                    if (*itt == *intersect_it)
                    {
                        if (intersect_it == polygon.edges_end() - 1)
                        {
                            go_to_next = 1;
                            break;
                        }
                        continue;
                    }

                    if (check_intersection(Triangle((*itt).source(), p, (*itt).target()), *intersect_it))
                    {
                        // cout << "intersects\n";
                        intersects = 1;
                        break;
                    }
                }
            }
            // an den kovw kamia akmi epestrepse
            if (!intersects)
            {
                visible_edges.push_back(*itt);
            }
        }
    }
}

const void Incrementing::Print_Polygon()
{
    if (output_file != "")
    {
        print_to_file(polygon, output_file,time);
    }
    else
    {
        for (Point p : polygon)
        {
            cout << p << "\n";
        }
        for (Segment edge : polygon.edges())
        {
            cout << edge << "\n";
        }
        cout << "incrementing algorithm"<< "\n";
        cout << "area" << polygon.area() << "\n";
        cout << "ratio"<<"\n";
    }
}