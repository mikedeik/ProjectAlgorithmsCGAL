#include "SimulatedAnnealing.h"

SimulatedAnnealing::SimulatedAnnealing(Polygon polygon, AnnealingType an_type) : starting_polygon(polygon),
                                                                                 type(an_type)
{
}

SimulatedAnnealing::~SimulatedAnnealing()
{

    starting_polygon.clear();
    new_polygon.clear();
}
