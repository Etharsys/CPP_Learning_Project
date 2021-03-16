#pragma once

#include "aircraft.hpp"

#include <memory>


class AircraftManager 
{
    private :
        std::vector<std::unique_ptr<Aircraft>> aircrafts;

    public :
        

}