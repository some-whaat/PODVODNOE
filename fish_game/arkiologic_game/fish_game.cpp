#include "Header.h"

bool is_working = true;


int main() {
    using RenderLayer = std::vector<std::shared_ptr<RendrbleObject>>;

    std::ifstream beach("beach.txt");
    
    World scr;

    std::shared_ptr<RenderLayer> bg;

    bg = std::make_shared<RenderLayer>();

    bg->push_back(std::make_shared<Picture>(beach));

//    std::vector<std::unique_ptr<RendrbleObject>> layer;
//    layer.push_back(std::make_unique<SinMovingObj>(66, 66, 0, 0, file, 1, 1));

    // Move the entire layer into addLayer
    scr.add_layer(bg, false);

    scr.process();
}
