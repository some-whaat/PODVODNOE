#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <windows.h>
#include <chrono>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <cctype>
#include <random>
#include <climits>
#include <conio.h>
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>
#include <cmath>
#define M_PIl 3.141592653589793238462643383279502884L


class Position;
class Circle;
class Rektangle;
class TextSquere;
class Picture;
class Screen;
class Player;
class NPC;

extern bool is_working;


int rand_int(int down_bord, int up_bord);
bool isSubset(const std::vector<int>& subset, const std::vector<int>& superset);
std::vector<std::vector<std::string>> read_objs_from_file(const std::string& file_str);
std::vector<std::vector<std::string>> read_objs_from_file(std::ifstream& file);

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

static nlohmann::json load_json(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open JSON file: " + filename);
    }
    nlohmann::json data;
    file >> data;
    return data;
}


class Position {
public:
    float x;
    float y;

    Position() : x(0), y(0) {}

    Position(std::vector<float> vec) {
        x = vec[0];
        y = vec[1];
    }

    Position(float in_x, float in_y) : x(in_x), y(in_y) {}

    Position(int x_up, int x_down, int y_up, int y_down) : x(rand_int(x_up, x_down)), y(rand_int(y_up, y_down)) {}

    explicit Position(const nlohmann::json& data) {
        if (data.contains("Position")) {
			x = data["Position"]["x"];
			y = data["Position"]["y"];
		}
        else {
            x = 0;
            y = 0;
        }
    }

    Position sum(Position pos);

    Position mins(Position pos);

    Position mult(float num);

    Position div(float num);

    float dist(const Position& other);

    float len();

    Position normalise();

    void follow(const Position& to_pos, float speed, float min_dist);

    void move_to(Position to_pos, float min_dist);

    void smooth_follow(Position to_pos, float min_dist, float speed, float max_len, float deltaTime = 1);

    Position get_pos();

    void set_pos(Position pos);

    void set_pos(float x_, float y_);

    Position coords_to_vec_space(Position coord_pos, int cols, int rows);

    void round();
};

class RendrbleObject : public Position {

protected:

    RendrbleObject() : Position(), is_steak_to_screen(false), is_render(true), add_paralax(0) {}

    explicit RendrbleObject(const nlohmann::json& data) : Position(data) {

        if (data.contains("RendrbleObject")) {
            if (data["RendrbleObject"].contains("is_steak_to_screen")) {
                is_steak_to_screen = data["RendrbleObject"]["is_steak_to_screen"];
            }
            is_render = data["RendrbleObject"]["is_render"];

			add_paralax = data["RendrbleObject"]["add_paralax"];

            auto color_arr = data["RendrbleObject"]["color"];

            color_attr = FOREGROUND_RED * color_arr[0] | FOREGROUND_GREEN * color_arr[1] | FOREGROUND_BLUE * color_arr[2];
        }
        else {
            x = 0;
            y = 0;
            is_steak_to_screen = false;
            is_render = true;
        }
    }

public:

    WORD color_attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    bool is_steak_to_screen = false;
    bool is_render = true;

    float add_paralax = 0;


    virtual void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) = 0;

    virtual bool is_inside(Position pos, float add_dist) = 0;

    virtual void action(std::shared_ptr<Player> player) = 0;

};

class Circle : public RendrbleObject {
public:
    float rad;   

    std::string rend_style;


    Circle() : rad(1) {
    }

    Circle(float in_x, float in_y, float radius) : rad(radius) {
		x = in_x;
		y = in_y;
    }

    Circle(Position in_pos, float radius) : rad(radius) {
        x = in_pos.x;
        y = in_pos.y;
    }

    bool is_in_circle(int in_x, int in_y, float add_rad);

    bool is_in_circle(Position other_pos, float add_rad);

    void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;

    void action(std::shared_ptr<Player> player) override {}
};

class Rektangle : public RendrbleObject {
public:
    float hight;
    float wighth;
    float rot_ang = 0;

    bool is_big = false;

    char fill;
    int add_val;

    bool is_whith_frame;
    bool is_frame;


    Rektangle() : RendrbleObject(), hight(0), wighth(0), add_val(0), is_whith_frame(false), is_frame(false), fill(' ') {}

    Rektangle(float in_x, float in_y, float in_wighth, float in_hight)
        : hight(in_hight), wighth(in_wighth), add_val(0), is_whith_frame(false), is_frame(false), fill(' ') {
        x = in_x;
        y = in_y;
    }

