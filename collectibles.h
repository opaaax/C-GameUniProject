#pragma once
#include <SFML/Graphics.hpp>
#include <random>

enum TypeOfCollectible{
    HEALING, DAMAGEBOOST
};

struct Crystal{
    int value;
    //https://www.sfml-dev.org/tutorials/2.6/graphics-shape.php
    sf::ConvexShape shape;
    bool isCollected = false;
    TypeOfCollectible typeOfCollectible;
};

struct LifeCrystal : Crystal{
    LifeCrystal(int healingValue, const sf::Window& window){
        typeOfCollectible = TypeOfCollectible::HEALING;

        srand(time(NULL));
        auto point = sf::Vector2f(rand()%window.getSize().x, rand()%window.getSize().y);

        this->value = healingValue;
        this->shape.setPointCount(4);
        this->shape.setPoint(0, point);
        this->shape.setPoint(1, point + sf::Vector2f(20.f, 15.f));
        this->shape.setPoint(2, point + sf::Vector2f(40.f, 0.f));
        this->shape.setPoint(3, point + sf::Vector2f(20.f, 40.f));
        this->shape.setFillColor(sf::Color::Green);
    }
};

struct GunCrystal : Crystal{
    GunCrystal(int damageValue, const sf::Window& window){
        typeOfCollectible = TypeOfCollectible::DAMAGEBOOST;

        srand(time(NULL) + 2137420); //adding something to the seed to differ it from life crystals
        auto point = sf::Vector2f(rand()%window.getSize().x, rand()%window.getSize().y);

        this->value = damageValue;
        this->shape.setPointCount(4);
        this->shape.setPoint(0, point);
        this->shape.setPoint(1, point + sf::Vector2f(20.f, 20.f));
        this->shape.setPoint(2, point + sf::Vector2f(40.f, 0.f));
        this->shape.setPoint(3, point + sf::Vector2f(20.f, 40.f));
        this->shape.setFillColor(sf::Color::Red);
    }
};
