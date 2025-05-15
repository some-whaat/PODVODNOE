#include "Header.h"


void NPC::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {
	AnimatbleObj::draw(buffer, screen);

	if (has_textbb) {
		text_bubble.draw(buffer, screen);
	}

}

void NPC::action(std::shared_ptr<Player> player) {
    if (data_base.find(state) == data_base.end()) {
        text_bubble.set_text("No dialogue available.", text_wight);
    }
    else {
        LogicActions& current_action = data_base[state];
        if (current_action.needed_item_id == -1) { // || current_action.needed_item_id in inventory
            state = current_action.next_state;
        }
        else {
            text_bubble.set_text("I need something else.098908989 98", text_wight);
        }
    }

}

void NPC::set_text() {
    text_bubble.set_text(data_base[state].dialogue, text_wight);
}