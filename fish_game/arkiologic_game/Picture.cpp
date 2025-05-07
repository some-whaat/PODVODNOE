#include "Header.h"

void Picture::draw(std::vector<std::string>* screen_vec, Screen& screen) {
    int i = 0;
    int y_coord = screen.coord_to_vec_space(y, 'y') - 1;

    for (int iy = y_coord - (hight / 2); iy < y_coord + (hight / 2) && iy < screen.rows; iy++) {
        int x_coord = screen.coord_to_vec_space(x, 'x');

        if (iy >= 0 && i < image_vec.size()) {
            int ii = 0;
            for (int ix = x_coord - (wighth / 2); ix <= x_coord + (wighth / 2 + 1) && ix < screen.cols * 2; ix++) {
                if (ii >= image_vec[i].size()) {
                    break;
                }
                if (ix >= 0 && image_vec[i][ii] != 'Ú') {
                    (*screen_vec)[iy][ix] = image_vec[i][ii];
                }
                ii++;
            }
        }
        i++;
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