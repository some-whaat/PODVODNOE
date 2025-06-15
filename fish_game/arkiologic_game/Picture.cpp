#include "Header.h"

void Picture::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {


    if (is_render == true) {


        int i = 0;
        int renderX = static_cast<int>(std::round(x)); // Player's x position (float)
        int renderY = static_cast<int>(std::round(y)); // Player's y position (float)

        Position camera_pos = screen.get_camera_pos();
        // Round camera offsets after parallax calculation
        int camOffsetX = static_cast<int>(std::round(camera_pos.x) * add_paralax);
        int camOffsetY = static_cast<int>(std::round(camera_pos.y) * add_paralax);

        //int x_coord = static_cast<int>(std::round((x + (screen.cols / 2.0f) - camera_pos.x + camera_pos.x * add_paralax) * 2.0f)); //screen.coord_to_vec_space(renderY, 'y') - camOffsetY;
        //int y_coord = static_cast<int>(std::round((screen.rows / 2.0f) - (y - camera_pos.y + camera_pos.y * add_paralax)));//screen.coord_to_vec_space(renderX, 'x') + camOffsetX;

        int y_coord = screen.coord_to_vec_space(renderY, 'y') - camOffsetY;
        int x_coord = screen.coord_to_vec_space(renderX, 'x') + camOffsetX;

        for (int iy = y_coord - (hight / 2); iy < y_coord + (hight / 2) && iy < screen.rows; iy++) {


            if (iy >= 0 && i < image_vec.size()) {
                int ii = 0;
                for (int ix = x_coord - (wighth / 2); ix <= x_coord + (wighth / 2) && ix < screen.cols * 2; ix++) {
                    if (ii >= image_vec[i].size()) {
                        break;
                    }
                    if (ix >= 0 && image_vec[i][ii] != '?') {
                        //(*screen_vec)[iy][ix] = image_vec[i][ii];

                        buffer[iy * screen.cols * 2 + ix].Char.AsciiChar = image_vec[i][ii];

                        buffer[iy * screen.cols * 2 + ix].Attributes = color_attr;
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