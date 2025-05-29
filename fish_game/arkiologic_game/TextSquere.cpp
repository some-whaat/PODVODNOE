#include "Header.h"


/*
void TextSquere::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {

    draw_frame(buffer, screen);

    int i = 0;
    int y_coord = (screen.coord_to_vec_space(y, 'y') - 1);

    for (int iy = y_coord - floor(hight / 2); iy < y_coord + ceil(hight / 2)  && iy < (&screen)->rows; iy++) {

        int x_coord = screen.coord_to_vec_space(x, 'x');

        if (iy >= 0 && x_coord + (wighth / 2) < (&screen)->cols * 2 && x_coord - (wighth / 2) > 0) {
            (*screen_vec)[iy].replace(floor(x_coord - (wighth / 2)), wighth, text_vec[i]);
        }
        i++;
    }
}
*/

void TextSquere::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {

    if (is_render == true) {
        int i = 0;

        int y_coord = screen.coord_to_vec_space(y, 'y');
        int x_coord = screen.coord_to_vec_space(x, 'x');

        if (is_steak_to_screen) {
            x_coord += screen.get_camera_pos().x * 2;
            y_coord -= screen.get_camera_pos().y;
        }


        if (follow_pos != nullptr) {
            int facing = sgn(screen.coord_to_vec_space(follow_pos->x, 'x') - screen.cols);

            text_follow(facing);
        }
        
        Rektangle::draw_frame(buffer, screen);


        for (int iy = y_coord - std::ceil(hight / 2) - int(int(hight) % 2 == 0); iy < y_coord + std::ceil(hight / 2) && iy < screen.rows; iy++) { // костыль

            if (iy >= 0 && i < text_vec.size()) {
                int ii = 0;
                for (int ix = x_coord - (wighth / 2); ix <= x_coord + (wighth / 2 + 1) && ix < screen.cols * 2; ix++) {
                    if (ii >= text_vec[i].size()) {
                        break;
                    }
                    if (ix >= 0) {
                        buffer[iy * screen.cols * 2 + ix].Char.AsciiChar = text_vec[i][ii];
                        buffer[iy * screen.cols * 2 + ix].Attributes = color_attr;
                    }

                    ii++;
                }
            }
            i++;
        }
    }

}

void TextSquere::text_follow(int facing) {
	if (follow) {
		x = follow_pos->x - (follow_wighth + wighth)/4 * sgn(facing);
		y = follow_pos->y + (follow_hight + hight)/2;
	}
}

void TextSquere::set_text(std::string in_text, int in_wighth) {

    text_vec.clear();

    if (in_wighth == 0) {
        wighth = in_text.size();
        text_vec.push_back(in_text);
        hight = 1;
    }
    else if (in_text.size() < in_wighth) {
        wighth = in_text.size();
        hight = 1;
        text_vec.push_back(in_text);
    }
    else {
        wighth = in_wighth;

        //std::string add_line = std::string(' ', in_text.size() % (int)wighth);
        //in_text += add_line;

        for (int i = 0; i < std::ceil(in_text.size() / wighth) * wighth; i += wighth) {
            std::string new_line = in_text.substr(i, wighth);

            if (new_line.size() < wighth) {
                int coun = wighth - new_line.size();
                for (int i = 0; i < coun; i++) {
                    new_line += ' ';
                }
            }


            text_vec.push_back(new_line);
        }

        hight = text_vec.size();
    }
}