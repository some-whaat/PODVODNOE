#include "Header.h"


void MovingObj::move(Screen& screen) {

    switch (moving_type)
    {
    case MovingType::none:

        break;

    case MovingType::wave:
        float result_x = x + (velocity.len() * float(fasing)) * screen.MBF / 25;
        x = result_x;
        y = wave_hight * (std::sin(wave_lenght * result_x + wave_offset)) + firt_pos.y;

        break;

    }



    /*
    float result_x = x + (velocity.len() * float(fasing) / 5) * screen.MBF / 5;
    x = result_x;
    y = velocity.x * result_x + velocity.y + wave_hight * (std::sin(wave_lenght * result_x + wave_offset)) + firt_pos.y;
    */
}

void MovingObj::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {
    AnimatbleObj::draw(buffer, screen);

    move(screen);
}