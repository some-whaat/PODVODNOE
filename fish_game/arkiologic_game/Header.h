#pragma once

#include <iostream>
#include <windows.h>
#include <chrono>
#include <vector>
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


class Position;
class Circle;
class Rektangle;
class TextSquere;
class Picture;
class Screen;
class Player;

extern bool is_working;


int rand_int(int down_bord, int up_bord);
std::vector<std::vector<std::string>> read_objs_from_file(const std::string& file_str);
std::vector<std::vector<std::string>> read_objs_from_file(std::ifstream& file);
template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}


class Position {
private:
    bool is_moving = false;

public:
    float x;
    float y;

    //float velosity;

    Position() : x(0), y(0) {}

    Position(std::vector<float> vec) {
        x = vec[0];
        y = vec[y];
    }

    Position(float in_x, float in_y) : x(in_x), y(in_y) {}

    void sum(Position pos);

    Position mins(Position pos);

    Position mult(float num);

    Position div(float num);

    float dist(const Position& other);

    float len();

    Position normalise();

    void follow(const Position& to_pos, float speed, float min_dist);

    void move_to(Position to_pos, float min_dist);

    void smooth_follow(Position to_pos, float min_dist, float speed, float max_len);

    Position get_pos();

    Position coords_to_vec_space(Position coord_pos, int cols, int rows);
};

class RendrbleObject : public Position {
public:

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


    Rektangle() : hight(0), wighth(0), add_val(0), is_whith_frame(false), is_frame(false), fill(' ') {}

    Rektangle(float in_x, float in_y, float in_wighth, float in_hight)
        : RendrbleObject(), hight(in_hight), wighth(in_wighth), add_val(0), is_whith_frame(false), is_frame(false), fill(' ') {
        x = in_x;
        y = in_y;
    }

    bool is_inside(Position pos, float add_dist) override final;

    bool is_in_rec(int in_x, int in_y, float add_dist);

    void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;

    void draw_frame(std::vector<CHAR_INFO>& buffer, Screen& screen);

    void action(std::shared_ptr<Player> player) override {}
};

class TextSquere : public Rektangle{
protected:
    std::vector<std::string> text_vec;

public:
    bool follow = false;
    Position* follow_pos = nullptr;
    int follow_wighth = 0;
    int follow_hight = 0;

    TextSquere() : text_vec({}) {}

