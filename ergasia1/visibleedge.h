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
    std::vector<Point> points,result;//result has the convex hull
    Polygon polygon;

public:
    VisibleEdge(std::vector<Point> points);
    ~VisibleEdge();
    void Create_ConvexHull();
    void Add_Point(Point);
};

#endif