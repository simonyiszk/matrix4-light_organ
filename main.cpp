#include <iostream>
#include "alsa_sampler.hpp"
#include "fft-real/ffft/FFTRealFixLen.h"
#include "color.hpp"
#include "hanning.hpp"
//#include "matrix4_display.h"
#include "matrix_animation.hpp"
#include "masks.hpp"

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
    uint8_t mely, magas, kozep, teljes;
    
    auto start = std::chrono::system_clock::now();
    auto start2 = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    
    while (1){
        rec.fillBuffer();
        
        std::array<float, buffer_size> samples = rec.getBuffer();
        
        for(size_t i=0;i<buffer_size;i++){
            samples[i]*=hanning<float, buffer_size>()[i];
        }
        
        fft.do_fft(fft_output.begin(), samples.begin());
        
        mely  = static_cast<uint8_t>(color::calc<sample_rate, 50,   160,  float, buffer_size, double>(fft_output) * 255.0 / 100.0);
        kozep = static_cast<uint8_t>(color::calc<sample_rate, 140,  350,  float, buffer_size, double>(fft_output) * 255.0 / 100.0);
        magas = static_cast<uint8_t>(color::calc<sample_rate, 2000, 6000, float, buffer_size, double>(fft_output) * 255.0 / 100.0);
        
        
        const size_t kezdo_sor = 10;
        teljes = static_cast<uint8_t>(color::calc<sample_rate, 50,   6000,  float, buffer_size, double>(fft_output) * static_cast<double>(kezdo_sor) / 100.0);
        
        end = std::chrono::system_clock::now();
        start2 = std::chrono::system_clock::now();
        Frame f(std::chrono::duration_cast<std::chrono::milliseconds>(end  - start));
        start = start2;
              
        
        
        for(size_t j=kezdo_sor-teljes; j<kezdo_sor+teljes;j++)
            for(size_t i=0;i<Frame::width;i++)
                f.getPixel(i, j).set(mely,kozep,magas);
        
        masks::mask_out(f, masks::minta);
            
        std::cout << f;
    }
        
    return 0;
}
