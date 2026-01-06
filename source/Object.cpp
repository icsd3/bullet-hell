#include "../headers/Object.h"

Object::Object(const sf::Vector2f &pos, const sf::Texture &tex)
    : Collider(pos), texture(&tex), sprite(tex)
{
}

Object::Object(const Object &other)
    : Collider(other), texture(other.texture), sprite(other.sprite)
{
}

Object &Object::operator=(const Object &other)
{
    if (this != &other)
    {
        Collider::operator=(other);
        texture = other.texture;
        sprite = other.sprite;
    }
    return *this;
}

void Object::load(const ObjectLoadParams &params)
{
    sf::Vector2f scale;

    if (params.scaleFactor.y == 0)
        scale = {params.scaleFactor.x / static_cast<float>(texture->getSize().x), params.scaleFactor.x / static_cast<float>(texture->getSize().x)};

    else
        scale = {params.scaleFactor.x / static_cast<float>(texture->getSize().x), params.scaleFactor.y / static_cast<float>(texture->getSize().y)};
    sprite.setScale(scale);

    sf::Vector2f size(scale.x * params.collisionBoxSizeFactor.x * texture->getSize().x, scale.y * params.collisionBoxSizeFactor.y * texture->getSize().y);

    Collider::load(size);

    collisionBox.setOrigin(sf::Vector2f(size.x * params.collisionBoxOriginFactor.x, size.y * params.collisionBoxOriginFactor.y));
    collisionBox.setPosition(sf::Vector2f(position.x + scale.x * params.collisionBoxPositionFactor.x * texture->getSize().x, position.y + scale.y * params.collisionBoxPositionFactor.y * texture->getSize().y));

    sf::FloatRect bounds = sprite.getLocalBounds();

    sprite.setOrigin(sf::Vector2f(bounds.size.x * params.spriteOriginFactor.x, bounds.size.y * params.spriteOriginFactor.y));
    sprite.setPosition(position);
}

void Object::doDraw(sf::RenderWindow &window) const
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(Utils::mapToScreen(position, window));
    drawSprite.scale(scaleFactor);
    window.draw(drawSprite);

    Collider::doDraw(window);
}

std::ostream &operator<<(std::ostream &os, const Object &object)
{
    os << "Object: (" << static_cast<const Collider &>(object) << ")";
    return os;
}