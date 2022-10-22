#if !defined(INCREMENTING)
#define INCREMENTING

#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
using std::vector;

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point;

void incrementing_algorithm(vector<Point> points);

#endif // INCREMENTING
