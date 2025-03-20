#pragma once
#include <SFML/Graphics.hpp>

enum TrapType {
    LAVA, ICE, BLADE
};

struct Trap{
    sf::ConvexShape shape;
    sf::Clock trapTimer;
    TrapType trapType;
    int damagePerSecond;
};


struct LavaFloor : Trap {

    LavaFloor(sf::Window const& window, int randSeed){
        this->damagePerSecond = 1;
        this->trapType = LAVA;

        srand(time(NULL) + randSeed);

        auto point = sf::Vector2f(rand()%(window.getSize().x-200) + 200, rand()%(window.getSize().y - 200) + 200);

        this->shape.setPointCount(3 + rand()%16);
        this->shape.setPoint(0, point);

        for(int i = 1; i < shape.getPointCount(); i++){
            auto randomisedVector2f = sf::Vector2f(rand() % 200 - 100, rand() % 200 - 100);
            this->shape.setPoint(i,shape.getPoint(i-1) + randomisedVector2f);
        }
        this->shape.setFillColor(sf::Color(217, 9, 9));
    };
};

struct IceFloor : Trap {
    IceFloor(sf::Window const& window, int randSeed){
        this->damagePerSecond = 0;
        this->trapType = ICE;

        srand(time(NULL) + randSeed);

        auto point = sf::Vector2f(rand()%(window.getSize().x - 200) + 200, rand()%(window.getSize().y - 200) + 200);

        this->shape.setPointCount(3 + rand()%4);
        this->shape.setPoint(0, point);

        for(int i = 1; i < shape.getPointCount(); i++){
            auto randomisedVector2f = sf::Vector2f(rand() % 100 - 50, rand() % 100 - 50);
            this->shape.setPoint(i,shape.getPoint(i-1) + randomisedVector2f);
        }
        this->shape.setFillColor(sf::Color(52, 189, 235));
    };
};

struct Blade : Trap {
    Blade(sf::Window const& window, int randSeed){
        this->damagePerSecond = 1;
        this->trapType = BLADE;

        srand(time(NULL) + randSeed);

        auto point = sf::Vector2f(rand()%(window.getSize().x - 200) + 200, rand()%(window.getSize().y - 200) + 200);

        this->shape.setPointCount(4);
        this->shape.setPoint(0, sf::Vector2f(0.f,0.f));
        this->shape.setPoint(1, shape.getPoint(0) + sf::Vector2f(25.f,0.f));
        this->shape.setPoint(2, shape.getPoint(1) + sf::Vector2f(0.f,100.f));
        this->shape.setPoint(3, shape.getPoint(2) + sf::Vector2f(-25.f,0.f));
        this->shape.setOrigin(this->shape.getGlobalBounds().getSize().x/2, this->shape.getGlobalBounds().getSize().y/2);
        this->shape.setPosition(point);

        this->shape.setFillColor(sf::Color(110, 110, 110));


    };
};