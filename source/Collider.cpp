#include "../headers/Collider.h"

Collider::Collider(const sf::Vector2f &pos)
    : position(pos)
{
}

Collider::Collider(const Collider &other)
    : position(other.position), collisionBox(other.collisionBox)
{
}

Collider &Collider::operator=(const Collider &other)
{
    if (this != &other)
    {
        position = other.position;
        collisionBox = other.collisionBox;
    }
    return *this;
}

void Collider::load(const sf::Vector2f &size)
{
    collisionBox.setPointCount(4);
    collisionBox.setPoint(0, sf::Vector2f(0.f, 0.f));
    collisionBox.setPoint(1, sf::Vector2f(size.x, 0.f));
    collisionBox.setPoint(2, sf::Vector2f(size.x, size.y));
    collisionBox.setPoint(3, sf::Vector2f(0.f, size.y));
    collisionBox.setPosition(position);
    collisionBox.setFillColor(sf::Color(200, 0, 0, 150));
}

void Collider::doDraw(sf::RenderWindow &window)
{
    if (Utils::changeDisplayBoxes(0) == 1)
    {
        sf::Vector2f scaleFactor = Utils::getScaleFactor(window);
        sf::ConvexShape drawCollisionBox = collisionBox;
        drawCollisionBox.setPosition(Utils::mapToScreen(collisionBox.getPosition(), window));
        drawCollisionBox.scale(scaleFactor);
        window.draw(drawCollisionBox);
    }
}

void Collider::draw(sf::RenderWindow &window)
{
    doDraw(window);
}

bool Collider::collidesWith(const sf::ConvexShape &other) const
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

bool Collider::collidesWith(const Collider &other) const
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

std::ostream &operator<<(std::ostream &os, const Collider &collider)
{
    os << "Position: (" << collider.position.x << ", " << collider.position.y << "), Size: ("
       << collider.collisionBox.getPoint(1).x - collider.collisionBox.getPoint(0).x << ", "
       << collider.collisionBox.getPoint(3).y - collider.collisionBox.getPoint(0).y << ")";
    return os;
}