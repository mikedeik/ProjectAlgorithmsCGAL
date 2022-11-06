#if !defined(UTIL)
#define UTIL

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <queue>
#include <iostream>
#include <fstream>
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
typedef CGAL::Polygon_2<Kernel> Polygon;
typedef std::vector<Point>::const_iterator PointIterator;
typedef std::vector<Segment>::const_iterator EdgeIterator;

/*  string filename : to path enos arxeiou me ta points
    vector<Point>* points : deiktis se vector pou krataei points
    diavazei ta points apo arxeio kai ta thetei mesa se dosmeno vector*/
void get_points_from_file(string filename, vector<Point> *points);

/*
    string filename : to path enos arxeiou opou tha graftei to output
    dhmiourgei arxeio me to output twn files
*/
void output_to_file(string filename);

/*  vector<Point>* points : deiktis se vector pou krataei points
    Sorter: sorter : kathorizei pws theloume na sortaristei to array
    analogws ton Sorter sortarei to array*/
void sort_points(vector<Point> *points, Sorter sorter);

/*Unsued*/
bool check_intersection_BFS(vector<Segment> edges);

/*  Epistrefei an to trigwno t kai to segment e temnontai*/
bool check_intersection(Triangle t, Segment e);

/* pairnei ws orisma ena polygwno(tou kyrtou perivlimatos), mia thesi (int), ena simeio, kai ena pointer se array apo Segments
   dianyei to polygwno me fora CCW ksekinwntas apo do to position_to_start edge kai prosthei tis
   kokkines akmes apo to shmeio p pros to kyrto perivlima */
void traverse_cw(const Polygon ch_polygon, int position_to_start, Point p, vector<Segment> *red_edges);

// pairnei ws orisma ena polygwno(tou kyrtou perivlimatos), mia thesi (int), ena simeio, kai ena pointer se array apo Segments
// dianyei to polygwno me fora CCW ksekinwntas apo do to position_to_start edge kai prosthei tis
// kokkines akmes apo to shmeio p pros to kyrto perivlima
void traverse_ccw(const Polygon ch_polygon, int position_to_start, Point p, vector<Segment> *red_edges);

void print_to_file(const Polygon ch_polygon,string filename,int time);

#endif // UTIL
