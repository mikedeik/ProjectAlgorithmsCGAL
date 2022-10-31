#if !defined(VISIBLEEDGE)
#define VISIBLEEDGE

#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include "util.h"
#include <CGAL/convex_hull_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point;
typedef CGAL::Polygon_2<Kernel> Polygon;
typedef Kernel::Segment_2 Segment;
typedef std::vector<Point> Points;

class VisibleEdge
{
private:
    std::vector<Point> points, convex_hull_points; // result has the convex hull
    Polygon polygon;
    vector<Segment> edges;
    void Create_Convex_Hull();

public:
    VisibleEdge(std::vector<Point> points);
    ~VisibleEdge();
    void Add_Point(Point);
    void Print_Edges();
};

#endif