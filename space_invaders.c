#include "raylib.h"
#define screen_width 1200
#define screen_height 800
#define max_bullet 100
#define e_max_bull 50
#define num_enemies 10
#define e_size 40

int score = 0;
int high_score = 0;
int life = 3;
int enemy_kill = 0;
int collision_count = 0; // to check the collision between the invisible point tand the wall
int down_timer = 0; // for how long the enemies will go down
int down_distance = screen_height/8;
int down_count = 0;

typedef struct{
    Vector2 position;
    Vector2 Size;
    bool active;
}Bullet;

typedef struct{
    float position;  // the invisible point that will repreent the collision of all enemies with the wall
    bool right;
}point;

typedef struct
{
    Vector2 position;
    Vector2 size;
    bool active;
    bool right;
}Enemy;

typedef struct{
    Vector2 position;
    int size;
}lives;

int main(void){

    Vector2 player = {screen_width/2 -30, 7*screen_height/8 - 35};
    bool player_active = true;
    Vector2 enemy = {screen_width/2-30, screen_height/8};
    Vector2 p_size = {50, 50};

    point lvl1 = {7*e_size + 210, true};

 
    Bullet bullets[max_bullet] = {0};
    Bullet e_bullets[e_max_bull] = {0};
    Enemy enemies[num_enemies] = {0};

    lives heart[life];

    for(int i = 0; i<life; i++){
        heart[i].size = 50;
        heart[i].position.y = screen_height - 60;
        heart[i].position.x = i*heart[i].size + 20;
    }


    bool paused = false;
    bool started = false;
    bool game_active = true;
    bool down = false;

    for(int i = 0; i<num_enemies; i++){
        enemies[i].active = true;
        enemies[i].right = true;
    }
     int x = 7*e_size + 210;
        for(int i = 0; i<num_enemies; i++){
            if(enemies[i].active){
                enemies[i].position.x = x;
                x += e_size + 30;
                enemies[i].position.y = screen_height/10;
                enemies[i].size = (Vector2){e_size,e_size};
            }
        }

    int t60 = 0;
    int countdown = 0;    

    InitWindow(screen_width, screen_height, "SPACE_INVADERS");
    SetTargetFPS(60);

    Texture2D background_texture = LoadTexture("background.png");
    Texture2D enemy_texture = LoadTexture("enemy_alien.png");
    Texture2D player_texture = LoadTexture("player_spaceship.png");
    Texture2D bullet_texture = LoadTexture("player_bullet.png");
    Texture2D enemy_bullet_texture = LoadTexture("enemy_bullet.png");
    Texture2D heart_texture = LoadTexture("heart.png");

    while(!WindowShouldClose()){

        if(!started){
            if(IsKeyPressed(KEY_ENTER)) started = true;
        }
       
        BeginDrawing();

        if(!started){
            ClearBackground(BLACK);
            DrawTexturePro(
                background_texture,
               (Rectangle){0,0,background_texture.width,background_texture.height},
               (Rectangle){0,0,screen_width,screen_height},
               (Vector2){0,0},
               0,
               WHITE
            );
            int length = MeasureText("PRESS ENTER TO START", 30);
            DrawText("PRESS ENTER TO START",(screen_width-length)/2, (screen_height-30)/2, 30, WHITE);
        }else{
            ClearBackground(BLACK);
            DrawTexturePro(
                background_texture,
                (Rectangle){0,0,background_texture.width,background_texture.height},
                (Rectangle){0,0,screen_width,screen_height},
                (Vector2){0,0},
                0,
                WHITE
            );

            if(game_active){

            DrawText(TextFormat("SCORE : %d", score), 20, 20, 20, WHITE);   

            for(int i = 0; i<life; i++){
                DrawTexturePro(
                    heart_texture,
                    (Rectangle){0,0,heart_texture.width, heart_texture.height},
                    (Rectangle){heart[i].position.x, heart[i].position.y, heart[i].size, heart[i].size},
                    (Vector2){0,0},
                    0,
                    WHITE
                );
            }    
                
            if(!paused){
                if(IsKeyPressed(KEY_P)) paused = true;
            } 
            else if(paused){
                if(IsKeyPressed(KEY_P)) paused = false;
            }
            
            int fontsize = 50;
            int textwidth = MeasureText("PAUSED", fontsize);
            if(paused){
                DrawText("PAUSED", 
                    (screen_width-textwidth)/2,
                    (screen_height-fontsize)/2, fontsize, 
                    WHITE);
            }

            for(int i = 0; i<num_enemies; i++){
                if(enemies[i].active){
                    DrawTexturePro(
                        enemy_texture,
                        (Rectangle){0,0,enemy_texture.width, enemy_texture.height},
                        (Rectangle){
                            enemies[i].position.x,
                            enemies[i].position.y,
                            e_size,
                            e_size
                        },
                        (Vector2){0,0},
                        0,
                        WHITE
                    );
                }
            }
            if(!paused){
                if(!down){
                    for(int i = 0; i<num_enemies; i++){
                        if(enemies[i].active){
                            if(enemies[i].right){
                                enemies[i].position.x += 5;
                                if(enemies[i].position.x> screen_width - (num_enemies - i)*(e_size+30)+30){
                                    enemies[i].position.x = screen_width - (num_enemies - i)*(e_size+30)+30;
                                    enemies[i].right = false;
                                }
                            }else{
                            enemies[i].position.x -= 5;
                            if(enemies[i].position.x<(i)*(e_size+30)){
                                enemies[i].position.x = (i)*(e_size+30);
                                enemies[i].right = true;
                                }
                            }
                        }
                    }

                    if(lvl1.right == true){ 
                    lvl1.position += 5;
                    if(lvl1.position > screen_width - (num_enemies)*(e_size+30)+30){
                         lvl1.right = false;
                         lvl1.position = screen_width - (num_enemies)*(e_size+30)+30;
                         collision_count++;
                    }
                    
                    }else{
                        lvl1.position -= 5;
                        if(lvl1.position < 0){
                            lvl1.right = true;
                            lvl1.position = 0;
                            collision_count++;
                        }
                    }

                }
            
                 if(collision_count == 3){
                   if(down_timer <= 10){
                        down = true;
                        down_distance += 4;
                        for(int i = 0; i<num_enemies; i++){
                           if(enemies[i].active) enemies[i].position.y += 4;
                        }
                        down_timer++;
                    }
                    else {
                        down_count++;
                        collision_count = 0;
                        down_timer = 0;
                        down = false;
                    }      
                }

                if(t60%20 == 0){
                    for(int i = 0; i<3; i++){
                        int shooter = GetRandomValue(0, num_enemies-1);
                        if(enemies[shooter].active == true && player_active == true){
                        for(int j = 0; j<e_max_bull; j++){
                            if (!e_bullets[j].active)
                                {       
                                e_bullets[j].active = true;
                                e_bullets[j].position = (Vector2){enemies[shooter].position.x + 17.5, enemies[shooter].position.y + 40};
                                e_bullets[j].Size = (Vector2){20,20};
                                break;
                                } 
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
                    if(e_bullets[j].position.y>screen_height){
                        e_bullets[j].active = false;
                    }
                    if(e_bullets[j].active){
                        DrawTexturePro(
                            enemy_bullet_texture,
                            (Rectangle){0,0, enemy_bullet_texture.width, enemy_bullet_texture.height},
                            (Rectangle){
                                e_bullets[j].position.x,
                                e_bullets[j].position.y,
                                e_bullets[j].Size.x,
                                e_bullets[j].Size.y
                            },
                            (Vector2){0,0},
                            0,
                            WHITE
                        );

                        if(!paused) e_bullets[j].position.y += 7;

                        Rectangle enemy_bullet_rect = {
                            e_bullets[j].position.x,
                            e_bullets[j].position.y,
                            e_bullets[j].Size.x,
                            e_bullets[j].Size.y
                        };
                        if(player_active == true){
                            if(CheckCollisionRecs(playerRect, enemy_bullet_rect)){
                                player_active = false;
                                e_bullets[j].active = false;
                                life--;
                            }
                        }
                    }
                }
            if(paused == false && player_active == false && life>0) countdown++;
            if(countdown == 180){
                player_active = true;
                countdown = 0;
            }    

            if(player_active){              
                DrawTexturePro(
                    player_texture,
                    (Rectangle){0, 0, player_texture.width, player_texture.height},
                    (Rectangle){
                        player.x,
                        player.y,
                        p_size.x,
                        p_size.y
                    },
                    (Vector2){0, 0},
                    0,
                    WHITE
                );
                if(!paused){
                    if(IsKeyDown(KEY_RIGHT)){
                        player.x += 5;
                    }
                    if(IsKeyDown(KEY_LEFT)){
                        player.x -= 5;
                    }
                    if(player.x<0) player.x = 0;
                    if(player.x>screen_width-30) player.x = screen_width - 30; 

                    if(IsKeyPressed(KEY_SPACE)){
                        for(int i = 0; i<max_bullet; i++){
                            if(!bullets[i].active){
                                bullets[i].active = true;
                                bullets[i].position = (Vector2){player.x+12, player.y-2};
                                bullets[i].Size = (Vector2){20,20};

                                break;
                            }
                        }
                    }
                }
            }

            for(int i = 0; i<max_bullet; i++){
                
                if(bullets[i].position.y<0){
                    bullets[i].active = false;
                }

                if(bullets[i].active){
                    if(!paused) bullets[i].position.y -= 7;
                    DrawTexturePro(
                        bullet_texture,
                        (Rectangle){0,0,bullet_texture.width, bullet_texture.height},
                        (Rectangle){bullets[i].position.x, bullets[i].position.y, bullets[i].Size.x, bullets[i].Size.y},
                        (Vector2){0,0},
                        0,
                        WHITE
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
                                enemy_kill++; 
                                if(down_count<=10){
                                    score += 100 - down_count*5;
                                }else{
                                    score += 50;
                                }
                            }  
                        }    
                    }       
                }
            }

            if(enemy_kill == num_enemies){
                score *= 2;
                score += life*200;
            }

            t60 += 1;
        }if(life == 0 || down_distance >= player.y){

            if(score>high_score) high_score = score; 

            game_active = false;
            int length1 = MeasureText("GAME OVER", 40);
            int length2 = MeasureText("PRESS ENTER TO RESTART", 20);
            int length3 = MeasureText(TextFormat("SCORE : %d", score), 25);
            int length4 = MeasureText(TextFormat("HIGH SCORE : %d", high_score),20);

            DrawText("GAME OVER",
                (screen_width-length1)/2,
                (screen_height-120)/2,
                40,
                WHITE
            );
            DrawText(
                TextFormat("SCORE : %d", score),
                (screen_width-length3)/2,
                screen_height/2 - 20,
                25,
                WHITE
            ); 
            DrawText(
                TextFormat("HIGH SCORE : %d", high_score),
                (screen_width-length4)/2,
                screen_height/2 + 10,
                20,
                WHITE
            );       
            DrawText(
                "(PRESS ENTER TO RESTART)",
                (screen_width-length2)/2,
                (screen_height+70)/2,
                20,
                YELLOW
            );        

        }if(enemy_kill == num_enemies){
            
            if(score>high_score) high_score = score; 

            game_active = false;
            int length1 = MeasureText("ENEMIES DESTROYED!", 30);
            int length2 = MeasureText("PRESS ENTER TO RESTART", 20);
            int length3 = MeasureText(TextFormat("SCORE : %d", score), 25);
            int length4 = MeasureText(TextFormat("HIGH SCORE : %d", high_score),20);

            DrawText(
                "ENEMIES DESTROYED!",
                (screen_width-length1)/2,
                (screen_height-110)/2,
                30,
                WHITE
            );
            DrawText(
                TextFormat("SCORE : %d", score),
                (screen_width-length3)/2,
                screen_height/2 - 20,
                25,
                WHITE
            ); 
            DrawText(
                TextFormat("HIGH SCORE : %d", high_score),
                (screen_width-length4)/2,
                screen_height/2 + 10,
                20,
                WHITE
            );       
            DrawText(
                "(PRESS ENTER TO RESTART)",
                (screen_width-length2)/2,
                (screen_height+70)/2,
                20,
                YELLOW
            ); 
            
        }

        if(!game_active){
            if(IsKeyPressed(KEY_ENTER)){
                score = 0;
                game_active = true;
                life = 3;
                enemy_kill = 0;

                player_active = true;
                player = (Vector2){screen_width/2 -30, 7*screen_height/8 - 35};

                for(int i = 0; i<num_enemies; i++){
                   enemies[i].active = true;
                   enemies[i].right = true;
                }
                int x = 7*e_size + 210;
                for(int i = 0; i<num_enemies; i++){
                    if(enemies[i].active){
                        enemies[i].position.x = x;
                        x += e_size + 30;
                        enemies[i].position.y = screen_height/10;
                        enemies[i].size = (Vector2){e_size,e_size};
                    }
                }

                for(int i = 0; i<e_max_bull; i++){
                    e_bullets[i].active = false;
                }

                for(int i = 0; i<max_bullet; i++){
                    bullets[i].active = false;
                }
            }
        }

    }    
        EndDrawing();
    }
    UnloadTexture(player_texture);
    UnloadTexture(bullet_texture);
    UnloadTexture(enemy_bullet_texture);
    UnloadTexture(enemy_texture);
    UnloadTexture(background_texture);
    UnloadTexture(heart_texture);
}