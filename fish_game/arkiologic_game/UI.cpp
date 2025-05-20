#include "Header.h"


void UI::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {
	x = poses_arr[screen_pos][0] * screen.cols;
	x -= wighth * sgn(x);

	y = poses_arr[screen_pos][1] * screen.rows / 2;
	y -= (hight + 1) * sgn(y);

	TextSquere::draw(buffer, screen);
}