#include <iostream>
// #include <CGAL/Exact_predicates_exact_constructions_kernel.h>
// #include <CGAL/Polygon_2.h>
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

typedef Kernel::Triangle_2 Triangle;

Incrementing::Incrementing(vector<Point> inc_points, Sorter inc_sorter) : points(inc_points),
                                                                          sorter(inc_sorter),
                                                                          top_red_edge_index(-1),
                                                                          bottom_red_edge_index(-1)
{
    sort_points(&points, sorter);

    if (CGAL::compare_y(points[2], points[1]) == 1)
    {
        edges.push_back(Segment(points[0], points[1]));
        edges.push_back(Segment(points[1], points[2]));
        edges.push_back(Segment(points[2], points[0]));
        set_red_edges(1, 2);
    }
    else
    {
        edges.push_back(Segment(points[0], points[2]));
        edges.push_back(Segment(points[2], points[1]));
        edges.push_back(Segment(points[1], points[0]));
        set_red_edges(0, 1);
    }

    // int index = 1;
    // while (index < 3)
    // {

    //     edges.push_back(Segment(points[index - 1], points[index]));
    //     index++;
    // }
    // edges.push_back(Segment(points[index - 1], points[0]));
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
    // keep timer
    auto start = high_resolution_clock::now();

    while (point_position < points.size())
    {

        Point t_source = edges[bottom_red_edge_index].source();
        Point t_target = edges[bottom_red_edge_index].target();
        Triangle triangle(t_source, points[point_position], t_target);

        Segment s1(t_source, points[point_position]);
        Segment s2(t_target, points[point_position]);

        // an einai syneutheiaka epeidi exw sortarei kai stis duo kateythinseis dialegoume tin top akmi
        if (!CGAL::compare_x(points[point_position], points[point_position - 1]))
        {
            t_source = edges[top_red_edge_index].source();
            t_target = edges[top_red_edge_index].target();

            edges.insert(edges.begin() + top_red_edge_index, Segment(points[point_position], t_target));
            edges.insert(edges.begin() + top_red_edge_index, Segment(t_source, points[point_position]));

            edges.erase(edges.begin() + top_red_edge_index + 2);
            set_red_edges(top_red_edge_index, top_red_edge_index + 1);
            point_position++;
            continue;
        }
        // an to trigwno pou ftiaxnetai me tin katw akmi kovei tin panw
        // tha paroume tin panw akmi
        // alliws tha paroume tin katw

        const auto result = CGAL::intersection(triangle, edges[top_red_edge_index]);

        if (result)
        {
            if (const Segment *s = boost::get<Segment>(&*result))
            {
                t_source = edges[top_red_edge_index].source();
                t_target = edges[top_red_edge_index].target();

                edges.insert(edges.begin() + top_red_edge_index, Segment(points[point_position], t_target));
                edges.insert(edges.begin() + top_red_edge_index, Segment(t_source, points[point_position]));

                edges.erase(edges.begin() + top_red_edge_index + 2);
                set_red_edges(top_red_edge_index, top_red_edge_index + 1);
            }
            else
            {
                edges.insert(edges.begin() + bottom_red_edge_index, Segment(points[point_position], t_target));
                edges.insert(edges.begin() + bottom_red_edge_index, Segment(t_source, points[point_position]));

                edges.erase(edges.begin() + bottom_red_edge_index + 2);
                set_red_edges(bottom_red_edge_index, bottom_red_edge_index + 1);
            }
        }
        point_position++;
    }

    // kwdikas gia na dimiourgisoume arxeio eksodou
    std::ofstream f("data.txt");
    CGAL::IO::set_binary_mode(f);

    // vazw tis koryfes sto polygono
    for (Segment edge : edges)
    {
        polygon.push_back(edge.source());
    }

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<seconds>(stop - start);

    cout << "Time taken by function: "
         << duration.count() << " seconds"
         << "\n";

    // ftiaxnw etsi tin eksodo gia sxediasmo se google colab
    for (Point p : polygon.vertices())
    {
        f << "[";
        f << p.x();
        f << " , ";
        f << p.y();
        f << "],";
        f << "\n";
    }
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
}
