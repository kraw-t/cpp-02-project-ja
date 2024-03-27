#include "simulator.h"
#include <fstream>
#include <sstream>
#include <utility>
#include <algorithm>
#include <cassert>

std::ifstream openFile(const std::string& filename);

Simulator::Simulator(int millisecondsDelay) : millisecondsDelay(millisecondsDelay) {}

void Simulator::initializeFromFile(const std::string& filename) {
    auto file { openFile(filename) };
    parseElevators(file);
    parsePassengers(file);
    parseInputs(file);
}

std::ifstream openFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Exception! Cannot open file!");
    }

    return file;
}

void Simulator::parseElevators(std::ifstream& file) {
    std::string line;
    std::getline(file, line);
    std::istringstream iss { line };

    int numberOfElevators {};
    int numberOfFloors {};
    int elevatorCapacity {};

    if (!(iss >> numberOfElevators >> numberOfFloors >> elevatorCapacity)) {
        throw std::runtime_error("Exception! Cannot parse the first line of the file!");
    }

    for (size_t i{0}; i < numberOfElevators; ++i){
        elevators_.emplace_back(numberOfFloors, elevatorCapacity);
    }
}

void Simulator::parsePassengers(std::ifstream& file) {
    std::string line;
    std::getline(file, line);
    std::istringstream iss { line };

    int numberOfPassengers {};

    if (!(iss >> numberOfPassengers)) {
        throw std::runtime_error("Exception! Cannot parse the number of passengers!");
    }

    while (numberOfPassengers-- && std::getline(file, line)) {
        iss = std::istringstream { line };

        int id;
        int weight;

        if (!(iss >> id >> weight)) {
            throw std::runtime_error("Exception! Cannot parse the remaining lines of the file!");
        }
        // TODO add duplicated id handling
        if(weight_map_.count(id) == 0){
            weight_map_.insert(std::make_pair(id, weight));
        }
    }
}

void Simulator::parseInputs(std::ifstream& file) {
    std::string line;
    while (std::getline(file, line)) {
        auto iss = std::istringstream { line };

        size_t time {};
        size_t id {};
        size_t pickUpFloor {};
        size_t dropOffFloor {};

        if (!(iss >> time >> id >> pickUpFloor >> dropOffFloor)) {
            throw std::runtime_error("Exception! Cannot parse the remaining lines of the file!");
        }

        // TODO add invalid id handling
        if(weight_map_.count(id) == 0){
            queue_.emplace_back(
                std::make_shared<Person>(id, weight_map_.at(id), time, pickUpFloor, dropOffFloor));
        }
    }
}

void Simulator::run() {
    std::sort(std::begin(queue_), std::end(queue_), [](const auto &a, const auto &b)
              { return a->time_ < b->time_; });

    for(auto & person: queue_){
        assert(person);
        while (current_time_ < person->time_)
        {
            std::for_each(std::begin(elevators_), std::end(elevators_), [](auto &elevator)
                          { elevator.process(); });
        }
        assignElevatorTo(person);
    }

    do{
        std::for_each(std::begin(elevators_), std::end(elevators_), [](auto &elevator)
                      { elevator.process(); });
    } while (std::any_of(std::begin(elevators_), std::end(elevators_), [](const auto &e)
                         { return !e.isIdle(); }));
}

// TODO: if needed optimize elevator selection. 
// Currently choose an elevator which is idle or moving the same direction as person's direction
// and closest to th pick up floor. 
void Simulator::assignElevatorTo(const std::shared_ptr<Person> & person){
    assert(elevators_.size() > 0);

    const auto move_to_opposite_or_already_passed = [&person](const Elevator &e)
    {
        if(person->isGoingUp())
            return e.isGoingDown() || (e.isGoingUp() && e.current_floor() > person->pick_up_floor_);
        return e.isGoingUp() || (e.isGoingDown() && e.current_floor() < person->pick_up_floor_);
    };
    const auto comparator = [&move_to_opposite_or_already_passed, &person](const Elevator &a, const Elevator &b)
    {
        if(move_to_opposite_or_already_passed(a))
            return false;
        if(move_to_opposite_or_already_passed(b))
            return true;
        return std::abs(static_cast<int>(a.current_floor()) - static_cast<int>(person->pick_up_floor_)) < 
                     std::abs(static_cast<int>(b.current_floor()) - static_cast<int>(person->pick_up_floor_)); 
    };
    std::sort(std::begin(elevators_), std::end(elevators_), comparator);

    // TODO: add handling on a case where all the elevators have no capacity to pick up this passenger.
    for(auto & elevator: elevators_){
        if(elevator.tryAddPassengerQueue(person))
            break;
    }
}