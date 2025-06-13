#include "Header.h"


void UI::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {
	x = float(poses_arr[screen_pos][0] * screen.cols);
	x -= wighth * float(sgn(x));

	y = float(poses_arr[screen_pos][1] * screen.rows / 2);
	y -= (hight + 1) * float(sgn(y));

	TextSquere::draw(buffer, screen);
}