#include "Header.h"

void Position::sum(Position pos) {

    x += pos.x;
    y += pos.y;
}

Position Position::mins(Position pos) {
    Position n_pos;

    n_pos.x = x - pos.x;
    n_pos.y = y - pos.y;

    return n_pos;
}

Position Position::mult(float num) {
    return Position(x * num, y * num);
}

Position Position::div(float num) {
    return Position(x / num, y / num);
}

float Position::dist(const Position& other) {
    return std::sqrt(pow((other.x - x), 2) + pow((other.y - y), 2));
}

float Position::len() {
    return std::sqrt(pow(x, 2) + pow(y, 2));
}

Position Position::normalise() {
    float len = this->len();
    if (len == 0) return *this;
    return this->div(len);
}

void Position::follow(const Position& to_pos, float speed, float min_dist = 0) {

    if (!(Position(x, y).dist(to_pos) <= min_dist)) {

        Position between_vec = Position(to_pos.x - x, to_pos.y - y);
        between_vec = between_vec.normalise();
        between_vec = between_vec.mult(speed);

        this->sum(between_vec);
        //return Position(x, y).sum(between_vec);
    }

}

void Position::move_to(Position to_pos, float min_dist = 0) {
    Position between_vec = to_pos.mins(*this);
    float len = between_vec.len();

    if (len > min_dist) {
        this->sum(between_vec.mult((len - min_dist) / len));
        //return Position(x, y).sum(between_vec.mult((len - min_dist) / len));
    }
    //return *this;
}

void Position::smooth_follow(Position to_pos, float min_dist, float speed, float max_len, float deltaTime) {
    Position current(x, y);
    float distance = current.dist(to_pos);

    if (distance > min_dist) {
        Position between_vec = to_pos.mins(current);
        between_vec = between_vec.normalise();

        // Add deltaTime to make frame-rate independent
        float step = min(speed * distance / max_len * deltaTime, distance);
        this->sum(between_vec.mult(step));
    }
}


Position Position::get_pos() {
	return Position(x, y);
}


Position Position::coords_to_vec_space(Position coord_pos, int cols, int rows) {
    return Position(coord_pos.x + cols / 2, ((rows / 2) - coord_pos.y) * 2);
}

void Position::round() {
	x = std::round(x);
	y = std::round(y);
}