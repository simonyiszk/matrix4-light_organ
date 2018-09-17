#pragma once
#include <tuple>
#include "matrix_animation.hpp"

namespace masks{
    using minta_type = std::array<const char[Frame::width + 1], Frame::height>;
    
    constexpr const minta_type minta = {
        "11100000000000001110000000000000",
        
        "11010000000000001110000000000000",
        
        "11000100000000001110000000000000",
        
        "11000010000000001110000000000000",
        
        "11000000100000001110000000000000",
        
        "11000000001000001110000000000000",
        
        "11000000000010001110000000000000",
        
        "11000000000000101110000000000000",
        
        "11000000000000011110000000000000",
        
        "11000000000000001110000000000000",
        
        "00000000000000001110000000000000",
        
        "00000000000000001110000000000000",
        
        "00000000000000001110000000000000"
    };
    
    void mask_out(Frame& f, const minta_type& mask){                
        for(size_t y=0; y< Frame::height; y++)
            for(size_t x=0; x< Frame::width; x++)
                if((minta.at(y))[x] == '0')
                    f.getPixel(x, y).set(0,0,0);
    }
}


