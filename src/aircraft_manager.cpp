#include "aircraft_manager.hpp"

#include <utility>

void AircraftManager::add(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts.emplace_back(std::move(aircraft));
}

bool AircraftManager::update()
{
    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                [this](std::unique_ptr<Aircraft>& aircraft_it) {
                    auto& aircraft = *aircraft_it;
                    try
                    {
                        return !aircraft.update();
                    }
                    catch (const AircraftCrash& ac)
                    {
                        std::cerr << ac.what() << std::endl;
                        nb_crash++;
                        return true;
                    }
                }), aircrafts.end());
    return true;
}

int AircraftManager::get_aircraft_number(const std::string& company)
{
    return std::count_if(aircrafts.begin(), aircrafts.end(), 
        [company](const std::unique_ptr<Aircraft>& aircraft){ 
            return company == aircraft->get_flight_num().substr(0, 2);} );
}
