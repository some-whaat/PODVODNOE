#include "Header.h"


void Player::draw(std::vector<std::string>* screen_vec, Screen& screen) {
	AnimatbleObj::draw(screen_vec, screen);

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