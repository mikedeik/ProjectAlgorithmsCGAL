#include "util.h"

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