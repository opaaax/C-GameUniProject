#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>

struct Player{
    sf::RectangleShape hitbox;
    sf::Text HPText;
    float playerMoveSpeed;
    float playerMoveSpeedCopy;
    int HP;
    int score;
    int wave = 1;
    bool isDuringMeleAnimation = false;
    bool isDashing = false;
    bool isSlowed = false;
    bool isDashingUnlocked = false;
    bool isSwordUnlocked = false;

    sf::CircleShape projectileShape;
    float projectileDamage;
    float projectileSpeed;

    sf::RectangleShape meleShape;
    float meleDamage;
    float meleSpeed;
    float meleActualRotation = 0;

    Player(sf::Font const& font){

        hitbox = sf::RectangleShape({20, 20});

        HPText = sf::Text(std::to_string(HP), font, 40);
        HPText.setPosition(hitbox.getPosition());
        HPText.setFillColor(sf::Color::Red);

        playerMoveSpeed = 2;
        playerMoveSpeedCopy = 2;
        HP = 2;
        score = 0;

        //https://stackoverflow.com/questions/70545678/c-sfml-how-to-rotate-a-character-from-its-center
        hitbox.setOrigin((sf::Vector2f)hitbox.getSize() / 2.f);
        hitbox.setFillColor(sf::Color::Black);

        projectileSpeed = 5;
        projectileShape = sf::CircleShape(4);
        projectileShape.setFillColor(sf::Color::Black);
        projectileDamage = 1;

        meleSpeed = 4;
        meleShape = sf::RectangleShape({10, 75});
        meleShape.setFillColor(sf::Color(111, 125, 148));
        meleShape.setPosition(hitbox.getPosition());
        meleDamage = 3;

    }


    auto Save(std::string const& fileName){
        std::ofstream file("../" + fileName + ".txt");

        fmt::println(file, "{}", this->HP);
        fmt::println(file, "{}", this->score);
        fmt::println(file, "{}", this->wave);
        fmt::println(file, "{}", this->projectileDamage);
        fmt::println(file, "{}", this->meleDamage);
        fmt::println(file, "{}", this->isDashingUnlocked);
        fmt::println(file, "{}", this->isSwordUnlocked);

        file.close();
    }

    auto Load(std::string const& fileName){

        auto file = std::fstream("../" + fileName + ".txt");
        auto word = std::string();

        file >> word;
        this->HP = std::stoi(word);

        file>>word;
        this->score = std::stoi(word);

        file>>word;
        this->wave = std::stoi(word);

        file>>word;
        this->projectileDamage = std::stof(word);

        file>>word;
        this->meleDamage = std::stof(word);

        file>>word;
        this->isDashingUnlocked = word == "true";

        file>>word;
        this->isSwordUnlocked = word == "true";

        file.close();

    }

};





