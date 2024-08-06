#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

const float SPEED_SNAKE = 10.f;
const int TIME = 150;

enum Status {
    stop, up, right, down, left
};

class Snake : public sf::Drawable {
private:
    bool life;
    bool flag;              // нужен чтобы не двигаться в обратном направлении, и чтобы выполнялась первая нажатая команда
    int count;              // количество вершин (если один квадратик то 6 вершин, два - 12 и тд)
    Status status;          // состояние куда двигается змейка
    sf::CircleShape food;
    
    sf::VertexArray body;   // массив вершин, из них рисуем змейку. совпадает с хитбоксом
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(food);
        target.draw(body);
    }
public:
    Snake(sf::Vector2f v = sf::Vector2f(0.f, 0.f)); // параметр задает положение змейки
    void setPosition(sf::Vector2f v);               // задает положение змейки 
    void checkPosition();
    void checkKeyboad();
    void move();
    void eat();
    void selfEat();
    void gameOver();
    bool checkFood();
    void print();
};

void updateGame(Snake &snake);

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(200, 320), "Snake");
    
    sf::RectangleShape wall1(sf::Vector2f(10.f, 150.f));
    wall1.setFillColor(sf::Color(128, 128, 128));
    wall1.setSize(sf::Vector2f(10.f, 320.f));

    sf::RectangleShape wall2(sf::Vector2f(10.f, 320.f));
    wall2.setFillColor(sf::Color(128, 128, 128));
    wall2.setPosition(190.f, 0.f);
    
    sf::RectangleShape wall3(sf::Vector2f(200.f, 10.f));
    wall3.setFillColor(sf::Color(128, 128, 128));

    sf::RectangleShape wall4(sf::Vector2f(200.f, 10.f));
    wall4.setFillColor(sf::Color(128, 128, 128));
    wall4.setPosition(0.f, 310.f);

    Snake snake(sf::Vector2f(60.f, 60.f));

    sf::Clock clock;
    
    while (window.isOpen())
    {
        sf::Time elapsed = clock.getElapsedTime();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            snake.checkKeyboad();
        }
        if (elapsed >= sf::milliseconds(TIME)) {
            updateGame(snake);
            elapsed = clock.restart();
        };
        window.clear();
        window.draw(wall1);
        window.draw(wall2);
        window.draw(wall3);
        window.draw(wall4);
        window.draw(snake);
        window.display();
    }

    return EXIT_SUCCESS;
}

void updateGame(Snake& snake)
{
    snake.checkKeyboad();
    snake.move();
    snake.checkPosition();
    snake.eat();
    snake.selfEat();
    snake.gameOver();
}

Snake::Snake(sf::Vector2f v) {
    life = true;
    flag = true;
    count = 6;
    status = stop;
    body.setPrimitiveType(sf::Triangles);
    body.resize(count);
    body[0].position = sf::Vector2f(60.f, 60.f);
    body[1].position = sf::Vector2f(60.f, 70.f);
    body[2].position = sf::Vector2f(70.f, 60.f);
    body[3].position = sf::Vector2f(70.f, 60.f);
    body[4].position = sf::Vector2f(60.f, 70.f);
    body[5].position = sf::Vector2f(70.f, 70.f);
    for (int i = 0; i < 6; ++i)
        body[i].color = sf::Color::Green;
    food.setFillColor(sf::Color::Yellow);
    food.setRadius(5.f);
    food.setPointCount(4);
    food.setPosition(sf::Vector2f(rand() % 18 * 10.f + 10.f, rand() % 30 * 10.f + 10.f));
}

void Snake::setPosition(sf::Vector2f v) {

    body[0] = v;
    body[1] = sf::Vector2f(v.x + 10.f, v.y);
    body[2] = sf::Vector2f(v.x, v.y + 10.f);
    body[3] = sf::Vector2f(v.x + 10.f, v.y);
    body[4] = sf::Vector2f(v.x, v.y + 10.f);
    body[5] = sf::Vector2f(v.x + 10.f, v.y + 10.f);
    
}

void Snake::checkPosition() {
    if (body[0].position.x <= 0.f)
        setPosition(sf::Vector2f(180.f, body[0].position.y));
    else if (body[0].position.x >= 190)
        setPosition(sf::Vector2f(10.f, body[0].position.y));
    else if (body[0].position.y <= 0)
        setPosition(sf::Vector2f(body[0].position.x, 300.f));
    else if (body[0].position.y >= 310)
        setPosition(sf::Vector2f(body[0].position.x, 10.f));
    for (int i = 0; i < count; ++i)
        body[i].color = sf::Color::Green; //цвет меняем в самом последнем действии после всех перемещений
}

void Snake::checkKeyboad() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ) {
        if (status != down && flag == true) {
            status = up;
            flag = false;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        if (status != up && flag == true) {
            status = down;
            flag = false;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        if (status != Status::right && flag == true) {
            status = Status::left;
            flag = false;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if (status != Status::left && flag == true) {
            status = Status::right;
            flag = false;
        }
    }
 
}

void Snake::move() {
    if (count > 6)
        for (int j = count / 6; j > 1; j -= 1)
            for (int i = (j - 1) * 6; i < j * 6; ++i) {
                body[i].position = body[i - 6].position;
            }
    switch (status) {
    case up:
        for (int i = 0; i < 6; ++i)
            body[i].position = sf::Vector2f(body[i].position.x, body[i].position.y - SPEED_SNAKE);
        break;
    case Status::right:
        for (int i = 0; i < 6; ++i)
            body[i].position = sf::Vector2f(body[i].position.x + SPEED_SNAKE, body[i].position.y);
        break;
    case down:
        for (int i = 0; i < 6; ++i)
            body[i].position = sf::Vector2f(body[i].position.x, body[i].position.y + SPEED_SNAKE);
        break;
    case Status::left:
        for (int i = 0; i < 6; ++i)
            body[i].position = sf::Vector2f(body[i].position.x - SPEED_SNAKE, body[i].position.y);
        break;
    default:
        break;
    }
    flag = true;
}

void Snake::eat() {
    if (body[0].position == food.getPosition()) {
        do {
            food.setFillColor(sf::Color::Yellow);
            food.setRadius(5.f);
            food.setPointCount(4);
            food.setPosition(sf::Vector2f(rand() % 18 * 10.f + 10.f, rand() % 30 * 10.f + 10.f));
        } while (!checkFood());
        body.resize(count + 6);
        count += 6;
        for (int i = count - 6; i < count; ++i) {
            body[i].position = body[i - 6].position;
        }
        for (int i = count - 6; i < count; ++i) {
            body[i].color = sf::Color::Green;
        }
    }
}

void Snake::selfEat() {
    for (int i = count - 6; i >12; i-= 6) {
        if (body[0].position == body[i].position) {
            life = false;

            break;
        }
    }
}

void Snake::gameOver() {
    if (life == false) {
        body.resize(6);
        count = 6;
        setPosition(sf::Vector2f(60.f, 60.f));
        status = stop;
        life = true;
    }
}

void Snake::print() {
    cout << status << endl;
}

bool Snake::checkFood() {
    for (int i = 0; i < count; i += 6)
        if (food.getPosition() == body[i].position)
            return false;
    return true;
}