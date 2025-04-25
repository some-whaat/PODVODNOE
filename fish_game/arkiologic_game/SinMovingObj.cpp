#include "Header.h"


void SinMovingObj::move() {
    float result_x = x + (speed * fasing);
    x = result_x;
    y = wave_hight * std::sin(wave_lenght * result_x + wave_offset);
}

void SinMovingObj::draw(std::vector<std::string>* screen_vec, Screen& screen) {
    Picture::draw(screen_vec, screen);

    move();
}