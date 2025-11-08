#include "../headers/Entities.h"
#include "../headers/Weapons.h"

class Enemy : public Entity
{
protected:
    Enemy(const bool &, const sf::Vector2f &, const bool &, const std::string &, const sf::Texture &, float, const int &, const Weapon &);
    int maxHealth;
    Weapon enemyWeapon;
    int currentHealth;

public:
    static Enemy spawnEnemy(const sf::Texture &, const sf::Vector2f &, float, const int &, const Weapon &);

    void loadEnemy(sf::RenderWindow &, const sf::Texture &);
    void drawEnemy(sf::RenderWindow &);
    void updateEnemy(const float &, const sf::Vector2f &);
    bool takeDamage(const int &);
    Projectile fireEnemyWeapon(const sf::Vector2f &, const sf::Texture &);
    bool canFireEnemyWeapon(const float &);

    Enemy(const Enemy &);
    Enemy &operator=(const Enemy &);
    ~Enemy() = default;
    friend std::ostream &operator<<(std::ostream &os, const Enemy &enemy);
};