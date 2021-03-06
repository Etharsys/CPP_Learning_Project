#pragma once

#include <algorithm>
#include <vector>

namespace GL {

// a displayable object can be displayed and has a z-coordinate indicating who
// is displayed before whom ;]

class Displayable
{
protected:
    float z = 0;

public:
    Displayable(const float z_) : z { z_ } { display_queue.emplace_back(this); }

    virtual ~Displayable()
    {
        display_queue.erase(std::find(display_queue.begin(), display_queue.end(), this));
    }

    virtual void display() const = 0;

    float get_z() const { return z; }

    // TASK_0 C-5
    // We make display_queue become a static field of the Displayable class.
    // This way, we can access it from the constructor and the destructor without having to define them
    // outside of the class.
    static inline std::vector<const Displayable*> display_queue;

    // Another solution would be to forward-declare Displayable, then put the display_queue definition, and
    // then put the Displayable class definition.
    // Another other solution would be to define the constructor and destructor in a cpp file (where it can
    // see the definition of display_queue).
};

struct disp_z_cmp
{
    bool operator()(const Displayable* a, const Displayable* b) const
    {
        const auto az = a->get_z();
        const auto bz = b->get_z();
        return (az == bz) ? (a > b) : (az > bz);
    }
};

} // namespace GL
