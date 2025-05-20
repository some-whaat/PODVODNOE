#include "Header.h"


void Player::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {
	AnimatbleObj::draw(buffer, screen);

    move();
}

void  Player::move() {

    if (++move_count >= 1/speed) {
        Position dir;

        if (GetAsyncKeyState(VK_LEFT))
        {
            dir.x += -1;

        }

        if (GetAsyncKeyState(VK_RIGHT))
        {
            dir.x += 1;

        }

        if (GetAsyncKeyState(VK_UP))
        {
            dir.y += 1;

        }

        if (GetAsyncKeyState(VK_DOWN))
        {
            dir.y += -1;

        }

        dir = dir.normalise();


        //sum(dir.mult(speed));
        sum(dir);

        move_count = 0;
    }
}