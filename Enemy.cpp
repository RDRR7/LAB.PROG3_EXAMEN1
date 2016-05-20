#include "Enemy.h"

Enemy::Enemy(double x, double y, double velocity)
{
    this->x=x;
    this->y=y;
    this->velocity=velocity;
}

Enemy::~Enemy()
{
    //dtor
}
