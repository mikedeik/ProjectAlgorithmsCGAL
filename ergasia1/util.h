#if !defined(UTIL)
#define UTIL

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <queue>

enum Sorter
{
    X_DESCENDING,
    X_ASCENDING,
    Y_DESCENDING,
    Y_ASCENDING
};

using std::cout;
using std::queue;
using std::string;
using std::vector;

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point;
typedef Kernel::Segment_2 Segment;
typedef Kernel::Triangle_2 Triangle;

// adds the points
void get_points_from_file(string filename, vector<Point> *points);
void sort_points(vector<Point> *points, Sorter sorter);
bool check_intersection_BFS(vector<Segment> edges);

#endif // UTIL
