#include "Header.h"


void NPC::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {
	AnimatbleObj::draw(buffer, screen);

    text_bubble.draw(buffer, screen);

}

void NPC::action(std::shared_ptr<Player> player) {

    bool is_pressed = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

    if (is_pressed && !is_actioning) {

        if (data_base.find(state) == data_base.end()) {
            text_bubble.set_text("No dialogue available.", text_wight);
        }
        else {
            LogicActions& current_action = data_base[state];

            if (player->does_has_item(current_action.needed_item_id)) {
                change_state(current_action.next_state);
            }
            else {
                text_bubble.set_text("I need something else.", text_wight);
            }
        }
    }

    is_actioning = is_pressed;

}

void NPC::set_text() {
    text_bubble.set_text(data_base[state].dialogue, text_wight);
}