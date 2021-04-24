#pragma once

#include "aircraft.hpp"

class AircraftFactory 
{
    public:
        const std::string get_airline(int airline) const;
        std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type, Tower& tower);
        std::unique_ptr<Aircraft> create_random_aircraft(Tower& tower);

    private:
        const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
        std::unordered_set<std::string> current_airlines;
};