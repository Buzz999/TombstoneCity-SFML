/*
 *  Copyright (C) 2008 Florent Bedoiseau (electronique.fb@free.fr)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <list>
#include <stdint.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

// BUG : White Cactus / Monster can be killed

/* Numero des tiles */
#define FLOOR      0
#define CACTUS_1   1
#define CACTUS_2   2
#define PLAYER_1   3
#define PLAYER_2   4
#define PLAYER_3   5
#define PLAYER_4   6
#define MONSTER_1  7
#define MONSTER_2  8
#define TIR_1      9
#define TIR_2      10
#define SPLASH     11
#define BUISSON_1  12
#define BUISSON_2  13
#define BLOCK_1    14
#define BLOCK_2    15
#define SCORE      16
#define BOARD      17
#define GAMEOVER   18
#define LOWER_SCREEN 19

#define OUT 666 // Why not ...

// @50Hz 1 tick=20ms
#if 0
#define PLAYER_TICK 5 /* 100ms */
#define PLAYER_TICK_CHANGE 2
#define SHOOT_TICK 10 /* 200ms */
#define MOVE_BUISSONS_TICK 6 /* 120ms */
#define CREATE_BUISSONS_TICK 200 /* 4s */
#define CREATE_MONSTERS_TICK 350 /* 7s */
#define CHANGE_CAGE_TICK 250 /* 5s */
#define MOVE_MONSTERS_TICK 5 /* 100ms */
#else
// @60Hz (PAL) 1 tick=16ms
#define PLAYER_TICK 6 /* 100ms */
#define PLAYER_TICK_CHANGE 2
#define SHOOT_TICK 12 /* 200ms */
#define MOVE_BUISSONS_TICK 7 /* 120ms */
#define CREATE_BUISSONS_TICK 240 /* 4s */
#define CREATE_MONSTERS_TICK 420 /* 7s */
#define CHANGE_CAGE_TICK 300 /* 5s */
#define MOVE_MONSTERS_TICK 6 /* 100ms */
#endif

#define NO_FIRE          0
#define FIRE_IN_PROGRESS 1
#define FIRE_SPLASH      2

class Game;
class FPoint;
class InputMapper;

class Object {
  public:

    Object (uint32_t tileNum, uint32_t ax, uint32_t ay, Game &game, bool print);
    ~Object ();

    void incXY (uint32_t ix, uint32_t iy);
    void setXY (uint32_t xx, uint32_t yy);

    uint32_t getX () const {
        return _x;
    }
    uint32_t getY () const {
        return _y;
    }
    void getXY (uint32_t &xx, uint32_t &yy) {
        xx=_x;
        yy=_y;
    }

    uint32_t getTileNumber () const {
        return _tileNumber;
    }
    void setTileNumber (uint32_t atile) {
        _tileNumber = atile;
    }

  private:
    uint32_t _x, _y;
    uint32_t _tileNumber;
    uint32_t _oldtileNumber;
    Game &_game;

    bool _print;

    // Not implemented
    Object ();
    Object& operator=(const Object &aObject);
    Object (const Object &aObject);
};

class FPoint {
  public:
    FPoint () : _x(0), _y (0) {}
    FPoint (uint32_t ax, uint32_t ay) : _x(ax), _y (ay) {}
    FPoint& operator=(const FPoint &aFPoint);
    FPoint (const FPoint &aFPoint);
    uint32_t getX () const {
        return _x;
    }
    uint32_t getY () const {
        return _y;
    }
    void getXY (uint32_t &xx, uint32_t &yy) const {
        xx=_x;
        yy=_y;
    }

  private:
    uint32_t _x;
    uint32_t _y;
};

class InputMapper {
  public:
    InputMapper ();
    ~InputMapper () { }

    bool readResourceFile (const string & filename);

    bool isDownPressed() const;
    bool isUpPressed() const;
    bool isLeftPressed() const;
    bool isRightPressed() const;
    bool isFirePressed() const;

    bool isTabPressed() const;
    bool isEnterPressed() const;
    bool isPausePressed() const;
    bool isQuitPressed () const;

    sf::Keyboard::Key getUpKey() const {
        return _up;
    }
    sf::Keyboard::Key getDownKey() const {
        return _down;
    }
    sf::Keyboard::Key getLeftKey() const {
        return _left;
    }
    sf::Keyboard::Key getRightKey() const {
        return _right;
    }
    sf::Keyboard::Key getFireKey() const {
        return _fire;
    }

    sf::Keyboard::Key getTabKey() const {
        return _tab;
    }
    sf::Keyboard::Key getEnterKey() const {
        return _enter;
    }
    sf::Keyboard::Key getQuitKey() const {
        return _quit;
    }
    sf::Keyboard::Key getPauseKey () const {
        return _pause;
    }

  protected:
    sf::Keyboard::Key _up;
    sf::Keyboard::Key _down;
    sf::Keyboard::Key _left;
    sf::Keyboard::Key _right;
    sf::Keyboard::Key _fire;

    sf::Keyboard::Key _tab;
    sf::Keyboard::Key _enter;
    sf::Keyboard::Key _quit;
    sf::Keyboard::Key _pause;
};

typedef list<Object*> ListOfObjects;
typedef list<FPoint> ListOfPoints;

