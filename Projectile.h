#pragma once
#include "SFML/Graphics.hpp"
#include <cmath>
#include <mutex>

struct Projectile{
    sf::CircleShape projectile;
    float ProjectileSpeed;
    bool hitSomething;
    double modifierX;
    double modifierY;

};


