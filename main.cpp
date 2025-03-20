#include <SFML/Graphics.hpp>
#include "gameWindow.h"
#include <cmath>
#include <thread>
#include <deque>
#include "player.h"
#include "Projectile.h"
#include "enemy.h"
#include "collectibles.h"
#include "portals.h"
#include "traps.h"
#include "shop.h"

auto main() -> int {
    srand(time(NULL));

    //https://www.sfml-dev.org/tutorials/2.6/system-time.php
    sf::Clock clock;
    sf::Clock enemyGruntClock;
    sf::Clock enemyFlyClock;
    sf::Clock enemyKnightClock;
    sf::Clock enemyHeavyClock;
    sf::Clock lifeCrystalClock;
    sf::Clock gunCrystalClock;
    sf::Clock dashClock;

    auto clocks = std::vector<std::reference_wrapper<sf::Clock>>{clock, enemyHeavyClock, enemyFlyClock, enemyGruntClock, enemyKnightClock, lifeCrystalClock, gunCrystalClock};

    auto randomHealthCrystalTimeInterval = rand()%25;

    srand(time(NULL)+1337);
    auto randomGunCrystalTimeInterval = rand()%25;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    settings.stencilBits = 8;

    auto game = Game(sf::RenderWindow(
            sf::VideoMode({1200, 800}), "Pixel Perfect", sf::Style::Titlebar, settings));
    game.window.setFramerateLimit(144);




    auto font = sf::Font();
    font.loadFromFile("../Fonts/Jacquard24-Regular.ttf");

    auto player = Player(font);
    player.hitbox.setPosition(game.window.getSize().x/2 - player.hitbox.getSize().x, game.window.getSize().y/2 - player.hitbox.getSize().y);

    auto scoreText = sf::Text("Score: ", font, 75);
    scoreText.setPosition(0, 0);
    scoreText.setFillColor(sf::Color::Black);

    auto bulletsToDraw = std::deque<Projectile>();
    auto enemiesToDraw = std::deque<Enemy>();
    auto crystalsToDraw = std::deque<Crystal>();
    auto trapsToDraw = std::vector<std::unique_ptr<Trap>>();
    //https://stackoverflow.com/questions/8200701/vectors-of-references-to-objects
    auto textToDraw = std::vector<std::reference_wrapper<sf::Text>>();

    auto portalsToDraw = std::vector<Portal>();
    portalsToDraw.emplace_back(sf::Vector2f(game.window.getSize().x / 5, game.window.getSize().y/2), sf::Color::Green, 1, 90);
    portalsToDraw.emplace_back(sf::Vector2f(game.window.getSize().x / 2.2, game.window.getSize().y/5), sf::Color::Yellow, 10, 0);
    portalsToDraw.emplace_back(sf::Vector2f(3.25 * game.window.getSize().x / 4, game.window.getSize().y/2), sf::Color::Red, 100, 90);

    auto shopItemsToDraw = std::vector<std::unique_ptr<shopitem>>();
    shopItemsToDraw.emplace_back(std::make_unique<shopitem>(dashShopItem(font)));
    shopItemsToDraw.emplace_back(std::make_unique<shopitem>(swordShopItem(font)));

    textToDraw.emplace_back(scoreText);

    game.titleScreen(player);

    auto gameRestart = [&player, &clock, &game, &font, &enemiesToDraw, &crystalsToDraw](){
        if(!game.isLoadedAfterRestart) {
            player = Player(font);
        }
        player.hitbox.setPosition(game.window.getSize().x/2 - player.hitbox.getSize().x, game.window.getSize().y/2 - player.hitbox.getSize().y);
        player.HPText = sf::Text(std::to_string(player.HP), font, 40);
        player.HPText.setPosition(player.hitbox.getPosition());
        player.HPText.setFillColor(sf::Color::Red);
        player.isDashingUnlocked = false;
        player.isSwordUnlocked = false;

        clock.restart();
        enemiesToDraw.clear();
        crystalsToDraw.clear();

        game.isWaveStarted = false;
        game.isRestarted = false;
        game.isLoadedAfterRestart = false;
    };

    while(game.window.isOpen()){

        float elapsed = clock.getElapsedTime().asSeconds();

        auto event = sf::Event();

        auto mouse = sf::Mouse::getPosition(game.window);
        //https://en.sfml-dev.org/forums/index.php?topic=16825.0
        auto angle = (atan2(
                mouse.y - player.hitbox.getPosition().y,
                mouse.x - player.hitbox.getPosition().x))*(180/3.141); //getting the angle of the mouse, relative to the player in the (-180, 180) range

        if(angle < 0){
            angle = 360 + angle; //changing the range of the angle to (0, 360) degrees
        }
        //changing the amount of rotation, based on the angle between the right side of the player and mouse
        player.hitbox.rotate((angle - player.hitbox.getRotation()));

        while(game.window.pollEvent(event)){

            switch(event.type) {

                case sf::Event::Closed:
                    game.window.close();
                    return 0;

                case sf::Event::LostFocus:
                    game.pause(player);
                    break;

                case sf::Event::KeyPressed:
                    if(event.key.scancode == sf::Keyboard::Scan::Escape){
                        game.pause(player);
                        break;
                    }
                    if(event.key.scancode == sf::Keyboard::Scan::LShift){
                        if(player.isDashingUnlocked){
                            player.isDashing = true;
                            dashClock.restart();
                        }
                    }

                case sf::Event::MouseButtonPressed:
                    if(event.mouseButton.button == sf::Mouse::Right){
                        //adding new bullets to a vector of circles
                        auto bullet = Projectile(player.projectileShape, player.projectileSpeed, false,
                                                 cos(player.hitbox.getRotation()*(3.14/180)),
                                                 sin(player.hitbox.getRotation()*(3.14/180))); //modifiers allow to change the move vector, based on the player rotation

                        bullet.projectile.setPosition(player.hitbox.getPosition());
                        bulletsToDraw.push_back(bullet);
                    }
                    if(event.mouseButton.button == sf::Mouse::Left && event.key.scancode != sf::Keyboard::Scan::A){ //bizzare bug with the A doing what the left mouse button is supposed to
                        if(!player.isDuringMeleAnimation && player.isSwordUnlocked) {
                            player.isDuringMeleAnimation = true;
                            player.meleShape.setPosition(player.hitbox.getPosition());
                            player.meleShape.setRotation(player.hitbox.getRotation() + 10);
                        }
                    }
            }
        }
        if(dashClock.getElapsedTime().asSeconds() > 0.1){
            player.isDashing = false;
            player.playerMoveSpeed = player.playerMoveSpeedCopy;
        }

        if(player.isDashing && !player.isSlowed){
            player.playerMoveSpeed = 8;
        }

        game.window.clear(sf::Color::White);

        //adding enemy objects into the enemy vector

        if(game.isWaveStarted) {
            //grunts
            if (enemyGruntClock.getElapsedTime().asSeconds() >= (5 - 0.01 * log(player.wave / 2 + 1))) {
                enemiesToDraw.emplace_back(
                        enemyGruntClock, sf::RectangleShape({20, 20}),
                        sf::Text("", font, 35), sf::Color(92, 0, 0),
                        game, 1, 5, false);

            }

            //flies
            if (player.wave >= 15) {
                if (enemyFlyClock.getElapsedTime().asSeconds() >= (6 - 0.01 * log(player.wave / 2))) {

                    enemiesToDraw.emplace_back(
                            enemyFlyClock, sf::RectangleShape({10, 10}),
                            sf::Text("", font, 20), sf::Color::Blue,
                            game, 2, 1, true);

                }
            }

            //heavies
            if (player.wave >= 100) {
                if (enemyHeavyClock.getElapsedTime().asSeconds() >= (12 - 0.5*log(player.wave / 2))) {

                    enemiesToDraw.emplace_back(
                            enemyHeavyClock, sf::RectangleShape({50, 50}),
                            sf::Text("", font, 20), sf::Color::Magenta,
                            game, 0.5, 100, false);

                }
            }

            //knights
            if (player.wave >= 400) {
                if (enemyKnightClock.getElapsedTime().asSeconds() >= (15 - 0.3 * log(player.wave / 2))) {

                    enemiesToDraw.emplace_back(
                            enemyKnightClock, sf::RectangleShape({75, 75}),
                            sf::Text("", font, 20), sf::Color::Yellow,
                            game, 1.5, 300, true);

                }
            }


            if (lifeCrystalClock.getElapsedTime().asSeconds() >= randomHealthCrystalTimeInterval) {
                srand(time(NULL));
                lifeCrystalClock.restart();
                randomHealthCrystalTimeInterval = rand() % 45;
                crystalsToDraw.emplace_back(LifeCrystal(1, game.window));
            }

            if (gunCrystalClock.getElapsedTime().asSeconds() >= randomGunCrystalTimeInterval) {
                srand(time(NULL) + 320);
                gunCrystalClock.restart();
                randomGunCrystalTimeInterval = rand() % 45;

                crystalsToDraw.emplace_back(GunCrystal(1, game.window));
            }

        } else {
            trapsToDraw.clear(); //clearing all traps
            for(auto const& portal : portalsToDraw){
                game.window.draw(portal.shape);
                elapsed = 0;

                if(portal.shape.getGlobalBounds().intersects(player.hitbox.getGlobalBounds())){
                    player.wave += portal.waveAdditionValue;
                    game.isWaveStarted = true;

                    switch(portal.waveAdditionValue){ //drawing traps for respective rooms
                        case 1 :
                                trapsToDraw.emplace_back(std::make_unique<Trap>(LavaFloor(game.window, rand()%1336)));
                                break;
                        case 10 :
                            for(int i = 0; i < 3; i++){
                                trapsToDraw.emplace_back(std::make_unique<Trap>(LavaFloor(game.window, rand()%2138)));
                                trapsToDraw.emplace_back(std::make_unique<Trap>(IceFloor(game.window, rand()%12)));
                            }
                            break;
                        case 100 :
                            for(int i = 0; i < 5; i++){
                                trapsToDraw.emplace_back(std::make_unique<Trap>(LavaFloor(game.window, rand()%68)));
                                trapsToDraw.emplace_back(std::make_unique<Trap>(IceFloor(game.window, rand()%22)));
                                trapsToDraw.emplace_back(std::make_unique<Trap>(Blade(game.window, rand()%44)));
                            }
                            break;

                    }

                    for(auto& cl : clocks){ //restarting all clocks
                        cl.get().restart();
                    }
                }

            }

            for(auto const& item : shopItemsToDraw){
                if(!item->isBought) {
                    game.window.draw(item->hitbox);
                    game.window.draw(item->itemName);
                    game.window.draw(item->priceName);

                    if(item->type == SWORD){
                        if(player.isSwordUnlocked){
                            item->isBought = true;
                        }
                    }

                    if(item->type == DASH){
                        if(player.isDashingUnlocked){
                            item->isBought = true;
                        }
                    }

                    if(player.hitbox.getGlobalBounds().intersects(item->hitbox.getGlobalBounds())){
                        if(player.score >= item->price) {
                            item->isBought = true;
                            player.score -= item->price;

                            if (item->type == SWORD) {
                                player.isSwordUnlocked = true;
                            } else {
                                player.isDashingUnlocked = true;
                            }

                        }

                    }

                }
            }
        }

        if(clock.getElapsedTime().asSeconds() > 30){ //setting the lenght of a wave to 30 seconds
            game.isWaveStarted = false;
        }

        //https://www.sfml-dev.org/tutorials/2.6/window-inputs.php

        player.isSlowed = false;
        for(auto& trap : trapsToDraw){
            if(player.hitbox.getGlobalBounds().intersects(trap->shape.getGlobalBounds())){
                if(trap->trapTimer.getElapsedTime().asSeconds() > 1){
                    trap->trapTimer.restart();
                    player.HP -= trap->damagePerSecond;
                }
                if(trap->trapType == ICE){
                    player.isSlowed = true;
                }
            }

            if(trap->trapType == BLADE){
                trap->shape.rotate(4);
            }
            game.window.draw(trap->shape);
        }

        if(player.isSlowed){
            player.playerMoveSpeed = 1;
        }


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            if(player.hitbox.getGlobalBounds().getPosition().y >= 0) {
                player.hitbox.move(0.f, -player.playerMoveSpeed);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            if(player.hitbox.getGlobalBounds().getPosition().y <= game.window.getSize().y - player.hitbox.getGlobalBounds().getSize().y) {
                player.hitbox.move(0.f, player.playerMoveSpeed);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            if(player.hitbox.getGlobalBounds().getPosition().x >= 0) {
                player.hitbox.move(-player.playerMoveSpeed, 0.f);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            if(player.hitbox.getGlobalBounds().getPosition().x <= game.window.getSize().x - player.hitbox.getGlobalBounds().getSize().x) {
                player.hitbox.move(player.playerMoveSpeed, 0.f);
            }
        }

        scoreText.setString("Score: " + std::to_string(player.score) + " wave: " + std::to_string(player.wave) + "\n" + std::to_string((int)elapsed));

        for(const auto& text : textToDraw){
            game.window.draw(text);
        }

        //iterating through all the bullets and animating them
        for(auto& circle : bulletsToDraw ){
            if(!circle.hitSomething){

                circle.projectile.setFillColor(sf::Color::Black);
                //making sure that bullets dont draw outside of the window
                if (circle.projectile.getPosition().x < 0 || circle.projectile.getPosition().x > game.window.getSize().x) {
                    circle.hitSomething = true;
                }
                if (circle.projectile.getPosition().y < 0 || circle.projectile.getPosition().y > game.window.getSize().y) {
                    circle.hitSomething = true;
                }

                circle.projectile.move(circle.ProjectileSpeed * circle.modifierX,
                                       circle.ProjectileSpeed * circle.modifierY);

                game.window.draw(circle.projectile);
            }
        }
        if(!bulletsToDraw.empty() && bulletsToDraw[0].hitSomething){
            bulletsToDraw.pop_front();
        }

        for(auto& crystal : crystalsToDraw){
            if(!crystal.isCollected){
                switch(crystal.typeOfCollectible){

                    case TypeOfCollectible::HEALING:
                        if (crystal.shape.getGlobalBounds().intersects(player.hitbox.getGlobalBounds())) {
                            player.HP += crystal.value;
                            crystal.isCollected = true;
                        }
                        game.window.draw(crystal.shape);
                        break;

                    case TypeOfCollectible::DAMAGEBOOST:
                        if (crystal.shape.getGlobalBounds().intersects(player.hitbox.getGlobalBounds())) {
                            player.projectileDamage += crystal.value;
                            crystal.isCollected = true;
                        }
                        game.window.draw(crystal.shape);
                        break;
                }
            }
        }
        if(!crystalsToDraw.empty() && crystalsToDraw[0].isCollected){
            crystalsToDraw.pop_front();
        }

        enemyFunctions::enemyDrawLogic(enemiesToDraw, bulletsToDraw, player, game);

        game.window.draw(player.hitbox);

        player.HPText.setString(std::to_string(player.HP));
        player.HPText.setPosition(player.hitbox.getPosition() + sf::Vector2f(20,20));
        game.window.draw(player.HPText);

        if(player.isDuringMeleAnimation){
            player.meleShape.rotate(-player.meleSpeed);
            player.meleActualRotation += player.meleSpeed;

            player.meleShape.setPosition(player.hitbox.getPosition());

            if(player.meleActualRotation > 180){
                player.isDuringMeleAnimation = false;
                player.meleActualRotation = 0;
            }

            game.window.draw(player.meleShape);
        }

        if(player.HP <= 0){
            game.defeatScreen(player.score, player);
        }

        if(game.isRestarted){
            gameRestart();
        }

        game.window.display();

    }
}

