#include "Header.h"


void UI::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {

	if (screen_pos != -1) {
		x = float(poses_arr[screen_pos][0] * screen.cols);
		x -= wighth * float(sgn(x));

		y = float(poses_arr[screen_pos][1] * screen.rows / 2);
		y -= (hight + 1) * float(sgn(y));
	}

	TextSquere::draw(buffer, screen);
}

void UIContainer::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {

	x = float(poses_arr[screen_pos][0] * screen.cols);
	x -= (wighth + total_width / 2) * float(sgn(x));

	y = float(poses_arr[screen_pos][1] * screen.rows / 2);
	y -= (hight + 1 + total_height / 2) * float(sgn(y));

	if (stuck_type_is_vertical) {
		float curr_y = y + total_height / 2;
		const float step_am = total_height / ui_els.size();

		for (UI ui_el : ui_els) {
			ui_el.set_pos(x, curr_y);
			ui_el.draw(buffer, screen);
			curr_y -= step_am;
		}
	}
}
