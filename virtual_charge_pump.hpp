//From: https://github.com/simonyiszk/siso_blocks_cpp
/*
 * Copyright (c) 2018 Kiss Adam <email>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef VIRTUAL_CHARGE_PUMP_H
#define VIRTUAL_CHARGE_PUMP_H

#include <stddef.h>
#include <limits>

/**
 * @todo write docs
 */

//TODO changeble time constant

template<typename input_type = unsigned int, typename storagetype = float, const unsigned int decimate_factor = 3> //delay: how many samples to do discharging after
class virtual_charge_pump
{
private:    
    storagetype out;
    unsigned int counter;
    
   const storagetype downfall_factor = 0.4;
    
public:
    /**
     * @todo write docs
     */
    const storagetype& pull(){
        return out;
    };
    
    void put(const input_type& sample){
        if(std::numeric_limits<input_type>::has_signaling_NaN){
            if(isnan(sample)){
                out = 0;
                return;
            }
        }
        
        if(out<sample)
            out=sample;
        else{
            counter--;
            if(counter == 0){
                storagetype temp = out * downfall_factor;
                out = (temp > sample) ? temp : sample;
                counter = decimate_factor;
            }
        }
    }
    
    const storagetype pass(const input_type& sample){
        put(sample);
        return pull();
    }
    
    virtual_charge_pump(const storagetype& downfall_factor):
    downfall_factor(downfall_factor)
    {    
        out     = 0;
        counter = decimate_factor;
    };
    
    void reset() noexcept{
        out  = std::numeric_limits<storagetype>::lowest();
        counter = decimate_factor;
    };
};

#endif // VIRTUAL_CHARGE_PUMP_H

