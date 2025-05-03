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

//enum mission_type {any, snake, ruins};

int rand_int(int down_bord, int up_bord);



class Position {
public:
    float x;
    float y;

    //float velosity;

    Position() : x(0), y(0) {}

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

    Position get_pos();

    Position coords_to_vec_space(Position coord_pos, int cols, int rows);
};

class RendrbleObject : public Position {
public:

    virtual void draw(std::vector<std::string>* screen_vec, Screen& screen) = 0;
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

    void draw(std::vector<std::string>* screen_vec, Screen& screen) override;
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

    bool is_in_rec(Position other_pos, float add_dist);

    bool is_in_rec(int in_x, int in_y, float add_dist);

    void draw(std::vector<std::string>* screen_vec, Screen& screen) override;

    void draw_frame(std::vector<std::string>* screen_vec, Screen& screen);
};

class TextSquere : public Rektangle{
public:
    std::vector<std::string> text_vec;

    TextSquere() : text_vec({}) {}

    TextSquere(std::string in_text, int in_wighth) {
        x, y = 0;

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

    void draw(std::vector<std::string>* screen_vec, Screen& screen) override;
};

class Picture : public Rektangle {
public:
    std::vector<std::string> image_vec;

    Picture() : Rektangle(), image_vec({}) {}

    Picture(std::ifstream& file) {
        x, y = 0;

        add_pic(file);
    }

    Picture(std::ifstream& file, int in_x, int in_y) {
        x = in_x;
        y = in_y;

        add_pic(file);
    }

    Picture(std::string& file, int in_x, int in_y) {
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

    void draw(std::vector<std::string>* screen_vec, Screen& screen) override;

protected:
    void add_pic(std::ifstream& file);

    void add_pic(std::string& file);
};

class MovingObj : public Picture {
private:
    float wave_hight_bound = 20;


public:
    float wave_hight;
    float wave_offset;
    float wave_lenght;
    float wave_sdvig;

    int fasing;

    float speed = 1;


    MovingObj(std::ifstream& file, int in_x, int in_y, int in_fasing, float in_speed, int max_x, int max_y)
        : Picture(file, in_x, in_y),
        speed(in_speed),
        fasing(in_fasing)
    {

        wave_hight = rand_int(-max_y + wave_hight_bound, max_y - wave_hight_bound);
        wave_offset = rand_int(-max_x, max_x);
        wave_lenght = (float)rand_int(-4, 4) / 10;
        wave_sdvig = y;
    }

    MovingObj(std::string& file_name, int in_x, int in_y, int in_fasing, float in_speed, int max_x, int max_y)
        : Picture(file_name, in_x, in_y),
        speed(in_speed),
        fasing(in_fasing)
    {

        wave_hight = rand_int(-max_y + wave_hight_bound, max_y - wave_hight_bound);
        wave_offset = rand_int(-max_x, max_x);
        wave_lenght = (float)rand_int(-4, 4) / 10;
        wave_sdvig = y;
    }

    void move(Screen& screen);
    void draw(std::vector<std::string>* screen_vec, Screen& screen) override;
};

class Player : public Picture {
public:
    unsigned int money = 0;
    int food = 0;


    Player() : Picture() {
        std::ifstream file("podvodnoe.txt");
        add_pic(file);
    }

    void draw(std::vector<std::string>* screen_vec, Screen& screen) override;
};

class Screen {

protected:
    using RenderLayer = std::vector<std::shared_ptr<RendrbleObject>>;

    std::vector<std::string> screen_vec;

    bool something_changed = true;

    /*
    std::vector<std::string> layer_order;  // Порядок рендеринга
    std::unordered_map<std::string, RenderLayer> layers;  // Слои по именам

    
    std::vector<RenderLayer*> render_order;  // Быстрый рендеринг
    std::unordered_map<std::string, std::unique_ptr<RenderLayer>> layers;  // Хранение
*/

    std::vector<std::shared_ptr<RenderLayer>> render_order;
    std::unordered_map<std::string, std::shared_ptr<RenderLayer>> layers;

    Position camera_pos = Position(0, 0);

    std::string rend_style = "0000000000";

    CONSOLE_SCREEN_BUFFER_INFO csbi;

public:
    //float deltatime;
    float MBF = 11; //milliseconds between frames BETTER NOT TO SET TO 0

    
    int cols, rows;

    Screen() {

        //_ren_objs = {};

        enshure_cols_rows();
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

    //void draw_pic(std::vector<std::string>* screen_vec, Screen& screen);

    void render();

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
public:

    World() {
        std::shared_ptr<RenderLayer> bg_fish;
        bg_fish = std::make_shared<RenderLayer>();

        for (int i = 0; i < 4; i++) {

            int fasing = rand_int(0, 1) == 0 ? -1 : 1;

            std::ifstream fish_f;
            if (fasing == 1) {
                fish_f.open("fish_right1.txt");
            }
            else {
                fish_f.open("fish_left1.txt");
            }

            bg_fish->push_back(std::make_shared<MovingObj>(
                fish_f,
                
                rand_int(-(cols / 2) + 10, (cols / 2) - 10),
                rand_int(-(rows / 2) + 10, (rows / 2) - 10),
                
                fasing,
                rand_int(1, 2),
                cols,
                rows
            ));
        }

        add_layer(bg_fish, true, "bg_fish");

        std::shared_ptr<RenderLayer> player;
        player = std::make_shared<RenderLayer>();

        player->push_back(std::make_shared<Player>());

        add_layer(player, true, "player");
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