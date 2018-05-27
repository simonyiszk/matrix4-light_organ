#pragma once
#include <limits>
#include <array>
#include <cmath>
#include "normalizer.hpp"

namespace color{
    constexpr const size_t under_trashhold_limit = 5;
    
    template<unsigned int sample_rate, unsigned int start_freq, unsigned int stop_freq, typename input_type, unsigned int buffer_size, typename storage_type>
    const storage_type calc(std::array<input_type, buffer_size> ffft_mag_spectrum) {
        static normalizer<storage_type, input_type> normalized;
        static size_t                               under_trashhold_counter;
        static size_t                               above_trashhold_counter;
        
        constexpr const size_t start_index = (size_t)(start_freq*buffer_size/sample_rate);
        constexpr const size_t stop_index  = (size_t)(stop_freq *buffer_size/sample_rate);

        double signal_power = 0.0;
        
        //Calculating strength
        for(size_t i= start_index; i < stop_index; i++)
            signal_power+=(ffft_mag_spectrum[i]*ffft_mag_spectrum[i])+(ffft_mag_spectrum[i+(buffer_size/2)]*ffft_mag_spectrum[i+(buffer_size/2)]); //Using law of perseval, and the calculation of complex absolute value
        signal_power=sqrt(signal_power); //Calculate value from power
        
        // Autoscale in case of turning the music down
        if (signal_power < (normalized.getMax() - normalized.getMin())*0.2){
            under_trashhold_counter++;
            if(under_trashhold_counter == under_trashhold_limit){
                normalized.decrease_top();
            }
        } else {
            under_trashhold_counter=0;
        }
        
        // Utóelnyomás TODO translate to english
        if (signal_power > (normalized.getMax() - normalized.getMin())*0.6){
            above_trashhold_counter++;
            if(above_trashhold_counter == under_trashhold_limit){
                normalized.increase_bottom();
            }
        } else {
            above_trashhold_counter=0;
        }
  
        //normalizing the output between 0f and 100f
        return normalized.pass(signal_power);
    }
};
