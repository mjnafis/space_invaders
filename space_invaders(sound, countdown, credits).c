#include "raylib.h"
#define screen_width 1200
#define screen_height 800
#define max_bullet 100
#define e_max_bull 50
#define num_enemies 10
#define e_size 40

int score = 0;
int high_score[3] = {0};
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

int level = 1;
int current_level = 0;
int selection = 1;

void DrawMenu(int selection){

    int titleSize = 40;
    int titleWidth = MeasureText("SPACE IBVADERS", titleSize);
    DrawText("SPACE INVADERS", (screen_width-titleWidth)/2, 60, titleSize, WHITE);

    char* menu_items[6] = {"START GAME", "LEADERBOARD", "MANUAL", "ABOUT US", "CREDITS", "EXIT"};

    int boxWidth = 340;
    int boxHeight = 90;
    int gap = 25;
    int totalHeight = 6*boxHeight + 5*gap;
    int startY = (screen_height - totalHeight)/2 + 40;
    int boxX = (screen_width - boxWidth)/2;

    for(int i = 0; i<6; i++){
        int boxY = startY + i*(boxHeight + gap);
        bool isSelected = (selection == i+1);

        Rectangle box = {boxX, boxY, boxWidth, boxHeight};
        if(isSelected){
            DrawRectangleRec(box, GRAY);
        }else{
            DrawRectangleRec(box, WHITE);
        }

        int txtSize = 30;
        int txtWidth = MeasureText(menu_items[i], txtSize);
        DrawText(menu_items[i], boxX + (boxWidth-txtWidth)/2, boxY + 30, txtSize, BLACK);

    }

}

void DrawLevelMenu(int level){

    int titleSize = 60;
    int titleWidth = MeasureText("SPACE INVADERS", titleSize);
    DrawText("SPACE INVADERS", (screen_width - titleWidth)/2, 110, titleSize, WHITE);

    int subSize = 24;
    int subWidth = MeasureText("SELECT DIFFICULTY", subSize);
    DrawText("SELECT DIFFICULTY", (screen_width - subWidth)/2, 195, subSize, GRAY);

    char* levels[3] = {"EASY", "MEDIUM", "HARD"};

    int boxWidth = 340;
    int boxHeight = 90;
    int gap = 25;
    int totalHeight = 3*boxHeight + 2*gap;
    int startY = (screen_height - totalHeight)/2 + 40;
    int boxX = (screen_width - boxWidth)/2;

    for(int i = 0; i<3; i++){
        int boxY = startY + i*(boxHeight + gap);
        bool isSelected = (level == i+1);

        Rectangle box = {boxX, boxY, boxWidth, boxHeight};

        if(isSelected){
            DrawRectangleRec(box, GRAY);
        }else{
            DrawRectangleRec(box, WHITE);
        }

        int labelSize = 30;
        int labelWidth = MeasureText(levels[i], labelSize);
        DrawText(levels[i], boxX + (boxWidth-labelWidth)/2, boxY + 30, labelSize, BLACK);

    }
    int hintSize = 18;
    int hintWidth = MeasureText("UP / DOWN TO CHOOSE   -   ENTER TO CONFIRM", hintSize);
    DrawText("UP / DOWN TO CHOOSE   -   ENTER TO CONFIRM", (screen_width - hintWidth)/2, startY + totalHeight + 50, hintSize, LIGHTGRAY);
}

