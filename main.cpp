#include <SFML/Graphics.hpp>
#include <thread>
#include <iostream>

#define rezx 1366
#define rezy 768


using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono;

class Player{
public:
    //sf::Color color;// = sf::Color(0, 255, 0)
    sf::Sprite bodyBlock;
    //sf::RectangleShape headBlock;
    sf::Vector2<int> move = {20, 0};
    sf::Vector2<int> trail[(1360/20)*(770/20)];
    int size = 0;
    bool loss = false;


    Player(sf::Vector2<int> facing, sf::Vector2<int> start, int player){
        size = 1;
        for (int i = 0; i < size; i++){
            trail[i] = sf::Vector2<int>((20*size)-(i*facing.x)+start.x, start.y);
        }
        move = facing;
        //bodyBlock.setFillColor(color);
        //bodyBlock.setSize(sf::Vector2f(20-1, 20-1));
        bodyBlock.setOrigin(10, 10);
        //headBlock.setFillColor(color);
        //headBlock.setSize(sf::Vector2f(20-1, 20-1));



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
        if ((sf::Vector2<int>(trail[0]+move).x > 1360-20) || (sf::Vector2<int>(trail[0]+move).x < 10) || (sf::Vector2<int>(trail[0]+move).y > 770-20) || (sf::Vector2<int>(trail[0]+move).y < 10)){
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

    int headRotation(){
        if (trail[1].x == trail[0].x){
            if (trail[1].y == trail[0].y+20){
                return 90;
            }else if (trail[1].y == trail[0].y-20){
                return -90;
            }
        }else{
            if (trail[1].x == trail[0].x-20){
                return 180;
            }else {
                return 0;
            }
        }
        return 0;
    }

    int bodyRotation(int position){
        sf::Vector2<int> backDifference = sf::Vector2<int>{(trail[position].x - trail[position+1].x), (trail[position].y - trail[position+1].y)};
        sf::Vector2<int> frontDifference = sf::Vector2<int>{(trail[position-1].x - trail[position].x), (trail[position-1].y - trail[position].y)};

        backDifference.y = -backDifference.y;
        frontDifference.y = -frontDifference.y;

        sf::Vector2<int> totalDifference = sf::Vector2<int>{backDifference.x + frontDifference.x, backDifference.y + frontDifference.y};


        if (totalDifference.y == 0){
            return 1000;
        }else if (totalDifference.x == 0){
            return 1090;
        }else if (backDifference.x == 20){
            if (frontDifference.y == 20){
                return 2000;
            }else{
                return 2270;
            }
        }else if (backDifference.x == -20){
            if (frontDifference.y == 20){
                return 2090;
            }else{
                return 2180;
            }
        }else if (backDifference.y == 20){
            if (frontDifference.x == 20){
                return 2180;
            }else{
                return 2270;
            }
        }else if (backDifference.y == -20){
            if (frontDifference.x == 20){
                return 2090;
            }else{
                return 2000;
            }
        }
        return 0;
    }
};

void calc(Player *player1, Player *player2){
    while (true){
        if ((*player1).out() || (*player1).cutSelf((*player1).trail[0]+(*player1).move) || (*player2).cutSelf((*player1).trail[0]+(*player1).move)) {
            (*player1).loss = true;
            break;
        }


        if ((*player2).out() || (*player1).cutSelf((*player2).trail[0]+(*player2).move) || (*player2).cutSelf((*player2).trail[0]+(*player2).move)) {
            (*player2).loss = true;
            break;
        }

        if((*player1).loss || (*player1).loss){
            break;
        }else{
            (*player2).increase();
            (*player1).increase();
        }

        sleep_for(nanoseconds(100000000));
    }
    //(*player1).bodyBlock.setFillColor(sf::Color(100, 100, 0));
    //(*player2).bodyBlock.setFillColor(sf::Color(100, 100, 0));
}

int main(){
    sf::Texture end1;
    sf::Texture end2;
    sf::Texture body1;
    sf::Texture body2;
    sf::Texture corner1;
    sf::Texture corner2;
    end1.loadFromFile("assets/p1_end.png");
    end2.loadFromFile("assets/p2_end.png");
    body1.loadFromFile("assets/p1_body.png");
    body2.loadFromFile("assets/p2_body.png");
    corner1.loadFromFile("assets/p1_corner.png");
    corner2.loadFromFile("assets/p2_corner.png");

    sf::Texture restart;
    restart.loadFromFile("assets/restart.png");
    sf::Sprite restartButton;
    restartButton.setTexture(restart);

    sf::RenderWindow window(sf::VideoMode(rezx, rezy), "Tron++");//, sf::Style::Fullscreen);

    sf::RectangleShape background;
    background.setFillColor(sf::Color(33, 33, 33));
    background.setSize(sf::Vector2f(rezx,rezy));

    sf::Texture titleScreenImage;
    titleScreenImage.loadFromFile("./assets/tron_loading_screen.png");
    sf::Sprite titleScreenSprite;
    titleScreenSprite.setTexture(titleScreenImage);

    int state = 0;
    sf::Event event{};

    while (state == 0 && window.isOpen()){


        while (window.pollEvent((event))){
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {window.close(); exit(0);}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) || sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
            {
                state = 1;
            }
        }
        window.clear();
        window.draw(background);
        window.draw(titleScreenSprite);

        window.display();
    }

    //actual game

    Player player1(sf::Vector2<int> {20, 0}, sf::Vector2<int> {80, 380}, 1);
    Player player2(sf::Vector2<int> {-20, 0}, sf::Vector2<int> {1260, 380}, 1);
    std::thread thread_(calc, &player1, &player2);

    while (window.isOpen()){

        while (window.pollEvent((event))){
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {window.close(); exit(0);}
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
        window.draw(background);



        player1.bodyBlock.setPosition(((player1).trail)[0].x, ((player1).trail)[0].y);
        player1.bodyBlock.setTexture(end1);
        player1.bodyBlock.setRotation((player1.headRotation()));
        window.draw((player1).bodyBlock);
        player1.bodyBlock.setRotation(0.f);
        for (int i = 1; i < player1.size-1; i++){
            player1.bodyBlock.setPosition(((player1).trail)[i].x, ((player1).trail)[i].y);
            int textureInfo = player1.bodyRotation(i);
            player1.bodyBlock.setRotation(textureInfo % 1000);
            if (textureInfo - (textureInfo % 1000) == 1000){
                player1.bodyBlock.setTexture(body1);
            }else{
                player1.bodyBlock.setTexture(corner1);
            }
            window.draw((player1).bodyBlock);
        }
        player1.bodyBlock.setPosition(((player1).trail)[player1.size-1].x, ((player1).trail)[player1.size-1].y);
        player1.bodyBlock.setTexture(end1);
        player1.bodyBlock.setRotation(0.f);
        window.draw((player1).bodyBlock);



        player2.bodyBlock.setPosition(((player2).trail)[0].x, ((player2).trail)[0].y);
        player2.bodyBlock.setTexture(end2);
        player2.bodyBlock.setRotation((player2.headRotation()));
        window.draw((player2).bodyBlock);
        player2.bodyBlock.setRotation(180);
        for (int i = 1; i < player2.size-1; i++){
            player2.bodyBlock.setPosition(((player2).trail)[i].x, ((player2).trail)[i].y);
            int textureInfo = player2.bodyRotation(i);
            player2.bodyBlock.setRotation(textureInfo % 1000);
            if (textureInfo - (textureInfo % 1000) == 1000){
                player2.bodyBlock.setTexture(body2);
            }else{
                player2.bodyBlock.setTexture(corner2);
            }
            window.draw((player2).bodyBlock);
        }
        player2.bodyBlock.setPosition(((player2).trail)[player2.size-1].x, ((player2).trail)[player2.size-1].y);
        player2.bodyBlock.setTexture(end2);
        player2.bodyBlock.setRotation(180);
        window.draw((player2).bodyBlock);

        window.display();
    }

    return 0;
}
