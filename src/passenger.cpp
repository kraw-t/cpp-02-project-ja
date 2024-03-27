#include "passenger.h"

Passenger::Passenger(int id, int weight) : id(id), weight(weight) {}

int Passenger::get_id() const
{
    return id;
}

int Passenger::get_weight() const
{
    return weight;
}