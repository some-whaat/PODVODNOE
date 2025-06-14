#include "Header.h"


void NPC::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {
	AnimatbleObj::draw(buffer, screen);

    text_bubble.draw(buffer, screen);

	for (std::shared_ptr<UI> ui : ui_elements) {
        ui->draw(buffer, screen);
	}

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
}


void NPC::process_logic(std::shared_ptr<Player> player) {
    bool result;

    result = attributes_check(player) && item_check(player);

    state = result ? data_base[state]["true_state"] : data_base[state]["false_state"];
}

void NPC::process_dialogue() {

    if (!seted_stuff) {
        text_bubble.set_text(data_base[state]["dialogue"], text_wight);
        seted_stuff = true;
    }

    bool is_pressed = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

    if (is_pressed && !is_actioning) {
        state = data_base[state]["next_state"]; 
        seted_stuff = false;
        is_actioning = false;
    }

    is_actioning = is_pressed;
}

void NPC::process_player_choice(std::shared_ptr<Player> player) {

    if (!seted_stuff) {
        // Clear previous UI elements before creating new ones
        ui_elements.clear();

        std::shared_ptr<UI> choice_cont = std::make_shared<UI>(
            data_base[state]["choices"], 20, 5, true, 0
        );
        ui_elements.push_back(choice_cont);
        seted_stuff = true;
        selected_el = 0;  // Reset selection when creating new UI
        choice_cont->set_big_el(selected_el);  // Set initial selection
    }

    // Input handling
    bool is_space_pressed = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
    bool is_right_pressed = (GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0;
    bool is_left_pressed = (GetAsyncKeyState(VK_LEFT) & 0x8000) != 0;

    if (is_left_pressed && !is_actioning) {
        selected_el = (selected_el - 1 + 2) % 2;  // handle wrap-around
        ui_elements[0]->set_big_el(selected_el);
    }

    if (is_right_pressed && !is_actioning) {
        selected_el = (selected_el + 1) % 2;  // handle wrap-around
        ui_elements[0]->set_big_el(selected_el);
    }

    if (is_space_pressed && !is_actioning) {
        state = data_base[state]["next_states"][selected_el];

        ui_elements.clear();
        seted_stuff = false;
    }

    is_actioning = is_space_pressed || is_right_pressed || is_left_pressed;
}

bool NPC::attributes_check(std::shared_ptr<Player> player) {
	bool result = true;

    if (data_base[state].contains("attributes_check")) {
        nlohmann::json& attributes_data = data_base[state]["attributes_check"];

        for (const auto& attr : attributes_data.items()) {
            std::string attr_name = attr.key();
            const auto& attr_data = attr.value()[1];
			const bool needed_attr_state = attr.value()[0];

			if (!player->attributes.contains(attr_name)) {
                throw std::runtime_error("player does not have this attribute" + attr_name);
			}

            if (attr_data.is_array()) {
                result = result && (isSubset(attr_data, player->attributes[attr_name]) == needed_attr_state);
            }
            else {
                result = result && ((player->attributes[attr_name] == attr_data) == needed_attr_state);
            }
        }

        return result;
    }

    return result;
}

bool NPC::item_check(std::shared_ptr<Player> player) {

    if (data_base[state].contains("mission_check")) {
        nlohmann::json& item_check_json = data_base[state]["mission_check"];

        return player->does_has_item(item_check_json["needed_item_id"]) == bool(item_check_json["needed_item_state"]);
    }

    return true;
}