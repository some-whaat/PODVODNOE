#include "Header.h"


void NPC::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {
	AnimatbleObj::draw(buffer, screen);

    text_bubble.draw(buffer, screen);

}

void NPC::action(std::shared_ptr<Player> player) {

    nlohmann::json& current_action = data_base[state]; // по ключу достаём значение

    if (current_action.contains("type")) {

        if (current_action["type"] == "logic") {
            process_logic(player);
        }
        else if (current_action["type"] == "dialogue") {
            process_dialogue();
        }
        else if (current_action["type"] == "player_choice") {
            process_player_choice(player);
        }
        else {
            throw std::runtime_error("does not have a type!");
        }
    }
    else {
        throw std::runtime_error("does not have a type!");
    }




    /*
	text_bubble.is_render = true;
    bool is_pressed = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

    if (is_pressed && !is_actioning) {


        if (data_base.find(state) == data_base.end()) {
            text_bubble.set_text("No dialogue available.", text_wight);
        }
        else {


            if (player->does_has_item(current_action["needed_item_id"])) {

                if (current_action["remove_item_id"] != -1) {
                    player->remove_item_from_inventory(current_action["remove_item_id"]);
				}


                state = current_action["next_state"];

                if (data_base.find(state) != data_base.end()) {
                    text_bubble.is_render = data_base[state]["remove_text_bbl"];
                    text_bubble.set_text(data_base[state]["dialogue"], text_wight);

                }
                else {
                    text_bubble.set_text("No dialogue available.", text_wight);
                }
            }
            else {
                text_bubble.set_text("I need something else.", text_wight);
            }
        }
    }

    is_actioning = is_pressed;*/

}

void NPC::set_text() {
    text_bubble.set_text(data_base[state]["dialogue"], text_wight);
}

void NPC::emply_state(int new_state) {
    state = new_state;

    if (data_base.find(state) != data_base.end()) {
        text_bubble.is_render = data_base[state]["remove_text_bbl"];
        text_bubble.set_text(data_base[state]["dialogue"], text_wight);

    }
    else {
        text_bubble.set_text("No dialogue available.", text_wight);
    }
}

void NPC::process_logic(std::shared_ptr<Player> player) {
    // тут вся логика (функции-проверки)

    bool result;

    result = mission_check(player) && rep_check(player) && item_check(player);

    // например если res -- true -> change state to smth
}

void NPC::process_dialogue() {

    text_bubble.set_text(data_base[state]["dialogue"], text_wight); // выводим текст

    bool is_pressed = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

    if (is_pressed && !is_actioning) {
        state = data_base[state]["next_state"]; 
    }

    is_actioning = is_pressed;
}

void NPC::process_player_choice(std::shared_ptr<Player> player) {

}

bool NPC::mission_check(std::shared_ptr<Player> player) {
    nlohmann::json& curr_state = data_base[state];

    if (curr_state.contains("mission_check")) {
        return player->is_mission_complete(curr_state["needed_mission_id"]) == player->is_mission_complete(curr_state["is_mission_complete"]);
    }

    return true;
}

bool NPC::rep_check(std::shared_ptr<Player> player) {
    return true;
}

bool NPC::item_check(std::shared_ptr<Player> player) {
    return true;
}