#if !defined(UTIL)
#define UTIL

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

enum Sorter
{
    X_DESCENDING,
    X_ASCENDING,
    Y_DESCENDING,
    Y_ASCENDING
};

using std::cout;
using std::string;
using std::vector;

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point;

// adds the points
void get_points_from_file(string filename, vector<Point> *points);
void sort_points(vector<Point> *points, Sorter sorter);

#endif // UTIL
