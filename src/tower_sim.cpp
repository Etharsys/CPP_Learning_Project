#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;



TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) }
{
    MediaPath::initialize(argv[0]);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GL::init_gl(argc, argv, "Airport Tower Simulation");

    create_keystrokes();

    // On doit ajouter le manager à la move_queue, afin que timer() appelle bien sa fonction update() à chaque
    // frame.
    GL::move_queue.emplace(&manager);
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

std::unique_ptr<Aircraft> TowerSimulation::create_aircraft(const AircraftType& type)
{
    assert(airport);
    auto ac = factory.create_aircraft(type, airport->get_tower());
    assert(ac != nullptr);
    return ac;
}

std::unique_ptr<Aircraft> TowerSimulation::create_random_aircraft()
{
    assert(airport);
    auto ac = factory.create_random_aircraft(airport->get_tower());
    assert(ac != nullptr);
    return ac;
}

// On doit supprimer le const sur create_keystrokes, car on a maintenant des inputs succeptibles de modifier
// le contenu de la simulation.
void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { manager.add(create_random_aircraft()); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });

    // TASK_0 C-2: framerate control
    // Framerate cannot equal 0 or the program would get stuck / crash.
    // Also, in a "real" program, the maximal framerate should always be capped (you can see why if you do the
    // bonus part).
    GL::keystrokes.emplace('z', []() { GL::ticks_per_sec = std::max(GL::ticks_per_sec - 1u, 1u); });
    GL::keystrokes.emplace('a', []() { GL::ticks_per_sec = std::min(GL::ticks_per_sec + 1u, 180u); });

    // TASK_0 C-2: pause
    // Since the framerate cannot be 0, we introduce a new variable to manage this info.
    // Also, it would make no sense to use the framerate to simulate the pause, cause how would we unpause if
    // the program is not running anymore ?
    GL::keystrokes.emplace('p', []() { GL::is_paused = !GL::is_paused; });

    for (auto i = '0'; i < '8'; ++i) {
        GL::keystrokes.emplace(i, [this, i]() { std::cout << manager.get_aircraft_number(factory.get_airline(i - '0')) << std::endl; });
    }

    GL::keystrokes.emplace('m', [this]() { std::cout << "Nombre de crashs : " << manager.nb_crash << std::endl; } );
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto& [key_pair, value_pair] : GL::keystrokes)
    {
        std::cout << key_pair << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    assert(!init);
    airport = new Airport { one_lane_airport, Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() } };

    GL::move_queue.emplace(airport);
    init = true;
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();
    init_aircraft_types();

    GL::loop();
}
