#include "raylib.h"
#define width 1200
#define height 800
#define max_bullet 100
#define e_max_bull 50
#define num_enemies 10
#define e_size 40

typedef struct{
    Vector2 position;
    Vector2 Size;
    float speed;
    bool active;
}Bullet;

typedef struct
{
    Vector2 position;
    Vector2 size;
    bool active;
    bool right;
}Enemy;

int main(void){

    Vector2 player = {width/2 -30, 7*height/8 - 35};
    bool player_active = true;
    Vector2 enemy = {width/2-30, height/8};
    Vector2 p_size = {30, 30};
 
    Bullet bullets[max_bullet] = {0};
    Bullet e_bullets[e_max_bull] = {0};
    Enemy enemies[num_enemies] = {0};

    for(int i = 0; i<num_enemies; i++){
        enemies[i].active = true;
        enemies[i].right = true;
    }
     int x = 7*e_size + 210;
        for(int i = 0; i<num_enemies; i++){
            if(enemies[i].active){
                enemies[i].position.x = x;
                x += e_size + 30;
                enemies[i].position.y = height/10;
                enemies[i].size = (Vector2){e_size,e_size};
            }
        }

    int t60 = 0;    

    InitWindow(width, height, "SPACE_INVADERS");
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for(int i = 0; i<num_enemies; i++){
            if(enemies[i].active){
                DrawRectangleV(enemies[i].position,enemies[i].size, RED);
            }
        }

        for(int i = 0; i<num_enemies; i++){
            if(enemies[i].active){
                if(enemies[i].right){
                    enemies[i].position.x += 10;
                    if(enemies[i].position.x> width - (num_enemies - i)*(e_size+30)+30){
                        enemies[i].position.x = width - (num_enemies - i)*(e_size+30)+30;
                        enemies[i].right = false;
                    }
                }else{
                  enemies[i].position.x -= 10;
                  if(enemies[i].position.x<(i)*(e_size+30)){
                      enemies[i].position.x = (i)*(e_size+30);
                      enemies[i].right = true;
                    }
                }
            }
        }

        if(t60%20 == 0){
            for(int i = 0; i<3; i++){
                int shooter = GetRandomValue(0, num_enemies-1);
                if(enemies[shooter].active){
                for(int j = 0; j<e_max_bull; j++){
                       if (!e_bullets[j].active)
                        {       
                        e_bullets[j].active = true;
                        e_bullets[j].position = (Vector2){enemies[shooter].position.x + 17.5, enemies[shooter].position.y + 40};
                        e_bullets[j].Size = (Vector2){5,10};
                        break;
                        } 
                    }
                }   
            }
        }

        Rectangle playerRect = {
            player.x,
            player.y,
            p_size.x,
            p_size.y
        };

        for(int j = 0; j<e_max_bull; j++){
                if(e_bullets[j].position.y>height){
                    e_bullets[j].active = false;
                }
                if(e_bullets[j].active){
                    DrawRectangleV(
                       e_bullets[j].position,
                       e_bullets[j].Size,
                       VIOLET
                    );
                    e_bullets[j].position.y += 7;

                    Rectangle enemy_bullet_rect = {
                        e_bullets[j].position.x,
                        e_bullets[j].position.y,
                        e_bullets[j].Size.x,
                        e_bullets[j].Size.y
                    };

                    if(CheckCollisionRecs(playerRect, enemy_bullet_rect)){
                        player_active = false;
                        e_bullets[j].active = false;
                    }
                }
            }

        if(player_active){       
            DrawRectangleV(player, p_size, BLACK);
            if(IsKeyDown(KEY_RIGHT)){
                player.x += 5;
            }
            if(IsKeyDown(KEY_LEFT)){
                player.x -= 5;
            }
            if(player.x<0) player.x = 0;
            if(player.x>width-30) player.x = width - 30; 

            if(IsKeyPressed(KEY_SPACE)){
                for(int i = 0; i<max_bullet; i++){
                    if(!bullets[i].active){
                        bullets[i].active = true;
                        bullets[i].position = (Vector2){player.x+12, player.y-2};
                        bullets[i].Size = (Vector2){5,10};

                        break;
                    }
                }
            }
        }

        for(int i = 0; i<max_bullet; i++){
            
            if(bullets[i].position.y<0){
                bullets[i].active = false;
            }

            if(bullets[i].active){
                bullets[i].position.y -= 7;
                DrawRectangleV(
                        bullets[i].position,
                        bullets[i].Size,
                        VIOLET
                        );
            
                Rectangle bulletRect = {
                        bullets[i].position.x,
                        bullets[i].position.y,
                        bullets[i].Size.x,
                        bullets[i].Size.y
                    };

                for(int j = 0; j<num_enemies;j++){
                     Rectangle enemyRect = {
                            enemies[j].position.x,
                            enemies[j].position.y,
                            e_size,  
                            e_size
                        }; 

                      if(enemies[j].active){
                        if(CheckCollisionRecs(enemyRect, bulletRect)){
                            enemies[j].active = false;
                            bullets[i].active = false;
                        }  
                    }    
                }       
            }
        }
        t60 += 1;
        EndDrawing();
    }
} 