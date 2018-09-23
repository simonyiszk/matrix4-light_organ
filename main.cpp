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
    alsa_sampler<buffer_size> rec(sample_rate, "sysdefault:CARD=USB");
    ffft::FFTRealFixLen<mylog2(buffer_size)> fft;
    std::array<float, buffer_size> fft_output;
    double mely, kozep, magas;
    matrix4::matrix4_display m4d("10.152.105.226");
    
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
          
        //std::cout << std::hex << samples[0] << " " << samples[1] << " " <<samples[2] << " " << std::endl;
        
        //std::cout << mely << ' ' << kozep << ' ' << magas << ' ' << std::endl;
        
        m4d.setPixel(matrix4::matrix4_display::left, 0, {0,uint8_t(kozep*2.55),0});
        m4d.setPixel(matrix4::matrix4_display::left, 1, {0,0,uint8_t(magas*2.55)});
        m4d.setPixel(matrix4::matrix4_display::left, 2, {uint8_t(mely*2.55),0,0});
        m4d.setPixel(matrix4::matrix4_display::left, 3, {uint8_t(mely*2.55),0,0});
        
        
        m4d.setPixel(matrix4::matrix4_display::right, 0, {0,uint8_t(kozep*2.55),0});
        m4d.setPixel(matrix4::matrix4_display::right, 2, {0,0,uint8_t(magas*2.55)});
        m4d.setPixel(matrix4::matrix4_display::right, 1, {uint8_t(mely*2.55),0,0});
        m4d.setPixel(matrix4::matrix4_display::right, 3, {uint8_t(mely*2.55),0,0});
    }
        
    return 0;
}
