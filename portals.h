#pragma once
#include <SFML/Graphics.hpp>

struct Portal{
    sf::RectangleShape shape;
    int waveAdditionValue;
    float rotation;

    Portal(sf::Vector2f position, sf::Color color, int waveAdditionValue, float rotation){
        this->rotation = rotation;
        this->waveAdditionValue = waveAdditionValue;

        shape = sf::RectangleShape(sf::Vector2f(100,40));
        shape.setOrigin(shape.getPosition().x/2, shape.getPosition().y/2);
        shape.setPosition(position);
        shape.setRotation(rotation);
        shape.setFillColor(color);

    }
};

