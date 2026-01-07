#pragma once

#include "../headers/Enemy.h"
#include "../headers/Projectile.h"
#include "../headers/Player.h"
#include "../headers/Door.h"

class Room
{
protected:
    Player &player;

    sf::Sprite backgroundSprite;

    std::vector<std::unique_ptr<Collider>> walls;
    std::vector<std::unique_ptr<Door>> doors;
    const sf::Texture *doorVertical;
    const sf::Texture *doorHorizontal;
    std::weak_ptr<Room> up;
    std::weak_ptr<Room> right;
    std::weak_ptr<Room> down;
    std::weak_ptr<Room> left;
    sf::Clock animationClock;
    bool isOpen = false;
    std::vector<std::pair<std::unique_ptr<Attack>, bool>> playerAttacks; // bool indicates that the attack is melee for projectile interactions
    int grid[GRID_SIZE_X][GRID_SIZE_Y];

    void animate(const unsigned int &);

    virtual void doLoad(std::weak_ptr<Room>, std::weak_ptr<Room>, std::weak_ptr<Room>, std::weak_ptr<Room>);
    virtual void doStart();
    virtual void doDraw(sf::RenderWindow &);
    virtual std::pair<int, std::weak_ptr<Room>> doUpdate(const float &);
    virtual int doCheckPlayerCollisions();
    virtual bool doCheckEntityCollisions(const Entity &);
    virtual void printDetails(std::ostream &) const;

public:
    explicit Room(Player &);
    Room(const Room &) = delete;
    Room &operator=(const Room &) = delete;
    virtual ~Room() = default;
    friend std::ostream &operator<<(std::ostream &, const Room &);

    void spawnPlayerProjectile(const sf::Vector2f &);
    void load(std::weak_ptr<Room>, std::weak_ptr<Room>, std::weak_ptr<Room>, std::weak_ptr<Room>);
    void start();
    void draw(sf::RenderWindow &);
    std::pair<int, std::weak_ptr<Room>> update(const float &);
    int checkPlayerCollisions();
    bool checkEntityCollisions(const Entity &);
    virtual bool isCleared() const;
};