void DrawManual(void){

    int titleSize = 50;
    int titleWidth = MeasureText("HOW TO PLAY", titleSize);
    DrawText("HOW TO PLAY", (screen_width-titleWidth)/2, 60, titleSize, WHITE);

    int sectionSize = 22;
    int rowSize = 22;
    int rowGap = 42;
    int sectionGap = 60;
    int leftColX = screen_width/2 - 260;
    int rightColX = screen_width/2 - 60;

    int y = 170;

    DrawText("CONTROLS", leftColX, y, sectionSize, GRAY);
    y += 40;

    DrawText("LEFT / RIGHT", leftColX, y, rowSize, WHITE);
    DrawText("Move your ship", rightColX, y, rowSize, LIGHTGRAY);
    y += rowGap;
    DrawText("SPACE", leftColX, y, rowSize, WHITE);
    DrawText("Fire a bullet", rightColX, y, rowSize, LIGHTGRAY);
    y += rowGap;
    DrawText("P", leftColX, y, rowSize, WHITE);
    DrawText("Pause / unpause", rightColX, y, rowSize, LIGHTGRAY);
    y += sectionGap;

    DrawText("OBJECTIVE", leftColX, y, sectionSize, GRAY);
    y += 40;
    DrawText("Destroy every alien before they reach your ship", leftColX, y, rowSize, WHITE);
    y += rowGap;
    DrawText("Avoid enemy fire - you only have 3 lives", leftColX, y, rowSize, WHITE);
    y += rowGap;
    DrawText("Survive as the aliens march ever closer", leftColX, y, rowSize, WHITE);
    y += sectionGap;

    DrawText("SCORING", leftColX, y, sectionSize, GRAY);
    y += 40;
    DrawText("Each kill", leftColX, y, rowSize, WHITE);
    DrawText("Up to 100 pts (less the longer aliens advance)", rightColX, y, rowSize, LIGHTGRAY);
    y += rowGap;

    DrawText("Clear bonus", leftColX, y, rowSize, WHITE);
    DrawText("Score x2, plus 200 pts per life remaining", rightColX, y, rowSize, LIGHTGRAY);

    int hintSize = 18;
    int hintWidth = MeasureText("PRESS BACKSPACE TO RETURN", hintSize);
    DrawText("PRESS BACKSPACE TO RETURN", (screen_width-hintWidth)/2, screen_height - 60, hintSize, YELLOW);
}

void DrawAboutUs(Texture2D nafis, Texture2D jabir)
{
    int titleSize = 50;
    int titleWidth = MeasureText("ABOUT US", titleSize);

    DrawText("ABOUT US",
             (screen_width - titleWidth) / 2,
             60,
             titleSize,
             WHITE);

    int imageWidth = 250;
    int imageHeight = 300;

    int person1X = 180;
    int person2X = 770;
    int imageY = 180;

    DrawTexturePro(
        nafis,
        (Rectangle){0, 0, nafis.width, nafis.height},
        (Rectangle){person1X, imageY, imageWidth, imageHeight},
        (Vector2){0, 0},
        0,
        WHITE
    );

    DrawTexturePro(
        jabir,
        (Rectangle){0, 0, nafis.width, nafis.height},
        (Rectangle){person2X, imageY, imageWidth, imageHeight},
        (Vector2){0, 0},
        0,
        WHITE
    );

    DrawText("Misbahul Jannat Nafis",
             person1X,
             imageY + imageHeight + 25,
             25,
             WHITE);

    DrawText("Musab Jabir",
             person2X,
             imageY + imageHeight + 25,
             25,
             WHITE);

    DrawText("ROLL: 2505136",
             person1X,
             imageY + imageHeight + 60,
             20,
             LIGHTGRAY);

    DrawText("ROLL: 2505137",
             person2X,
             imageY + imageHeight + 60,
             20,
             LIGHTGRAY);

    int hintSize = 18;
    int hintWidth = MeasureText("PRESS BACKSPACE TO RETURN", hintSize);

    DrawText("PRESS BACKSPACE TO RETURN",
             (screen_width - hintWidth) / 2,
             screen_height - 60,
             hintSize,
             YELLOW);
}

void DrawCredits(void){

    int titleSize = 50;
    int titleWidth = MeasureText("CREDITS", titleSize);

    DrawText(
        "CREDITS",
        (screen_width - titleWidth) / 2,
        40,
        titleSize,
        WHITE
    );

    int headingSize = 22;
    int textSize = 20;

    int leftX = 120;
    int rightX = 450;

    int y = 130;

    DrawText("SPRITES", leftX, y, headingSize, GRAY);
    DrawText("AI generated", rightX, y, textSize, WHITE);
    y += 65;

    DrawText("BACKGROUND MUSIC", leftX, y, headingSize, GRAY);
    DrawText("Soaring Over the Space", rightX, y, textSize, WHITE);
    y += 30;
    DrawText("(From Sonic Adventure 2)", rightX, y, textSize, LIGHTGRAY);
    y += 65;

    DrawText("PLAYER SHOOTING SOUND", leftX, y, headingSize, GRAY);
    DrawText("Star Wars blaster sound effect", rightX, y, textSize, WHITE);
    y += 30;
    DrawText("(myinstants.com)", rightX, y, textSize, LIGHTGRAY);
    y += 65;

    DrawText("PLAYER DEATH SOUND", leftX, y, headingSize, GRAY);
    DrawText("Blue screen of death", rightX, y, textSize, WHITE);
    y += 30;
    DrawText("(myinstants.com)", rightX, y, textSize, LIGHTGRAY);
    y += 65;

    DrawText("ENEMY DEATH SOUND", leftX, y, headingSize, GRAY);
    DrawText("Geometry Dash death", rightX, y, textSize, WHITE);
    y += 30;
    DrawText("(myinstants.com)", rightX, y, textSize, LIGHTGRAY);
    y += 65;

    DrawText("COUNTDOWN EFFECT", leftX, y, headingSize, GRAY);
    DrawText("(myinstants.com)", rightX, y, textSize, LIGHTGRAY);

    int hintSize = 18;
    int hintWidth = MeasureText("PRESS BACKSPACE TO RETURN", hintSize);

    DrawText(
        "PRESS BACKSPACE TO RETURN",
        (screen_width - hintWidth) / 2,
        screen_height - 45,
        hintSize,
        YELLOW
    );
}

