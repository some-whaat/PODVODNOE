#include "Header.h"



void Screen::enshure_cols_rows() {
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    cols = (csbi.srWindow.Right - csbi.srWindow.Left + 1) / 2;
    rows = (csbi.srWindow.Bottom - csbi.srWindow.Top + 1);

    init_buffers();
}

int Screen::coord_to_vec_space(float coord, char coord_name) {

    switch (coord_name) {
    case 'x':
        return static_cast<int>(std::round((coord + (cols / 2.0f) - camera_pos.x) * 2.0f));
    case 'y':
        return static_cast<int>(std::round((rows / 2.0f) - (coord - camera_pos.y)));
    default:
        return 0;
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

    enshure_cols_rows();

    something_changed = true;
    if (something_changed) {

        std::fill(backBuffer.begin(), backBuffer.end(), CHAR_INFO{ L' ', 0x07 });


        for (const std::shared_ptr<RenderLayer>& layer : render_order) {

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


        Sleep(MBF);

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

void Screen::load_layer_from_json(const nlohmann::json& layer_content) {

    //int layer_ind = layer_content.contains("layer_ind") ? int(layer_content["layer_ind"]) : -1;


    // Create render layer for this group
    auto render_layer = std::make_shared<RenderLayer>();

    // ���������� ���� �������� � ����
    for (auto& obj_type : layer_content.items()) {

        std::string type_name = obj_type.key();
        nlohmann::json objects = obj_type.value();

        if (type_name == "ParticleSystem") {

            for (auto& obj : objects.items()) {

                std::cout << obj.key() << std::endl;

                auto particle_system_shared = std::make_shared<Screen::ParticleSystem>(obj.value(), *this);
                render_layer->push_back(particle_system_shared);

                std::cout << "ParticleSystem added" << std::endl;
            }

            add_layer(render_layer, -1);

            continue;
        }

        if (type_name != "layer_ind" && type_name != "do_needs_player") {

            // ���������� ������� ������� ����
            for (auto& obj : objects.items()) {
                //std::string obj_name = obj.key();
                nlohmann::json obj_data = obj.value();

                std::shared_ptr<RendrbleObject> render_obj;

                render_obj = create_object(type_name, obj_data);

                if (render_obj) {
                    render_layer->push_back(render_obj);
                }
                else {
                    throw std::runtime_error("unknown object type: " + type_name);
                }

                render_layer->push_back(render_obj);
            }

            add_layer(render_layer, -1);
        }
    }
}




Position vectorToRectanglePoint(const Position& direction, float rectWidth, float rectHeight) {
    float halfWidth = rectWidth / 2.0f;
    float halfHeight = rectHeight / 2.0f;

    // Handle zero vector case
    if (direction.x == 0.0f && direction.y == 0.0f) {
        return Position(0.0f, 0.0f);
    }

    // Compute scaling factors to reach rectangle boundaries
    float tx = (direction.x != 0.0f) ? (halfWidth / std::abs(direction.x)) : INFINITY;
    float ty = (direction.y != 0.0f) ? (halfHeight / std::abs(direction.y)) : INFINITY;

    // Find the smallest scaling factor (closest intersection)
    float t = min(tx, ty);

    // Compute the intersection point
    return Position(direction.x * t, direction.y * t);
}


void Screen::ParticleSystem::add_particle() {

    //float radius = sqrt(pow(screen_ptr.cols, 2) + pow(screen_ptr.rows, 2)) / 2;


    std::shared_ptr<MovingObj> particle = std::make_shared<MovingObj>(particles_constructors[rand_int(0, particles_constructors.size() - 1)]);

    float ang = std::atanf(particle->velocity.y/particle->velocity.x);
    ang = particle->velocity.x > 0 ? ang + M_PIl : ang;
    ang = abs(particle->velocity.y) > abs(particle->velocity.x) ? ang + M_PIl : ang;

    int add_val = 7; // MAGIC NUMBER
	Position spawn_pos = vectorToRectanglePoint(particle->velocity.mult(-1).sum(Position(rand_int(-3, 3), rand_int(-3, 3))), screen_ptr.cols + add_val, screen_ptr.rows + add_val); // MAGIC NUMBER (��� ��������)

    particle->set_pos(spawn_pos.x + screen_ptr.camera_pos.x, spawn_pos.y + screen_ptr.camera_pos.y);
    /*
    switch (particles_spawn_type)
    {
    case Screen::ParticleSystem::ParticlesSpawnType::left_right:

		particle->set_pos(
            fasing * -((screen_ptr.cols / 2) + 5) + screen_ptr.camera_pos.x, // MAGIC NUMBER
            rand_int(-(screen_ptr.rows / 2), (screen_ptr.rows / 2)) + screen_ptr.camera_pos.y
        );

        break;

    case Screen::ParticleSystem::ParticlesSpawnType::up_down: // not finished yet
        break;

    default:
        break;
    }
*/
    particles.push_back(particle);
};

void Screen::ParticleSystem::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {  
   for (const std::shared_ptr<RendrbleObject> particle : particles) {  
       particle->draw(buffer, screen);  
   }  

   float f = particles.size();  
   for (int i = 0; f + i < on_screen_particles_count; i++) {  
       add_particle();  
   }  

   for (int i = 0; i < particles.size(); i++) {  
       if (screen_ptr.is_inside_screen(*particles[i], 11)) {  

           particles.erase(particles.begin() + i);  
       }  
   }  
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
