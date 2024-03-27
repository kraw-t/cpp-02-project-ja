#pragma once
#include <cstddef>
#include <queue>
#include <list>
#include "person.h"

class Elevator{
public:
    enum class State 
    {
        GoingUp,
        GoingDown,
        Idle 
    };
    Elevator(size_t num_of_floors, size_t max_weight) : num_of_floors_(num_of_floors), max_weight_(max_weight){};

    bool isGoingUp() const { return current_state_ == State::GoingUp; };
    bool isGoingDown() const { return current_state_ == State::GoingDown; };
    bool isIdle() const { return current_state_ == State::Idle; };
    bool tryAddPassengerQueue(const std::shared_ptr<Person> & person);
    void process();

    size_t current_floor() const { return current_floor_; };

private:
    void maybePickUpOrDropOff();
    void updateState();
    void move();

    constexpr static size_t speed_{1};
    std::list<std::shared_ptr<Person>> passengers_;
    std::queue<std::shared_ptr<Person>> passengers_to_pick_up_;
    size_t num_of_floors_;
    size_t max_weight_;
    size_t current_floor_{0};
    size_t current_weight_{0};
    size_t expected_weight_increase_{0};
    State current_state_{State::Idle};
};

