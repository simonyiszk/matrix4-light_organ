#pragma once
#include <chrono>
#include <array>
#include <ostream>
#include <string>

class pixel{
public:
    uint8_t r, g, b;
    float alpha;
    
    pixel(): r(0), g(0), b(0), alpha(.0) {};
    
    void set(const uint8_t& r, const uint8_t& g, const uint8_t& b, const float& alpha = 1.0){
        this->r     = r;
        this->g     = g;
        this->b     = b;
        this->alpha = alpha;
    }
};

std::ostream& operator<<(std::ostream& os, const pixel& p){
    char tmp[11];
    snprintf(tmp, 11, "0x%.2X%.2X%.2X%.2X, ", uint8_t(255*p.alpha), p.r, p.g, p.b);
    os << tmp;
    return os;
}

class Frame{
public:
    static const size_t width  = 32;
    static const size_t height = 26;
    
private:
    std::chrono::milliseconds duration;
    
    using emelet = std::array<pixel, Frame::width>;
    
    std::array<emelet, Frame::height> data;
    
    struct indexGuard{
        indexGuard(const size_t& x, const size_t& y){
            if(x >= Frame::width || y >= Frame::height) throw std::out_of_range(std::string{"Invalid pixel index: x="} + std::to_string(x) + std::string{" y="} + std::to_string(y) ); 
        }
    };
    
public:
    //a szamozas a bal felso sarokban kezdodik
    
    pixel& getPixel(size_t x, size_t y) 
    { 
        indexGuard a(x, y);
        return data[y][x];
        
    }
    
    const pixel& getPixel(size_t x, size_t y) const
    { 
        indexGuard a(x, y);
        return data[y][x];
        
    }
    
    const auto& getDuration() const noexcept { return duration; }
    
    Frame() = delete;
    Frame(const Frame&) = default;
    Frame(Frame&&) = default;
    Frame(const std::chrono::milliseconds duration): duration(duration) {}
    Frame& operator=(const Frame&) = default;
};

std::ostream& operator<<(std::ostream& os, const Frame& f){   
    os << "frame({" << std::endl;
    for(size_t y=0;y<Frame::height;y++){
        for(size_t x=0;x<Frame::width;x++)
            os << f.getPixel(x,y) << ", ";
        os << std::endl;
    }
        
    os << "}, " << static_cast<std::chrono::milliseconds>(f.getDuration()).count() << ")" << std::endl << std::endl;
    return os;
}
