#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include "util.h"
#include "incrementing.h"

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point;

int main(int argc, char *argv[])
{

    vector<Point> points;
    get_points_from_file("data/images/euro-night-0000010.instance", &points);
    incrementing_algorithm(points);

    return 0;
}