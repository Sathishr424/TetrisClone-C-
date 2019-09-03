#include "gameSystem.h"
#include <iostream>
#include <ctime>
#include <String>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

RenderWindow window(VideoMode(500, 600), "Tetris");
int blocks[7][4][4] =
{
    0, 0, 0, 0,//I shape
    1, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0,

    0, 0, 0, 0,//J shape
    0, 1, 0, 0,
    0, 1, 1, 1,
    0, 0, 0, 0,

    0, 0, 0, 0,//L shape
    0, 0, 0, 1,
    0, 1, 1, 1,
    0, 0, 0, 0,

    0, 0, 0, 0,//O shape
    0, 1, 1, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,

    0, 0, 0, 0,//S shape
    0, 0, 1, 1,
    0, 1, 1, 0,
    0, 0, 0, 0,

    0, 0, 0, 0,//Z shape
    0, 1, 1, 0,
    0, 0, 1, 1,
    0, 0, 0, 0,

    0, 0, 0, 0,//T shape
    0, 0, 1, 0,
    0, 1, 1, 1,
    0, 0, 0, 0,
};

gameSystem::gameSystem()
{
    srand(time(0));
    font.loadFromFile("resources\\font.ttf");
    text.setFont(font);
    bgroundT.loadFromFile("resources\\background.png");
    bgroundS.setTexture(bgroundT);
    bgroundS.setPosition(0, 0);
    //srand(time(0));
    bg = RectangleShape(Vector2f(500, 600));
    bg.setFillColor(Color::White);
    bg.setPosition(0,0);
    epT.loadFromFile("resources\\sample.png");
    epS.setTexture(epT);
    currentBlockSetter();
    for (int i=0; i<23; i++){
        grid.push_back({0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    }
    box = RectangleShape(Vector2f(30, 30));
    box.setFillColor(Color(221, 221, 221));
    box.setOutlineColor(Color::Black);
    box.setOutlineThickness(1);
    bT.loadFromFile("resources\\spriteSheet.png");
    bS.setTexture(bT);
    rstart = RectangleShape(Vector2f(130, 30));
    rstart.setPosition(355, 500);
    rstart.setFillColor(Color(112, 146, 190));
    gameT.loadFromFile("resources\\gameOver.png");
    gameS.setTexture(gameT);
    gameS.setPosition(0, 0);
    //bgBuffer.loadFromFile("backgroundMusic.mp3");
    bgSound.openFromFile(musics[song]);
    //bgSound.setPlayingOffset(seconds(2.5f));
    bgSound.setVolume(100.0f);
    bgSound.play();
    fallB.loadFromFile("resources\\sfx_sounds_falling.wav");
    fallS.setBuffer(fallB);
    movB.loadFromFile("resources\\sfx_sounds_move.wav");
    movS.setBuffer(movB);
    fallS.setVolume(100.0f);
    rotB.loadFromFile("resources\\sfx_sound_rotate.wav");
    rotS.setBuffer(rotB);
    rotS.setVolume(100.0f);
    clearB.loadFromFile("resources\\sfx_sounds_clear.wav");
    clearS.setBuffer(clearB);
    clearS.setVolume(100.0f);
    clearS.setLoop(true);
    gameB.loadFromFile("resources\\sfx_sound_gameover.wav");
    gameM.setBuffer(gameB);
    gameM.setVolume(100.0f);
    clearS.setVolume(100.0f);
    midT.loadFromFile("resources\\bgMiddle.png");
    midS.setTexture(midT);
    pinB.loadFromFile("resources\\sfx_sounds_pause_in.wav");
    pinS.setBuffer(pinB);
    pinS.setVolume(100.0f);
    poutB.loadFromFile("resources\\sfx_sounds_pause_out.wav");
    poutS.setBuffer(poutB);
    poutS.setVolume(100.0f);
    trans = RectangleShape(Vector2f(300, 570));
    trans.setFillColor(Color(255, 147, 147, 128));
}

void gameSystem::textDisplayText(String t, int x, int y, int s, Color c)
{
    text.setString(t);
    text.setPosition(x, y);
    text.setFillColor(c);
    text.setStyle(Text::Bold);
    text.setCharacterSize(s);
    text.setOrigin(0, 0);
    window.draw(text);
}

void gameSystem::checkHighscore()
{
    ofstream output;
    ifstream input;
    input.open("resources\\config.ini");
    string line;
    while (getline(input, line)){
        highScore = stoi(line);
    }
    input.close();
    if (highScore < score){
        output.open("resources\\config.ini");
        output << to_string(score);
        output.close();
        highScore = score;
    }
    textDisplayText(to_string(highScore), 355, 60, 20);
}

void gameSystem::gameLoop()
{
    while (window.isOpen()){
        Event event;
        while (window.pollEvent(event)){
            if (event.type == Event::EventType::Closed) window.close();
        }
        window.clear();
        window.draw(bgroundS);
        window.draw(midS);
        restart();
        if (_pause){
            drawBlocks();
            checkPossible();
            textDisplayText("Paused", 100, 300, 30, Color::White);
            window.draw(trans);
        }else{
            movement();
            update();
        }
        window.display();
    }
}

void gameSystem::drawBlocks()
{
    textDisplayText(to_string(score), 355, 140, 20);
    textDisplayText(to_string(level), 355, 433, 20);
    int x = 0;
    rig = 0;
    dow = 0;
    for (int i=0; i<120; i+=30){
        for (int j=0; j<120; j+=30){
            if (current[i/30][j/30] == 1){
                if (x == 0) {lef = i+dx; x = 1;}
                if (j+dy > dow) dow = j+dy;
                if (i+dx > rig) rig = i+dx;
                //bS.setColor(col);
                bS.setTextureRect(shapes[r]);
                bS.setPosition(i+dx, j+dy);
                window.draw(bS);
            }
        }
    }

    for (int i=0; i<23; i++){
        for (int j=0; j<10; j++){
            if (grid[i][j] != 0){
            bS.setTextureRect(shapes[grid[i][j]-1]);
            bS.setPosition(j*30, i*30);
            window.draw(bS);}
        }
    }
    checkHighscore();

    for (int i=0; i<120; i+=30){
        for (int j=0; j<120; j+=30){
            if (blocks[_next][i/30][j/30] == 1){
                bS.setTextureRect(shapes[_next]);
                bS.setPosition(i+355, j+235);
                window.draw(bS);
            }
        }
    }
}

void gameSystem::scoreShower(int x, int y, int s)
{
    textDisplayText(("+" + to_string(score-subScore1)), x, y, 10, Color::White);

}

void gameSystem::currentBlockSetter()
{
    r = _next;
    r1 = r;
    _next = rand()%(sizeof(blocks)/sizeof(blocks[0]));
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            current[i][j] = blocks[r][i][j];
        }
    }
}

