#pragma once

#include <array>
#include <cmath>

template<typename out_type, const size_t size>
constexpr const std::array<out_type, size> hanning(){
     std::array<out_type, size> out = {0};
    
    for(size_t i=0;i<size;i++)
        out[i]=0.5*(1-cos(2.0*M_PI/(size-1)));
    
    return out;
}
