
#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<list>
#include <math.h>
#include <stdlib.h>
#include "Bullet.h"
#include "Enemy.h"

#define PI 3.14159265

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background,*character, *bullet, *enemy, *enemy_1, *enemy_2;
SDL_Rect rect_background,rect_character, rect_bullet, rect_enemy;

int screen_width = 500;
int screen_height= 250;

double getAngle(double distance_x, double distance_y)
{
    double angle = atan (distance_y/distance_x) * 180 / PI;
    if(distance_x<0)
    {
        angle=180+angle;
    }
    if(distance_x>0 && distance_y<0)
    {
        angle=360+angle;
    }
    return angle;
}

bool isOutOfBounds(int x, int y)
{
    return x>screen_width || x<0 || y>screen_height || y<0;
}

bool collisionCheck(SDL_Rect r1, SDL_Rect r2)
{
    if(r1.x > r2.x+r2.w)//Muy a la derecha
        return false;
    if(r1.x+r1.w < r2.x)//Muy a la izquierda
        return false;
    if(r1.y > r2.y+r2.h)//Muy abajo
        return false;
    if(r1.y+r1.h < r2.y)//Muy arriba
        return false;
    return true;
}

int main( int argc, char* args[] )
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, screen_width/*WIDTH*/, screen_height/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    //Init textures
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"fondo.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;

    character = IMG_LoadTexture(renderer, "personaje.png");
    SDL_QueryTexture(character, NULL, NULL, &w, &h);
    rect_character.x = 218;
    rect_character.y = 93;
    rect_character.w = w;
    rect_character.h = h;

    bullet = IMG_LoadTexture(renderer, "bullet.png");
    SDL_QueryTexture(bullet, NULL, NULL, &w, &h);
    rect_bullet.x = 0;
    rect_bullet.y = 100;
    rect_bullet.w = w;
    rect_bullet.h = h;

    enemy = IMG_LoadTexture(renderer, "enemy.png");
    SDL_QueryTexture(enemy, NULL, NULL, &w, &h);
    enemy_1 = IMG_LoadTexture(renderer, "enemy_1.png");
    SDL_QueryTexture(enemy_1, NULL, NULL, &w, &h);
    enemy_2 = IMG_LoadTexture(renderer, "enemy_2.png");
    SDL_QueryTexture(enemy_2, NULL, NULL, &w, &h);
    rect_enemy.x = 0;
    rect_enemy.y = 100;
    rect_enemy.w = w;
    rect_enemy.h = h;

    list<Bullet*>bullets;
    list<Enemy*>enemies;
    int frame=0;
    int lifes=3;
    int enemiesKilled=0;

    //Main Loop
    double player_velocity=3;
    double bullet_velocity=8;
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return 0;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        if( currentKeyStates[ SDL_SCANCODE_UP ] )
        {
            rect_character.y-=player_velocity;
        }
        if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
        {
            rect_character.y+=player_velocity;
        }
        if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
        {
            rect_character.x+=player_velocity;
        }
        if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
        {
            rect_character.x-=player_velocity;
        }

        int mouse_x, mouse_y;
        SDL_GetMouseState( &mouse_x, &mouse_y );

        if(frame%30==0)
        {
            double dist_x = mouse_x - rect_character.x;
            double dist_y = rect_character.y - mouse_y;
            bullets.push_back(new Bullet(rect_character.x,rect_character.y,getAngle(dist_x,dist_y)));
        }

        if(frame%150==0)
        {
            enemies.push_back(new Enemy(0,rand()%(250-rect_enemy.h), rand()%3+1));
        }

        SDL_RenderCopy(renderer, background, NULL, &rect_background);

        for(list<Bullet*>::iterator i=bullets.begin();
            i!=bullets.end();
            i++)
        {
            Bullet* bullet_actual = *i;
            rect_bullet.x=bullet_actual->x;
            rect_bullet.y=bullet_actual->y;
            SDL_RenderCopy(renderer, bullet, NULL, &rect_bullet);
            bullet_actual->x+=cos(-bullet_actual->angle*PI/180)*bullet_velocity;
            bullet_actual->y+=sin(-bullet_actual->angle*PI/180)*bullet_velocity;

            if(isOutOfBounds(bullet_actual->x,bullet_actual->y))
            {
                list<Bullet*>::iterator to_erase=i;
                i--;
                bullets.erase(to_erase);
                delete bullet_actual;
            }
        }

        for(list<Enemy*>::iterator i=enemies.begin();
            i!=enemies.end();
            i++)
        {
            Enemy* current_enemy = *i;
            rect_enemy.x=current_enemy->x;
            rect_enemy.y=current_enemy->y;
            //current_enemy->x+=current_enemy->velocity;
            double e_dist_x = rect_character.x - rect_enemy.x;
            double e_dist_y = rect_enemy.y - rect_character.y;
            current_enemy->x+=cos(-getAngle(e_dist_x, e_dist_y)*PI/180)*current_enemy->velocity;
            current_enemy->y+=sin(-getAngle(e_dist_x, e_dist_y)*PI/180)*current_enemy->velocity;
            if(current_enemy->velocity==1){
                SDL_RenderCopy(renderer, enemy, NULL, &rect_enemy);
            }
            else if(current_enemy->velocity==2){
                SDL_RenderCopy(renderer, enemy_1, NULL, &rect_enemy);
            }
            else{
                SDL_RenderCopy(renderer, enemy_2, NULL, &rect_enemy);
            }

            if(isOutOfBounds(current_enemy->x,current_enemy->y))
            {
                list<Enemy*>::iterator to_erase=i;
                i--;
                enemies.erase(to_erase);
                delete current_enemy;
            }
        }

        SDL_RenderCopy(renderer, character, NULL, &rect_character);

        for(list<Enemy*>::iterator i=enemies.begin();
            i!=enemies.end();
            i++)
        {
            Enemy*current_enemy = *i;
            for(list<Bullet*>::iterator j=bullets.begin();
                j!=bullets.end();
                j++)
            {
                Bullet*current_bullet = *j;

                rect_enemy.x = current_enemy->x;
                rect_enemy.y = current_enemy->y;

                rect_bullet.x = current_bullet->x;
                rect_bullet.y = current_bullet->y;

                if(collisionCheck(rect_enemy,rect_bullet))
                {
                    enemiesKilled++;
                    cout<<"Enemies killed: "<<enemiesKilled<<endl;
                    if(enemiesKilled%10==0){
                        lifes++;
                        cout<<"Extra life"<<endl;
                    }
                    list<Bullet*>::iterator to_erase=j;
                    j--;
                    bullets.erase(to_erase);
                    delete current_bullet;

                    list<Enemy*>::iterator to_erase_enemy=i;
                    i--;
                    enemies.erase(to_erase_enemy);
                    delete current_enemy;
                    break;
                }
            }
            if(collisionCheck(rect_enemy, rect_character)){
                lifes--;
                list<Enemy*>::iterator to_erase_enemy=i;
                i--;
                enemies.erase(to_erase_enemy);
                delete current_enemy;
                cout<<"Lifes: "<<lifes+1<<endl;
                if(lifes<0){
                    cout<<"Thanks for playing"<<endl;
                    exit(0);
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(17);
        frame++;
    }

	return 0;
}
