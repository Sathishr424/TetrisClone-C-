#ifndef GAMESYSTEM_H
#define GAMESYSTEM_H
#include <iostream>
#include <vector>
#include <String>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;


class gameSystem
{
    public:
        gameSystem();
        void gameLoop();
        void drawBlocks();
        void currentBlockSetter();
        void update();
        void movement();
        void rotateBlock();
        bool checkCollison(int x, int y);
        bool collision(int x, int y, int x1, int y1);
        void blocksAdder();
        void checkRows();
        void textDisplayText(String t, int x, int y, int s, Color c=Color(0, 0, 0));
        bool gameOver();
        void checkPossible();
        void checkHighscore();
        void restart();
        bool clearRow(int x);
        void pause();
        void scoreShower(int x, int y, int s=0);

    private:
        //For Sounds and Musics
        SoundBuffer fallB, movB, rotB, clearB, gameB, pinB, poutB;
        Sound fallS, movS, rotS, clearS, gameM, pinS, poutS;
        Music bgSound;
        //End
        RectangleShape bg, box, rstart, trans;
        Font font;
        Text text;
        Texture block, epT, bT, bgroundT, gameT, midT;
        vector<vector<int>> grid;
        Sprite s, epS, bS, bgroundS, gameS, midS;
        vector<Vector2f> objects;
        int current[4][4];
        int r = 0, lef = 0, _next = rand()%7, rig = 0, dx = 90, dy= -180, dow = 0, r1 = 0, c =0;
        int dx2, dy2;
        Clock times, times2, times3, over, pause_, clearA1, clearA2;
        int fps = 40, subScore = 0;
        int duplicateX, duplicateY, duplicateX1, duplicateY1;
        int align = 0;
        int tetris = 0, tetris1 = 0;
        bool scorer = 0, tet = 0;
        String rowClear = "classic";
        bool z = 0;
        String quotes[7] = {
                    "Superb!",
                    "Wow!",
                    "Awesome!",
                    "Tetris!!",
                    "Marvelous!",
                    "WooHoo!",
                    "Fantastic!"
        };
        String musics[5] = {
                "resources\\backgroundMusic1.ogg", "resources\\backgroundMusic2.ogg", "resources\\backgroundMusic3.ogg",
                 "resources\\backgroundMusic4.ogg", "resources\\backgroundMusic5.ogg"
        };
        int song = rand()%5;
        String word;
        bool downs = 0, wait = 0, overGame = 0, stop = 0, fall = 0 ,mov = 0, rot = 0, _clear = 0, over_ = 0, gameOver_ = 0, _pause = 0;
        int score = 0, highScore = 0, level = 1, subScore1=0;
        IntRect shapes[9] = {IntRect(0, 0, 30, 30), IntRect(30, 0, 30, 30), IntRect(60, 0, 30, 30), IntRect(90, 0, 30, 30),
                            IntRect(120, 0, 30, 30), IntRect(150, 0, 30, 30), IntRect(180, 0, 30, 30), IntRect(210, 0, 30, 30), IntRect(240, 0, 30, 30)};
};

#endif // GAMESYSTEM_H
