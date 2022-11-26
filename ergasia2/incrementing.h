#if !defined(INCREMENTING)
#define INCREMENTING

#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <queue>
#include "util.h"
#include <CGAL/convex_hull_2.h>

using std::vector;

typedef Kernel::Segment_2 Segment;

// void incrementing_algorithm(vector<Point> points);

// Klasi tou Incrementing algorithm
class Incrementing
{
private:
    vector<Point> points, convex_hull;        // duo Point vectors ena gia ta arxika points kai ena gia ta convex hull points
    Polygon polygon, ch_polygon;              // duo polygwna ena gia tin teliki polygwniki grammi ki ena gia ton CH
    Sorter sorter;                            // o sorter pou kathorizei to pws tha sortaristoun ta Points
    int point_position;                       // enas akeraios pou krataei tin thesi tou Point pou tha prosthesoume sto arxiko array
    vector<Segment> red_edges, visible_edges; // duo Segment arrays ena gia ta red edges ki ena gia ta visible edges tis polygwnikis grammis
    string output_file;
    int time;

    /*  Pairnei orisma ena Point p
        Private synartisi pou vriskei tis kokkines akmes*/
    const void find_red_edges(Point p);

    /*  Pairnei orisma ena Point p
        Private synartisi pou vriskei tis orates akmes tis polygwnikis grammis*/
    const void find_visible_edges(Point p);

public:
    // constructor
    Incrementing(vector<Point> inc_points, Sorter inc_sorter, string output);
    // destructor
    ~Incrementing();

    /* Typwnei ta Points tis polygwnikis grammis */
    const void Print_Points();

    /* Typwnei ta edges tis polygwnikis grammis */
    const void Print_Edges();

    /* Epistrefei an to polygwno einai aplo*/
    const bool Simple();

    /* Dhmiourgei tin polugwniki grammi */
    const void Create_Polygon_Line();

    const void Print_Polygon();

    /* Epistrefei to neo polygwno */
    const Polygon Get_Simple_Polygon();
};

#endif // INCREMENTING
