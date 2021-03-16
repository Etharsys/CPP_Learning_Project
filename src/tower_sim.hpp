#pragma once

#include "aircraft_manager.hpp"

#include <memory>

class Aircraft;
struct AircraftType;
class Airport;

class TowerSimulation
{
private:
    bool help        = false;
    Airport* airport = nullptr;
    AircraftManager manager;

    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    [[nodiscard]] std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type) const;
    [[nodiscard]] std::unique_ptr<Aircraft> create_random_aircraft() const;

    void create_keystrokes();
    void display_help() const;

    void init_airport();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
};
