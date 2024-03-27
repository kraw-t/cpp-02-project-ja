#pragma once
#include <cstddef>
#include <cassert>

struct Person{
    enum class State
    {
        Waiting,
        Onboard,
        Completed,
    };
    Person(size_t id, size_t weight, size_t time, size_t pick_up_floor, size_t drop_off_floor)
        : id_(id), weight_(weight), time_(time), pick_up_floor_(pick_up_floor), drop_off_floor_(drop_off_floor){};

    bool isGoingUp() const { return pick_up_floor_ < drop_off_floor_; };
    bool isGoingDown() const { return !isGoingUp(); };
    void exitElevator() { assert(state_ == State::Onboard);
        state_ = State::Completed;
    };

    size_t id_;
    size_t weight_;
    size_t time_;
    size_t pick_up_floor_;
    size_t drop_off_floor_;
    State state_{State::Waiting};
};