    TextSquere(std::string in_text, int in_wighth) {
        x, y = 0;

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

            //std::string add_line = std::string(' ', in_text.size() % (int)wighth);
            //in_text += add_line;

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

class Picture : public Rektangle {
public:
    std::vector<std::string> image_vec;

	float add_paralax = 0;

    Picture() : Rektangle(), image_vec({}) {}

    Picture(const std::string& file) {
        x, y = 0;

        add_pic(file);
    }

    Picture(std::ifstream& file, int in_x, int in_y) {
        x = in_x;
        y = in_y;

        add_pic(file);
    }

    Picture(const std::string& file, int in_x, int in_y, float in_add_paralax = 0) : add_paralax(in_add_paralax) {
        x = in_x;
        y = in_y;

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


public:
    float wave_hight;
    float wave_offset;
    float wave_lenght;
    float wave_sdvig;

    int fasing;

    Position velocity;

    MovingObj() {}

    MovingObj(std::string file_name, int in_x, int in_y, int in_fasing, Position in_velocity, int max_x, int max_y, float in_anim_speed = 1, float in_add_paralax = 0)
        : AnimatbleObj(file_name, in_anim_speed, in_x, in_y),
        velocity(in_velocity),
        fasing(in_fasing) {

		add_paralax = in_add_paralax;
        wave_hight = rand_int(-max_y + wave_hight_bound, max_y - wave_hight_bound);
        wave_offset = rand_int(-max_x, max_x);
        wave_lenght = static_cast<float>(rand_int(-4, 4)) / 10;
        wave_sdvig = y;

        firt_pos = Position(in_x, in_y);
    }

    /*
    MovingObj(std::string& file_name, int in_x, int in_y, int in_fasing, Position in_velocity, int max_x, int max_y, float in_anim_speed = 1)
        : AnimatbleObj(file_name, in_anim_speed, in_x, in_y),
        velocity(in_velocity),
        fasing(in_fasing)
    {

        wave_hight = rand_int(-max_y + wave_hight_bound, max_y - wave_hight_bound);
        wave_offset = rand_int(-max_x, max_x);
        wave_lenght = (float)rand_int(-4, 4) / 10;
        wave_sdvig = y;
        
		firt_pos = Position(in_x, in_y);
    }*/

    void move(Screen& screen);
    void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;
    void action(std::shared_ptr<Player> player) override {}
};

/*
class NPC : public MovingObj {
private:

    struct LogicActions {
        int needed_item_id;
        std::string text;
        int next_state;
    };


    bool has_textbb = false;
    TextSquere text_bubble = TextSquere();

public:
    int id;
    int state = 0;

    std::unordered_map<int, LogicActions> data_base;

    NPC(int in_id, std::string& file_name, std::unordered_map<int, LogicActions> data_base, int in_x, int in_y, int in_fasing, Position in_velocity, int max_x, int max_y, float in_anim_speed = 1) : MovingObj(file_name, in_x, in_y, in_fasing, in_velocity, max_x, max_y, in_anim_speed = 1), text_bubble(TextSquere()), id(in_id) {
        
    }
};*/

class NPC : public MovingObj {
private:
    struct LogicActions {
        int needed_item_id;
        std::string dialogue;
        int next_state;
        int reward_item_id;
    };

    bool has_textbb = false;
    TextSquere text_bubble = TextSquere();

    int text_wight = 20;

public:
    int id;
    int state = 0;
    std::unordered_map<int, LogicActions> data_base;


    NPC(std::string json_file) {

        std::ifstream file(json_file);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open JSON file: " + json_file);
        }

        nlohmann::json npc_data;
        file >> npc_data;

        id = npc_data["id"];
        read_anim_frames(npc_data["sprite_filepame"]);
        x = npc_data["x"];
        y = npc_data["y"];
        velocity = Position(npc_data["velocity"]);
        anim_speed = npc_data["anim_speed"];

        //text_bubble.fill = '%';
        text_bubble.add_val = 2;
        text_bubble.follow = true;
        text_bubble.follow_pos = this;
		text_bubble.follow_wighth = wighth;
        text_bubble.follow_hight = hight;
        
        for (const auto& state : npc_data["states"].items()) {
            const std::string& state_str = state.key();
            const auto& state_data = state.value();
            int state_id = std::stoi(state_str);
            data_base[state_id] = {
                state_data["needed_item_id"],
                state_data["dialogue"],
                state_data["next_state"],
                state_data["reward_item_id"]
            };
        }
        has_textbb = true;
        set_text();
    }


    void load_from_json(const std::string& json_file) {
        std::ifstream file(json_file);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open JSON file: " + json_file);
        }

        nlohmann::json npc_data;
        file >> npc_data;

        id = npc_data["npc_id"];
        for (const auto& state : npc_data["states"].items()) {
            const std::string& state_str = state.key();
            const auto& state_data = state.value();
            int state_id = std::stoi(state_str);
            data_base[state_id] = {
                state_data["needed_item_id"],
                state_data["dialogue"],
                state_data["next_state"],
                state_data["reward_item_id"]
            };
        }
    }


    //std::string npc_action(int player_item_id, int& player_reward_item);

    void set_text();

    void action(std::shared_ptr<Player> player) override;

    void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;
};


class Player : public AnimatbleObj {
public:
    float speed = 1;


    Player() : AnimatbleObj("podvodnoe.txt", 22, 0, 0) {
        //add_pic("podvodnoe.txt");
    }

    void draw(std::vector<CHAR_INFO>& buffer, Screen& screen) override;

    void move();
};

/*
class DataBase {
private:
    using RenderLayer = std::vector<std::shared_ptr<RendrbleObject>>;

public:
	std::unordered_map<int, std::vector<std::string>> data_base;
	DataBase() {
		//data_base = {};
	}
	void add_data(std::string name, std::vector<std::string> data);
	void add_data(std::string name, std::string data);
	std::vector<std::string> get_data(std::string name);
};*/

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

public:

    Position camera_pos = Position(0, 0);

    //float deltatime;
    float MBF = 2; //milliseconds between frames BETTER NOT TO SET TO 0

    
    int cols, rows;

    Screen() {

        //_ren_objs = {};
        enshure_cols_rows();

        std::fill(backBuffer.begin(), backBuffer.end(), CHAR_INFO{ L' ', 0x07 });

        /*
        for (int i = 0; i < cols; i++) {
            screen_vec.push_back(std::string(rows, ' '));
        }
        */
    }

