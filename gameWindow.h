#pragma once
#include <SFML/Graphics.hpp>
#include <fmt/ranges.h>
#include <fmt/core.h>
#include <deque>
#include "player.h"


struct TextInput {
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({400,150}), "Press Enter To Confirm");
    sf::Text displayText = sf::Text();
    std::string text;

    TextInput(sf::Font font){
        displayText.setFont(font);
        displayText.setCharacterSize(100);
        displayText.setFillColor(sf::Color::White);
        displayText.setPosition(0.f,0.f);
    }

};

struct Game {
    sf::RenderWindow window;
    bool ispaused = false;
    bool isTitleScreen = true;
    bool isRestarted = false;
    bool isRestartScreen = false;
    bool isWaveStarted = false;
    bool isLoadedAfterRestart = false;

    auto getWindowMiddle() const{
        return sf::Vector2<float>(window.getSize().x/2, window.getSize().y/2);
    }

    //pause screen implementation
    auto pause(Player& player) -> void {

        ispaused = true;

        auto font = sf::Font();
        font.loadFromFile("../Fonts/Jacquard24-Regular.ttf");

        auto pausedText = sf::Text("Paused", font, 100);
        pausedText.setPosition(getWindowMiddle().x - pausedText.getCharacterSize()*1.5,
                               getWindowMiddle().y - pausedText.getCharacterSize()*2);
        pausedText.setFillColor(sf::Color::White);

        auto resumeText = sf::Text("Resume", font, 75);
        resumeText.setPosition(pausedText.getPosition() + sf::Vector2f(25.f,110));
        resumeText.setFillColor(sf::Color::White);

        auto resumeButton = sf::RectangleShape({200, 100});
        resumeButton.setPosition(resumeText.getPosition() + sf::Vector2f(5, 5));
        resumeButton.setFillColor(sf::Color::Black);

        auto saveText = sf::Text("Save", font, 75);
        saveText.setPosition(resumeText.getPosition() + sf::Vector2f(40.f,80));
        if(isWaveStarted){
            saveText.setFillColor(sf::Color(90, 94, 99));
        }else{
            saveText.setFillColor(sf::Color::White);
        }

        auto saveButton = sf::RectangleShape({125, 80});
        saveButton.setPosition(saveText.getPosition() + sf::Vector2f(5, 15));
        saveButton.setFillColor(sf::Color::Black);

        auto quitText = sf::Text("Quit", font, 75);
        quitText.setPosition(saveText.getPosition() + sf::Vector2f(5.f,80));
        quitText.setFillColor(sf::Color::Red);

        auto quitButton = sf::RectangleShape({120, 70});
        quitButton.setPosition(quitText.getPosition() + sf::Vector2f(0.f, 20));
        quitButton.setFillColor(sf::Color::Black);

        while(ispaused) {

            auto event = sf::Event();

            while(window.pollEvent(event)){
                switch(event.type) {

                    case sf::Event::Closed:
                        window.close();

                    case sf::Event::KeyPressed:
                        if(event.key.scancode == sf::Keyboard::Scan::Escape){
                            ispaused = false;
                            break;
                        }

                    case sf::Event::MouseButtonPressed:
                        if(event.mouseButton.button == sf::Mouse::Left){
                            auto mousePos = sf::Mouse::getPosition(window);
                            if(resumeButton.getGlobalBounds().contains({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)})){
                                ispaused = false;
                            }
                            if(quitButton.getGlobalBounds().contains({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)})){
                                window.close();
                                ispaused = false;
                            }
                            if(saveText.getGlobalBounds().contains({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)}) && !isWaveStarted){

                                TextInput textInput = TextInput(font);
                                while(textInput.window.isOpen()){

                                    while(textInput.window.pollEvent(event)){

                                        if(event.type == sf::Event::Closed){
                                            textInput.window.close();
                                        }

                                        if(event.type == sf::Event::KeyPressed){
                                            if(event.key.scancode == sf::Keyboard::Scan::Enter){
                                                player.Save(textInput.text);
                                                textInput.window.close();
                                            }
                                            if(event.key.scancode == sf::Keyboard::Scan::Escape){
                                                textInput.window.close();
                                            }
                                            if(event.key.scancode == sf::Keyboard::Scan::Backspace){
                                                textInput.text.pop_back();
                                            }
                                        }

                                        //https://en.sfml-dev.org/forums/index.php?topic=148.0
                                        if(event.type == sf::Event::TextEntered){
                                            if(event.text.unicode < 0x80 && event.text.unicode != 0x08){
                                                textInput.text += (char)event.text.unicode;
                                            }
                                        }
                                    }
                                    textInput.text.erase(textInput.text.begin(), std::find_if(textInput.text.begin(), textInput.text.end(), [](auto c){
                                        return !std::isspace(c);
                                    }));

                                    auto wpiszTekst = sf::Text(textInput.text, font,100);
                                    wpiszTekst.setFillColor(sf::Color::White);


                                    textInput.window.clear(sf::Color::Black);
                                    textInput.window.draw(wpiszTekst);
                                    textInput.window.display();
                                }
                            }
                        }
                        break;
                }
            }

            window.clear(sf::Color::Black);
            window.draw(pausedText);
            window.draw(resumeButton);
            window.draw(resumeText);
            window.draw(saveButton);
            window.draw(saveText);
            window.draw(quitButton);
            window.draw(quitText);
            window.display();
        }
    }

    auto titleScreen(Player& player) -> void{
        //https://www.sfml-dev.org/tutorials/2.6/audio-sounds.php
        //https://www.sfml-dev.org/documentation/2.6.1/classsf_1_1Sound.php

        auto font = sf::Font();
        font.loadFromFile("../Fonts/Jacquard24-Regular.ttf");

        auto titleText = sf::Text("Pixel Perfect", font, 100);
        titleText.setPosition(getWindowMiddle().x - titleText.getCharacterSize() * 2.25,
                              getWindowMiddle().y - titleText.getCharacterSize() * 2);
        titleText.setFillColor(sf::Color::White);

        auto startText = sf::Text("Start", font, 75);
        startText.setPosition(titleText.getPosition() + sf::Vector2f(150.f, 110));
        startText.setFillColor(sf::Color::Yellow);

        auto startButton = sf::RectangleShape({200, 100});
        startButton.setPosition(startText.getPosition() + sf::Vector2f(5, 5));
        startButton.setFillColor(sf::Color::Black);

        auto loadText = sf::Text("Load", font, 75);
        loadText.setPosition(startText.getPosition() + sf::Vector2f(0.f, 80));
        loadText.setFillColor(sf::Color::White);

        auto loadButton = sf::RectangleShape({125, 80});
        loadButton.setPosition(loadText.getPosition() + sf::Vector2f(5, 15));
        loadButton.setFillColor(sf::Color::Black);

        auto quitText = sf::Text("Quit", font, 75);
        quitText.setPosition(loadText.getPosition() + sf::Vector2f(5.f, 80));
        quitText.setFillColor(sf::Color::Red);

        auto quitButton = sf::RectangleShape({120, 70});
        quitButton.setPosition(quitText.getPosition() + sf::Vector2f(0.f, 20));
        quitButton.setFillColor(sf::Color::Black);

        while(isTitleScreen){

            auto event = sf::Event();

            while(window.pollEvent(event)){
                switch(event.type) {

                    case sf::Event::Closed:
                        window.close();
                        break;

                    case sf::Event::KeyPressed:

                    case sf::Event::MouseButtonPressed:
                        if(event.mouseButton.button == sf::Mouse::Left){
                            auto mousePos = sf::Mouse::getPosition(window);
                            if(startButton.getGlobalBounds().contains({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)})){
                                isTitleScreen = false;
                            }
                            if(quitButton.getGlobalBounds().contains({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)})){
                                window.close();
                                isTitleScreen = false;
                            }
                            if(loadText.getGlobalBounds().contains({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)})){

                                TextInput textInput = TextInput(font);
                                while(textInput.window.isOpen()){

                                    while(textInput.window.pollEvent(event)){

                                        if(event.type == sf::Event::Closed){
                                            textInput.window.close();
                                        }

                                        if(event.type == sf::Event::KeyPressed){
                                            if(event.key.scancode == sf::Keyboard::Scan::Enter){
                                                player.Load(textInput.text);
                                                isTitleScreen = false;
                                                textInput.window.close();
                                            }
                                            if(event.key.scancode == sf::Keyboard::Scan::Escape){
                                                textInput.window.close();
                                            }
                                            if(event.key.scancode == sf::Keyboard::Scan::Backspace){
                                                textInput.text.pop_back();
                                            }
                                        }

                                        if(event.type == sf::Event::TextEntered){
                                            if(event.text.unicode < 0x80 && event.text.unicode != 0x08){
                                                textInput.text += (char)event.text.unicode;
                                            }
                                        }
                                    }
                                    textInput.text.erase(textInput.text.begin(), std::find_if(textInput.text.begin(), textInput.text.end(), [](auto c){
                                        return !std::isspace(c);
                                    }));

                                    auto wpiszTekst = sf::Text(textInput.text, font,100);
                                    wpiszTekst.setFillColor(sf::Color::White);


                                    textInput.window.clear(sf::Color::Black);
                                    textInput.window.draw(wpiszTekst);
                                    textInput.window.display();
                                }
                            }
                        }
                        break;
                }
            }


            window.clear(sf::Color::Black);
            window.draw(titleText);
            window.draw(startButton);
            window.draw(startText);
            window.draw(loadButton);
            window.draw(loadText);
            window.draw(quitButton);
            window.draw(quitText);
            window.display();
        }
    }

    auto defeatScreen(int score, Player& player){
        isRestartScreen = true;

        auto font = sf::Font();
        font.loadFromFile("../Fonts/Jacquard24-Regular.ttf");

        auto defeatText = sf::Text("Defeat", font, 100);
        defeatText.setPosition(getWindowMiddle().x - defeatText.getCharacterSize() * 1.25,
                               getWindowMiddle().y - defeatText.getCharacterSize() * 2);
        defeatText.setFillColor(sf::Color::Red);

        auto RestartText = sf::Text("Restart", font, 75);
        RestartText.setPosition(defeatText.getPosition() + sf::Vector2f(15.f, 110));
        RestartText.setFillColor(sf::Color::White);

        auto restartButton = sf::RectangleShape({200, 100});
        restartButton.setPosition(RestartText.getPosition() + sf::Vector2f(5, 5));
        restartButton.setFillColor(sf::Color::Black);

        auto loadText = sf::Text("Load", font, 75);
        loadText.setPosition(RestartText.getPosition() + sf::Vector2f(30.f, 80));
        loadText.setFillColor(sf::Color::White);

        auto loadButton = sf::RectangleShape({125, 80});
        loadButton.setPosition(loadText.getPosition() + sf::Vector2f(5, 15));
        loadButton.setFillColor(sf::Color::Black);

        auto quitText = sf::Text("Quit", font, 75);
        quitText.setPosition(loadText.getPosition() + sf::Vector2f(5.f, 80));
        quitText.setFillColor(sf::Color::Red);

        auto quitButton = sf::RectangleShape({120, 70});
        quitButton.setPosition(quitText.getPosition() + sf::Vector2f(0.f, 20));
        quitButton.setFillColor(sf::Color::Black);

        auto scoreText = sf::Text("Score: " + std::to_string(score), font, 75);
        scoreText.setPosition(quitText.getPosition() + sf::Vector2f(-35.f, 140));
        scoreText.setFillColor(sf::Color::White);

        while(isRestartScreen){

            auto event = sf::Event();

            while(window.pollEvent(event)){
                switch(event.type) {

                    case sf::Event::Closed:
                        window.close();
                        break;

                    case sf::Event::MouseButtonPressed:
                        if(event.mouseButton.button == sf::Mouse::Left){
                            auto mousePos = sf::Mouse::getPosition(window);
                            if(restartButton.getGlobalBounds().contains({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)})){
                                isRestartScreen = false;
                                isRestarted = true;
                            }
                            if(loadText.getGlobalBounds().contains({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)})){

                                TextInput textInput = TextInput(font);
                                while(textInput.window.isOpen()){

                                    while(textInput.window.pollEvent(event)){

                                        if(event.type == sf::Event::Closed){
                                            textInput.window.close();
                                        }

                                        if(event.type == sf::Event::KeyPressed){
                                            if(event.key.scancode == sf::Keyboard::Scan::Enter){
                                                player.Load(textInput.text);
                                                isRestartScreen = false;
                                                textInput.window.close();
                                            }
                                            if(event.key.scancode == sf::Keyboard::Scan::Escape){
                                                textInput.window.close();
                                            }
                                            if(event.key.scancode == sf::Keyboard::Scan::Backspace){
                                                textInput.text.pop_back();
                                            }
                                        }

                                        if(event.type == sf::Event::TextEntered){
                                            if(event.text.unicode < 0x80 && event.text.unicode != 0x08){
                                                textInput.text += (char)event.text.unicode;
                                            }
                                        }
                                    }
                                    textInput.text.erase(textInput.text.begin(),
                                                         std::find_if(textInput.text.begin(), textInput.text.end(), [](auto const& c){
                                        return !std::isspace(c);
                                    }));

                                    auto wpiszTekst = sf::Text(textInput.text, font,100);
                                    wpiszTekst.setFillColor(sf::Color::White);


                                    textInput.window.clear(sf::Color::Black);
                                    textInput.window.draw(wpiszTekst);
                                    textInput.window.display();

                                    isRestarted = true;
                                    isLoadedAfterRestart = true;
                                }
                            }

                            if(quitButton.getGlobalBounds().contains({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)})){
                                window.close();
                                isRestartScreen = false;
                            }
                        }
                        break;
                }
            }

            window.clear(sf::Color::Black);
            window.draw(defeatText);
            window.draw(restartButton);
            window.draw(RestartText);
            window.draw(loadButton);
            window.draw(loadText);
            window.draw(quitButton);
            window.draw(quitText);
            window.draw(scoreText);
            window.display();
        }
    }

};
