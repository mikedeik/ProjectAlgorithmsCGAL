#if !defined(INCREMENTING)
#define INCREMENTING

#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <queue>
#include "util.h"

using std::vector;

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point;
typedef CGAL::Polygon_2<Kernel> Polygon;
typedef Kernel::Segment_2 Segment;

void incrementing_algorithm(vector<Point> points);

class Incrementing
{
private:
    vector<Point> points;
    Polygon polygon;
    Sorter sorter;
    int point_position;
    vector<Segment> edges;
    int top_red_edge_index;
    int bottom_red_edge_index;
    void set_red_edges(int s1_index, int s2_index);

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
