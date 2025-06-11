#include "Header.h"


void World::process() {
    std::vector<std::shared_ptr<RendrbleObject>>* npcs = get_layer("NPCs");
    std::shared_ptr<RendrbleObject> ui_down = (*get_layer("ui_layer"))[0];
    //std::shared_ptr<RendrbleObject> player = (*get_layer("player"))[0];

    auto previousTime = std::chrono::high_resolution_clock::now();

	while (true) {

        auto currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(currentTime - previousTime).count();
        previousTime = currentTime;

        // Cap delta time to avoid spiral of death
        deltaTime = min(deltaTime, 0.2f);

        /*
        //fishy_stuff
        float f = bg_fish->size();
        for (int i = 0; f + i < 5; i++) {

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

                camera_pos.x + ((cols) / 2 + 2) * -fasing,
                camera_pos.y + rand_int(-(rows / 2) + 10, (rows / 2) - 10),

                fasing,
                Position(rand_int(-2, 2), rand_int(-2, 2)),
                cols,
                rows,
                -0.6f
            ));
        }

        
        for (int i = 0; i < bg_fish->size(); i++) {
            if ((*bg_fish)[i]->x < -cols / 2 - 8 + camera_pos.x || (*bg_fish)[i]->x > cols / 2 + 8 + camera_pos.x || (*bg_fish)[i]->y < -rows / 2 - 8 + camera_pos.y || (*bg_fish)[i]->y > rows / 2 + 8 + camera_pos.y) {
                bg_fish->erase(bg_fish->begin() + i);
            }
        }*/


		//something_changed = true;

        bool is_ui_down = false;
        for (std::shared_ptr<RendrbleObject> npc : *npcs) {
            if (player->is_inside(*npc, collision_add_val_to_NPC)) {
                npc->action(player);
                is_ui_down = true;
            }
        }
        ui_down->is_render = is_ui_down;
        
        

        // БАГ :(((((((((((((((((
        camera_pos.smooth_follow(player->get_pos(), 1, camera_speed, 9); // убрать magic numbers
        //camera_pos.round();
        //camera_pos.move_to(player->get_pos(), 1);

        //camera_pos = player->get_pos();

		render();
	}
}