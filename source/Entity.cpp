#include "../headers/Entity.h"

Entity::Entity(const sf::Vector2f &pos, sf::Texture &tex, const float spd, const int mh)
    : Object(pos, tex), speed(spd), maxHealth(mh), currentHealth(mh)
{
}

Entity::Entity(const Entity &other)
    : Object(other), hitBox(other.hitBox), speed(other.speed), maxHealth(other.maxHealth), currentHealth(other.currentHealth)
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

void Entity::load(const float &scaleFactor, const sf::Vector2f &sizeFactor, const sf::Vector2f &originFactor, const sf::Vector2f &positionFactor, const int &pointCount, const std::vector<sf::Vector2f> &pointFactors)
{
    Object::load(scaleFactor, sizeFactor, originFactor, positionFactor);

    sf::FloatRect bounds = sprite.getLocalBounds();
    hitBox.setFillColor(sf::Color(0, 200, 0, 150));

    hitBox.setPointCount(pointCount);
    for (int i = 0; i < pointCount; i++)
    {
        hitBox.setPoint(i, {bounds.position.x + pointFactors[i].x * bounds.size.x, bounds.position.y + pointFactors[i].y * bounds.size.y});
    }

    hitBox.setOrigin(sprite.getOrigin());
    hitBox.setPosition(sprite.getPosition());
    hitBox.setScale(sprite.getScale());
}

void Entity::doDraw(sf::RenderWindow &window) const
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

void Entity::transform(const sf::Vector2f &movement, const float &threshold, const sf::Angle &angle)
{
    sprite.move(movement);
    sprite.rotate(angle);
    collisionBox.move(movement);
    collisionBox.rotate(angle);
    hitBox.move(movement);
    hitBox.rotate(angle);

    if (movement.x > threshold)
    {
        sprite.setScale(sf::Vector2f(-std::abs(sprite.getScale().x), sprite.getScale().y));
    }
    else if (movement.x < -threshold)
    {
        sprite.setScale(sf::Vector2f(std::abs(sprite.getScale().x), sprite.getScale().y));
    }

    position = sprite.getPosition();
}

bool Entity::doTakeDamage(const int &dmg)
{
    currentHealth -= dmg;
    if (currentHealth <= 0)
    {
        currentHealth = 0;
        return true;
    }
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

    auto getPoint = [](const sf::ConvexShape &s, const sf::Transform &t, size_t i)
    {
        return t.transformPoint(s.getPoint(i));
    };

    auto project = [](const auto &getPointFunc, const sf::ConvexShape &s, const sf::Transform &t, const sf::Vector2f &axis, float &min, float &max)
    {
        sf::Vector2f p0 = getPointFunc(s, t, 0);
        min = max = p0.x * axis.x + p0.y * axis.y;

        for (size_t i = 1; i < s.getPointCount(); ++i)
        {
            sf::Vector2f p = getPointFunc(s, t, i);
            float proj = p.x * axis.x + p.y * axis.y;
            if (proj < min)
                min = proj;
            if (proj > max)
                max = proj;
        }
    };

    for (int i = 0; i < 2; ++i)
    {
        const sf::ConvexShape &shape = (i == 0 ? hitBox : other.hitBox);
        const sf::Transform &transform = (i == 0 ? hitBoxTransform : otherHitBoxTransform);

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

std::ostream &operator<<(std::ostream &os, const Entity &entity)
{
    os << "Entity (";
    os << static_cast<const Object &>(entity);
    os << ", Speed: " << entity.speed;
    os << ", Max Health: " << entity.maxHealth;
    os << ", Current Health: " << entity.currentHealth << ")";
    return os;
}