    explicit Rektangle(const nlohmann::json& data) : RendrbleObject(data) {

		if (data.contains("Rektangle")) {
			hight = data["Rektangle"]["hight"];
			wighth = data["Rektangle"]["wighth"];
			add_val = data["Rektangle"]["add_val"];
			is_whith_frame = data["Rektangle"]["is_whith_frame"];
			is_frame = data["Rektangle"]["is_frame"];
			//fill = data["Rektangle"]["fill"];
            
			if (data.contains("rot_ang")) {
				rot_ang = data["rot_ang"];
			}
		}
		else {
            Rektangle::Rektangle();
		}
    }

    bool is_inside(Position pos, float add_dist) override final;

    bool is_in_rec(int in_x, int in_y, float add_dist);

    void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;

    void draw_frame(std::vector<CHAR_INFO>& buffer, Screen& screen);

    void action(std::shared_ptr<Player> player) override {}
};

class TextSquere : public Rektangle {
protected:
    std::vector<std::string> text_vec;

public:
    bool follow = false;
    Position* follow_pos = nullptr;
    int follow_wighth = 0;
    int follow_hight = 0;

    TextSquere() : Rektangle(), text_vec({}) {}

    TextSquere(std::string in_text, int in_wighth) {
        x = 0;
        y = 0;

        add_val = 2;
		set_text(in_text, in_wighth);
    }

    TextSquere(std::string in_text, int in_wighth, bool _is_steak_to_screen) {
        x = 0;
        y = 0;
        is_steak_to_screen = _is_steak_to_screen;

        add_val = 2;
        set_text(in_text, in_wighth);
    }

