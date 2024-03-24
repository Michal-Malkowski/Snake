#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(0));

    const int length=800,width=600;
    sf::Vector2i sizes(40,40);
    sf::Vector2f lastPosition,lastPosition2;
    sf::RenderWindow window(sf::VideoMode(length,width),"Snake");
    window.setFramerateLimit(30);

    sf::RectangleShape head(sf::Vector2f(sizes.x,sizes.y));
    head.setPosition(sf::Vector2f(length/2-sizes.x,width/2-sizes.x));
    head.setFillColor(sf::Color::Blue);

    sf::CircleShape apple(sizes.x/2);
    apple.setPosition(sf::Vector2f(rand()%(length-sizes.x),rand()%(width-sizes.y)));
    apple.setFillColor(sf::Color::Red);

    std::vector <sf::RectangleShape> tail;
    sf::RectangleShape tailSegment(sf::Vector2f(sizes.x,sizes.y));
    tailSegment.setFillColor(sf::Color::Green);
    tailSegment.setPosition(sf::Vector2f(-100,-100));

    sf::CircleShape bonus(sizes.x/2);
    bonus.setPosition(sf::Vector2f(-100,-100));

    sf::RectangleShape obstacle[4];

    int points=0,dir=4,speed=100;
    bool st=true,bon=false,bon2=false,obst=false,immortality=false,lose=false;
    int turning[4],largest=0;
    for(int i=0;i<4;i++) turning[i]=0;

    sf::Clock clock,clock2,bonusClock;
    sf::Time time,time2,bonusTime;

    sf::SoundBuffer buffer;
    sf::Sound sound;

    sf::Text text;
    sf::Font font;
    if(!font.loadFromFile("arial.ttf")) std::cout<<"Cant load font";

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type==sf::Event::Closed||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        lastPosition=head.getPosition();
        lastPosition2=head.getPosition();

        ///kierunek ruchu
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)&&dir!=2)
        {
            dir=1;
            turning[dir-1]++;
            for(int i=1;i<4;i++)
                if(turning[i]>0) turning[i]++;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)&&dir!=1)
        {
            dir=2;
            turning[dir-1]++;
            for(int i=0;i<4;i++)
                if(turning[i]>0&&i!=dir-1) turning[i]++;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)&&dir!=4)
        {
            dir=3;
            turning[dir-1]++;
            for(int i=0;i<4;i++)
                if(turning[i]>0&&i!=dir-1) turning[i]++;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)&&dir!=3)
        {
            dir=4;
            turning[dir-1]++;
            for(int i=0;i<4;i++)
                if(turning[i]>0&&i!=dir-1) turning[i]++;
        }

        largest=0;
        for(int i=0;i<4;i++)
            if(largest<turning[i]) largest=turning[i];

        if(st)
        {
            if(largest==turning[0]&&largest!=0)
            {
                lastPosition.y-=sizes.y;
                turning[0]=0;
            }
            else if(largest==turning[1]&&largest!=0)
            {
                lastPosition.y+=sizes.y;
                turning[1]=0;
            }
            else if(largest==turning[2]&&largest!=0)
            {
                lastPosition.x-=sizes.x;
                turning[2]=0;
            }
            else if(largest==turning[3]&&largest!=0)
            {
                lastPosition.x+=sizes.x;
                turning[3]=0;
            }
            else if(dir==1) lastPosition.y-=sizes.y;
            else if(dir==2) lastPosition.y+=sizes.y;
            else if(dir==3) lastPosition.x-=sizes.x;
            else if(dir==4) lastPosition.x+=sizes.x;
        }
        ///bonusy
        time2=clock2.getElapsedTime();
        if(time2.asSeconds()>10&&!bon)
        {
            int random=rand()%50;
            if(random<=2)
            {
                bonusClock.restart();
                clock2.restart();
                bon=true;
                if(random==0)
                {
                    bonus.setFillColor(sf::Color::Black);
                    bonus.setPosition(sf::Vector2f((rand()%(length-sizes.x)/sizes.x)*sizes.x,(rand()%(width-sizes.y)/sizes.y)*sizes.y));
                }
                else if(random==1)
                {
                    bonus.setFillColor(sf::Color(100,100,100));
                    do bonus.setPosition(sf::Vector2f((rand()%(length-sizes.x)/sizes.x)*sizes.x,(rand()%(width-sizes.y)/sizes.y)*sizes.y));
                    while(bonus.getPosition().x<=2*sizes.y||bonus.getPosition().x>=length-(2*sizes.y)||
                          bonus.getPosition().y<=2*sizes.x||bonus.getPosition().y>=width-(2*sizes.x));
                }
                else
                {
                    bonus.setFillColor(sf::Color::Yellow);
                    bonus.setPosition(sf::Vector2f((rand()%(length-sizes.x)/sizes.x)*sizes.x,(rand()%(width-sizes.y)/sizes.y)*sizes.y));
                }
            }
        }
        if(bon) bonusTime=bonusClock.getElapsedTime();
        if(bonusTime.asSeconds()>8)
        {
            bon=false;
            bonus.setPosition(sf::Vector2f(-100,-100));
            speed=100;
            if(immortality)
            {
                immortality=false;
                for(int i=0;i<tail.size();i++) tail[i].setFillColor(sf::Color::Green);
            }
            if(obst)
            {
                obst=false;
                for(int i=0;i<4;i++) obstacle[i].setPosition(sf::Vector2f(-10000,-10000));
            }
        }
        if(immortality) for(int i=0;i<tail.size();i++) tail[i].setFillColor(sf::Color::Yellow);
        ///zebranie jablka
        if(head.getGlobalBounds().intersects(apple.getGlobalBounds()))
        {
            points++;
            if(!obst) do apple.setPosition(sf::Vector2f((rand()%(length-sizes.x)/sizes.x)*sizes.x,(rand()%(width-sizes.y)/sizes.y)*sizes.y));
            while(head.getGlobalBounds().intersects(apple.getGlobalBounds()));
            else do apple.setPosition(sf::Vector2f((rand()%(length-sizes.x)/sizes.x)*sizes.x,(rand()%(width-sizes.y)/sizes.y)*sizes.y));
            while(head.getGlobalBounds().intersects(apple.getGlobalBounds())&&apple.getPosition().x<sizes.x&&apple.getPosition().x>length-sizes.x
                  &&apple.getPosition().y<sizes.y&&apple.getPosition().y>width-sizes.y);
            tail.push_back(tailSegment);
            if(!buffer.loadFromFile("apple.wav"))
                std::cout<<"cant load sound";
            sound.setBuffer(buffer);
            sound.play();
        }
        ///zebranie bonusu
        if(head.getGlobalBounds().intersects(bonus.getGlobalBounds()))
        {
            points++;
            tail.push_back(tailSegment);
            bonusClock.restart();
            bon=true;
            bonus.setPosition(sf::Vector2f(-100,-100));
            if(bonus.getFillColor()==sf::Color::Black)
            {   ///szybkosc
                if(rand()%2==0)
                {
                    speed=50;
                    if(!buffer.loadFromFile("black_bonus.wav"))
                        std::cout<<"cant load sound";
                    sound.setBuffer(buffer);
                    sound.play();
                }
                else
                {
                    speed=200;
                    if(!buffer.loadFromFile("black_bonus2.wav"))
                        std::cout<<"cant load sound";
                    sound.setBuffer(buffer);
                    sound.play();
                }
            }
            else if(bonus.getFillColor()==sf::Color(100,100,100))
            {   ///przeszkoda
                if(rand()%2==0)
                {
                    for(int i=0;i<=1;i++) obstacle[i].setSize(sf::Vector2f(length,sizes.y));
                    for(int i=2;i<=3;i++) obstacle[i].setSize(sf::Vector2f(sizes.x,width));
                    obstacle[0].setPosition(sf::Vector2f(0,0));
                    obstacle[1].setPosition(sf::Vector2f(0,width-sizes.y));
                    obstacle[2].setPosition(sf::Vector2f(0,0));
                    obstacle[3].setPosition(sf::Vector2f(length-sizes.x,0));
                    for(int i=0;i<4;i++) obstacle[i].setFillColor(sf::Color(100,100,100));
                    for(int i=0;i<4;i++) obstacle[i].setOutlineColor(sf::Color::Black);
                    for(int i=0;i<4;i++) obstacle[i].setOutlineThickness(5);
                    obst=true;
                    if(!buffer.loadFromFile("black_bonus2.wav"))
                        std::cout<<"cant load sound";
                    sound.setBuffer(buffer);
                    sound.play();
                }///niesmiertelnosc
                else
                {
                    immortality=true;
                    if(!buffer.loadFromFile("black_bonus.wav"))
                        std::cout<<"cant load sound";
                    sound.setBuffer(buffer);
                    sound.play();
                }
            }
            else if(bonus.getFillColor()==sf::Color::Yellow)
            {   ///+2 lub -1
                if(rand()%2==0)
                {
                    points++;
                    tail.push_back(tailSegment);
                    if(!buffer.loadFromFile("black_bonus.wav"))
                        std::cout<<"cant load sound";
                    sound.setBuffer(buffer);
                    sound.play();
                }
                else
                {
                    points-=2;
                    for(int i=0;i<2;i++) tail.pop_back();
                    if(!buffer.loadFromFile("black_bonus2.wav"))
                        std::cout<<"cant load sound";
                    sound.setBuffer(buffer);
                    sound.play();
                }
            }
        }
        ///kolizja z ogonem
        for(int i=0;i<tail.size();i++)
            if(head.getGlobalBounds().intersects(tail[i].getGlobalBounds())&&!immortality) lose=true;
        ///kolizja ze scianami (o ile sa)
        if(obst&&!immortality)
        {
            for(int i=0;i<4;i++)
                if(head.getGlobalBounds().intersects(obstacle[i].getGlobalBounds())) lose=true;
        }
        ///ruch weza
        if(st)
        {
            if(head.getPosition().x<0) head.setPosition(length-sizes.x,lastPosition.y);
            else if(head.getPosition().x>length-sizes.x) head.setPosition(0,lastPosition.y);
            else if(head.getPosition().y<0) head.setPosition(lastPosition.x,width-sizes.y);
            else if(head.getPosition().y>width-sizes.y) head.setPosition(lastPosition.x,0);
            else head.setPosition(lastPosition.x,lastPosition.y);
            if(tail.size()>0)
            {
                lastPosition=tail[0].getPosition();
                tail[0].setPosition(lastPosition2);
            }
            for(int i=1;i<tail.size();i++)
            {
                if(i%2!=0)
                {
                    lastPosition2=tail[i].getPosition();
                    tail[i].setPosition(lastPosition);
                }
                else
                {
                    lastPosition=tail[i].getPosition();
                    tail[i].setPosition(lastPosition2);
                }
            }
        }
        ///szybkosc poruszania sie
        st=false;
        if(time.asMilliseconds()>speed)
        {
            st=true;
            clock.restart();
        }
        time=clock.getElapsedTime();

        std::string pointsText="Punkty: ";
        std::string ss;
        sprintf((char*)ss.c_str(),"%d",points);
        std::string str = ss.c_str();
        pointsText+=str;

        if(lose)
        {
            while(window.isOpen())
            {
                while(window.pollEvent(event))
                {
                    if(event.type==sf::Event::Closed||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                        window.close();
                }
                text.setString("Przegrales");
                text.setCharacterSize(75);
                text.setPosition(sf::Vector2f(length/2-length/4,width/3));
                text.setFillColor(sf::Color::Black);
                window.clear(sf::Color::White);
                window.draw(text);
                window.display();
            }
        }

        window.clear(sf::Color::White);
        text.setFont(font);
        text.setString(pointsText);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        if(obst) for(int i=0;i<4;i++) window.draw(obstacle[i]);
        window.draw(text);
        window.draw(head);
        window.draw(bonus);
        window.draw(apple);
        for(int i=0;i<tail.size();i++) window.draw(tail[i]);
        window.display();
    }

    return 0;
}
