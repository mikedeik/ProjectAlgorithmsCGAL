#include <iostream>
#include "visibleedge.h"

VisibleEdge::VisibleEdge(std::vector<Point> points, string output_file)
{
    this->points = points;

    for (Point p : points)
    {
        added_points.push_back(0);
    }

    time = 0;
    Create_Convex_Hull();
    Create_Polygon();
    if (output_file != "")
    {
        print_to_file(polygon, output_file, time);
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
        cout << "visible edge algorithm"
             << "\n";
        cout << "area" << polygon.area() << "\n";
        cout << "ratio"
             << "\n";
    }
}

VisibleEdge::~VisibleEdge()
{
}

void VisibleEdge::Add_Point(Point p1)
{
    polygon.push_back(p1);
}
// Initializes convex_hull_points and adds them to the polygon class
void VisibleEdge::Create_Convex_Hull()
{
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(convex_hull_points));
    std::cout << convex_hull_points.size() << " points on the convex hull" << std::endl;

    int position = 0;

    for (Point point : convex_hull_points)
    {

        int position_of_added = Find_Index_Of_Point_In_Vector(point, points);

        added_points[position_of_added] = 1;

        polygon.push_back(point);
    }
}

void VisibleEdge::Print_Edges()
{
}

bool VisibleEdge::Is_Point_Included_In_Polygonal_Chain(Point p)
{

    for (int i = 0; i < polygon.size(); i++)
    {
        if (polygon[i].x() == p.x() && polygon[i].y() == p.y())
        {
            return 1;
        }
    }
    return 0;
}

void VisibleEdge::Create_Polygon()
{
    auto start = std::chrono::high_resolution_clock::now();

    points_not_in_chain.clear();

    int position = 0;
    for (Point p : points)
    {
        if (added_points[position] == 0)
        {
            points_not_in_chain.push_back(p);
        }
        position++;
    }

    while (points_not_in_chain.size() > 0)
    {

        for (auto it = polygon.edges_begin(); it != polygon.edges_end(); ++it)
        {
            nearest_poits.push_back(Find_Nearest_Point_To_Segment(*it));
        }

        int random_edge_index = random() % (polygon.edges().size() - 1);

        Triangle t((*(polygon.edges_begin() + random_edge_index)).source(), nearest_poits[random_edge_index], (*(polygon.edges_begin() + random_edge_index)).target());

        int intersects = 0;

        for (Segment edge : polygon.edges())
        {

            if (*(polygon.edges_begin() + random_edge_index) == edge)
            {
                continue;
            }

            if (check_intersection(t, edge))
            {
                intersects = 1;
                break;
            }
        }

        if (intersects)
        {
            continue;
        }

        polygon.insert(polygon.begin() + random_edge_index + 1, nearest_poits[random_edge_index]);

        points_not_in_chain.erase(points_not_in_chain.begin() + Find_Index_Of_Point_In_Vector(nearest_poits[random_edge_index], points_not_in_chain));

        nearest_poits.clear();
    }
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    time = duration.count();
}

Point VisibleEdge::Find_Nearest_Point_To_Segment(Segment s)
{
    Point temp = points_not_in_chain[0];
    FT min_dist = CGAL::squared_distance(s, points_not_in_chain[0]);
    for (Point p : points_not_in_chain)
    {
        FT dist = CGAL::squared_distance(s, p);
        if (dist < min_dist)
        {
            min_dist = dist;
            temp = p;
        }
    }
    return temp;
}

int VisibleEdge::Find_Index_Of_Point_In_Vector(Point p, vector<Point> v)
{
    PointIterator it = find(v.begin(), v.end(), p);
    return it - v.begin();
}

int VisibleEdge::Find_Index_In_Polygon(Point p)
{
    PointIterator it = find(polygon.begin(), polygon.end(), p);
    return it - polygon.begin();
}

FT VisibleEdge::Get_Polygon_Area()
{
    return polygon.area();
}

const Polygon VisibleEdge::Get_Simple_Polygon()
{
    return polygon;
}