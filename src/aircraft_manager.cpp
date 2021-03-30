#include "aircraft_manager.hpp"

#include <utility>

void AircraftManager::add(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts.emplace_back(std::move(aircraft));
}

bool AircraftManager::update()
{
    aircrafts.erase( std::remove_if(aircrafts.begin(), aircrafts.end(),
            [](const std::unique_ptr<Aircraft>& aircraft){ //try catch ici
                bool up;
                try {
                    up = !aircraft->update();
                } catch (const AircraftCrash& ac) {
                    std::cerr << "aircraft has crash" << std::endl;
                }
                return up; }), aircrafts.end());

    /*for (auto aircraft_it = aircrafts.begin(); aircraft_it != aircrafts.end();)
    {
        // On doit déréférencer 2x pour obtenir une référence sur l'Aircraft : une fois pour déréférencer
        // l'itérateur, et une deuxième fois pour déréférencer le unique_ptr.
        auto& aircraft = **aircraft_it;
        if (aircraft.update())
        {
            ++aircraft_it;
        }
        else
        {
            aircraft_it = aircrafts.erase(aircraft_it);
        }
    }*/

    return true;
}

int AircraftManager::get_aircraft_number(const std::string& company)
{
    return std::count_if(aircrafts.begin(), aircrafts.end(), 
        [company](const std::unique_ptr<Aircraft>& aircraft){ 
            return company == aircraft->get_flight_num().substr(0, 2);} );
}
