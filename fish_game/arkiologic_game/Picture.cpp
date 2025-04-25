#include "Header.h"


/*
void fit_to_screen() {
    x = clamp(x, -(cols / 2) + (wighth / 2), (cols / 2) - (wighth / 2));
    y = clamp(y, -(rows / 2) + (hight / 2), (rows / 2) - (hight / 2));
}*/

void Picture::draw(std::vector<std::string>* screen_vec, Screen& screen) {
    int i = 0;
    int y_coord = (screen.coord_to_vec_space(y, 'y') - 1);

    for (int iy = y_coord - (hight / 2); iy < y_coord + (hight / 2) && iy < (&screen)->rows; iy++) {

        int x_coord = screen.coord_to_vec_space(x, 'x');

        //(*screen_vec)[iy].replace(max(ceil(wighth / 2), x - floor(wighth / 2)), min(wighth, (&screen)->cols * 2 - x_coord - (wighth / 2)), image_vec[i]);

        if (iy >= 0 && x_coord + floor(wighth / 2) < (&screen)->cols * 2 && x_coord - ceil(wighth / 2) > 0 && i < image_vec.size()) {
            (*screen_vec)[iy].replace(x_coord - (wighth / 2), wighth, image_vec[i]);
            //(*screen_vec)[rows - 6].replace(cols - 6, image.size(), image);
        }
        i++;
    }
}

void Picture::add_pic(std::ifstream& file) {

    std::string str;
    while (std::getline(file, str))
    {
        image_vec.push_back(str);
    }

    hight = image_vec.size();
    wighth = image_vec[0].size();
}