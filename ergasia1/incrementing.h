#if !defined(INCREMENTING)
#define INCREMENTING

#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <queue>
#include "util.h"

using std::vector;

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point;
typedef Kernel::Segment_2 Segment;

void incrementing_algorithm(vector<Point> points);

class Incrementing
{
private:
    vector<Point> points;
    vector<Segment> segments;
    Sorter sorter;

public:
    Incrementing(vector<Point> inc_points, Sorter inc_sorter);
    ~Incrementing();
    void Sort_Points();
    const void Print_Points();
};

#endif // INCREMENTING
