#include <iostream>
#include "util.h"
#include "incrementing.h"
#include "visibleedge.h"

#include <map>

using namespace std;

int main(int argc, char **argv)
{

    map<string, Sorter> sorter;

    string inp = argv[1];

    sorter.insert(pair<string, Sorter>("1a", X_ASCENDING));
    sorter.insert(pair<string, Sorter>("1b", X_DESCENDING));
    sorter.insert(pair<string, Sorter>("2a", Y_ASCENDING));
    sorter.insert(pair<string, Sorter>("2b", Y_DESCENDING));
    vector<Point> points;
    // get_points_from_file("data/images/euro-night-0000010.instance", &points);
    get_points_from_file("data/images/euro-night-0000010.instance", &points);

    // get_points_from_file("test.txt", &points);

    Incrementing algo(points, sorter[inp]);
    // algo.Print_Points();
    // cout << "----Edges----\n";
    // algo.Print_Edges();

    // cout << algo.Simple() << "\n";

    algo.Create_Polygon_Line();

    // cout << algo.Simple() << "\n";
    // VisibleEdge algo2(points);
    return 0;
}