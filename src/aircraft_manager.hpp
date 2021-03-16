#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"

#include <memory>
#include <vector>

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;

public:
    void add(std::unique_ptr<Aircraft> aircraft);

    bool update() override;

    int get_aircraft_number(const std::string& company);
};