int main(void){

    Vector2 player = {screen_width/2 -30, 7*screen_height/8 - 35};
    bool player_active = true;
    Vector2 enemy = {screen_width/2-30, screen_height/8};
    Vector2 p_size = {50, 50};

    point lvl1 = {7*e_size + 210, true};

 
    Bullet bullets[max_bullet] = {0};
    Bullet e_bullets[e_max_bull] = {0};
    Enemy enemies[num_enemies*3] = {0};

    lives heart[life];

    for(int i = 0; i<life; i++){
        heart[i].size = 50;
        heart[i].position.y = screen_height - 60;
        heart[i].position.x = i*heart[i].size + 20;
    }

    bool menu_active = true;
    bool lvl_menu_active = false;
    bool manual_active = false;
    bool about_us_active = false;
    bool credits_active = false;
    bool paused = false;
    bool started = false;
    bool down = false;
    bool exit = false;   

    int t60 = 0;
    int countdown = 0;
    int start_countdown = 0;    


    InitWindow(screen_width, screen_height, "SPACE_INVADERS");
    SetTargetFPS(60);
    InitAudioDevice();

    Music background_music = LoadMusicStream("Soarin' Over the Space (Cosmic Wall).mp3");
    Music countdown_music = LoadMusicStream("countdown-3-2-1-go (mp3cut.net).mp3");
    SetMusicVolume(background_music, 2.0f);
    SetMusicVolume(countdown_music, 2.0f);

    Sound player_shoot = LoadSound("ufo-laser-blaster-hit-02.mp3");
    Sound player_death = LoadSound("blue-screen-of-death.mp3");
    Sound enemy_death = LoadSound("death-sound-geometry-dash.mp3");
    SetSoundVolume(player_shoot, 2.0f);
    SetSoundVolume(player_death, 2.0f);
    SetSoundVolume(enemy_death, 2.7f);

    PlayMusicStream(background_music);

    Texture2D background_texture = LoadTexture("background2.png");
    Texture2D enemy_texture = LoadTexture("enemy_alien.png");
    Texture2D player_texture = LoadTexture("player_spaceship.png");
    Texture2D bullet_texture = LoadTexture("player_bullet.png");
    Texture2D enemy_bullet_texture = LoadTexture("enemy_bullet.png");
    Texture2D heart_texture = LoadTexture("heart.png");
    Texture2D nafis = LoadTexture("nafis.png");
    Texture2D jabir = LoadTexture("jabir.png");

    while(!WindowShouldClose() && !exit){  
        UpdateMusicStream(background_music);
        UpdateMusicStream(countdown_music);       

        BeginDrawing();

        if(!started && menu_active){
            ClearBackground(BLACK);
            DrawTexturePro(
                background_texture,
               (Rectangle){0,0,background_texture.width,background_texture.height},
               (Rectangle){0,0,screen_width,screen_height},
               (Vector2){0,0},
               0,
               WHITE
            );
            if(IsKeyPressed(KEY_DOWN)){
                selection++;
                if(selection > 6) selection = 1;
            }
            if(IsKeyPressed(KEY_UP)){
                selection--;
                if(selection < 1) selection = 6;
            }
            if(IsKeyPressed(KEY_ENTER) && selection == 1){
                lvl_menu_active = true;
                menu_active = false;
            }
            if(IsKeyPressed(KEY_ENTER) && selection == 6){
                exit = true;
            }
            if(IsKeyPressed(KEY_ENTER) && selection == 3){
                manual_active = true;
                menu_active = false;
            }
            if(IsKeyPressed(KEY_ENTER) && selection == 4){
                about_us_active = true;
                menu_active = false;
            }
            if(IsKeyPressed(KEY_ENTER) && selection == 5){
                credits_active = true;
                menu_active = false;
            }        
            
            DrawMenu(selection);

        }else if(lvl_menu_active){
            ClearBackground(BLACK);
            DrawTexturePro(
                background_texture,
               (Rectangle){0,0,background_texture.width,background_texture.height},
               (Rectangle){0,0,screen_width,screen_height},
               (Vector2){0,0},
               0,
               WHITE
            );
            if(IsKeyPressed(KEY_DOWN)){
                level++;
                if(level > 3) level = 1;
            }
            if(IsKeyPressed(KEY_UP)){
                level--;
                if(level < 1) level = 2;
            }
            if(IsKeyPressed(KEY_ENTER)){
                started = true;
                start_countdown = 180;
                lvl_menu_active = false;
                PlayMusicStream(countdown_music);
            }
            if(IsKeyPressed(KEY_BACKSPACE)){
                lvl_menu_active = false;
                menu_active = true;
            }
            int index = 0;
            for(int i = 0; i<level; i++){
                int x = 7*e_size + 210;
                for(int j = 0; j<num_enemies; j++){
                    index = i*10 + j;
                    enemies[index].active = true;
                    enemies[index].right = true;
                    enemies[index].position.x = x;
                    x += e_size + 30;
                    enemies[index].position.y = screen_height/10 + i*(e_size+30);
                    enemies[index].size = (Vector2){e_size,e_size};
                }
            }         
            DrawLevelMenu(level);

        }else if(manual_active){

            ClearBackground(BLACK);
            DrawTexturePro(
                background_texture,
               (Rectangle){0,0,background_texture.width,background_texture.height},
               (Rectangle){0,0,screen_width,screen_height},
               (Vector2){0,0},
               0,
               WHITE
            );
            DrawManual();
            if(IsKeyPressed(KEY_BACKSPACE)){
                manual_active = false;
                menu_active = true;
            }

        }else if(about_us_active){

            ClearBackground(BLACK);
            DrawTexturePro(
                background_texture,
               (Rectangle){0,0,background_texture.width,background_texture.height},
               (Rectangle){0,0,screen_width,screen_height},
               (Vector2){0,0},
               0,
               WHITE
            );
            DrawAboutUs(nafis, jabir);
            if(IsKeyPressed(KEY_BACKSPACE)){
                about_us_active = false;
                menu_active = true;
            }

        }else if(credits_active){

            ClearBackground(BLACK);

            DrawTexturePro(
                background_texture,
                (Rectangle){0,0,background_texture.width,background_texture.height},
                (Rectangle){0,0,screen_width,screen_height},
                (Vector2){0,0},
                0,
                WHITE
            );
            DrawCredits();
            if(IsKeyPressed(KEY_BACKSPACE)){
                credits_active = false;
                menu_active = true;
            }

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
            if(start_countdown > 0){
                start_countdown--;

                if(start_countdown == 0){
                    StopMusicStream(countdown_music);
                }
            }

            if(start_countdown > 0){
                int seconds = (start_countdown + 59) / 60;
                const char *text = TextFormat("%d", seconds);
                int width = MeasureText(text, 60);

                DrawText(
                    text,
                    (screen_width - width) / 2,
                    (screen_height - 60) / 2,
                    60,
                    WHITE
                );
            }


            if(started){

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

            for(int i = 0; i<num_enemies*level; i++){
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
            if(!paused && start_countdown == 0){
                if(!down){
                    for(int i = 0; i<num_enemies*level; i++){
                        if(enemies[i].active){
                            if(enemies[i].right){
                                enemies[i].position.x += 5;
                                if(enemies[i].position.x> screen_width - (num_enemies - i%10)*(e_size+30)+30){
                                    enemies[i].position.x = screen_width - (num_enemies - i%10)*(e_size+30)+30;
                                    enemies[i].right = false;
                                }
                            }else{
                            enemies[i].position.x -= 5;
                            if(enemies[i].position.x<(i%10)*(e_size+30)){
                                enemies[i].position.x = (i%10)*(e_size+30);
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
                        for(int i = 0; i<num_enemies*level; i++){
                           if(enemies[i].active) enemies[i].position.y += 4 + level;
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

                if(t60%((1+level)*10) == 0){
                    for(int i = 0; i<2*level; i++){
                        int shooter = GetRandomValue(0, num_enemies*level-1);
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

                        if(!paused) e_bullets[j].position.y += 7 + level;

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
                                PlaySound(player_death);
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
                if(!paused && start_countdown == 0){
                    if(IsKeyDown(KEY_RIGHT)){
                        player.x += 5;
                    }
                    if(IsKeyDown(KEY_LEFT)){
                        player.x -= 5;
                    }
                    if(player.x<0) player.x = 0;
                    if(player.x>screen_width-50) player.x = screen_width - 50; 

                    if(IsKeyPressed(KEY_SPACE)){
                        for(int i = 0; i<max_bullet; i++){
                            if(!bullets[i].active){
                                bullets[i].active = true;
                                bullets[i].position = (Vector2){player.x+12, player.y-2};
                                bullets[i].Size = (Vector2){20,20};
                                PlaySound(player_shoot);

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

                    for(int j = 0; j<num_enemies*level;j++){
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
                                PlaySound(enemy_death);
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

            if(enemy_kill == num_enemies*level){
                score *= 2;
                score += life*200;
            }

            t60 += 1;
        }if(life == 0 || down_distance >= player.y - (level-1)*(30+e_size)){

            if(score>high_score[level-1]) high_score[level-1] = score; 
            started = false;
            int length1 = MeasureText("GAME OVER", 60);
            int length2 = MeasureText("PRESS ENTER TO RESTART", 22);
            int length3 = MeasureText(TextFormat("SCORE : %d", score), 30);
            int length4 = MeasureText(TextFormat("HIGH SCORE : %d", high_score[level-1]),25);

            DrawText("GAME OVER",
                (screen_width-length1)/2,
                220,
                60,
                WHITE
            );
            DrawText(
                TextFormat("SCORE : %d", score),
                (screen_width-length3)/2,
                330,
                30,
                WHITE
            ); 
            DrawText(
                TextFormat("HIGH SCORE : %d", high_score[level-1]),
                (screen_width-length4)/2,
                390,
                25,
                WHITE
            );       
            DrawText(
                "(PRESS ENTER TO CONTINUE)",
                (screen_width-length2)/2,
                500,
                22,
                YELLOW
            );        

        }if(enemy_kill == num_enemies*level){
            
            if(score>high_score[level-1]) high_score[level-1] = score; 
            started = false;
            int length1 = MeasureText("ENEMIES DESTROYED!", 50);
            int length2 = MeasureText("PRESS ENTER TO RESTART", 22);
            int length3 = MeasureText(TextFormat("SCORE : %d", score), 30);
            int length4 = MeasureText(TextFormat("HIGH SCORE : %d", high_score[level-1]),25);

            DrawText(
                "ENEMIES DESTROYED!",
                (screen_width-length1)/2,
                220,
                50,
                WHITE
            );
            DrawText(
                TextFormat("SCORE : %d", score),
                (screen_width-length3)/2,
                330,
                30,
                WHITE
            ); 
            DrawText(
                TextFormat("HIGH SCORE : %d", high_score[level-1]),
                (screen_width-length4)/2,
                390,
                25,
                WHITE
            );       
            DrawText(
                "(PRESS ENTER TO CONTINUE)",
                (screen_width-length2)/2,
                500,
                22,
                YELLOW
            ); 
            
        }

        if(!started){
            if(IsKeyPressed(KEY_ENTER)){
                lvl_menu_active = true;
                down = false;
                score = 0;
                life = 3;
                enemy_kill = 0; 
                collision_count = 0;
                down_timer = 0;
                down_distance = screen_height/8;
                down_count = 0;
                lvl1 = (point){7*e_size + 210, true};

                player_active = true;
                player = (Vector2){screen_width/2 -30, 7*screen_height/8 - 35};

                int index = 0;
                for(int i = 0; i<level; i++){
                    int x = 7*e_size + 210;
                    for(int j = 0; j<num_enemies; j++){
                        index = i*10 + j;
                        enemies[index].active = true;
                        enemies[index].right = true;
                        enemies[index].position.x = x;
                        x += e_size + 30;
                        enemies[index].position.y = screen_height/10 + i*(e_size+30);
                        enemies[index].size = (Vector2){e_size,e_size};
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

    UnloadMusicStream(background_music);
    UnloadMusicStream(countdown_music);

    UnloadSound(player_shoot);
    UnloadSound(player_death);
    UnloadSound(enemy_death);

    CloseAudioDevice();

    UnloadTexture(player_texture);
    UnloadTexture(bullet_texture);
    UnloadTexture(enemy_bullet_texture);
    UnloadTexture(enemy_texture);
    UnloadTexture(background_texture);
    UnloadTexture(heart_texture);
    UnloadTexture(nafis);
    UnloadTexture(jabir);

    CloseWindow();

    
}