#include <SFML/Graphics.hpp>
#include <thread>
#include <vector>
#include <iostream>
#include<set>
using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono;

class Player{
public:
    //sf::Color color;// = sf::Color(0, 255, 0)
    sf::RectangleShape bodyBlock;
    sf::RectangleShape headBlock;
    //sf::RectangleShape headBlock;
    sf::Vector2<int> move = {20, 0};
    sf::Vector2<int> trail[(1000/20)*(1600/20)];
    int size = 0;


    Player(sf::Color color, sf::Vector2<int> facing, sf::Vector2<int> start){
        size = 1;
        for (int i = 0; i < size; i++){
            trail[i] = sf::Vector2<int>((20*size)-(i*facing.x)+start.x, start.y);
        }
        move = facing;
        bodyBlock.setFillColor(color);
        bodyBlock.setSize(sf::Vector2f(20-1, 20-1));
        headBlock.setFillColor(color);
        headBlock.setSize(sf::Vector2f(20-1, 20-1));
    }

    void increase(){
        sf::Vector2<int> head = trail[0]+move;
        for (int i = size; i > 0; i--)
            trail[i] = trail[i - 1];
        size++;
        trail[0] = head;
        //delete &move;
    }

    void movePlayer(){
        sf::Vector2<int> temp = trail[0]+move;
        for(int i=size;i>=0;i--)
        {
            trail[i+1] = trail[i];
        }
        trail[0] = temp;
    }

    bool out(){
        if ((sf::Vector2<int>(trail[0]+move).x > 1600-20) || (sf::Vector2<int>(trail[0]+move).x < 0) || (sf::Vector2<int>(trail[0]+move).y > 1000-20) || (sf::Vector2<int>(trail[0]+move).y < 0)){
            return true;
        }
        return false;
    }

    bool cutSelf(sf::Vector2<int> pos){
        for (int i = 0; i < size; i++){
            if (pos == trail[i]){
                return true;
            }
        }
        return false;
    }
};


void calc(Player *player1, Player *player2){
    while (true){
        if ((*player1).out() || (*player1).cutSelf((*player1).trail[0]+(*player1).move) || (*player2).cutSelf((*player1).trail[0]+(*player1).move))break;
        else if ((*player1).size <= 10000) (*player1).increase();
        else (*player1).movePlayer();

        if ((*player2).out() || (*player1).cutSelf((*player2).trail[0]+(*player2).move) || (*player2).cutSelf((*player2).trail[0]+(*player2).move))break;
        else if ((*player2).size <= 10000) (*player2).increase();
        else (*player2).movePlayer();

        sleep_for(nanoseconds(200000000));
    }
    (*player1).bodyBlock.setFillColor(sf::Color(100, 100, 0));
    (*player2).bodyBlock.setFillColor(sf::Color(100, 100, 0));
}


int main(){

    Player player1(sf::Color(255, 0, 255), sf::Vector2<int> {20, 0}, sf::Vector2<int> {80, 500});
    Player player2(sf::Color(0, 255, 255), sf::Vector2<int> {-20, 0}, sf::Vector2<int> {1600-80, 500});

    std::thread thread_(calc, &player1, &player2);

    sf::RenderWindow window(sf::VideoMode(1600, 1000), "Tron++");//, sf::Style::Fullscreen);


    while (window.isOpen()){
        sf::Event event;

        while (window.pollEvent((event))){
            if(event.type == sf::Event::Closed) {window.close(); exit(0);}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                if((player1.trail[0]+sf::Vector2<int>(0, -20)) != player1.trail[1]){
                    player1.move = sf::Vector2<int>(0, -20);
                }
            }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                if((player1.trail[0]+sf::Vector2<int>(0, 20)) != player1.trail[1]){
                    player1.move = sf::Vector2<int>(0, 20);
                }
            }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                if((player1.trail[0]+sf::Vector2<int>(-20, 0)) != player1.trail[1]){
                    player1.move = sf::Vector2<int>(-20, 0);
                }
            }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                if((player1.trail[0]+sf::Vector2<int>(20, 0)) != player1.trail[1]){
                    player1.move = sf::Vector2<int>(20, 0);
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)){
                if((player2.trail[0]+sf::Vector2<int>(0, -20)) != player2.trail[1]){
                    player2.move = sf::Vector2<int>(0, -20);
                }
            }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)){
                if((player2.trail[0]+sf::Vector2<int>(0, 20)) != player2.trail[1]){
                    player2.move = sf::Vector2<int>(0, 20);
                }
            }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)){
                if((player2.trail[0]+sf::Vector2<int>(-20, 0)) != player2.trail[1]){
                    player2.move = sf::Vector2<int>(-20, 0);
                }
            }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)){
                if((player2.trail[0]+sf::Vector2<int>(20, 0)) != player2.trail[1]){
                    player2.move = sf::Vector2<int>(20, 0);
                }
            }
        }

        window.clear();
        player1.bodyBlock.setPosition(((player1).trail)[0].x, ((player1).trail)[0].y);
        window.draw((player1).bodyBlock);
        for (int i = 1; i < player1.size; i++){
            player1.headBlock.setPosition(((player1).trail)[i].x, ((player1).trail)[i].y);
            window.draw((player1).headBlock);

        }

        player2.bodyBlock.setPosition(((player2).trail)[0].x, ((player2).trail)[0].y);
        window.draw((player2).bodyBlock);
        for (int i = 1; i < player2.size; i++){
            player2.headBlock.setPosition(((player2).trail)[i].x, ((player2).trail)[i].y);
            window.draw((player2).headBlock);

        }



        window.display();
    }

    return 0;
}