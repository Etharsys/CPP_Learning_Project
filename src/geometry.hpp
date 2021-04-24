#pragma once

#include <algorithm>
#include <numeric>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>

template<typename Type, size_t Size>
struct Point
{
    Point() = default;

    Point(Type x, Type y) : values { x, y } { static_assert(Size == 2); }

    Point(Type x, Type y, Type z) : values { x, y, z } { static_assert(Size == 3); }

    Type& operator[] (size_t size) 
    { 
        assert(size < Size); 
        return values[size]; 
    }

    const Type& operator[] (size_t size) const
    {
        assert(size < Size);
        return values[size];
    }

    Type& x()       { return values[0]; }
    Type  x() const { return values[0]; }

    Type& y()       { static_assert(Size >= 2); return values[1]; }
    Type  y() const { static_assert(Size >= 2); return values[1]; }

    Type& z()       { static_assert(Size >= 3); return values[2]; }
    Type  z() const { static_assert(Size >= 3); return values[2]; }

    Point& operator+= (const Point& other)
    {
        std::transform( values.begin(), values.end(), 
            other.values.begin(), values.begin() , std::plus<Type>() );
        return *this; 
    }

    Point& operator-= (const Point& other)
    {
        std::transform( values.begin(), values.end(), 
            other.values.begin(), values.begin() , std::minus<Type>() );
        return *this;
    }

    Point& operator*= (const Point& other)
    {
        std::transform( values.begin(), values.end(), 
            other.values.begin(), values.begin() , std::multiplies<Type>() );
        return *this;
    }

    Point& operator*= (const float scalar)
    {
        std::transform( values.begin(), values.end(), 
            values.begin() , [scalar](Type v) { return v * scalar; } );
        return *this;
    }

    Point operator+ (const Point& other) const
    {
        Point result = *this;
        result += other;
        return result;
    }

    Point operator- (const Point& other) const
    {
        Point result = *this;
        result -= other;
        return result; 
    }

    Point operator* (const Point& other) const
    {
        Point result = *this;
        result *= other;
        return result; 
    }

    Point operator* (const float scalar) const
    {
        Point result = *this;
        result *= scalar;
        return result; 
    }

    Point operator- () const
    {
        Point result = *this;
        std::transform( values.begin(), values.end(), 
            result.values.begin() , [](Type v) { return -v; } );
        return result; 
    }

    float length() const 
    { 
        return std::sqrt(
            std::reduce( values.begin(), values.end(), 0, 
                [](Type v1, Type v2){ return v1*v1 + v2*v2; } )
            );
    }
    //return std::sqrt(x() * x() + y() * y() + z() * z());

    float distance_to(const Point& other) const 
    { return (*this - other).length(); }

    Point& normalize(const float target_len = 1.0f)
    {
        const float current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point& cap_length(const float max_len)
    {
        assert(max_len > 0);

        const float current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }

    std::array<Type, Size> values {};
};


using Point2D = Point<float, 2>;

using Point3D = Point<float, 3>;


inline void test_generic_points2()
{
    Point<float, 2> p1;
    Point<float, 2> p2;
    [[maybe_unused]] auto p3 = p1 + p2;
    p1 += p2;
    p1 *= 3; // ou 3.f, ou 3.0 en fonction du type de Point

    // Point2D { 3, 5, 6 }; // ne compile pas ! On essai de construire un tableau de taille 2 avec 3 éléments ! :(
    // Point3D { 3, 4 };    // eh be la compile (3 eme arguments vaut n'importe quoi)
    Point2D { 3, 4 };
    Point3D { 5, 6, 7 };
}


// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}
