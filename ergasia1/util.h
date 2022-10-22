#if !defined(UTIL)
#define UTIL

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <sstream>
#include <fstream>

using std::cout;
using std::string;
using std::vector;

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Polygon_2<Kernel> Polygon;
typedef Kernel::Point_2 Point;
typedef Kernel::Segment_2 Segment_2;

void get_points_from_file(string filename, vector<Point> *points);

#endif // UTIL
