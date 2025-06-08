#include "Header.h"


void MovingObj::move(Screen& screen) {

    switch (moving_type)
    {
    case MovingType::none:

        break;

    case MovingType::wave:

        if (abs(velocity.x) > abs(velocity.y)) { // мюдн онрнл йсдюрн оепедбхмсрэ врна бя╗бпелъ ме оепеявхршбюкняэ
            float result_x = float(x) + (velocity.len() * float(sgn(velocity.x))) * screen.deltaTime;
            x = result_x;
            y = ang_coef * (x - firt_pos.x) + wave_hight * (std::sin(wave_lenght * (result_x - firt_pos.x) + wave_offset)) + firt_pos.y;
        }
        else {
            float result_y = float(y) + (velocity.len() * float(sgn(velocity.y))) * screen.deltaTime;
            y = result_y;
            x = ang_coef * (y - firt_pos.y) + wave_hight * (std::sin(wave_lenght * (result_y - firt_pos.y) + wave_offset)) + firt_pos.x;
        }

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