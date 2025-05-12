#include "Header.h"


void NPC::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {
	AnimatbleObj::draw(buffer, screen);

	if (has_textbb) {
		text_bubble.draw(buffer, screen);
	}

}

//void NPC::collision(std::shared_ptr<RendrbleObject> other_pos) {}