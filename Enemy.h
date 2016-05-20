#ifndef ENEMY_H
#define ENEMY_H


class Enemy
{
    public:
        double x;
        double y;
        double velocity;
        Enemy(double x, double y, double velocity);
        virtual ~Enemy();
    protected:
    private:
};

#endif // ENEMY_H
