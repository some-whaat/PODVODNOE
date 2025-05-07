#include "Header.h"


void Player::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {
	AnimatbleObj::draw(buffer, screen);

    move();
}

void  Player::move() {

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

    dir.normalise();


	sum(dir.mult(speed));
}