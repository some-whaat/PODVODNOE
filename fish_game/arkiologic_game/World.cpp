#include "Header.h"


void World::process() {
    //std::vector<std::shared_ptr<RendrbleObject>>* npcs = get_layer("NPCs");
    //std::shared_ptr<RendrbleObject> ui_down = (*get_layer("ui_layer"))[0];
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
        /*
        for (std::shared_ptr<RendrbleObject> npc : *npcs) {
            if (player->is_inside(*npc, collision_add_val_to_NPC)) {
                npc->action(player);
                is_ui_down = true;
            }
        }*/
        //ui_down->is_render = is_ui_down;
        
        

        // БАГ :(((((((((((((((((
        camera_pos.smooth_follow(player->get_pos(), 1, camera_speed, 9); // убрать magic numbers
        //camera_pos.round();
        //camera_pos.move_to(player->get_pos(), 1);

        //camera_pos = player->get_pos();

		render();


        for (int layer_ind : actions_layers_indxs) { // надо как-то поумнее сделать кншн

            for (std::shared_ptr<RendrbleObject>& obj : *render_order[layer_ind]) {

                if (auto npc = std::dynamic_pointer_cast<NPC>(obj)) {
                    npc->action(player);
                }
                else {
					throw std::runtime_error("can not convert to NPC: ");
                }
                
            }
            
        }

	}
}

void  World::extract_from_single_json(const nlohmann::json& layers_data) {

    for (auto& layer : layers_data.items()) {
        std::string layer_name = layer.key();
        nlohmann::json layer_content = layer.value();

        int layer_ind = layer_content.contains("layer_ind") ? int(layer_content["layer_ind"]) : -1;

        // handle Player layer separately
        if (layer_name == "Player") {
            player = std::make_shared<Player>(layer_content);

            auto player_layer = std::make_shared<RenderLayer>();
            player_layer->push_back(player);
            replace_layer(player_layer, layer_ind, "player");

            continue;
        }

        // Create render layer for this group
        auto render_layer = std::make_shared<RenderLayer>();

        // перебираем типы объектов в слое
        for (auto& obj_type : layer_content.items()) {

            std::string type_name = obj_type.key();
            nlohmann::json objects = obj_type.value();

            if (type_name == "ParticleSystem") {

                for (auto& obj : objects.items()) {

                    std::cout << obj.key() << std::endl;

                    auto particle_system_shared = std::make_shared<Screen::ParticleSystem>(obj.value(), *this);
                    render_layer->push_back(particle_system_shared);

                    std::cout << "ParticleSystem added" << std::endl;
                }

                replace_layer(render_layer, layer_ind, layer_name);

                continue;
            }

            if (type_name != "layer_ind") {

                // перебираем объекты данного типа
                for (auto& obj : objects.items()) {
                    //std::string obj_name = obj.key();
                    nlohmann::json obj_data = obj.value();

                    std::shared_ptr<RendrbleObject> render_obj;

                    render_obj = create_object(type_name, obj_data);

                    if (render_obj) {
                        render_layer->push_back(render_obj);
                    }
                    else {
                        throw std::runtime_error("unknown object type: " + type_name);
                    }

                    render_layer->push_back(render_obj);
                }

                replace_layer(render_layer, layer_ind, layer_name);
            }
        }

    }


    /*
    std::shared_ptr<UI> ui_down = std::make_shared<UI>("press space to interact", 0, 0);
    ui_down->is_render = false;

    std::shared_ptr<RenderLayer> ui_layer = std::make_shared<RenderLayer>();
    ui_layer->push_back(ui_down);
    add_layer(ui_layer, -1, "ui_layer");*/
}