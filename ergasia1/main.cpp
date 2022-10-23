#include <iostream>
#include "util.h"
#include "incrementing.h"
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
    get_points_from_file("data/images/euro-night-0000010.instance", &points);

    Incrementing algo = Incrementing(points, sorter[inp]);
    algo.Print_Points();
    Point p(2, 3);

    return 0;
}