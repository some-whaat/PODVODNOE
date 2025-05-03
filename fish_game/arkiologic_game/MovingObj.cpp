#include "Header.h"


void MovingObj::move(Screen& screen) {
    float result_x = x + (speed * float(fasing) / 5);// *screen.MBF / 76;
    x = result_x;
    y = wave_hight * std::sin(wave_lenght * result_x + wave_offset);
}

void MovingObj::draw(std::vector<std::string>* screen_vec, Screen& screen) {
    Picture::draw(screen_vec, screen);

    move(screen);
}