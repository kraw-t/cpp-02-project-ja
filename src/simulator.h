#pragma once

#include "building.h"

#include <string>
#include <vector>
#include <map>

class Simulator
{
public:
    Simulator(int millisecondsDelay);

public:
    void run(const std::string &filename);

private:
    Building getBuildingFromFile(std::ifstream &file);
    std::vector<Passenger> getPassengersFromFile(std::ifstream &file);
    std::vector<std::map<std::string, int>> getElevatorInfoFromFile(std::ifstream &file);

private:
    int millisecondsDelay;
};