void gameSystem::restart()
{
    Vector2i mouse = Mouse::getPosition(window);
    if (Mouse::isButtonPressed(Mouse::Left)){
        if (mouse.x <= 455 && mouse.x >= 355 && mouse.y <= 530 && mouse.y >= 500){
            overGame = 1;
        }
    }if (mouse.x <= 485 && mouse.x >= 355 && mouse.y <= 530 && mouse.y >= 500){
            rstart.setFillColor(Color(55, 84, 123));
        }
    else{
        rstart.setFillColor(Color(112, 146, 190));
    }
    window.draw(rstart);
    textDisplayText("RESTART", 365, 505, 20);

    bool p=0;
    if (Keyboard::isKeyPressed(Keyboard::P)) p=1;

    if (pause_.getElapsedTime().asMilliseconds() >= 50){
        if (p) {
            if (_pause) {_pause=0; pinS.play(); bgSound.play();}
            else {_pause = 1; poutS.play(); bgSound.pause();}
        }
        pause_.restart();
    }
    ifstream input;
    input.open("resources\\ClearRowStyle.ini");
    string line;
    while (getline(input, line)){
        rowClear = line;
    }
    input.close();
}

void gameSystem::update()
{
    if (stop) {bgSound.play(); stop=0;}
    bgSound.setVolume(100.0f);
    if (fall && fallS.getPlayingOffset().asSeconds() >= fallB.getDuration().asSeconds()) {fallS.stop(); fall = 0;}
    if (mov && movS.getPlayingOffset().asSeconds() >= movB.getDuration().asSeconds()) {movS.stop(); mov = 0;}
    if (rot && rotS.getPlayingOffset().asSeconds() >= rotB.getDuration().asSeconds()) {rotS.stop(); rot = 0;}
    clearS.stop();
    if (dow < 540 && collision(0, 30, dx, dy)){
        checkPossible();
    }
    drawBlocks();
    if (dow >= 540 || !collision(0, 30, dx, dy)){
        dy = dy-(dy%30);
        wait = 1;
    }
    if ((dow >= 540 || !collision(0, 30, dx, dy)) && !z){
        wait = 0;
        score += 120;
        bgSound.setVolume(20.0f);
        fallS.play();
        fall = 1;
        blocksAdder();
        currentBlockSetter();
        duplicateX = dx+20; duplicateY = dow;
        if (duplicateX < 20) duplicateX = 30;
        duplicateX1 = duplicateX-30; duplicateY1 = duplicateY-30;
        lef = 0; rig = 0; dx = 90; dy= -150; dow = 0;

    }if (overGame)
    {
        bgSound.stop();
        stop = 1;
        overGame = 0;
        grid.clear();
        for (int i=0; i<23; i++){
            grid.push_back({0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
        }
        currentBlockSetter();
        wait = 0;
        lef = 0; rig = 0; dx = 90; dy= -180; dow = 0;score = 0, level = 1, subScore = 0, subScore1 = 0, fps = 40;

    }
    if (gameOver_){
        over.restart();
        bgSound.stop();
        stop = 1;
        gameOver_ = 0;
        gameM.play();
        while (over.getElapsedTime().asSeconds() < 2.5f){
            window.clear();
            window.draw(gameS);
            textDisplayText(to_string(score), 164, 350, 20);
            window.display();}
        gameM.stop();
        grid.clear();
        for (int i=0; i<23; i++){
            grid.push_back({0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
        }
        currentBlockSetter();
        lef = 0; rig = 0; dx = 90; dy= -180; dow = 0;score = 0, level = 1, subScore = 0, subScore1 = 0, fps = 40;
    }
    checkRows();
    if (score != subScore1 && !scorer){
        scorer=1;
    }
    if (scorer){
        if (duplicateY > duplicateY1){
            scoreShower(duplicateX-15, duplicateY-15);
            duplicateY-=1;
        }else{
            scorer=0;
            subScore1 = score;
        }
    }
    if (bgSound.getStatus() == 0){
        if (song != 4) song++;
        else song=0;
        bgSound.openFromFile(musics[song]);
        bgSound.play();
    }

}

void gameSystem::checkPossible()
{
    int y;
    int con = 0;
    for (int i=0; i<120; i+=30){
        for (int j=0; j<120; j+=30){
            if (current[i/30][j/30] == 1){
                if (j+dy2 > y) y = j+dy2;
                con+=30;
            }
        }
    }
    if (dow > 0){
    while(y < 540){
        if (!collision(0, 30, dx2, dy2)) break;
        dy2+=30;
        for (int i=0; i<120; i+=30){
        for (int j=0; j<120; j+=30){
            if (current[i/30][j/30] == 1){
                if (j+dy2 > y) y = j+dy2;
                con+=30;
            }
        }
    }
    }

     for (int i=0; i<120; i+=30){
        for (int j=0; j<120; j+=30){
            if (current[i/30][j/30] == 1){
                epS.setPosition(i+dx2, j+(dy2-dy2%30));
                window.draw(epS);
            }
        }
    }}

}

bool gameSystem::gameOver()
{
    for (int i=0; i<10; i++){
        if (grid[0][i] != 0){
            return true;
        }
    }
    return false;
}

void gameSystem::blocksAdder()
{
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            if (current[i][j] == 1){
                if (((j*30)+(dy-dy%30)+4)/30 > 0){
                grid[((j*30)+(dy-dy%30)+4)/30][((i*30)+dx)/30] = r+1;}
                else gameOver_ = 1;
            }
        }
    }
}

void gameSystem::movement()
{
    downs = 0;
    if (score > subScore+2000){
        level+=1;
        if (fps != 10) fps--;
        subScore = score;
    }
    bool up=0,down=0,left=0,right=0;

    if (Keyboard::isKeyPressed(Keyboard::Left)) left=1;
    if (Keyboard::isKeyPressed(Keyboard::Right)) right=1;
    if (Keyboard::isKeyPressed(Keyboard::Down)) {down=1; downs = 1;}
    if (Keyboard::isKeyPressed(Keyboard::Up)) up=1;
    if (Keyboard::isKeyPressed(Keyboard::Z)) z=1;
    else z=0;
    if (Keyboard::isKeyPressed(Keyboard::Delete)) objects.clear();

    if (times3.getElapsedTime().asMilliseconds() > fps){
        if (dow < 540 && collision(0, 30, dx, dy)) dy += 3;
        times3.restart();
    }
    if (times.getElapsedTime().asMilliseconds() > 50){
    if (left && lef-30 >= 0 && collision(-30, 0, dx, dy)) {dx -= 30; bgSound.setVolume(20.0f); movS.play(); mov = 1;}
    if (right && rig+30 < 300 && collision(30, 0, dx, dy)) {dx += 30; bgSound.setVolume(20.0f); movS.play(); mov = 1;}
    times.restart();}
    if (times2.getElapsedTime().asMilliseconds() > 100){
        if (up) {rotateBlock();
        bgSound.setVolume(20.0f);
        times2.restart();}
    }
    if (down && dow < 540 && dow > 0 && collision(0, 30, dx, dy)) {dy = dy-(dy%30); dy += 30;}
    dx2 = dx; dy2 = dy;
}

void gameSystem::rotateBlock()
{
    int temp[4][4];
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            temp[i][j] = current[i][j];
        }
    }
    current[0][0] = temp[3][0];
    current[0][1] = temp[2][0];
    current[0][2] = temp[1][0];
    current[0][3] = temp[0][0];

    current[1][0] = temp[3][1];
    current[1][1] = temp[2][1];
    current[1][2] = temp[1][1];
    current[1][3] = temp[0][1];

    current[2][0] = temp[3][2];
    current[2][1] = temp[2][2];
    current[2][2] = temp[1][2];
    current[2][3] = temp[0][2];

    current[3][0] = temp[3][3];
    current[3][1] = temp[2][3];
    current[3][2] = temp[1][3];
    current[3][3] = temp[0][3];

    lef = 0; rig = 0; dow = 0;

    int x = 0;
    for (int i=0; i<120; i+=30){
        for (int j=0; j<120; j+=30){
            if (current[i/30][j/30] == 1){
                if (x == 0) {lef = i+dx; x = 1;}
                if (j+dy > dow) dow = j+dy;
                if (i+dx > rig) rig = i+dx;
            }
        }
    }
    if ((lef < 0 || rig >= 300) || !collision(0, 0, dx, dy) || wait){
        for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            current[i][j] = temp[i][j];
        }
    }
    }else{
        rotS.play();
        rot = 1;
    }
}

