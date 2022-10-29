#if !defined(INCREMENTING)
#define INCREMENTING

#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <queue>
#include "util.h"
#include <CGAL/convex_hull_2.h>

using std::vector;

typedef CGAL::Polygon_2<Kernel> Polygon;
typedef Kernel::Segment_2 Segment;

void incrementing_algorithm(vector<Point> points);

class Incrementing
{
private:
    vector<Point> points, convex_hull;
    Polygon polygon, ch_polygon;
    Sorter sorter;
    int point_position;
    vector<Segment> edges, red_edges;
    const void find_red_edges(Point p);

public:
    Incrementing(vector<Point> inc_points, Sorter inc_sorter);
    ~Incrementing();
    const void Print_Points();
    const void Print_Edges();
    void Add_Point(Point p1);
    const bool Simple();
    const void Create_Polygon_Line();
    const void testing();
    void Create_Polygon_Line_2();
};

#endif // INCREMENTING
