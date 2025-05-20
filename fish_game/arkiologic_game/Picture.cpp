#include "Header.h"

void Picture::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {

/*
    const int y_coord = screen.coord_to_vec_space(y, 'y') - 1;
    const int start_y = max(0, y_coord - (hight / 2));
    const int end_y = min(screen.rows - 1, y_coord + (hight / 2));

    const int x_coord = screen.coord_to_vec_space(x, 'x');
    const int start_x = max(0, x_coord - (5 / 2));
    const int end_x = min(screen.cols * 2 - 1, x_coord + (wighth / 2 + 1));

    for (int iy = start_y; iy <= end_y; iy++) {
        const int buffer_row = iy * screen.cols * 2;
        for (int ix = start_x; ix <= end_x; ix++) {
            if (image_vec[iy - start_y][ix - start_x] != 'Ú') {
                buffer[buffer_row + ix].Char.AsciiChar =
                    image_vec[iy - start_y][ix - start_x];
            }
        }
    }*/

    if (is_render == true) {

        int i = 0;
        Position camera_pos = screen.get_camera_pos();
        int y_coord = screen.coord_to_vec_space(y, 'y') - 1 - camera_pos.y * add_paralax;
        int x_coord = screen.coord_to_vec_space(x, 'x') + camera_pos.x * add_paralax;

        for (int iy = y_coord - (hight / 2); iy < y_coord + (hight / 2) && iy < screen.rows; iy++) {


            if (iy >= 0 && i < image_vec.size()) {
                int ii = 0;
                for (int ix = x_coord - (wighth / 2); ix <= x_coord + (wighth / 2 + 1) && ix < screen.cols * 2; ix++) {
                    if (ii >= image_vec[i].size()) {
                        break;
                    }
                    if (ix >= 0 && image_vec[i][ii] != 'Ú') {
                        //(*screen_vec)[iy][ix] = image_vec[i][ii];

                        buffer[iy * screen.cols * 2 + ix].Char.AsciiChar = image_vec[i][ii];
                    }
                    ii++;
                }
            }
            i++;
        }
    }
    
}

void Picture::add_pic(std::ifstream& file) {
    std::string str;
    while (std::getline(file, str)) {
        image_vec.push_back(str);
    }

    hight = image_vec.size();
    wighth = !image_vec.empty() ? image_vec[0].size() : 0;
}

void Picture::add_pic(std::string file_str) {
    std::ifstream file(file_str);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + file_str);
    }

    std::string str;
    while (std::getline(file, str)) {
        image_vec.push_back(str);
    }

    hight = image_vec.size();
    wighth = !image_vec.empty() ? image_vec[0].size() : 0;
}