typedef list<Object*>::iterator ListOfObjectsIterator;
typedef list<FPoint>::iterator ListOfPointsIterator;

class Game {
  public:
    enum GameState { S_INIT, S_RUN, S_PAUSE, S_END };
    enum ShootItem { SI_NONE, SI_MONSTER, SI_BUSH };

    Game ();
    ~Game ();

    bool playSound (uint32_t idx);
    void stopSound ();

    void Process ();
    bool StateMachine ();
    void Display (sf::RenderWindow &window);

    uint32_t GetBoard (uint32_t x, uint32_t y) const {
        return _board [y][x];
    }
    void SetBoard (uint32_t x, uint32_t y, uint32_t tileNum) {
        _board [y][x]=tileNum;
    }


  private:
    // Sound
    sf::SoundBuffer _deadBuffer;
    sf::SoundBuffer _firedBuffer;
    sf::SoundBuffer _monsterBuffer;
    sf::SoundBuffer _themeBuffer;

    sf::Sound _soundChannel;

    bool loadSound ();



    // textures for tiles
    map<uint32_t, sf::Texture> _mapOfTextures;
    sf::Texture &getTexture (uint32_t tileNum) { return _mapOfTextures[tileNum]; }

    bool loadTexture (uint32_t tileNumber, const string &filename);
    bool loadTextures ();

    // Textures for characters
    vector<sf::Texture> _vectOfCaracters;
    sf::Texture &getCharacterTexture (uint32_t i) { return _vectOfCaracters.at(i); }

    void buildCharacterTextures ();
    uint32_t GetPatternNumber (const char c);


    GameState _gameState;

    InputMapper _inputMapper;

    float _scale;

    // PLAYER
    Object *_player_Object;
    uint32_t _player_direction;
    bool _player_blocked;

    // FIRE
    Object *_fire_Object;
    uint32_t _fire_direction;
    uint32_t _fire_in_progress;

    // Splash
    Object *_splash_Object;
    uint32_t _time_splash;
    ShootItem _shoot_item;

    /* MONSTER */
    uint32_t _mx, _my; /* Position possible de creation d'un monstre */
    bool _monsterToBeCreated;

    /* CACTUS */
    uint32_t _cx, _cy; /* position du cactus blanc */

    ListOfObjects _mListOfCactus; /* CACTUS */
    ListOfObjects _mListOfBuissons; /* BUISSONS */
    ListOfObjects _mListOfMonsters; /* MONSTERS */
    ListOfObjects _mListOfCages; /* CAGE */

    uint32_t _time_create_monster;
    uint32_t _time_move_monster;
    uint32_t _time_create_buissons;
    uint32_t _time_move_buissons;
    uint32_t _time_player;

    uint32_t _board [24][28];
    uint32_t _score;
    uint32_t _day;
    uint32_t _life;

    // Changing
    void PrepareGame ();
    void ChangeDay ();
    void ChangeLife ();

    // Creation
    void CreateCactus (uint32_t tileNum, uint32_t x, uint32_t y);
    void CreateMonster (uint32_t tileNum, uint32_t x, uint32_t y);
    void CreateCage (uint32_t tileNum, uint32_t x, uint32_t y);

    void CreateAllCactus ();
    void CreateAllBuissons ();
    void CreateAllCages (uint32_t tileCage);
    void CreatePlayer ();
    void CreateFire (uint32_t aTile, uint32_t x, uint32_t y);
    void CreateSplash (uint32_t x, uint32_t y);

    // Time
    void TimeCreateMonsters ();
    void TimeCreateBuissons ();
    void TimeMovePlayer ();
    void TimeMoveBuissons ();
    void TimeMoveMonsters ();
    void TimeFirePlayer ();
    void TimeFire();
    void TimeShoot ();

    // Checking
    uint32_t CheckBounces (uint32_t x, uint32_t y) const;
    uint32_t CheckObstacles (uint32_t x, uint32_t y) const;

    // Showing
    void DisplayObject (sf::RenderWindow &window, Object *obj);
    void DisplayListOfObjects (sf::RenderWindow &window, const ListOfObjects &mListOfObjects);
    void DisplayString (sf::RenderWindow &window, const string &s, uint32_t x, uint32_t y);
    void DisplayValue (sf::RenderWindow &window, uint32_t value, uint32_t col, uint32_t row);

    // Management
    bool ComputeNewMonsterPosition ();
    ListOfPoints ComputeThreeCactusList (uint32_t x, uint32_t y);

    void MoveMonsters ();
    bool MoveMonsterX (Object *aObject, int32_t dx);
    bool MoveMonsterY (Object *aObject, int32_t dy);
    void ReplaceCactusByMonster (uint32_t x, uint32_t y);
    bool RemoveXYObject (ListOfObjects &aListOfObject, uint32_t x, uint32_t y);
    bool CheckIfBlocked () const;
    void FindEmptySpace (uint32_t &x, uint32_t &y) const;

    // Destruction
    void DeleteAllObjects ();
    void DeleteListOfObjects (ListOfObjects &objects);
    void DeleteFire ();
    void DeleteSplash ();
    void DeletePlayer ();
};

void ShakeDirection (uint32_t tab_dir[]);
int sign (int val);

