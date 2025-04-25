#include "Header.h"

bool is_working = true;


int main() {
    std::ifstream file("diver.txt");
    Screen scr;

    // Create the layer vector explicitly
    std::vector<std::unique_ptr<RendrbleObject>> layer;
    layer.push_back(std::make_unique<SinMovingObj>(66, 66, 0, 0, file, 1, 1));

    // Move the entire layer into addLayer
    scr.addLayer("cr", std::move(layer), true);

    while (is_working) {
        scr.render();
    }
}
