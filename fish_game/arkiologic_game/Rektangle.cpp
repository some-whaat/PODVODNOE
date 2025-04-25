#include "Header.h"



bool Rektangle::is_in_rec(Position other_pos, float add_dist) {
	return other_pos.x < x + (wighth / 2) + add_dist && other_pos.x > x - (wighth / 2) - add_dist && other_pos.y < y + (hight / 2) + add_dist && other_pos.y > y - (hight / 2) - add_dist;
}

bool Rektangle::is_in_rec(int in_x, int in_y, float add_dist) {
    //return abs(cos(rot_ang)*x + sin(rot_ang)*y) + abs(cos(rot_ang) * x + sin(rot_ang) * y);
    return in_x < x + (wighth / 2) + add_dist && in_x > x - (wighth / 2) - add_dist && in_y < y + (hight / 2) + add_dist && in_y > y - (hight / 2) - add_dist;
}

void Rektangle::draw(std::vector<std::string>* screen_vec, Screen& screen) {
    int y_coord = (screen.coord_to_vec_space(y, 'y') - 1);
    int x_coord = screen.coord_to_vec_space(x, 'x');

    float now_hight = add_val + hight;
    float now_wighth = add_val + wighth;

    std::string line = std::string(now_wighth, fill);

    for (int iy = y_coord - floor(now_hight / 2); iy < y_coord + ceil(now_hight / 2) && iy < (&screen)->rows; iy++) {

        if (iy >= 0 && x_coord + (now_wighth / 2) < (&screen)->cols * 2 && x_coord - (now_wighth / 2) > 0) {
            (*screen_vec)[iy].replace(x_coord - (now_wighth / 2), now_wighth, line);
            //(*screen_vec)[rows - 6].replace(cols - 6, image.size(), image);
        }
    }
}

void Rektangle::draw_frame(std::vector<std::string>* screen_vec, Screen& screen) {
    int y_coord = (screen.coord_to_vec_space(y, 'y') - 1);
    int x_coord = screen.coord_to_vec_space(x, 'x');

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

    for (int iy = y_coord - floor(now_hight / 2) + 1; iy < y_coord + ceil(now_hight / 2) - 1 && iy < (&screen)->rows; iy++) {

        if (iy >= 0 && x_coord + (now_wighth / 2) < (&screen)->cols * 2 && x_coord - (now_wighth / 2) > 0 && x_coord - (now_wighth / 2) + now_wighth < (*screen_vec)[iy].size()) {
            (*screen_vec)[iy].replace(x_coord - (now_wighth / 2), now_wighth, mid_lines);
            //(*screen_vec)[rows - 6].replace(cols - 6, image.size(), image);
        }
    }

    if (x_coord - (now_wighth / 2) >= 0 && y_coord + ceil(now_hight / 2) - 1 >= 0 && y_coord + ceil(now_hight / 2) - 1 <= (&screen)->rows) {
        (*screen_vec)[y_coord + ceil(now_hight / 2) - 1].replace(x_coord - (now_wighth / 2), now_wighth, bord_lines);
     }
    if (x_coord - (now_wighth / 2) >= 0 && y_coord - floor(now_hight / 2) >= 0 && y_coord - floor(now_hight / 2) <= (&screen)->rows) {
        (*screen_vec)[y_coord - floor(now_hight / 2)].replace(x_coord - (now_wighth / 2), now_wighth, bord_lines);
    }
}