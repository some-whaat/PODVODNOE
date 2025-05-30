#include "Header.h"



void Screen::enshure_cols_rows() {
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    cols = (csbi.srWindow.Right - csbi.srWindow.Left + 1) / 2;
    rows = (csbi.srWindow.Bottom - csbi.srWindow.Top + 1);

    init_buffers();
}

int Screen::coord_to_vec_space(int coord, char coord_name) {
    //this->enshure_cols_rows();

    switch (coord_name)
    {
    case 'x':
        return (coord + (cols / 2) - camera_pos.x) * 2;
        break;

    case 'y':
        return (rows / 2) - (coord - camera_pos.y);
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

void Screen::add_layer(std::shared_ptr<RenderLayer> layer, int position_to_incert_to, std::string name) { // position_to_incert_to = -1 to insert to the end, 0 to insert to the beginning
    
    if (position_to_incert_to == -1 || position_to_incert_to >= render_order.size()) {
        render_order.push_back(layer);
    }
    else {
        render_order.insert(render_order.begin() + position_to_incert_to, layer);
    }

    if (name != "") {
        layers[name] = layer;
    }
}

std::vector<std::shared_ptr<RendrbleObject>>* Screen::get_layer(const std::string& name) {
    auto it = layers.find(name);
    
	if (it == layers.end()) {
		std::cerr << "Layer not found: " << name << std::endl;
		return nullptr;
	}
    return it->second.get();
}

void Screen::render() {
    //std::chrono::steady_clock::time_point previous_time = std::chrono::steady_clock::now();

    enshure_cols_rows();

    something_changed = true;
    if (something_changed) {

        std::fill(backBuffer.begin(), backBuffer.end(), CHAR_INFO{ L' ', 0x07 });


        int i = 0; // for debug
        for (const std::shared_ptr<RenderLayer>& layer : render_order) {
			i++;
            if (!layer) {
				//throw std::runtime_error("Render layer is null!" + i);
				continue;
            }

            for (const std::shared_ptr<RendrbleObject>& obj : *layer) {
                obj->draw(backBuffer, *this);
            }
        }

        swap_buffers();
        something_changed = false;


/*
        system("cls");

        enshure_cols_rows();

        screen_vec = {};


        for (int i = 0; i < rows; i++) {
            screen_vec.emplace_back(cols * 2, ' ');
        }

        for (const auto& layer : render_order) {
            for (const auto& obj : *layer) {
                obj->draw(&screen_vec, *this);
            }
        }

       
        for (auto* layer : render_order) {
            for (const auto& obj : *layer) obj->draw(&screen_vec, *this);
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
*/

        Sleep(MBF);

        //deltatime = (std::chrono::steady_clock::now() - previous_time).count();
    }
    something_changed = false;
}

void Screen::init_buffers() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    backBuffer.resize(cols * rows * 2, { L' ', 0x07 });
}

void Screen::swap_buffers() {
    COORD bufferSize = { static_cast<SHORT>(cols * 2), static_cast<SHORT>(rows) };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, static_cast<SHORT>(cols * 2 - 1), static_cast<SHORT>(rows - 1)};

    WriteConsoleOutput(hConsole, backBuffer.data(), bufferSize, bufferCoord, &writeRegion);
}


void Screen::ParticleSystem::add_particle() {
    int fasing = rand_int(0, 1) == 0 ? -1 : 1;


    std::shared_ptr<MovingObj> particle = std::make_shared<MovingObj>(particles_constructors[fasing]);

    switch (particles_spawn_type)
    {
    case Screen::ParticleSystem::ParticlesSpawnType::left_right:

		particle->change_pos(
            fasing * -((screen_ptr.cols / 2) + 5) + screen_ptr.camera_pos.x, // MAGIC NUMBER
            rand_int(-(screen_ptr.rows / 2), (screen_ptr.rows / 2)) + screen_ptr.camera_pos.y
        );

        break;

    case Screen::ParticleSystem::ParticlesSpawnType::up_down: // not finished yet
        break;

    default:
        break;
    }

    particles.push_back(particle);
};

void Screen::ParticleSystem::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {
    for (const std::shared_ptr<RendrbleObject> particle : particles) {
        particle->draw(buffer, screen);
    }


    float f = particles.size();
    for (int i = 0; f + i < 5; i++) {

        add_particle();
    }

    // убиваем лишних
    for (int i = 0; i < particles.size(); i++) {
        if (this->particles[i]->x < -screen_ptr.cols / 2 - 8 + screen_ptr.camera_pos.x ||
            this->particles[i]->x > screen_ptr.cols / 2 + 8 + screen_ptr.camera_pos.x ||
            this->particles[i]->y < -screen_ptr.rows / 2 - 8 + screen_ptr.camera_pos.y ||
            this->particles[i]->y > screen_ptr.rows / 2 + 8 + screen_ptr.camera_pos.y) {

            particles.erase(particles.begin() + i);
        }
    }

};

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
