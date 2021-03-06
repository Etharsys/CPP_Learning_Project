#pragma once

#include "waypoint.hpp"

#include <map>
#include <utility>
#include <vector>

class Airport;
class Aircraft;

class Tower
{
private:
    // TASK_0 C-6
    // We can use a map or an unordered map, in order to retrieve the terminal index from the Aircraft in
    // O(1).
    using AircraftToTerminal = std::map<const Aircraft*, size_t>;

    Airport& airport;
    // aircrafts may reserve a terminal
    // if so, we need to save the terminal number in order to liberate it when the craft leaves
    AircraftToTerminal reserved_terminals = {};

    WaypointQueue get_circle() const;

public:
    Tower(Airport& airport_) : airport { airport_ } {}

    // produce instructions for aircraft
    WaypointQueue get_instructions(Aircraft& aircraft);
    void arrived_at_terminal(const Aircraft& aircraft);
};
