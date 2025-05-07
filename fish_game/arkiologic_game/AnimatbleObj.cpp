#include "Header.h"



void AnimatbleObj::animation() {
	anim_calls++;

	if (anim_calls >= anim_speed) {
		curr_frame_ind = (curr_frame_ind + 1) % anim_frames.size();

		image_vec = anim_frames[curr_frame_ind];

		anim_calls = 0;
	}
}

void AnimatbleObj::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {
	animation();

	Picture::draw(buffer, screen);
}