#pragma once
#include<string.h> //memset
#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdexcept>
#include<unistd.h>

namespace matrix4{
    enum commands{
        turn_12v_off_left = 0x00,
        turn_12v_off_right = 0x01,
        reset_left_panel  = 0x02,
        reset_right_panel = 0x03,
        reboot  = 0x04,
        get_status = 0x05,
        use_internal_anim = 0x10,
        use_external_anim = 0x20,
        blank = 0x30,
        delete_anim_network_buffer = 0x06
    };

class matrix4_display{
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
public:
    matrix4_display(const char* ip_address){
        if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        {
            throw std::runtime_error("socket");
        }
    
        memset((char *) &si_other, 0, sizeof(si_other));
        si_other.sin_family = AF_INET;   
        if (inet_aton(ip_address , &si_other.sin_addr) == 0)
            throw std::runtime_error("inet_aton() failed Maybe bad IP address?");
        si_other.sin_port = htons(2000);
        
        uint8_t external_anim_command[]= {'S', 'E', 'M', matrix4::use_external_anim};
        if (sendto(s, external_anim_command, sizeof(external_anim_command), 0 , (struct sockaddr *) &si_other, slen)==-1)
                throw std::runtime_error("Cannot switch to external anim mode");
        
        si_other.sin_port = htons(3000);
    }
    
    matrix4_display(const matrix4_display&) = delete;
    
    matrix4_display& operator=(const matrix4_display&) = delete;
    
    void send(uint8_t w, uint8_t r, uint8_t g, uint8_t b){
        for(uint8_t i = 0; i <4; i++){
            uint8_t message[]= {w, i, r, g, b};
            
            //send the message
            if (sendto(s, message, 5, 0 , (struct sockaddr *) &si_other, slen)==-1)
                throw std::runtime_error("sendto");
        }
    }
    
    ~matrix4_display() noexcept{
        close(s);
    }
};
    
};
