#pragma once
#include<SFML/Graphics.hpp>
#include <utility>

struct Enemy{
    sf::RectangleShape hitbox;
    sf::Text HpText;
    float enemyMoveSpeed;
    int HP;
    bool isUnpredictable;

    Enemy(
            sf::Clock& enemyClock,
            sf::RectangleShape enemyShape,
            sf::Text enemyText,
            sf::Color enemyColor,
            Game& game,
            float moveSpeed,  int HP, float isUnpredictable)

            {
                enemyClock.restart();
                srand(time(NULL));

                this->hitbox = std::move(enemyShape);
                this->HpText = std::move(enemyText);
                this->enemyMoveSpeed = moveSpeed;
                this->HP = HP;
                this->isUnpredictable = isUnpredictable;
                this->hitbox.setOrigin((sf::Vector2f)this->hitbox.getSize() / 2.f);
                this->hitbox.setFillColor(enemyColor);
                this->HpText.setFillColor(sf::Color::Black);

                if(rand()%2 >=1){
                    this->hitbox.setPosition(0, rand()%game.window.getSize().y);
                } else {
                    this->hitbox.setPosition(rand()%game.window.getSize().x, 0);
                }

    }

};

namespace enemyFunctions {
    auto enemyDrawLogic(std::deque<Enemy> &enemiesVector, std::deque<Projectile> &projectiles, Player &player,
                        Game &game) {

        for (auto &enemy: enemiesVector) {
            if (enemy.HP > 0) {
                auto enemyAngle = (atan2(
                        (player.hitbox.getPosition().y - enemy.hitbox.getPosition().y),
                        player.hitbox.getPosition().x - enemy.hitbox.getPosition().x)) * (180 / 3.141);


                enemy.hitbox.rotate(enemyAngle - enemy.hitbox.getRotation());

                if (!enemy.isUnpredictable) {
                    enemy.hitbox.move(enemy.enemyMoveSpeed * cos(enemy.hitbox.getRotation() * (3.14 / 180)),
                                      enemy.enemyMoveSpeed * sin(enemy.hitbox.getRotation() * (3.14 / 180)));
                } else {
                    enemy.hitbox.move(enemy.enemyMoveSpeed * cos(enemy.hitbox.getRotation()),
                                      enemy.enemyMoveSpeed * sin(enemy.hitbox.getRotation()));
                    // not turning radians into degrees for unpredictability
                }

                enemy.HpText.setString(std::to_string(enemy.HP));
                enemy.HpText.setPosition(enemy.hitbox.getPosition());

                if (enemy.hitbox.getGlobalBounds().intersects(player.hitbox.getGlobalBounds())) {
                    enemy.HP = 0;
                    player.HP -= 1;
                }


                for (auto &projectile : projectiles) {
                    if (enemy.hitbox.getGlobalBounds().intersects(projectile.projectile.getGlobalBounds()) &&
                        enemy.HP > 0 && !projectile.hitSomething) {
                        enemy.HP -= player.projectileDamage;
                        projectile.hitSomething = true;

                        if (enemy.HP <= 0) {
                            player.score++;
                        }
                    }
                }

                if(enemy.hitbox.getGlobalBounds().intersects(player.meleShape.getGlobalBounds()) && enemy.HP > 0
                && player.isDuringMeleAnimation){
                    enemy.HP -= player.meleDamage;

                    if(enemy.HP <= 0){
                        player.score++;
                    }
                }

                game.window.draw(enemy.hitbox);
                game.window.draw(enemy.HpText);
            }
        }
        if(!enemiesVector.empty() && enemiesVector[0].HP <= 0){
            enemiesVector.pop_front();
        }
    }
}