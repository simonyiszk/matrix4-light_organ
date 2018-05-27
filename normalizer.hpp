//Used from: https://github.com/simonyiszk/siso_blocks_cpp/commit/97e215530f5b4d7d79ba3be9ef3d336935d1892d
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

#ifndef NORMALIZER_H
#define NORMALIZER_H

#include <stddef.h>
#include <array>
#include <limits>
#include <cmath>
#include <math.h>

/**
 * @todo write docs
 */
template<typename storagetype, typename input_type>
class normalizer
{
private:
    storagetype min, max;
    
    storagetype out;
public:
    /**
     * @todo write docs
     */
    virtual const storagetype& pull() /*noexcept*/;
    virtual void put(const input_type& sample);
    
    const storagetype& getMin() { return min; }
    const storagetype& getMax() { return max; }
    
    const storagetype& pass(const input_type& sample){
        put(sample);
        return pull();
    }
    
    normalizer();
    
    void reset() noexcept;
    
    void decrease_top(){
        max*=0.9;
    }
    
    void increase_bottom(){
        min*=1.2;
    }
};

template<typename storagetype, typename input_type> const storagetype& normalizer<storagetype, input_type>::pull()
{
    return out;
}

template<typename storagetype, typename input_type> normalizer<storagetype, input_type>::normalizer()
{
    min  = std::numeric_limits<storagetype>::max();
    max  = std::numeric_limits<storagetype>::lowest();
    
    out  = 0;
}

template <typename storagetype, typename input_type>
void normalizer<storagetype, input_type>::put(const input_type& sample)
{
    if(std::numeric_limits<input_type>::has_signaling_NaN){
        if(isnan(sample)){
            out = 0;
            return;
        }
    }
    
    if(std::numeric_limits<input_type>::has_infinity){
        if(isinf(sample)){
            out = 100;
            return;
        }
    }
        
    if(max<sample)
        max=sample;
    
    if(min>sample)
        min=sample;
    
    if((max - min) < std::numeric_limits<storagetype>::epsilon()){
        out = 0;
        return;
    }
        
    out = (sample - min)/(max-min)*100;
}

template<typename storagetype, typename input_type> void normalizer<storagetype, input_type>::reset() noexcept
{
    min  = std::numeric_limits<storagetype>::max();
    max  = std::numeric_limits<storagetype>::lowest();
}


#endif // NORMALIZER_H
