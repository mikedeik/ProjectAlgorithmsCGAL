#include "util.h"

// sorting vasi x fthinousa
bool Sort_Desc_X(Point p1, Point p2)
{
    return CGAL::compare_x(p1, p2) < 0;
}

// sorting vasi x auksousa
bool Sort_Asc_X(Point p1, Point p2)
{
    return CGAL::compare_x(p1, p2) > 0;
}

// sorting vasi y fthinousa
bool Sort_Desc_Y(Point p1, Point p2)
{
    return CGAL::compare_y(p1, p2) < 0;
}

// sorting vasi y auksousa
bool Sort_Asc_Y(Point p1, Point p2)
{
    return CGAL::compare_y(p1, p2) > 0;
}

void get_points_from_file(string filename, vector<Point> *points)
{
    string temp;
    std::ifstream myfile(filename);
    Point p;
    while (getline(myfile, temp))
    {
        if (temp[0] == '#')
        {
            continue;
        }

        temp.erase(0, 2);
        std::istringstream input(temp);
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