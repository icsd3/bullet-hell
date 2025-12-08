#include "../headers/Entities.h"
#include <iostream>

Entity::Entity(const sf::Vector2f &pos, const bool &ori, sf::Texture &tex, const float spd, const int mh)
    : Object(pos, ori, tex), speed(spd), maxHealth(mh), currentHealth(mh)
{
}

Entity::Entity(const Entity &other)
    : Object(other), speed(other.speed), maxHealth(other.maxHealth), currentHealth(other.currentHealth), hitBox(other.hitBox)
{
}

Entity &Entity::operator=(const Entity &other)
{
    if (this != &other)
    {
        Object::operator=(other);
        speed = other.speed;
        maxHealth = other.maxHealth;
        currentHealth = other.currentHealth;
        hitBox = other.hitBox;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Entity &entity)
{
    os << "Entity (";
    os << static_cast<const Object &>(entity);
    os << ", Speed: " << entity.speed << ")";
    return os;
}
 
void Entity::doLoad()
{
    Object::doLoad();

    float scale = 60.f / static_cast<float>(texture.value()->getSize().x);
    sprite.value().setScale(sf::Vector2f(scale, scale));
    sf::Vector2f size(scale * 0.7f * texture.value()->getSize().x, scale * 0.6f * texture.value()->getSize().y);
    collisionBox.setPointCount(4);
    collisionBox.setPoint(0, sf::Vector2f(0.f, 0.f));
    collisionBox.setPoint(1, sf::Vector2f(size.x, 0.f));
    collisionBox.setPoint(2, sf::Vector2f(size.x, size.y));
    collisionBox.setPoint(3, sf::Vector2f(0.f, size.y));

    collisionBox.setOrigin(sf::Vector2f(size.x / 2.f, size.y));
    collisionBox.setPosition(sf::Vector2f(position.x, position.y + scale * 0.5f * texture.value()->getSize().x));

    sf::FloatRect bounds = sprite.value().getLocalBounds();
    sf::Transform t = sprite.value().getTransform();
    hitBox.setFillColor(sf::Color(0, 200, 0, 150));
    hitBox.setPointCount(6);
    hitBox.setPoint(0, t.transformPoint({bounds.position.x + 4.5f / 14 * bounds.size.x, bounds.position.y}));
    hitBox.setPoint(1, t.transformPoint({bounds.position.x + 9.5f / 14 * bounds.size.x, bounds.position.y}));
    hitBox.setPoint(2, t.transformPoint({bounds.position.x + bounds.size.x, bounds.position.y + 4.5f / 14 * bounds.size.y}));
    hitBox.setPoint(3, t.transformPoint({bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y}));
    hitBox.setPoint(4, t.transformPoint({bounds.position.x, bounds.position.y + bounds.size.y}));
    hitBox.setPoint(5, t.transformPoint({bounds.position.x, bounds.position.y + 4.5f / 14 * bounds.size.y}));
}

void Entity::doDraw(sf::RenderWindow &window)
{
    Object::doDraw(window);

    if (Utils::changeDisplayBoxes(0) == 2)
    {
        sf::Vector2f scaleFactor = Utils::getScaleFactor(window);
        sf::ConvexShape drawHitBox = hitBox;
        drawHitBox.setPosition(Utils::mapToScreen(hitBox.getPosition(), window));
        drawHitBox.scale(scaleFactor);
        window.draw(drawHitBox);
    }
}

bool Entity::doTakeDamage(const int &dmg)
{
    currentHealth -= dmg;
    if (currentHealth <= 0)
        return true;
    return false;
}

bool Entity::takeDamage(const int &dmg)
{
    return doTakeDamage(dmg);
}

int Entity::doHits(const Entity &other) const
{
    sf::Transform hitBoxTransform = hitBox.getTransform();
    sf::Transform otherHitBoxTransform = other.hitBox.getTransform();

    auto getPoint = [](const sf::ConvexShape& s, const sf::Transform& t, size_t i) 
    {
        return t.transformPoint(s.getPoint(i));
    };

    auto project = [](const auto& getPointFunc, const sf::ConvexShape& s, const sf::Transform& t, const sf::Vector2f& axis, float& min, float& max)
    {
        sf::Vector2f p0 = getPointFunc(s, t, 0);
        min = max = p0.x * axis.x + p0.y * axis.y;

        for (size_t i = 1; i < s.getPointCount(); ++i) 
        {
            sf::Vector2f p = getPointFunc(s, t, i);
            float proj = p.x * axis.x + p.y * axis.y;
            if (proj < min) min = proj;
            if (proj > max) max = proj;
        }
    };

    for (int i = 0; i < 2; ++i)
    {
        const sf::ConvexShape& shape = (i == 0 ? hitBox : other.hitBox);
        const sf::Transform& transform = (i == 0 ? hitBoxTransform : otherHitBoxTransform);

        for (size_t j = 0; j < shape.getPointCount(); j++)
        {
            sf::Vector2f currentPoint = getPoint(shape, transform, j);
            sf::Vector2f nextPoint = getPoint(shape, transform, (j + 1) % shape.getPointCount());

            sf::Vector2f edge = nextPoint - currentPoint;
            sf::Vector2f axis(-edge.y, edge.x);

            float len = std::sqrt(axis.x * axis.x + axis.y * axis.y);

            if (!len) 
                continue;

            axis.x /= len;
            axis.y /= len;

            float minA, maxA, minB, maxB;

            project(getPoint, hitBox, hitBoxTransform, axis, minA, maxA);
            project(getPoint, other.hitBox, otherHitBoxTransform, axis, minB, maxB);

            if (maxA < minB || maxB < minA)
                return 0;
        }
    }

    return 1;
}

int Entity::hits(const Entity &other) const
{
    return doHits(other);
}