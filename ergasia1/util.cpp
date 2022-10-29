#include "util.h"

// sorting vasi x fthinousa
// an einai suneutheiaka sort kata y
bool Sort_Desc_X(Point p1, Point p2)
{

    if (!(CGAL::compare_x(p1, p2)))
    {
        return CGAL::compare_y(p1, p2) < 0;
    }

    return CGAL::compare_x(p1, p2) < 0;
}

// sorting vasi x auksousa
// an einai suneutheiaka sort kata y
bool Sort_Asc_X(Point p1, Point p2)
{
    if (!(CGAL::compare_x(p1, p2)))
    {
        return CGAL::compare_y(p1, p2) > 0;
    }
    return CGAL::compare_x(p1, p2) > 0;
}

// sorting vasi y fthinousa
bool Sort_Desc_Y(Point p1, Point p2)
{
    if (!(CGAL::compare_y(p1, p2)))
    {
        return CGAL::compare_x(p1, p2) < 0;
    }
    return CGAL::compare_y(p1, p2) < 0;
}

// sorting vasi y auksousa
bool Sort_Asc_Y(Point p1, Point p2)
{
    if (!(CGAL::compare_y(p1, p2)))
    {
        return CGAL::compare_x(p1, p2) > 0;
    }
    return CGAL::compare_y(p1, p2) > 0;
}

void get_points_from_file(string filename, vector<Point> *points)
{
    string line;
    std::ifstream myfile(filename);
    Point p;
    while (getline(myfile, line))
    {
        if (line[0] == '#')
        {
            continue;
        }
        std::istringstream input(line);
        int to_drop;
        input >> to_drop;
        input >> p;
        points->push_back(p);
    }
}

void sort_points(vector<Point> *points, Sorter sorter)
{
    switch (sorter)
    {
    case X_ASCENDING:
        sort(points->begin(), points->end(), Sort_Asc_X);
        break;
    case X_DESCENDING:
        sort(points->begin(), points->end(), Sort_Desc_X);
        break;
    case Y_ASCENDING:
        sort(points->begin(), points->end(), Sort_Asc_Y);
        break;
    case Y_DESCENDING:
        sort(points->begin(), points->end(), Sort_Desc_Y);
        break;
    default:
        break;
    }
}

bool check_intersection_BFS(vector<Segment> edges, int position, Triangle t)
{
    queue<Segment> bag;
    bool visited[edges.size()];
    Segment edge;
    visited[position] = true;

    bag.push(edges[position]);

    int is_first_edge = 1;
    while (!bag.empty())
    {
        edge = bag.front();
        bag.pop();

        if (!is_first_edge)
        {
            const auto result = CGAL::intersection(t, edge);
            if (result)
            {
                if (const Segment *s = boost::get<Segment>(&*result))
                {
                    cout << "intersects\n";
                    return 1;
                }
            }
        }

        if (position == edges.size() - 1 || !position)
        {
            continue;
        }

        if (!visited[position + 1])
        {
            visited[position + 1] = true;
            bag.push(edges[position + 1]);
        }

        if (!visited[position - 1])
        {
            visited[position + 1] = true;
            bag.push(edges[position + 1]);
        }

        is_first_edge = 0;
    }

    return 0;
}