    /* сделать конструктор для JSON'ов
    Screen() {
        enshure_cols_rows();

    }*/

    void add_layer(std::shared_ptr<RenderLayer> layer, bool on_top = true, std::string name = "");

    RenderLayer* get_layer(const std::string& name);

    void enshure_cols_rows();

    int coord_to_vec_space(int coord, char coord_name);

    char pix_calc(int x, int y);

	Position get_camera_pos() {
		return camera_pos;
	}

    //void draw_pic(std::vector<std::string>* screen_vec, Screen& screen);

    void render();

    void init_buffers();

    void swap_buffers();

    virtual void process() = 0;
    /*
    void text_seq_render(std::vector<TextSquere> text_seq);

    void add_vert_text(std::vector<std::string> texts, int step, int text_whith);

    void show_vert_text(std::vector<std::string> texts, int step, int text_whith);

    void show_text_and_pic(std::string text, Picture pic);

    bool yes_no_choice(std::string text);
*/
};

class World : public Screen {
private:
    std::shared_ptr<Player> player;


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

        player = std::make_shared<Player>();

        std::shared_ptr<RenderLayer> player_l;
        player_l = std::make_shared<RenderLayer>();

        player_l->push_back(player);

        add_layer(player_l, true, "player");
    }

    void process() override;
};


/*class loot : public Picture {
public:
    std::string name;
    int cost;

    loot() : name("") {}

    loot(std::ifstream& file, std::string in_name, int in_cost, int in_x, int in_y) : name(in_name), cost(in_cost) {
        x = in_x;
        y = in_y;

        std::string str;
        while (std::getline(file, str))
        {
            image_vec.push_back(str);
        }

        hight = image_vec.size();
        wighth = image_vec[0].size();
    }
};

class equipment : public loot {
public:
    int for_what_mission;

    equipment() {}

    equipment(std::ifstream& file, std::string in_name, int in_cost, int in_for_what_mission, int in_x, int in_y) : for_what_mission(in_for_what_mission) {
        
        name = in_name;
        cost = in_cost;
        
        x = in_x;
        y = in_y;

        std::string str;
        while (std::getline(file, str))
        {
            image_vec.push_back(str);
        }

        hight = image_vec.size();
        wighth = image_vec[0].size();
    }
};*/



