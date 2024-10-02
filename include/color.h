#pragma once

#include <iostream>

#include "Vec3.h"
using color = Vec3;

void writeColor(std::ostream &out, const color& pixel){
    auto r = pixel.x();
    auto g = pixel.y();
    auto b = pixel.z();

    int rbyte = static_cast<int>(255.999 * r);
    int gbyte = static_cast<int>(255.999 * g);
    int bbyte = static_cast<int>(255.999 * b);

    out << rbyte << " " << gbyte << " " << bbyte << "\n";
}