bool gameSystem::checkCollison(int x, int y)
{
    for (int i=0; i<23; i++){
        for (int j=0; j<10; j++){
            if (grid[i][j] != 0){
                if (i*30 == y && j*30 == x){
                    return false;
                }
            }
    }
    }
    return true;
}

bool gameSystem::collision(int x, int y, int x1, int y1)
{
    for (int i=0; i<120; i+=30){
        for (int j=0; j<120; j+=30){
            if (current[i/30][j/30] == 1){
                if (!checkCollison(i+(x1)+x, j+(y1 - y1%30)+y)) return false;
                //if (!checkCollison(i+(x1)+x, j+(y1 - y1%30)+y)) return false;
            }
        }
    }
    return true;
}

void gameSystem::checkRows()
{
    vector<int> row;
    int rows = 0;
    for (int i=0; i<23; i++){
        bool isFilled = true;
        for (int j=0; j<10; j++){
            if (grid[i][j] == 0){
                isFilled = false;
            }
        }
        if (isFilled){
            int tmp = 0;
            row.push_back(i);
            if (rowClear != "classic"){
            clearS.play();
            while (!clearRow(i)){
                grid[i][tmp] = 0;
                tmp++;
                window.draw(bgroundS);
                window.draw(midS);
                drawBlocks();
                restart();
                bgSound.setVolume(20.0f);
                _clear = 1;
                window.display();
            }
            grid.erase(grid.begin()+i);
            grid.insert(grid.begin(), {0, 0, 0, 0, 0, 0, 0, 0, 0, 0});}
            if (rows != 0)
            rows += (1*rows);
            else rows++;
            duplicateY = i*30; duplicateX = 115;
            duplicateY1 = duplicateY-30; duplicateX1 = duplicateX-30;
        }
    }
    if (rowClear == "classic"){
    if (row.size() > 0){
        clearS.play();
        _clear = 1;
        for (int k=0; k<13; k++){
            for (int j=0; j<10; j++){
                for (unsigned int i=0; i<row.size(); i++){
                grid[row[i]][j] = 9;}
            }
            window.draw(bgroundS);
            window.draw(midS);
            drawBlocks();
            restart();
            window.display();
        }clearS.play();
        for (int k=0; k<15; k++){
            for (int j=0; j<10; j++){
                for (unsigned int i=0; i<row.size(); i++){
                grid[row[i]][j] = 8;}
            }
            window.draw(bgroundS);
            window.draw(midS);
            drawBlocks();
            restart();
            window.display();
        }
    }
    for (unsigned int i=0; i<row.size(); i++){
        grid.erase(grid.begin()+row[i]);
        grid.insert(grid.begin(), {0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    }row.clear();}
    if (rows == 1) score += rows*100;
    else score += rows*100*(level);
    if (rows >= 1 && !tet){
        tetris = 300;
        tetris1 = 300-30;
        tet = 1;
        if (rows >= 4) word = "On Fire!!";
        else word = quotes[rand()%7];
    }
    if (tet){
        if (tetris > tetris1){
        textDisplayText(word, 150, tetris, 15, Color::White); tetris--;}
        else tet = 0;
    }
}

bool gameSystem::clearRow(int x)
{
    for (int i=0; i<10; i++){
        if (grid[x][i] != 0) return false;
    }
    return true;
}










