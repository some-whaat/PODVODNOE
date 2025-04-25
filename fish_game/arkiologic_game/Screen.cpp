#include "Header.h"



void Screen::enshure_cols_rows() {
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    cols = (csbi.srWindow.Right - csbi.srWindow.Left + 1) / 2;
    rows = (csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
}

int Screen::coord_to_vec_space(int coord, char coord_name) {
    //this->enshure_cols_rows();

    switch (coord_name)
    {
    case 'x':
        return (coord + (cols / 2)) * 2 + camera_pos.x;
        break;

    case 'y':
        return (rows / 2) - coord + camera_pos.y;
        break;

    default:
        break;
    }
}

/*
char Screen::pix_calc(int x, int y) {
    for (std::vector<Circle>*& circs_vec : _circles) {
        for (Circle circ : *circs_vec) {
            if (circ.is_in_circle(x, y, 0)) {

                float dist = circ.dist(Position(x, y));

                return rend_style[(int)((dist / circ.rad) * 10)];
            }
        }
    }

    for (Circle circ : *_Circles[1]) {
        if (circ.is_in_circle(x, y, 0)) {

            float dist = circ.dist(position(x, y));

            return rend_style[(int)((dist / circ.rad) * 10)];
            //return std::string(2, ".....::#@@"[(int)((dist / piece.rad) * 10)]);
            //"@@00%**+:."
            //"@&%#0*+=-:"
            //".....::#@@"
        }
    }

    return ' ';
}*/

void Screen::addLayer(const std::string& name, RenderLayer&& layer, bool on_top = true) {
    if (layers.find(name) == layers.end()) {
        if (on_top) {
            layer_order.push_back(name);
        }
        else {
            layer_order.insert(layer_order.begin(), name);
        }
    }
    layers[name] = std::move(layer);
}

void Screen::render() {
    something_changed = true;
    if (something_changed) {
        system("cls");

        enshure_cols_rows();

        screen_vec = {};


        for (int i = 0; i < rows; i++) {
            screen_vec.emplace_back(cols * 2, ' ');
        }
       

        for (const std::string name : layer_order) {
            for (const auto& obj : layers[name]) {
                obj->draw(&screen_vec, *this);
            }
        }

        for (int i = 0; i < rows; i++) {

            if (!(i == rows - 1)) {
                std::cout << screen_vec[i] << std::endl;
            }
            else {
                std::cout << screen_vec[i];
            }
        }

        Sleep(MBF);
    }
    something_changed = false;
}

/*
void Screen::text_seq_render(std::vector<TextSquere> text_seq) {
    MBF = 200;
    while (GetAsyncKeyState(VK_SPACE)) {}
    something_changed = true;

    while (text_seq.size() != 0) {

        if (something_changed) {
            system("cls");
            enshure_cols_rows();

            screen_vec = {};
            for (int i = 0; i < rows; i++) {
                screen_vec.emplace_back(cols * 2, ' ');
            }


            text_seq[0].draw(&screen_vec, *this);


            for (int i = 0; i < rows; i++) {

                if (!(i == rows - 1)) {
                    std::cout << screen_vec[i] << std::endl;
                }
                else {
                    std::cout << screen_vec[i];
                }
            }

            something_changed = false;
        }


        if (GetAsyncKeyState(VK_SPACE)) {
            text_seq.erase(text_seq.begin());
            something_changed = true;

            while (GetAsyncKeyState(VK_SPACE)) {}
        }

        Sleep(MBF);
    }
    MBF = 77;
}

void Screen::add_vert_text(std::vector<std::string> texts, int step, int text_whith) {
    int i = 0;

    for (int y = (((int)texts.size() - 1) / 2) * step; y >= -(((int)texts.size() - 1) / 2) * step && i < texts.size(); y -= step) {
        _text.emplace_back(0, y, texts[i], text_whith);

        i++;
    }
}

void Screen::show_vert_text(std::vector<std::string> texts, int step, int text_whith) {
    _pictures.clear();
    _circles.clear();
    _text.clear();
    _frames.clear();

    add_vert_text(texts, step, text_whith);

    something_changed = true;
    render();
    while (!GetAsyncKeyState(VK_SPACE)) {}
    while (GetAsyncKeyState(VK_SPACE)) {}
}

void Screen::show_text_and_pic(std::string text, Picture pic) {
    while (GetAsyncKeyState(VK_SPACE)) {}
    _pictures.clear();
    _circles.clear();
    _text.clear();
    _frames.clear();


    pic.x = 0;
    pic.y = 0;

    _text.emplace_back(0, rows / 2 - 4, text, 0);
    _pictures.push_back(pic);

    something_changed = true;
    render();

    while (!GetAsyncKeyState(VK_SPACE)) {}
}

bool Screen::yes_no_choice(std::string text) {
    while (GetAsyncKeyState(VK_SPACE)) {}
    _pictures.clear();
    _circles.clear();
    _text.clear();
    _frames.clear();

    int selected_el = 0;

    _text.emplace_back(-6, 0, "no", 0);
    _text.emplace_back(6, 0, "yes", 0);

    _text.emplace_back(0, 10, text, 0);

    _text[selected_el].is_big = true;
    something_changed = true;
    render();

    while (!GetAsyncKeyState(VK_SPACE))
    {
        if (GetAsyncKeyState(VK_LEFT)) {

            while (GetAsyncKeyState(VK_LEFT)) {}

            selected_el -= 1;

            if (selected_el < 0) {
                selected_el = 1;
            }

            for (int i = 0; i < 2; i++) {
                _text[i].is_big = i == selected_el;
            }
            something_changed = true;
        }

        if (GetAsyncKeyState(VK_RIGHT)) {

            while (GetAsyncKeyState(VK_RIGHT)) {}

            selected_el += 1;

            if (selected_el >= 2) {
                selected_el = 0;
            }

            for (int i = 0; i < 2; i++) {
                _text[i].is_big = i == selected_el;
            }
            something_changed = true;
        }

        render();
    }

    while (GetAsyncKeyState(VK_SPACE)) {}
    return selected_el;
}
*/
