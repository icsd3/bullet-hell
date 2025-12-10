#include "../headers/Object.h"

Object::Object(const sf::Vector2f &pos, const bool &ori, const sf::Texture &tex)
    : position(pos), orientation(ori), sprite(tex), texture(&tex)
{
}

Object::Object(const sf::Vector2f &pos, const bool &ori, const sf::Vector2f &size)
    : position(pos), orientation(ori), sprite(std::nullopt), texture(std::nullopt)
{
    collisionBox.setPointCount(4);
    collisionBox.setPoint(0, sf::Vector2f(0.f, 0.f));
    collisionBox.setPoint(1, sf::Vector2f(size.x, 0.f));
    collisionBox.setPoint(2, sf::Vector2f(size.x, size.y));
    collisionBox.setPoint(3, sf::Vector2f(0.f, size.y));
    collisionBox.setPosition(position);
}

Object::Object(const Object &other)
    : position(other.position), orientation(other.orientation), sprite(other.sprite), texture(other.texture), collisionBox(other.collisionBox)
{
}

Object &Object::operator=(const Object &other)
{
    if (this != &other)
    {
        position = other.position;
        orientation = other.orientation;
        sprite = other.sprite;
        collisionBox = other.collisionBox;
    }
    return *this;
}

bool Object::collidesWith(const sf::ConvexShape &other) const
{
    sf::Transform collisionBoxTransform = collisionBox.getTransform();
    sf::Transform otherCollisionBoxTransform = other.getTransform();

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
        const sf::ConvexShape& shape = (i == 0 ? collisionBox : other);
        const sf::Transform& transform = (i == 0 ? collisionBoxTransform : otherCollisionBoxTransform);

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

            project(getPoint, collisionBox, collisionBoxTransform, axis, minA, maxA);
            project(getPoint, other, otherCollisionBoxTransform, axis, minB, maxB);

            if (maxA < minB || maxB < minA)
                return 0;
        }
    }

    return 1;
}

bool Object::collidesWith(const Object &other) const
{
    sf::Transform collisionBoxTransform = collisionBox.getTransform();
    sf::Transform otherCollisionBoxTransform = other.collisionBox.getTransform();

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
        const sf::ConvexShape& shape = (i == 0 ? collisionBox : other.collisionBox);
        const sf::Transform& transform = (i == 0 ? collisionBoxTransform : otherCollisionBoxTransform);

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

            project(getPoint, collisionBox, collisionBoxTransform, axis, minA, maxA);
            project(getPoint, other.collisionBox, otherCollisionBoxTransform, axis, minB, maxB);

            if (maxA < minB || maxB < minA)
                return 0;
        }
    }

    return 1;
}

void Object::doDraw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    if (sprite)
    {
        sf::Sprite drawSprite = *sprite;
        drawSprite.setPosition(Utils::mapToScreen(position, window));
        drawSprite.scale(scaleFactor);
        window.draw(drawSprite);
    }

    if (Utils::changeDisplayBoxes(0) == 1)
    {
        sf::ConvexShape drawCollisionBox = collisionBox;
        drawCollisionBox.setPosition(Utils::mapToScreen(collisionBox.getPosition(), window));
        drawCollisionBox.scale(scaleFactor);
        window.draw(drawCollisionBox);
    }
}

void Object::draw(sf::RenderWindow &window)
{
    doDraw(window);
}

void Object::doLoad()
{
    if (sprite)
    {
        float scale = 114.f / static_cast<float>(texture.value()->getSize().x);
        sprite.value().setScale(sf::Vector2f(scale, scale));
        sf::FloatRect bounds = sprite.value().getLocalBounds();
        sprite.value().setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
        sprite.value().setPosition(position);
        sf::Vector2f size(scale * texture.value()->getSize().x, scale * texture.value()->getSize().y);
        collisionBox.setPointCount(4);
        collisionBox.setPoint(0, sf::Vector2f(0.f, 0.f));
        collisionBox.setPoint(1, sf::Vector2f(size.x, 0.f));
        collisionBox.setPoint(2, sf::Vector2f(size.x, size.y));
        collisionBox.setPoint(3, sf::Vector2f(0.f, size.y));
        
        collisionBox.setOrigin(sf::Vector2f(size.x / 2.f, size.y / 2.f));
        collisionBox.setPosition(position);
        
        if (orientation)
        {
            sprite.value().setScale(sf::Vector2f(-std::abs(sprite.value().getScale().x), sprite.value().getScale().y));
        }
        else
        {
            sprite.value().setScale(sf::Vector2f(std::abs(sprite.value().getScale().x), sprite.value().getScale().y));
        }
    }

    collisionBox.setFillColor(sf::Color(200, 0, 0, 150));
}

void Object::load()
{
    doLoad();
}

std::ostream &operator<<(std::ostream &os, const Object &object)
{
    os << "Object (Position: (" << object.position.x << ", " << object.position.y << "), Orientation: " << (object.orientation ? "right" : "left") << ")";
    return os;
}