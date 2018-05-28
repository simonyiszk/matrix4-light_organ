#include <iostream>
#include "alsa_sampler.hpp"
#include "fft-real/ffft/FFTRealFixLen.h"
#include "color.hpp"
#include "hanning.hpp"
#include "matrix4_display.h"

constexpr const size_t buffer_size = 1024;
constexpr const unsigned int sample_rate = 44100;

constexpr const unsigned mylog2(unsigned a){
    if ( a == 0 ) 
        throw std::logic_error("Cannot ligraitmize 0");
    
    if ( a == 1 )
        return 0;
    
    unsigned ret = mylog2(a>>1) + 1;
    
    if ( (1 << ret) != a)
        throw std::logic_error("Rounding was neccessary");
    return ret;
}

int main(int argc, char **argv) {
    alsa_sampler<buffer_size> rec(sample_rate);
    ffft::FFTRealFixLen<mylog2(buffer_size)> fft;
    std::array<float, buffer_size> fft_output;
    double mely, kozep, magas;
    matrix4::matrix4_display m4d("10.152.104.67");
    
    while (1){
        rec.fillBuffer();
        
        std::array<float, buffer_size> samples = rec.getBuffer();
        
        for(size_t i=0;i<buffer_size;i++){
            samples[i]*=hanning<float, buffer_size>()[i];
        }
        
        fft.do_fft(fft_output.begin(), samples.begin());
        
        mely  = color::calc<sample_rate, 50,   160,  float, buffer_size, double>(fft_output);
        kozep = color::calc<sample_rate, 140,  350,  float, buffer_size, double>(fft_output);
        magas = color::calc<sample_rate, 2000, 6000, float, buffer_size, double>(fft_output);
                
        //std::cout << mely << ' ' << kozep << ' ' << magas << ' ' << std::endl;
        
        m4d.send(0,uint8_t(mely*2.55),uint8_t(kozep*2.55),uint8_t(magas*2.55));
    }
        
    return 0;
}
