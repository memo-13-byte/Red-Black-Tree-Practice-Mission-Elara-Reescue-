#include "Sector.h"

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary

}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}

bool Sector::isLeftChild() {
    return (parent != nullptr && parent->left == this);
}

Sector* Sector::uncle() {

    if (parent == nullptr || parent->parent == nullptr) {
        return nullptr;
    }

    if (parent->isLeftChild()) {
        return parent->parent->right;
    } else {
        return parent->parent->left;
    }
}

Sector *Sector::getParent() {
    return parent;
}
