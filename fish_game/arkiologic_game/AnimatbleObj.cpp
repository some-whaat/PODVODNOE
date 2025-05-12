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

void AnimatbleObj::read_anim_frames(const std::string& file_name) {
	anim_frames = read_objs_from_file(file_name);

	image_vec = anim_frames[0];
	hight = image_vec.size();
	wighth = image_vec[0].size();
}