/*
class fishing_game : public Screen {
private:
    int got_fish = 0;
    int how_much_fish_you_need = 12;

    float rod_speed = 3;
    float fish_speed = 1.;

    unsigned int onscreen_fish_am = 5;

    Player* player;

public:

    class fish : public Picture {
    public:
        float wave_hight;
        float wave_offset;
        float wave_lenght;
        float wave_sdvig;

        int fasing;

        float speed = 0.85;

        // Updated constructor to initialize Picture with the file stream and coordinates
        fish(int max_x, int max_y, int in_x, int in_y, std::ifstream& file, int in_fasing, float in_speed)
            : Picture(file, in_x, in_y), // Call Picture constructor with file and coordinates
            speed(in_speed),
            fasing(in_fasing)
        {
            // Initialize wave parameters
            wave_hight = rand_int(-max_y + 33, max_y - 33);
            wave_offset = rand_int(-max_x, max_x);
            wave_lenght = (float)rand_int(-4, 4) / 10;
            wave_sdvig = y;
        }

        void move() {
            float result_x = x + (speed * fasing);
            x = result_x;
            y = wave_hight * std::sin(wave_lenght * result_x + wave_offset);
        }
    };

    std::vector<fish> fish_vec;

    fishing_game(Player* in_player) {
        player = in_player;


        for (int i = 0; i < onscreen_fish_am; i++) {

            int fasing = rand_int(0, 1) == 0 ? -1 : 1;

            std::ifstream fish_f;
            if (fasing == 1) {
                fish_f.open("fish_right1.txt");
            }
            else {
                fish_f.open("fish_left1.txt");
            }

            fish_vec.emplace_back(cols, rows, rand_int(-(cols / 2) + 10, (cols / 2) - 10), rand_int(-(rows / 2) + 10, (rows / 2) - 10), fish_f, fasing, fish_speed);
        }

        //MBF = 22;
    }

    void action();

    void procces_collisions();
};


class Mission : public Screen {
public:
    std::vector<TextSquere> aftertroduction_text_;
    std::vector<TextSquere> introduction_text_;
    std::vector<TextSquere> end_text_;
    loot result_loot;

    TextSquere food_text;
    float food_spending = 0.2f;
    float food_spend = 0;

    Player* player;

    std::chrono::steady_clock::time_point begin_time;

    int this_mission_type;
    int difficulty_lewel;

    Mission() {
        begin_time = std::chrono::steady_clock::now();
        food_text = TextSquere("", 0);
        _text.push_back(food_text);
    }

    virtual void action() = 0;
    virtual void get_possible_loot() = 0;
    virtual void change_dificulty() = 0;

    void take_the_food();

    void sorry_you_died();

    void intro();
    void outtro();

};

class fishing_game : public Screen {
private:
    int got_fish = 0;
    int how_much_fish_you_need = 12;

    float rod_speed = 3;
    float fish_speed = 1.;

    unsigned int onscreen_fish_am = 5;

    Player* player;

public:

    class fish : public Picture {
    public:
        float wave_hight;
        float wave_offset;
        float wave_lenght;
        float wave_sdvig;

        int fasing;

        float speed = 0.85;

        // Updated constructor to initialize Picture with the file stream and coordinates
        fish(int max_x, int max_y, int in_x, int in_y, std::ifstream& file, int in_fasing, float in_speed)
            : Picture(file, in_x, in_y), // Call Picture constructor with file and coordinates
            speed(in_speed),
            fasing(in_fasing)
        {
            // Initialize wave parameters
            wave_hight = rand_int(-max_y + 33, max_y - 33);
            wave_offset = rand_int(-max_x, max_x);
            wave_lenght = (float)rand_int(-4, 4) / 10;
            wave_sdvig = y;
        }

        void move() {
            float result_x = x + (speed * fasing);
            x = result_x;
            y = wave_hight * std::sin(wave_lenght * result_x + wave_offset);
        }
    };

    std::vector<fish> fish_vec;

    fishing_game(Player* in_player) {
        player = in_player;


        for (int i = 0; i < onscreen_fish_am; i++) {

            int fasing = rand_int(0, 1) == 0 ? -1 : 1;

            std::ifstream fish_f;
            if (fasing == 1) {
                fish_f.open("fish_right1.txt");
            }
            else {
                fish_f.open("fish_left1.txt");
            }
            
            fish_vec.emplace_back(cols, rows, rand_int(-(cols / 2) + 10, (cols / 2) - 10), rand_int(-(rows / 2) + 10, (rows / 2) - 10), fish_f, fasing, fish_speed);
        }

        //MBF = 22;
    }

    void action();

    void procces_collisions();
};*/

/*
class Selecters : public Screen {
protected:
    int dist_bet_frames = 3;
    bool is_quit = false;

    Player* player;
    int grid_side = 3;
    int selected_el = 0;
public:

    Selecters() {
        MBF = 111;
    }

    
    template <class T>
    void add_grid_els(std::vector<T> el_vector);

    void add_srats();

    void action();

    void selecting();

    virtual void select_actions() = 0;
};*/

/*
class Menu : public Selecters {
private:
    std::vector<std::string> select_options = { "go to a mission", "go to the shop", "buy 1 food for 3 money", "sell the loot you found", "look at the tools you have", "go to the musium", "quit"};

    //std::vector<TextSquere> select_texts;
    int selected_el = 0;

    int text_whith = 36;

public:
    Menu(Player* in_player) {
        player = in_player;

        int i = 0;

        add_vert_text(select_options, 5, text_whith);

        for (int i = 0; i < select_options.size(); i++) {
            _text[i].is_big = i == selected_el;
        }
    }

    void action();

    void buy_food(int how_much);

    void select_actions();
};


class Inventory_sell : public Selecters {
public:
    Inventory_sell(Player* in_player) {
        player = in_player;

        add_invent_items();
    }

    void sell();

    void add_invent_items();

    void select_actions();

    void congrats_you_won();
};

class Inventory_tools : public Selecters {
public:
    Inventory_tools(Player* in_player) {
        player = in_player;

        add_invent_items();
    }

    void add_invent_items();

    void select_actions();
};

class Inventory_return : public Selecters {
private:
    int to_return = 0;

public:

    Inventory_return(Player* in_player) {
        player = in_player;

        add_invent_items();
    }

    void add_invent_items();

    void select_actions();

    int get_to_return();
};

*/