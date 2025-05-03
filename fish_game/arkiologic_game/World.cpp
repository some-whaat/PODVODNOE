#include "Header.h"


void World::process() {
	while (true) {

        float f = get_layer("bg_fish")->size();
        for (int i = 0; f + i < 5; i++) {

            int fasing = rand_int(0, 1) == 0 ? -1 : 1;

            std::ifstream fish_f;
            if (fasing == 1) {
                fish_f.open("fish_right1.txt");
            }
            else {
                fish_f.open("fish_left1.txt");
            }

            get_layer("bg_fish")->push_back(std::make_shared<MovingObj>(
                fish_f,

                (cols/2 - 3) * -fasing,
                rand_int(-(rows / 2) + 10, (rows / 2) - 10),

                fasing,
                rand_int(1, 2),
                cols,
                rows
            ));
        }

        for (int i = 0; i < get_layer("bg_fish")->size(); i++) {
            if ((*get_layer("bg_fish"))[i]->x < -cols / 2 - 8 || (*get_layer("bg_fish"))[i]->x > cols / 2 + 8) {
                get_layer("bg_fish")->erase(get_layer("bg_fish")->begin() + i);
            }
        }


		//something_changed = true;

        Position dir;

        if (GetAsyncKeyState(VK_LEFT))
        {
            dir.x += -1;
            something_changed = true;
        }

        if (GetAsyncKeyState(VK_RIGHT))
        {
            dir.x += 1;
            something_changed = true;
        }

        if (GetAsyncKeyState(VK_UP))
        {
            dir.y += 1;
            something_changed = true;
        }

        if (GetAsyncKeyState(VK_DOWN))
        {
            dir.y += -1;
            something_changed = true;
        }

        dir.normalise();

        (*get_layer("player"))[0]->sum(dir.mult(2));

		render();
	}
}