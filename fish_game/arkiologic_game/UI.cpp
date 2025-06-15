#include "Header.h"

/*
void UI::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {

	if (screen_pos != -1) {
		x = float(poses_arr[screen_pos][0] * screen.cols);
		x -= wighth * float(sgn(x));

		y = float(poses_arr[screen_pos][1] * screen.rows / 2);
		y -= (hight + 1) * float(sgn(y));
	}

	TextSquere::draw(buffer, screen);
}*/


void UI::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {

	x = float(poses_arr[screen_pos][0] * screen.cols);
	x -= (total_width / 2) * float(sgn(x));

	y = float(poses_arr[screen_pos][1] * screen.rows / 2);
	y -= (1 + total_height / 2) * float(sgn(y));

	if (stuck_type_is_vertical) {
		float curr_y = y + total_height / 2;
		//const int step_am = std::floor(total_height / ui_els.size());

		ui_els[0].set_pos(x, curr_y);
		ui_els[0].draw(buffer, screen);
		for (int i = 1; i < ui_els.size(); i++) {

			TextSquere& ui_el = ui_els[i];
			int dist = std::ceil(ui_els[i - 1].hight / 2 + ui_el.hight / 2);
			curr_y -= (dist + spacing);

			ui_el.set_pos(x, curr_y);
			ui_el.draw(buffer, screen);
			
		}
	}
}

void UI::set_big_el(int el_ind) {
	for (int i = 0; i < ui_els.size(); i++) {
		ui_els[i].is_big = (i == el_ind);
	}
}