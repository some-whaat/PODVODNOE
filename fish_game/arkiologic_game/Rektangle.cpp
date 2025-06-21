#include "Header.h"



bool Rektangle::is_inside(Position other_pos, float add_dist) {
	return other_pos.x < x + (wighth / 2) + add_dist && other_pos.x > x - (wighth / 2) - add_dist && other_pos.y < y + (hight / 2) + add_dist && other_pos.y > y - (hight / 2) - add_dist;
}

bool Rektangle::is_in_rec(int in_x, int in_y, float add_dist) {
    //return abs(cos(rot_ang)*x + sin(rot_ang)*y) + abs(cos(rot_ang) * x + sin(rot_ang) * y);
    return in_x < x + (wighth / 2) + add_dist && in_x > x - (wighth / 2) - add_dist && in_y < y + (hight / 2) + add_dist && in_y > y - (hight / 2) - add_dist;
}

void Rektangle::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {

    if (is_render == true) {

        int y_coord = (screen.coord_to_vec_space(y, 'y') - 1);
        int x_coord = screen.coord_to_vec_space(x, 'x');

        if (is_steak_to_screen) {
            x_coord += screen.get_camera_pos().x * 2;
            y_coord -= screen.get_camera_pos().y;
        }

        float now_hight = add_val + hight;
        float now_wighth = add_val + wighth;

        //std::string line = std::string(now_wighth, fill);

        for (int iy = y_coord - floor(now_hight / 2); iy < y_coord + ceil(now_hight / 2) && iy < screen.rows; iy++) {

            if (iy >= 0) {

                for (int ix = x_coord - (now_wighth / 2); ix <= x_coord + (now_wighth / 2 + 1) && ix < screen.cols * 2; ix++) {

                    if (ix >= 0) {
                        buffer[iy * screen.cols * 2 + ix].Char.AsciiChar = fill;
                        buffer[iy * screen.cols * 2 + ix].Attributes = color_attr;
                    }
                }
            }
        }
    }
}

void Rektangle::draw_frame(std::vector<CHAR_INFO>& buffer, Screen& screen) {
    int y_coord = (screen.coord_to_vec_space(y, 'y') - 1);
    int x_coord = screen.coord_to_vec_space(x, 'x');

    if (is_steak_to_screen) {
        x_coord += screen.get_camera_pos().x * 2;
        y_coord -= screen.get_camera_pos().y;
    }

    float now_hight = add_val + hight;
    float now_wighth = add_val + wighth;

    std::string bord_lines;
    std::string mid_lines;

    if (is_big) {
        bord_lines = std::string(now_wighth, '#');
        mid_lines = '#' + std::string(now_wighth - 2, ' ') + '#';
    }
    else {
        bord_lines = '+' + std::string(now_wighth - 2, '-') + '+';//std::string(now_wighth, '-');
        mid_lines = '|' + std::string(now_wighth - 2, ' ') + '|';
    }


    for (int iy = y_coord - floor(now_hight / 2) + 1; iy < y_coord + ceil(now_hight / 2) - 1 && iy < screen.rows; iy++) {

        if (iy >= 0) {
            int ii = 0;
            for (int ix = x_coord - (now_wighth / 2); ix <= x_coord + (now_wighth / 2 + 1) && ix < screen.cols * 2; ix++) {

                if (ii >= now_wighth) {
                    break;
                }
                
                if (ix >= 0) {
                    buffer[iy * screen.cols * 2 + ix].Char.AsciiChar = mid_lines[ii];
                    buffer[iy * screen.cols * 2 + ix].Attributes = color_attr;
                }

                ii++;
            }
        }
    }

	int up_y = y_coord + ceil(now_hight / 2) - 1;
	int down_y = y_coord - floor(now_hight / 2);

    if (up_y >= 0 && up_y < screen.rows) {
        int ii = 0;
        for (int ix = x_coord - (now_wighth / 2); ix <= x_coord + (now_wighth / 2 + 1) && ix < screen.cols * 2; ix++) {

            if (ii >= now_wighth) {
                break;
            }

            if (ix >= 0) {
                buffer[up_y * screen.cols * 2 + ix].Char.AsciiChar = bord_lines[ii];
                buffer[up_y * screen.cols * 2 + ix].Attributes = color_attr;
            }

            ii++;
        }
    }
    if (down_y >= 0 && down_y < screen.rows) {
        int ii = 0;
        for (int ix = x_coord - (now_wighth / 2); ix <= x_coord + (now_wighth / 2 + 1) && ix < screen.cols * 2; ix++) {
            
            if (ii >= now_wighth) {
                break;
            }

            if (ix >= 0) {
                buffer[down_y * screen.cols * 2 + ix].Char.AsciiChar = bord_lines[ii];
                buffer[down_y * screen.cols * 2 + ix].Attributes = color_attr;
            }

            ii++;
        }
    }
}