#pragma once
#include <SFML/Graphics.hpp>

enum itemType{
    SWORD, DASH
};

struct shopitem {
    int price;
    bool isBought = false;
    itemType type;
    sf::Text itemName;
    sf::Text priceName;
    sf::RectangleShape hitbox;
};

struct dashShopItem : shopitem {
    explicit dashShopItem(const sf::Font& font){
        this->type = DASH;
        this->price = 10;
        this->itemName = sf::Text("Dash", font, 35);
        this->priceName = sf::Text("price: "+std::to_string(price), font, 45);
        this->hitbox = sf::RectangleShape(sf::Vector2f(125, 100));

        itemName.setFillColor(sf::Color::Black);
        priceName.setFillColor(sf::Color::Black);

        itemName.setPosition(sf::Vector2f(450, 550));
        priceName.setPosition(itemName.getPosition() + sf::Vector2f(0, 40));

        hitbox.setFillColor(sf::Color::Green);
        hitbox.setPosition(itemName.getPosition());

    }
};

struct swordShopItem : shopitem {
    explicit swordShopItem(const sf::Font& font){
        this->type = SWORD;
        this->price = 15;
        this->itemName = sf::Text("Sword", font, 35);
        this->priceName = sf::Text("price: "+std::to_string(price), font, 45);
        this->hitbox = sf::RectangleShape(sf::Vector2f(125, 100));

        itemName.setFillColor(sf::Color::Black);
        priceName.setFillColor(sf::Color::Black);

        itemName.setPosition(sf::Vector2f(650, 550));
        priceName.setPosition(itemName.getPosition() + sf::Vector2f(0, 40));

        hitbox.setFillColor(sf::Color::Green);
        hitbox.setPosition(itemName.getPosition());
    }
};