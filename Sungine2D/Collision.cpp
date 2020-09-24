#include "Collision.h"

bool Collision::CheckCollision(Entity& e1, Entity& e2)
{
    //X-axis
    bool collisionX = e1.GetPosition().x + e1.GetSize().x >= e2.GetPosition().x &&
        e2.GetPosition().x + e2.GetSize().x >= e1.GetPosition().x;

    //Y-axis
    bool collisionY = e1.GetPosition().y + e1.GetSize().y >= e2.GetPosition().y &&
        e2.GetPosition().y + e2.GetSize().y >= e1.GetPosition().y;

    //Collides only on both axes.
    return collisionX && collisionY;
}
