#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <thread>
#include <atomic>
#include <X11/Xlib.h>
using namespace sf;
using namespace std;

class player{
public:
    atomic<int> posx, posy, score, bound;
    RenderWindow* winpointer;
    bool move;

    player(){
        score = 0;
        posx = 0;
        posy = 0;
    }

    bool operator== (const player pl){
        if (pl.posx == this->posx && pl.posy == this->posy && pl.score == this->score){
            return true;
        }
        return false;
    }
};

struct tile{
    int posx, posy, color;
    bool object;
    tile(){
        posx = 0;
        posy = 0;
        color = 0;
        object = false;
    }
};

void* thread1(void* v1){
    player*p1 = (player*)v1;
    Event event;
    bool lastState = false;

    while(p1->winpointer->isOpen()){

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {   
                //move left
                if (lastState == false){
                    if (p1->posx >= 50){
                        p1->posx -= 50;
                        lastState = true;
                    }    
                }
                continue;       
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                //move right...
                if (lastState == false){
                    if (p1->posx < p1->bound - 50){
                        p1->posx += 50;
                        lastState = true;
                    }  
                }
                continue;    
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                //move up
                if (lastState == false){
                    if (p1->posy >= 50){
                        p1->posy -= 50;
                        lastState = true;
                    }    
                }
                continue; 
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                //move down
                if (lastState == false){
                    if (p1->posy < p1->bound - 50){
                        p1->posy += 50;
                        lastState = true;
                    }  
                }
                continue; 
            }
        lastState = false;
        //sf::sleep(milliseconds(2));
    }
    pthread_exit(NULL);
}

void* thread2 (void* v1){
    player*p1 = (player*)v1;
    Event event;
    bool lastState = false;

    while(p1->winpointer->isOpen()){

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                // move left...
                if (lastState == false){
                    if (p1->posx >= 50){
                        p1->posx -= 50;
                        lastState = true;
                    }    
                }
                continue; 
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                //move right...
                if (lastState == false){
                    if (p1->posx < p1->bound - 50){
                        p1->posx += 50;
                        lastState = true;
                    }  
                }
                continue; 
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                //move up
                if (lastState == false){
                    if (p1->posy >= 50){
                        p1->posy -= 50;
                        lastState = true;
                    }    
                }
                continue; 
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                //move down
                if (lastState == false){
                    if (p1->posy < p1->bound - 50){
                        p1->posy += 50;
                        lastState = true;
                    }  
                }
                continue; 
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                p1->winpointer->close();
            }
        lastState = false;
        //sf::sleep(milliseconds(1));
    }
    pthread_exit(NULL);
}

int main(){
    XInitThreads();
    //Calculating amount of squares in the board
    srand(time(0));
    int randnum = rand()%90 + 10;
    randnum *= 5; 
    int num = 0450; 
    num /= randnum;
    num %= 25;  
    if (num < 10){ num += 15; }
    int bound = num*50;

    //creating the window    
    RenderWindow window(VideoMode(bound, bound + 40), "BoardGame");

    RectangleShape shape(Vector2f(50,50)), shape2(Vector2f(50,50));
    Texture t1, t2, t3;
    t1.loadFromFile("goku.png");
    t2.loadFromFile("ball.png");
    t3.loadFromFile("vegeta.png");
    shape.setTexture(&t1);
    shape2.setTexture(&t3);
    window.setKeyRepeatEnabled(false);

    //initializes the board
    RectangleShape *sha = new RectangleShape[num*num];
    int shainitcount = 0, objcount = 0;
    tile** arr;
    arr = new tile*[num];
    for (int i = 0; i < num; i++){
        arr[i] = new tile[num];

        for (int x = 0; x < num; x++){
            arr[i][x].posx = i*50;
            arr[i][x].posy = x*50;
            if (rand()%20 == 1){
                arr[i][x].object = true;
                objcount++;
            }

            sha[shainitcount].setSize(Vector2f(49,49));
            sha[shainitcount].setPosition(Vector2f(arr[i][x].posx,arr[i][x].posy));
            shainitcount++;
        }
    }
    
    //INITIALIZING THE OBJECTS
    window.setActive();
    player p1, p2; p2.bound = bound; p1.bound = bound; p1.winpointer = &window; p2.winpointer = &window;
    p2.posx = bound-50;    

    //INITIALIZING THE THREADS
    pthread_t tid1, tid2; pthread_attr_t attr1, attr2;
    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_DETACHED);
    pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_DETACHED);

    window.setActive();

    //LAUNCHING THE THREADS
    pthread_create(&tid1, &attr1, thread1, &p1);
    pthread_create(&tid2, &attr2, thread2, &p2);

    //PHYSICS AND RENDERING
    while (window.isOpen())
    {
        //HANDLING INPUT FOR P1
        Event event;

        if (arr[p1.posx/50][p1.posy/50].object == true){
            arr[p1.posx/50][p1.posy/50].object = false;
            p1.score += 10;
            sha[(p1.posx/50)*num + (p1.posy/50)].setTexture(NULL);
            objcount--;
        }

        //HANDLES P2 COLLISIONS
        if (arr[p2.posx/50][p2.posy/50].object == true){
            arr[p2.posx/50][p2.posy/50].object = false;
            p2.score += 10;
            sha[(p2.posx/50)*num + (p2.posy/50)].setTexture(NULL);
            objcount--;
        }
        if (objcount == 0){
            window.close();
            if (p1.score > p2.score){
                cout << "Winner! P1" << endl;
                cout << "p1 score: " << p1.score << endl;
                cout << "p2 score: " << p2.score << endl;
            }else{
                cout << "Winner! P2" << endl;
                cout << "p1 score: " << p1.score << endl;
                cout << "p2 score: " << p2.score << endl;
            }          
        }


        window.clear();
        //DISPLAYING THE ACTORS
        int count = 0;
        for (int i = 0; i < num; i++){
            for (int x = 0; x < num; x++){
                if (arr[i][x].object == true){
                    sha[count].setTexture(&t2);
                }else{
                    if (arr[i][x].color == 1){
                        sha[count].setFillColor(Color::Green);
                    }
                    else{
                        sha[count].setFillColor(Color::White);
                    }
                }
                window.draw(sha[count]);
                count++;
            }
        }
        shape.setPosition(Vector2f(p1.posx, p1.posy));
        shape2.setPosition(Vector2f(p2.posx, p2.posy));
        window.draw(shape);
        window.draw(shape2);

        //DISPLAYING THE SCORES        
        string p1scr = "p1 score: ", p2scr = "p2 score: ";
        p1scr += to_string(p1.score); 
        p2scr += to_string(p2.score);
        sf::Font font;
        font.loadFromFile("ARIBL0.ttf");
        Text text;

        text.setFont(font);
        text.setString(p1scr);
        text.setCharacterSize(30); // in pixels, not points!
        text.setPosition(20, num*50+2);
        text.setFillColor(Color::Green);
        window.draw(text);
        text.setString(p2scr);
        text.setFillColor(Color::Red);
        text.setPosition(num*50-250, num*50+2);
        window.draw(text);

        window.display();
        
    }
}