    TextSquere(int in_x, int in_y, std::string in_text, float in_wighth) {
        x = in_x;
        y = in_y;

        if (in_wighth == 0) {
            wighth = in_text.size();
            text_vec.push_back(in_text);
            hight = 1;
        }
        else {
            wighth = in_wighth;

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

    void text_follow(int facing);

    void set_text(std::string in_text, int in_wighth);

    void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;

    void action(std::shared_ptr<Player> player) override {}
};

class UI : public RendrbleObject {
protected:
    int poses_arr[5][2] = {
    {0, -1}, // 0 down
    {0, 1},  // 1 up 
    {1, 1},  // 2 riht up
    {-1, 1},  // 3 left up
    {0, 0} // 4 center
    };

    int screen_pos;

    std::vector<TextSquere> ui_els;
    bool stuck_type_is_vertical; // true -- vertical, false -- horisontal
    int el_wighth = 0;
    int spacing = 5;

    float total_width = 0.0f;
    float total_height = 0.0f;

public:

    UI() : RendrbleObject(), screen_pos(4) {}

    UI(std::string in_text, float in_wighth, int screen_pos_) : screen_pos(screen_pos_) {
        ui_els.emplace_back(in_text, in_wighth, true);
    }

    UI(std::vector<std::string> text_list, int in_el_wighth, int in_spacing, bool stuck_type, int in_screen_pos) : RendrbleObject(), el_wighth(in_el_wighth), spacing(in_spacing), stuck_type_is_vertical(stuck_type) {

        screen_pos = in_screen_pos;

        for (std::string text : text_list) {
            ui_els.emplace_back(text, el_wighth, true);

            if (stuck_type_is_vertical) {
                total_width += ui_els.back().wighth;
                total_height += ui_els.back().hight;
            }
            else {
                throw std::runtime_error("не доделано есчо");
            }
        }

        total_width += (ui_els.size() - 1) * spacing;
        total_height += (ui_els.size() - 1) * spacing;
    }

    void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;

    void action(std::shared_ptr<Player> player) override {}

    void set_big_el(int el_ind);

    bool is_inside(Position pos, float add_dist) override {
        return false;
    }
};


class Picture : public Rektangle {
protected:

    std::vector<std::string> image_vec;

    Position resigual_movement = Position(); // рабочая

public:
    Picture() : Rektangle(), image_vec({}) {}

    explicit Picture(std::string json_file) : Picture(load_json(json_file)) {}

    explicit Picture(const nlohmann::json& data) : Rektangle(data) {
        if (data.contains("Picture")) {

            if (data["Picture"].contains("sprite_filepame")) {
                add_pic(data["Picture"]["sprite_filepame"]);
            }
        }
        else {
            image_vec = {};
        }
    }

    Picture(std::ifstream& file, int in_x, int in_y) {
        x = in_x;
        y = in_y;

        add_pic(file);
    }

    Picture(const std::string& file, int in_x, int in_y, float in_add_paralax = 0) {
        x = in_x;
        y = in_y;

		add_paralax = in_add_paralax;

        add_pic(file);
    }


    Picture(std::vector<std::string> in_image_vec, int in_x, int in_y) : image_vec(in_image_vec) {
        x = in_x;
        y = in_y;

        hight = image_vec.size();
        wighth = image_vec[0].size();
    }

    void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;
    void action(std::shared_ptr<Player> player) override {}

protected:
    void add_pic(std::ifstream& file);

    void add_pic(std::string file);
};

class AnimatbleObj : public Picture {
protected:
    float anim_speed = 1;

    int anim_calls = 0;
    int curr_frame_ind = 0;

public:
    std::vector<std::vector<std::string>> anim_frames;

    AnimatbleObj() : anim_frames({ {} }) {}

    explicit AnimatbleObj(std::string json_file) : AnimatbleObj(load_json(json_file)) {}

    explicit AnimatbleObj(const nlohmann::json& data) : Picture(data) {

		if (data.contains("AnimatbleObj")) {
			anim_speed = data["AnimatbleObj"]["anim_speed"];

            read_anim_frames(data["AnimatbleObj"]["animated_sprite_filepame"]);
		}
		else {
			anim_speed = 1;
            anim_frames = {};
		}
    }

    AnimatbleObj(const std::string& file_name, float in_anim_speed, int in_x, int in_y)
        : anim_speed(in_anim_speed) {

        x = in_x;
        y = in_y;

        read_anim_frames(file_name);
    }

    void read_anim_frames(const std::string& file_str);
    void animation();
    void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;
    void action(std::shared_ptr<Player> player) override {}
};

class MovingObj : public AnimatbleObj {
private:
    float wave_hight_bound = 20;
    Position firt_pos;


    float wave_hight;
    float wave_offset;
    float wave_lenght;
    float wave_sdvig;

    float ang_coef;

public:

    enum class MovingType {
        none,
        wave,
        random_wave,
        straight, // not implemented
        circle, // not implemented
        random // not implemented
    };

    MovingType moving_type = MovingType::wave;

    Position velocity;

    MovingObj() {}


    explicit MovingObj(std::string json_file) : MovingObj(load_json(json_file)) {}

    explicit MovingObj(const nlohmann::json& data) : AnimatbleObj(data) {

        if (data.contains("MovingObj")) {

            if (data["MovingObj"].contains("velocity")) {
                velocity = Position(data["MovingObj"]["velocity"][0], data["MovingObj"]["velocity"][1]);
            }
            else {
                if (data["MovingObj"].contains("rand_velocity")) {

                    velocity = Position(
                        data["MovingObj"]["rand_velocity"][0],
                        data["MovingObj"]["rand_velocity"][1],
                        data["MovingObj"]["rand_velocity"][2],
                        data["MovingObj"]["rand_velocity"][3]);
                }
                else {
                    velocity = Position();
                }
            }

            ang_coef = velocity.x == 0 ? 0 : velocity.y / velocity.x;

			std::map<std::string, MovingType> moving_type_data = {
                {"none", MovingType::none},
                {"wave", MovingType::wave},
                {"random_wave", MovingType::random_wave},
				{"straight", MovingType::straight}, // не сделано
				{"circle", MovingType::circle}, // не сделано
				{"random", MovingType::random} // не сделано
			};

            moving_type = moving_type_data[data["MovingObj"]["moving_type"]];

            switch (moving_type)
            {
			case MovingType::none:
				velocity = Position(0, 0);
				wave_hight = 0;
				wave_offset = 0;
				wave_lenght = 0;
				wave_sdvig = 0;
				break;

            case MovingType::wave:
				wave_hight = data["MovingObj"]["wave"]["wave_hight"];
				wave_offset = data["MovingObj"]["wave"]["wave_offset"];
				wave_lenght = data["MovingObj"]["wave"]["wave_lenght"];
                wave_sdvig = y;
				break;

            case MovingType::random_wave:
				int max_x = data["MovingObj"]["random_wave"]["max_x"];
				int max_y = data["MovingObj"]["random_wave"]["max_y"];

                wave_hight = rand_int(-max_y + wave_hight_bound, max_y - wave_hight_bound);
                wave_offset = rand_int(-max_x, max_x);
                wave_lenght = static_cast<float>(rand_int(-4, 4)) / 10;
                wave_sdvig = y;
				break;

		 // надо конечн доделать движений
            }

            moving_type = moving_type == MovingType::random_wave ? MovingType::wave : moving_type;
        }
        else {
            velocity = Position(0, 0);
        }

        firt_pos = Position(x, y);
    }

    MovingObj(std::string file_name, int in_x, int in_y, int in_fasing, Position in_velocity, int max_x, int max_y, float in_anim_speed = 1, float in_add_paralax = 0)
        : AnimatbleObj(file_name, in_anim_speed, in_x, in_y),
        velocity(in_velocity) {

		add_paralax = in_add_paralax;
        wave_hight = rand_int(-max_y + wave_hight_bound, max_y - wave_hight_bound);
        wave_offset = rand_int(-max_x, max_x);
        wave_lenght = static_cast<float>(rand_int(-4, 4)) / 10;
        wave_sdvig = y;

        firt_pos = Position(in_x, in_y);
    }

	void set_pos(int x_, int y_) {
		x = x_;
		y = y_;

        firt_pos = get_pos();
	}

    void move(Screen& screen);
    void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;
    void action(std::shared_ptr<Player> player) override {}
};


class NPC : public MovingObj {

    bool is_actioning = false; // служебная переменная, чтобы не было постоянного нажатия на пробел
    bool seted_stuff = false;
    int selected_el = 0;
    int choice_count = 0;
	int dialogue_ind = 0;


    std::shared_ptr<Player> player;
    int dist_to_interact = 15;

protected:

    enum class StateType {
        logic,
        dialogue,
        player_choice
    };

    TextSquere text_bubble = TextSquere();

    int text_wight = 20;

    int state = 0;
    std::unordered_map<int, nlohmann::json> data_base;

	std::vector<std::shared_ptr<UI>> ui_elements;



public:

	NPC() : MovingObj() {
		text_bubble.add_val = 2;
		text_bubble.follow = true;
		text_bubble.follow_pos = this;
		text_bubble.follow_wighth = wighth;
		text_bubble.follow_hight = hight;
	}

    explicit NPC(std::string json_file) : NPC(load_json(json_file)) {}

    explicit NPC(const nlohmann::json& data) : MovingObj(data) {

        text_bubble.add_val = 2;
        text_bubble.follow = true;
        text_bubble.follow_pos = this;
        text_bubble.follow_wighth = wighth;
        text_bubble.follow_hight = hight;

        if (data.contains("NPC")) {

            nlohmann::json npc_data = data["NPC"];

            if (npc_data.contains("dist_to_interact")) {
                dist_to_interact = npc_data["dist_to_interact"];
            }

            state = npc_data["defult_state"];
            text_bubble.is_render = npc_data["does_has_dialogue_on"];

            for (const auto& state_ : npc_data["states"].items()) {
                const std::string& state_str = state_.key();
                const auto& state_data = state_.value();
                int state_id = std::stoi(state_str);
                data_base[state_id] = state_data;
            }
		}
        else {
            throw std::runtime_error("NPC can't find itself (no definition for the NPC in a JSON)");
        }
    }

    void action(std::shared_ptr<Player> player) override;

    void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;

    void process_logic(std::shared_ptr<Player> player);
    void process_dialogue();
    void process_player_choice(std::shared_ptr<Player> player);
    void process_npc_action(std::shared_ptr<Player> player);

    bool attributes_check(std::shared_ptr<Player> player);
    bool item_check(std::shared_ptr<Player> player);
};


class Player : public AnimatbleObj {
private:
    float move_count = 0;

    std::unordered_map<int, std::shared_ptr<AnimatbleObj>> inventory;
	std::vector<std::shared_ptr<AnimatbleObj>> inventory_vec; // костыль, чтобы можно было отрисовать инвентарь в линеечку


	std::unordered_map<int, nlohmann::json> inventory_data_base; // id : json filename



public:

    nlohmann::json attributes = {};
    float speed = 1;


    explicit Player(std::string json_file) : Player(load_json(json_file)) {}

    explicit Player(const nlohmann::json& data)
        : AnimatbleObj(data),
        move_count(0),
        inventory(),
        inventory_vec(),
        inventory_data_base(),
        attributes(),
        speed(1) {

		if (data.contains("Player")) {
			speed = data["Player"]["speed"];

			attributes = data["Player"]["attributes"];

            for (const auto& item : data["Player"]["items"].items()) {
                const auto& item_data = item.value();
                int item_id = std::stoi(item.key());
                inventory_data_base[item_id] = item_data;
            }

            for (int item_id : data["Player"]["inventory"]) {
                add_item_to_inventory(item_id);
            }
		}
		else {
            throw std::runtime_error("no definition for the Player in a JSON");
		}
    }

    void add_item_to_inventory(int item_id);

    void remove_item_from_inventory(int item_id);

    bool does_has_item(int item_id);

    void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;

    void move(float deltaTime, Screen& screen);
};


class Screen {

protected:

    HANDLE hConsole;
    std::vector<CHAR_INFO> backBuffer;

    using RenderLayer = std::vector<std::shared_ptr<RendrbleObject>>;

    std::vector<std::string> screen_vec;

    bool something_changed = true;

    std::vector<std::shared_ptr<RenderLayer>> render_order;
    std::unordered_map<std::string, std::shared_ptr<RenderLayer>> layers;


    std::string rend_style = "0000000000";

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    Position camera_pos = Position(0, 0);
    float camera_speed = 1;

    float MBF = 2; //milliseconds between frames BETTER NOT TO SET TO 0


    int border_poses[2][2] = {
    {-60, 60}, // 0 по иксу от-до
    {-40, 40},  // 1 по игрику от-до
    };

public:

    float deltaTime;

    
    int cols, rows;

    Screen() {

        enshure_cols_rows();

        std::fill(backBuffer.begin(), backBuffer.end(), CHAR_INFO{ L' ', 0x07 });
    }


    void add_layer(std::shared_ptr<RenderLayer> layer, int position_to_incert_to, std::string name = "");

    void load_layer_from_json(const nlohmann::json& data);

	void replace_layer(std::shared_ptr<RenderLayer> layer, int ind, const std::string& name = "") {
		if (ind < 0 || ind >= render_order.size()) {
            throw std::runtime_error("Index out of bounds: " + ind);
			return;
		}
        render_order[ind] = layer;

        if (name != "") {
            layers[name] = layer;
        }
	}

    std::vector<std::shared_ptr<RendrbleObject>>* get_layer(const std::string& name);

    void enshure_cols_rows();

    int coord_to_vec_space(float coord, char coord_name);

    bool is_inside_screen(Position pos, float add_val = 0) {
        return (pos.x < -cols / 2 - add_val + camera_pos.x ||
            pos.x > cols / 2 + add_val + camera_pos.x ||
            pos.y < -rows / 2 - add_val + camera_pos.y ||
            pos.y > rows / 2 + add_val + camera_pos.y);
    }

    bool is_inside_borders(Position pos, float add_val = 0) {

        return (
            pos.x > border_poses[0][0] &&
            pos.x < border_poses[0][1] &&
            pos.y > border_poses[1][0] &&
            pos.y < border_poses[1][1]
            );
    }

	Position get_camera_pos() {
        return camera_pos;
        //return Position(round(camera_pos.x), round(camera_pos.y));
	}

    //void draw_pic(std::vector<std::string>* screen_vec, Screen& screen);

    void render();

    void init_buffers();

    void swap_buffers();

    virtual void process() = 0;



    class ParticleSystem : public RendrbleObject {

		RenderLayer particles;

		enum class ParticlesSpawnType {
			left_right,
			up_down // not finished yet
		};


        std::vector<nlohmann::json> particles_constructors; // direction :  MovingObj JSON for this direction
        int constructors_amount; // amount of constructors (to not check it every time)

        int on_screen_particles_count;

		Screen& screen_ptr; // возможно нужно поменять на std::shared_ptr<Screen> или std::weak_ptr<Screen>

    public:

        ParticleSystem(nlohmann::json data, Screen& in_screen_ptr) : RendrbleObject(data), screen_ptr(in_screen_ptr) {

			on_screen_particles_count = int(data["on_screen_particles_count"]);
            bool first_spawn_particles_randomly = data["first_spawn_particles_randomly"];


			//particles_spawn_type = moving_type_data[data["particles_spawn_type"]];
			
            for (const auto& particle_data : data["particles_constructors"].items()) {

                const nlohmann::json& particle_json = particle_data.value();

                particles_constructors.push_back(particle_json);
            }


            for (int i = 0; i < on_screen_particles_count; i++) {

                if (first_spawn_particles_randomly) {

                    int fasing = rand_int(0, 1) == 0 ? -1 : 1;
                    std::shared_ptr<MovingObj> particle = std::make_shared<MovingObj>(particles_constructors[rand_int(0, particles_constructors.size() - 1)]);

                    particle->set_pos(
                        rand_int(-(screen_ptr.cols / 2) + 10, (screen_ptr.cols / 2) - 10) + screen_ptr.camera_pos.x,
                        rand_int(-(screen_ptr.rows / 2) + 10, (screen_ptr.rows / 2) - 10) + screen_ptr.camera_pos.y
                    );

                    particles.push_back(particle);
                }
                else {
                    add_particle();
                }                               
            }
        }

        void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;

        void add_particle();

		void action(std::shared_ptr<Player> player) override {};

        bool is_inside(Position pos, float add_dist) override {
            return false;
        };

    };

};

std::unique_ptr<RendrbleObject> create_object(const std::string& classNameStr, nlohmann::json data);

class World : public Screen {
private:
    std::shared_ptr<Player> player;
    float collision_add_val_to_NPC = 0;

    std::vector<int> actions_layers_indxs;

public:

    World() {
        std::shared_ptr<RenderLayer> bg_fish;
        bg_fish = std::make_shared<RenderLayer>();

        for (int i = 0; i < 4; i++) {

            int fasing = rand_int(0, 1) == 0 ? -1 : 1;

            std::string fish_f;
            if (fasing == 1) {
                fish_f = "fish_right1.txt";
            }
            else {
                fish_f = "fish_left1.txt";
            }

            bg_fish->push_back(std::make_shared<MovingObj>(
                fish_f,
                
                rand_int(-(cols / 2) + 10, (cols / 2) - 10),
                rand_int(-(rows / 2) + 10, (rows / 2) - 10),
                
                fasing,
                Position(rand_int(1, 2), rand_int(1, 2)),
                cols,
                rows,
				-0.4f
            ));
        }

        add_layer(bg_fish, true, "bg_fish");

        player = std::make_shared<Player>(std::string("Player_draft.json"));

        std::shared_ptr<RenderLayer> player_l;
        player_l = std::make_shared<RenderLayer>();

        player_l->push_back(player);

        add_layer(player_l, true, "player");


        std::shared_ptr<UI> ui_down = std::make_shared<UI>("press space to interact", 0, 0);
        ui_down->is_render = false;

        std::shared_ptr<RenderLayer> ui_layer = std::make_shared<RenderLayer>();

        ui_layer->push_back(ui_down);

        add_layer(ui_layer, true, "ui_layer");
    }

    explicit World(std::string world_json_filename) {

        enshure_cols_rows();

        
		nlohmann::json world_json = load_json(world_json_filename);

        camera_speed = world_json["camera_speed"];
        collision_add_val_to_NPC = world_json["collision_add_val_to_NPC"];
        border_poses[0][0] = world_json["border_poses"][0][0];
        border_poses[0][1] = world_json["border_poses"][0][1];
        border_poses[1][0] = world_json["border_poses"][1][0];
        border_poses[1][1] = world_json["border_poses"][1][1];

		
        auto& layers_data = world_json["layers"];
        render_order.resize(layers_data.size() + render_order.size());

        if (layers_data.contains("layers_order")) {

            std::string layer_names_filename = layers_data["layers_order"];
            std::ifstream layer_names(layer_names_filename);
            std::string layer_filename;

            if (!layer_names.is_open()) {
                throw std::runtime_error("Failed to open file " + layer_names_filename);
            }

            while (std::getline(layer_names, layer_filename)) {
                
                nlohmann::json layer_content = load_json(layer_filename);

                
                if (layer_filename == "Player.json") {
                    player = std::make_shared<Player>(layer_content);

                    auto player_layer = std::make_shared<RenderLayer>();
                    player_layer->push_back(player);
                    add_layer(player_layer, -1, "player");

                    continue;
                }

                load_layer_from_json(layer_content);

                if (layer_content.contains("do_needs_player")) {
                    if (layer_content["do_needs_player"]) {
                        actions_layers_indxs.push_back(render_order.size() - 1); // !! надо по хорошому исправить, чтобы load_layer_from_json возвращал индекс нового слоя и использовать его, ато так не robust
                    }
                }
               
            }

            layer_names.close();


        }
        else {
            extract_from_single_json(layers_data);
        }
    }

    void process() override;

    void extract_from_single_json(const nlohmann::json& world_json);
};

#endif