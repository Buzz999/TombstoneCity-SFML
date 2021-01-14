/*
 *  Copyright (C) 2008-2018 Florent Bedoiseau (electronique.fb@free.fr)
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

// TODO : Add sound

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <sys/time.h>
#include <string.h>

#include "Tombstone.h"

// FONT
const char *TabDefaultCharPattern [96] = {
    /* Set 1 */
    "0000000000000000",
    "0010101010100010",
    "0028282800000000",
    "0028287C287C2828",
    "0038545038145438",
    "0060640810204C0C",
    "0020505020544834",
    "0008081000000000",

    /* Set 2 */
    "0008102020201008",
    "0020100808081020",
    "000028107C102800",
    "000010107C101000",
    "0000000000301020",
    "000000007C000000",
    "0000000000003030",
    "0000040810204000",

    /* Set 3 */
    "0038444444444438",
    "0010301010101038",
    "003844040810207C",
    "0038440418044438",
    "00081828487C0808",
    "007C407804044438",
    "0018204078444438",
    "007C040810202020",

    /* Set 4 */
    "0038444438444438",
    "003844443C040830",
    "0000303000303000",
    "0000303000301020",
    "0008102040201008",
    "0000007C007C0000",
    "0020100804081020",
    "0038440408100010",

    /* Set 5 */
    "0038445C545C4038",
    "003844447C444444",
    "0078242438242478",
    "0038444040404438",
    "0078242424242478",
    "007C40407840407C",
    "007C404078404040",
    "003C40405C444438",

    /* Set 6 */
    "004444447C444444",
    "0038101010101038",
    "0004040404044438",
    "0044485060504844",
    "004040404040407C",
    "00446C5454444444",
    "00446464544C4C44",
    "007C44444444447C",

    /* Set 7 */
    "0078444478404040",
    "0038444444544834",
    "0078444478504844",
    "0038444038044438",
    "007C101010101010",
    "0044444444444438",
    "0044444428281010",
    "0044444454545428",

    /* Set 8 */
    "0044442810284444",
    "0044442810101010",
    "007C04081020407C",
    "0038202020202038",
    "0000402010080400",
    "0038080808080838",
    "0000102844000000",
    "000000000000007C",

    /* Set 9 */
    "0000201008000000",
    "00000038447C4444",
    "0000007824382478",
    "0000003C4040403C",
    "0000007824242478",
    "0000007C4078407C",
    "0000007C40784040",
    "0000003C405C4438",

    /* Set 10 */
    "00000044447C4444",
    "0000003810101038",
    "0000000808084830",
    "0000002428302824",
    "000000404040407C",
    "000000446C544444",
    "0000004464544C44",
    "0000007C4444447C",

    /* Set 11 */
    "0000007844784040",
    "0000003844544834",
    "0000007844784844",
    "0000003C40380478",
    "0000007C10101010",
    "0000004444444438",
    "0000004444282810",
    "0000004444545428",

    /* Set 12 */
    "0000004428102844",
    "0000004428101010",
    "0000007C0810207C",
    "0018202040202018",
    "0010101000101010",
    "0030080804080830",
    "0000205408000000",
    "0000000000000000"
};

// --------------------------------
// FPOINT
// --------------------------------
FPoint::FPoint (const FPoint& aFPoint) {
    _x=aFPoint._x;
    _y=aFPoint._y;
}

FPoint& FPoint::operator=(const FPoint& aFPoint) {
    if (this==&aFPoint)
        return *this;
    _x=aFPoint._x;
    _y=aFPoint._y;
    return *this;
}

// -------------------------------
// Object
// -------------------------------
Object::Object (uint32_t tileNum, uint32_t ax, uint32_t ay, Game &game, bool print=true) : _x(ax), _y(ay), _tileNumber (tileNum), _game (game), _print (print) {
    _oldtileNumber = _game.GetBoard (_x, _y); // Getting the previous value
    if (_print)
        _game.SetBoard (_x, _y, _tileNumber);
}

Object::~Object () {
    if (_print)
        _game.SetBoard (_x, _y, _oldtileNumber); // Restoring
}

void Object::incXY (uint32_t ix, uint32_t iy) {
    if (_print)
        _game.SetBoard (_x, _y, _oldtileNumber); // Restoring

    _x+=ix;
    _y+=iy; // Moving : increment
    if (_print) {
        _oldtileNumber = _game.GetBoard (_x, _y);
        _game.SetBoard (_x, _y, _tileNumber);
    }
}

void Object::setXY (uint32_t xx, uint32_t yy) {
    if (_print)
        _game.SetBoard (_x, _y, _oldtileNumber); // Restoring

    _x=xx;
    _y=yy; // Moving : Absolute

    if (_print) {
        _oldtileNumber = _game.GetBoard (_x, _y);
        _game.SetBoard (_x, _y, _tileNumber);
    }
}

InputMapper::InputMapper () :
    _up(sf::Keyboard::Up),
    _down (sf::Keyboard::Down),
    _left (sf::Keyboard::Left),
    _right(sf::Keyboard::Right),
    _fire (sf::Keyboard::Q),

    _tab (sf::Keyboard::Tab),
    _enter (sf::Keyboard::Return),
    _quit (sf::Keyboard::Escape),
    _pause (sf::Keyboard::P) {
}

bool InputMapper::isDownPressed () const {
    return sf::Keyboard::isKeyPressed(_down);
}

bool InputMapper::isUpPressed () const {
    return sf::Keyboard::isKeyPressed(_up);
}

bool InputMapper::isLeftPressed() const {
    return sf::Keyboard::isKeyPressed(_left);
}

bool InputMapper::isRightPressed() const {
    return sf::Keyboard::isKeyPressed(_right);
}

bool InputMapper::isFirePressed() const {
    return sf::Keyboard::isKeyPressed(_fire);
}

bool InputMapper::isTabPressed() const {
    return sf::Keyboard::isKeyPressed(_tab);
}

bool InputMapper::isPausePressed() const {
    return sf::Keyboard::isKeyPressed(_pause);
}

bool InputMapper::isQuitPressed () const {
    return sf::Keyboard::isKeyPressed(_quit);
}

bool InputMapper::isEnterPressed () const {
    return sf::Keyboard::isKeyPressed (_enter);
}

bool InputMapper::readResourceFile (const string &resfile) {
    ifstream filein( resfile.c_str() );
    if( filein ) {

        string line;
        while( getline (filein, line) ) {
            printf ("Reading mapper resource file ...");
            istringstream istr(line);

            string tag;
            istr >> tag;
            if (tag != "KEY")
                continue; // Skipping the line

            string keyDir;
            istr >> keyDir;

            string keyValue;
            istr >> keyValue;
            uint32_t key = atoi (keyValue.c_str());

            if (keyDir == "UP")
                _up = (sf::Keyboard::Key) key;
            else if (keyDir == "DOWN")
                _down = (sf::Keyboard::Key) key;
            else if (keyDir == "LEFT")
                _left = (sf::Keyboard::Key) key;
            else if (keyDir == "RIGHT")
                _right = (sf::Keyboard::Key) key;
            else if (keyDir == "FIRE")
                _fire = (sf::Keyboard::Key) key;

            else if (keyDir == "TAB")
                _enter = (sf::Keyboard::Key) key;
            else if (keyDir == "ENTER")
                _enter = (sf::Keyboard::Key) key;
            else if (keyDir == "QUIT")
                _quit = (sf::Keyboard::Key) key;
            else if (keyDir == "PAUSE")
                _pause = (sf::Keyboard::Key) key;
        }
    } else {
        ofstream fileout( resfile.c_str() );

        if (!fileout) {
            printf ("%s failed : Unable to open %s for writing\n", __FUNCTION__, resfile.c_str());
            return false;
        }

        printf ("Writing mapper resource file %s ...", resfile.c_str());

        fileout << "KEY " << " UP " << _up << endl;
        fileout << "KEY " << " DOWN " << _down << endl;
        fileout << "KEY " << " LEFT " << _left << endl;
        fileout << "KEY " << " RIGHT " << _right << endl;
        fileout << "KEY " << " FIRE " << _fire << endl;

        fileout << "KEY " << " TAB " << _tab << endl;
        fileout << "KEY " << " ENTER " << _enter << endl;
        fileout << "KEY " << " QUIT " << _quit << endl;
        fileout << "KEY " << " PAUSE " << _pause << endl;

        fileout << endl;
    }
    return true;
}

//
// SOUND
//
bool Game::loadSound () {
    if (!_deadBuffer.loadFromFile ("wav/dead.wav"))
        return false;
    if (!_firedBuffer.loadFromFile ("wav/fire.wav"))
        return false;
    if (!_monsterBuffer.loadFromFile ("wav/monster.wav"))
        return false;
    if (!_themeBuffer.loadFromFile ("wav/theme.wav"))
        return false;
    return true;
}

bool Game::playSound (uint32_t idx) {
    switch (idx) {
    case 0:
        _soundChannel.setBuffer (_deadBuffer);
        break;
    case 1:
        _soundChannel.setBuffer (_firedBuffer);
        break;
    case 2:
        _soundChannel.setBuffer (_monsterBuffer);
        break;
    case 3:
        _soundChannel.setBuffer (_themeBuffer);
        break;
    default:
        return false;
        break;
    }

    _soundChannel.play();
    return true;
}

void Game::stopSound () {
    _soundChannel.stop();
}


// -------------------------------
// GAME
// -------------------------------
bool Game::StateMachine () {
    switch (_gameState) {
    case S_INIT:
        if (_inputMapper.isFirePressed()) {
            _gameState = S_RUN;
            while (_inputMapper.isFirePressed()); // WAIT
            stopSound();
        }
        if (_inputMapper.isEnterPressed()) {
            _gameState = S_RUN;
            while (_inputMapper.isEnterPressed()); // WAIT
            stopSound();
        } else if (_inputMapper.isQuitPressed()) {
            while (_inputMapper.isQuitPressed()); // WAIT
            return true;
        }
        break;

    case S_RUN:
        if (_inputMapper.isPausePressed()) {
            _gameState = S_PAUSE;
            while (_inputMapper.isPausePressed()); // WAIT
        } else if (_inputMapper.isQuitPressed()) {
            _gameState = S_INIT;
            while (_inputMapper.isQuitPressed()); // WAIT
        }
        break;

    case S_PAUSE:
        if (_inputMapper.isPausePressed()) {
            _gameState = S_RUN;
            while (_inputMapper.isPausePressed()); // WAIT
        } else if (_inputMapper.isQuitPressed()) {
            _gameState = S_INIT;
            while (_inputMapper.isQuitPressed()); // WAIT
        }
        break;

    case S_END:
        if (_inputMapper.isEnterPressed()) {
            _gameState = S_INIT;
            while (_inputMapper.isQuitPressed()); // WAIT
            playSound(3); // theme
        }
        break;


    default:
        break;
    }
    return false;
}

void Game::Process () {

    if (_gameState != S_RUN)
        return;

    TimeMoveBuissons ();
    TimeMoveMonsters ();
    TimeMovePlayer ();
    TimeFirePlayer ();
    TimeFire ();
    TimeShoot ();
    TimeCreateMonsters ();
    TimeCreateBuissons ();
}

Game::Game () : _scale (1.0f), _player_Object(0), _fire_Object(0), _splash_Object(0) {
    _gameState = S_INIT;
    _score=0; /* POPULATION */
    _day=1;
    _life=10; /* SHOONERS */

    loadTextures (); // Sprites
    buildCharacterTextures (); // Characters for the score

    loadSound(); // Sound

    _inputMapper.readResourceFile ("resource.cfg");

    PrepareGame ();
}

Game::~Game () {
    stopSound();
    DeleteAllObjects ();
}

bool Game::CheckIfBlocked () const {
    uint32_t nb=0;
    for (uint32_t x=12; x<=16; x+=2) {
        uint32_t c=GetBoard (x, 7);
        if (c==CACTUS_1 || c==CACTUS_2)
            nb++;
        c=GetBoard (x, 15);
        if (c==CACTUS_1 || c==CACTUS_2)
            nb++;
    }

    for (uint32_t y=9; y<=13; y+=2) {
        uint32_t c=GetBoard (10, y);
        if (c==CACTUS_1 || c==CACTUS_2)
            nb++;
        c=GetBoard (18, y);
        if (c==CACTUS_1 || c==CACTUS_2)
            nb++;
    }

    if (nb!=12)
        return false;
    return true; // Blocked
}

void Game::ChangeDay () {
    _day++;
    PrepareGame ();
}

void Game::PrepareGame () {

    for (uint32_t j=0; j < 24; j++) {
        for (uint32_t i=0; i < 28; i++) {
            _board [j][i]=FLOOR;
        }
    }

    _fire_Object=0;
    _fire_direction=0;
    _fire_in_progress=NO_FIRE;

    _shoot_item=SI_NONE;

    DeleteAllObjects ();

    /* Autre */
    _mx=0; // Monster
    _my=0;
    _cx=0; // Cactus
    _cy=0;

    _monsterToBeCreated=false;

    /* Les flags de temps */
    _time_create_monster=0; /* Pas de monstre a creer */
    _time_move_monster=0; /* Pas de monstre a deplacer */
    _time_move_buissons=0; /* Pas de buisson a deplacer */
    _time_create_buissons=0; /* Pas de buisson a creer */
    _time_splash=0; /* Pas de splash */

    CreateAllCactus ();
    CreateAllBuissons ();
    CreateAllCages (BLOCK_1);
    CreatePlayer ();
}

void Game::CreatePlayer () {
    if (_player_Object) {
        delete _player_Object;
        _player_Object = 0;
    }

    _player_Object=new Object (PLAYER_1, 14, 11, *this);
    _player_blocked=false;
    _time_player=0;
    _player_direction=1; /* Up */
}

void Game::DeletePlayer () {
    if (_player_Object) {
        delete _player_Object;
        _player_Object=0;
        _player_direction=1;
        _player_blocked=false;
    }
}

void Game::CreateFire (uint32_t tileNumber, uint32_t x, uint32_t y) {
    if (_fire_Object) {
        delete _fire_Object;
        _fire_Object = 0;
    }

    _fire_Object=new Object (tileNumber, x, y, *this, false);
}

void Game::DeleteFire () {
    if (_fire_Object) {
        delete _fire_Object;
        _fire_Object=0;
        _fire_direction=0;
        _fire_in_progress=NO_FIRE;
    }
}

void Game::CreateSplash (uint32_t x, uint32_t y) {
    if (_splash_Object) {
        delete _splash_Object;
        _splash_Object = 0;
        _time_splash = 0;
    }

    _splash_Object=new Object (SPLASH, x, y, *this);
    _time_splash = SHOOT_TICK;
}

void Game::DeleteSplash () {
    if (_splash_Object) {
        delete _splash_Object;
        _splash_Object=0;
    }
}

void Game::DeleteAllObjects () {
    DeleteListOfObjects (_mListOfBuissons);
    DeleteListOfObjects (_mListOfCactus);
    DeleteListOfObjects (_mListOfMonsters);
    DeleteListOfObjects (_mListOfCages);

    DeleteSplash();
    DeletePlayer ();
    DeleteFire ();
}

void Game::CreateCactus (uint32_t tileNum, uint32_t x, uint32_t y) {
    Object *aObject = new Object (tileNum, x, y, *this);
    _mListOfCactus.push_back (aObject);
}

void Game::CreateMonster (uint32_t tileNum, uint32_t x, uint32_t y) {
    Object *aObject = new Object (tileNum, x, y, *this);
    _mListOfMonsters.push_back (aObject);
}

void Game::CreateCage (uint32_t tileNum, uint32_t x, uint32_t y) {
    Object *aObject = new Object (tileNum, x, y, *this);
    _mListOfCages.push_back (aObject);
}

void Game::ChangeLife () {
    playSound (0);

    if (_life <= 0) { /* Game Over */

        CreatePlayer ();
        _gameState = S_END;
        return;
    }

    /* Restart board */
    _fire_in_progress=NO_FIRE;
    _life--;

    DeleteListOfObjects (_mListOfCages);
    DeleteFire ();
    DeletePlayer ();

    CreateAllCages (BLOCK_1);
    CreatePlayer ();
}

void Game::CreateAllCages (uint32_t tileCage) {
    DeleteListOfObjects (_mListOfCages);

    for (uint32_t y=8; y <= 14; y+=2) {
        for (uint32_t x=11; x <= 17; x+=2) {
            CreateCage (tileCage, x, y);
        }
    }
}

void Game::CreateAllCactus () {
    uint32_t tab_dir [8]; /* 8 directions a scruter */
    DeleteListOfObjects(_mListOfCactus);

    uint32_t n=5 + (4 * (_day - 1)); /* Le nombre de cactus total / 2 */
    uint32_t x1 = 0;
    uint32_t y1 = 0;
    uint32_t x2 = 0;
    uint32_t y2 = 0;

    for (uint32_t nb=0; nb < n; ++nb) {
        bool trouve_second=false;
        while (!trouve_second) {
            /* On cherche une premiere case vide */
            bool trouve_first=false;
            while (!trouve_first) {
                x1=rand () % 28;
                y1=(rand () % 18) + 3;
                if (CheckObstacles (x1, y1)==FLOOR)
                    trouve_first=true;
            }

            ShakeDirection (tab_dir);
            for (uint32_t i=0; (i < 8) && (trouve_second==false); ++i) {
                switch (tab_dir [i]) {
                case 0:
                    x2=x1;
                    y2=y1 - 1;
                    break;
                case 1:
                    x2=x1 + 1;
                    y2=y1 - 1;
                    break;
                case 2:
                    x2=x1 + 1;
                    y2=y1;
                    break;
                case 3:
                    x2=x1 + 1;
                    y2=y1 + 1;
                    break;
                case 4:
                    x2=x1;
                    y2=y1 + 1;
                    break;
                case 5:
                    x2=x1 - 1;
                    y2=y1 + 1;
                    break;
                case 6:
                    x2=x1 - 1;
                    y2=y1;
                    break;
                case 7:
                    x2=x1 - 1;
                    y2=y1 - 1;
                    break;
                default:
                    break;
                }

                if (CheckObstacles (x2, y2)==FLOOR)
                    trouve_second=1;
            }
        }

        /* On cree le Object et on l'ajoute a la liste */
        Object *aObject1 = new Object (CACTUS_1, x1, y1, *this);
        _mListOfCactus.push_back (aObject1);

        Object *aObject2 = new Object (CACTUS_1, x2, y2, *this);
        _mListOfCactus.push_back (aObject2);
    }
}

void Game::CreateAllBuissons () {
    const uint32_t n=20; // Number of bushes
    DeleteListOfObjects(_mListOfBuissons);

    for (uint32_t nb=0; nb < n; ++nb) {
        uint32_t x, y;
        FindEmptySpace (x,y);
        Object *aObject = new Object (BUISSON_1, x, y, *this);
        _mListOfBuissons.push_back (aObject);
    }
}

void Game::DeleteListOfObjects (ListOfObjects &objects) {
    for (ListOfObjectsIterator it = objects.begin(); it != objects.end(); ++it) {
        Object *obj = *it;
        if (obj)
            delete obj;
    }
    objects.clear ();
}

/***************/
/* PERIODIQUES */
/***************/
void Game::TimeMovePlayer () {
    if (_time_player!=0) {
        _time_player--;
        return;
    }
    _time_player=PLAYER_TICK;

    uint32_t x, y;
    _player_Object->getXY(x,y);

    if (_inputMapper.isDownPressed()) { /* DOWN */
        if (_player_Object->getTileNumber() != PLAYER_3) {
            _player_Object->setTileNumber (PLAYER_3);
            _player_direction=3;
            _time_player=PLAYER_TICK_CHANGE;
        } else if (y < 20 && _board [y+1][x]==0) {
            _player_Object->incXY(0, 1);
        }
    } else if (_inputMapper.isUpPressed()) { /* UP */
        if (_player_Object->getTileNumber () != PLAYER_1) {
            _player_Object->setTileNumber (PLAYER_1);
            _player_direction=1;
            _time_player=PLAYER_TICK_CHANGE;
        } else if (y > 3 && _board [y-1][x]==0) {
            _player_Object->incXY (0, -1);
        }
    } else if (_inputMapper.isLeftPressed ()) { /* LEFT */
        if (_player_Object->getTileNumber () != PLAYER_4) {
            _player_Object->setTileNumber (PLAYER_4);
            _player_direction=4;
            _time_player=PLAYER_TICK_CHANGE;
        } else if (x > 0 && _board [y][x-1]==0) {
            _player_Object->incXY(-1, 0);
        }
    } else if (_inputMapper.isRightPressed ()) { /* RIGHT */
        if (_player_Object->getTileNumber () != PLAYER_2) {
            _player_Object->setTileNumber (PLAYER_2);
            _player_direction=2;
            _time_player=PLAYER_TICK_CHANGE;
        } else if (x < 27 && _board [y][x+1]==0) {
            _player_Object->incXY(1, 0);
        }
    } else if (_inputMapper.isTabPressed()) { /* PANIC */
        _player_Object->setXY(14, 11);
        _player_direction=1; /* Up */
        _player_Object->setTileNumber (PLAYER_1);
        _score=0;
    }
}


void Game::TimeFirePlayer () {
    if (_fire_in_progress != NO_FIRE)
        return;

    if (!_inputMapper.isFirePressed ())
        return; // No fire wanted

    playSound (1);

    _fire_in_progress=FIRE_IN_PROGRESS;
    _fire_direction=_player_direction;
    _shoot_item=SI_NONE;

    uint32_t tile_direction=TIR_1;
    // Initial direction of fire (vertical or horizontal)
    switch (_fire_direction) {
    case 1:
    case 3:
        tile_direction=TIR_1; // UP - DOWN
        break;

    case 2:
    case 4:
        tile_direction=TIR_2; // RIGHT - LEFT
        break;
    default:
        break;
    }

    uint32_t ax,ay;
    _player_Object->getXY(ax, ay);
    //_fire_Object=new Object (tile_direction, ax, ay, *this); // !!! DO NOT MARK THE BOARD
    CreateFire (tile_direction, ax, ay);

}

void Game::TimeFire () {
    if (_fire_in_progress != FIRE_IN_PROGRESS)
        return;

    uint32_t ax, ay;
    _fire_Object->getXY(ax,ay);

    // Computing next position of the fire
    switch (_fire_direction) {
    case 1:
        ay -= 1;
        break; // UP
    case 2:
        ax += 1;
        break; // RIGHT
    case 3:
        ay += 1;
        break; // DOWN
    case 4:
        ax -= 1;
        break; // LEFT
    default:
        break;
    }

    // Checking the position of the fire
    int bounces=CheckBounces (ax, ay);

    switch (bounces) {
    case FLOOR:
        _fire_Object->setXY (ax, ay); // Moving the fire
        break;

    case OUT:
    case BLOCK_1:
    case BLOCK_2:
    case CACTUS_1:
    case CACTUS_2:
        DeleteFire (); // No more fire needed
        _fire_in_progress=NO_FIRE;
        break;

    case BUISSON_1:
    case BUISSON_2:
        playSound (2);
        DeleteFire ();

        _fire_in_progress=FIRE_SPLASH;
        _shoot_item=SI_BUSH;

        RemoveXYObject (_mListOfBuissons, ax, ay);
        CreateSplash (ax, ay);

        _score+=100;
        break;

    case MONSTER_1:
    case MONSTER_2: {
        playSound (2);
        DeleteFire ();

        _fire_in_progress=FIRE_SPLASH;
        _shoot_item=SI_MONSTER;

        RemoveXYObject (_mListOfMonsters, ax, ay);
        CreateSplash (ax, ay);
        _score+=150;
    }
    break;

    default:
        break;
    }
}

void Game::TimeShoot () {
    if (_fire_in_progress != FIRE_SPLASH)
        return;

    if (_time_splash > 0) {
        _time_splash--;
        return;
    }

    uint32_t x,y;
    _splash_Object->getXY (x,y); // Position of the item
    DeleteSplash (); // Removing the Splash

    // Creating or changing according to the fired item
    if (_shoot_item==SI_MONSTER) {
        CreateCactus (CACTUS_1, x, y);
        ReplaceCactusByMonster (x, y);
    } else if (_shoot_item==SI_BUSH) {
        ;
    }

    _shoot_item=SI_NONE;
    _fire_in_progress=NO_FIRE;
}

void Game::TimeCreateBuissons () {
    if ( _mListOfBuissons.empty ()==true) {
        if (_time_create_buissons >= CREATE_BUISSONS_TICK) {
            _time_create_buissons=0;
            CreateAllBuissons ();
        } else
            _time_create_buissons++;
    }
}

void Game::TimeMoveBuissons () {
    if ( _mListOfBuissons.empty ()==true)
        return;

    if (_time_move_buissons==MOVE_BUISSONS_TICK) {
        uint32_t px, py;
        _player_Object->getXY (px,py);

        ListOfObjectsIterator it=_mListOfBuissons.begin();
        while (it != _mListOfBuissons.end ()) {
            Object *aObject=*it;
            ++it;
            uint32_t x,y;
            aObject->getXY (x,y);


            bool moving = false;
            if ((x < px) && (y==py)) { // Moving left
                if ( CheckObstacles (x - 1, y)==FLOOR) {
                    moving=true;
                    aObject->incXY (-1, 0);
                }
            } else if ((x > px) && (y==py)) { // Moving right
                if ( CheckObstacles (x + 1, y)==FLOOR) {
                    moving=true;
                    aObject->incXY (1, 0);
                }
            } else if ((y < py) && (x==px)) { // Moving up
                if ( CheckObstacles (x, y - 1)==FLOOR) {
                    moving=true;
                    aObject->incXY (0, -1);
                }
            } else if ((y > py) && (x==px)) { // Moving down
                if ( CheckObstacles (x, y + 1)==FLOOR) {
                    moving=true;
                    aObject->incXY (0, 1);
                }
            }


            if (moving) { // Changing aspect
                if (aObject->getTileNumber()==BUISSON_1)
                    aObject->setTileNumber (BUISSON_2);
                else
                    aObject->setTileNumber (BUISSON_1);
            }
        }

        _time_move_buissons=0;
    } else {
        _time_move_buissons++;
    }
}

void Game::FindEmptySpace (uint32_t &x, uint32_t &y) const {
    // Try to find an empty space
    while (1) {
        x=rand () % 28;
        y=(rand () % 18) + 3;
        if (CheckObstacles (x, y)==FLOOR)
            return;
    }
}

void Game::TimeCreateMonsters () {
    /* Changement du type de cage au bout de 5 secondes pour annoncer l'apparition d'un monstre */
    if (_mListOfMonsters.size() > 40)
        return; // No more than 40 monster

    if (_time_create_monster==CHANGE_CAGE_TICK) {

        if (CheckIfBlocked ()) { // Is there an exit into the cages ?
            if (_player_blocked==false) { // Stick it up !
                uint32_t xx,yy;
                FindEmptySpace (xx,yy);
                _player_Object->setXY(xx,yy);
                _player_blocked=true;
            }
        } else {
            _player_blocked=false;
        }

        CreateAllCages (BLOCK_2);
        _time_create_monster++;

        /* On regarde si l'on peut creer un monstre */
        if ((_monsterToBeCreated=ComputeNewMonsterPosition ())==true) {
            /* mx, my, cx et cy sont initialises */
            /* On remplace le cactus pres duquel va apparaitre le monstre */
            if (RemoveXYObject (_mListOfCactus, _cx, _cy))
                CreateCactus (CACTUS_2, _cx, _cy);
        }
    }

    if (_time_create_monster >= CREATE_MONSTERS_TICK) {
        _time_create_monster=0;

        /* Changement de jour si plus possible de creer de monstres et plus de monstre */
        if (!_monsterToBeCreated && _mListOfMonsters.empty())
            ChangeDay ();

        else {
            /* Soit il reste des monstres, soit il faut en creer 1 */
            /* Remplacement des cages et apparation d'un monstre au bout de 7 secondes */

            CreateAllCages (BLOCK_1);

            /* On remplace le cactus pres duquel va apparaitre le monstre */
            /* et on cree un monstre si le cactus existe encore */
            if (RemoveXYObject (_mListOfCactus, _cx, _cy)) { /* Si cactus existe encore on doit creer un monstre */

                CreateCactus (CACTUS_1, _cx, _cy);

                if (_monsterToBeCreated) {
                    _monsterToBeCreated=false;
                    CreateMonster (MONSTER_1, _mx, _my);
                }
            } else { /* Le cactus n'existe plus, on n'a plus besoin de creer le monstre */
                _monsterToBeCreated=false;
            }
        }
    } else
        _time_create_monster++;
}

void Game::TimeMoveMonsters () {
    if (_time_move_monster >= MOVE_MONSTERS_TICK) {
        _time_move_monster = 0;
        MoveMonsters ();
    } else
        _time_move_monster++;
}

/***************/
/* UTILITAIRES */
/***************/
bool Game::MoveMonsterX (Object *aObject, int32_t dx) {
    /* On cherche en premier un depacement en horizontal */
    int32_t incx=sign (dx);
    int32_t incy=0;
    if (incx==1 || incx==-1) {
        uint32_t check=CheckObstacles (aObject->getX() + incx, aObject->getY() + incy);

        /* Hit ? */
        if (check==PLAYER_1 || check==PLAYER_2 || check==PLAYER_3 || check==PLAYER_4) {
            ChangeLife ();
            return true;
        } else if (check==FLOOR) { /* Deplacement possible */
            aObject->incXY (incx, incy);
            return true;
        } else { /* Il faut essayer un deplacement en y */
            incy=(rand () % 3) -1;
            if (incy==0)
                incx=sign (dx);
            else
                incx=0;

            uint32_t check=CheckObstacles (aObject->getX () + incx, aObject->getY () + incy);

            /* Hit ? */
            if (check==PLAYER_1 || check==PLAYER_2 || check==PLAYER_3 || check==PLAYER_4) {
                ChangeLife ();
                return true;
            } else if (check==FLOOR) { /* Deplacement possible */
                aObject->incXY (incx, incy);
                return true;
            }
        }
    }

    return false;
}

bool Game::MoveMonsterY (Object *aObject, int32_t dy) {
    /* On cherche en premier un deplacement en vertical */
    int32_t incy=sign (dy);
    int32_t incx=0;
    if (incy==1 || incy==-1) {
        uint32_t check=CheckObstacles (aObject->getX() + incx, aObject->getY () + incy);

        /* Hit ? */
        if (check==PLAYER_1 || check==PLAYER_2 || check==PLAYER_3 || check==PLAYER_4) {
            ChangeLife ();
            return true;
        } else if (check==FLOOR) { /* Deplacement possible */
            aObject->incXY (incx, incy);
            return true;
        } else { /* Il faut essayer un deplacement en x */
            /* incx=sign (dx); */
            incx=(rand () % 3) -1;
            if (incx==0)
                incy=sign (dy);
            else
                incy=0;

            uint32_t check=CheckObstacles (aObject->getX () + incx, aObject->getY () + incy);

            /* Hit ? */
            if (check==PLAYER_1 || check==PLAYER_2 || check==PLAYER_3 || check==PLAYER_4) {
                ChangeLife ();
                return true;
            } else if (check==FLOOR) { /* Deplacement possible */
                aObject->incXY (incx, incy);
                return true;
            }
        }
    }

    return false;
}

void Game::MoveMonsters () {
    uint32_t xp, yp;
    _player_Object->getXY (xp,yp);

    ListOfObjectsIterator it= _mListOfMonsters.begin ();

    while (it != _mListOfMonsters.end ()) {
        Object *aObject=*it;
        ++it;

        /* Changement de look */
        if (aObject->getTileNumber ()==MONSTER_1)
            aObject->setTileNumber (MONSTER_2);
        else
            aObject->setTileNumber (MONSTER_1);

        int32_t dx=xp - aObject->getX ();
        int32_t dy=yp - aObject->getY ();

        /* Tirage au sort pour ne pas privilegier une direction */
        if (rand () % 2==0) {
            /* Si on peut se deplacer en X on ne se deplace pas en Y */
            if (!MoveMonsterX (aObject, dx))
                MoveMonsterY (aObject, dy);
        } else {
            /* Si on peut se deplacer en Y on ne se deplace pas en X */
            if (!MoveMonsterY (aObject, dy))
                MoveMonsterX (aObject, dx);
        }
    }
}

/* Calcule la position d'un nouveau monstre sans le creer */
/* 1 : Position obtenue */
/* 0 : pas de monstre en vu */
bool Game::ComputeNewMonsterPosition () {
    ListOfObjectsIterator it= _mListOfCactus.begin ();
    while (it != _mListOfCactus.end ()) {
        Object *aObject=*it;
        ++it;
        uint32_t x, y;
        aObject->getXY (x,y);

        _cx=x;
        _cy=y;

        /* X+1, Y */
        if (CheckObstacles (x+1, y)==CACTUS_1) {
            if (CheckObstacles (x, y-1)==FLOOR) {
                _mx=x;
                _my=y-1;
                return true;
            }
            if (CheckObstacles (x, y+1)==FLOOR) {
                _mx=x;
                _my=y+1;
                return true;
            }
            if (CheckObstacles (x+1, y-1)==FLOOR) {
                _mx=x+1;
                _my=y-1;
                return true;
            }
            if (CheckObstacles (x+1, y+1)==FLOOR) {
                _mx=x+1;
                _my=y+1;
                return true;
            }
        }

        /* X-1, Y */
        if (CheckObstacles (x-1, y)==CACTUS_1) {
            if (CheckObstacles (x, y-1)==FLOOR) {
                _mx=x;
                _my=y-1;
                return true;
            }
            if (CheckObstacles (x, y+1)==FLOOR) {
                _mx=x;
                _my=y+1;
                return true;
            }
            if (CheckObstacles (x-1, y-1)==FLOOR) {
                _mx=x-1;
                _my=y-1;
                return true;
            }
            if (CheckObstacles (x-1, y+1)==FLOOR) {
                _mx=x-1;
                _my=y+1;
                return true;
            }
        }

        /* X, Y+1 */
        if (CheckObstacles (x, y+1)==CACTUS_1) {
            if (CheckObstacles (x-1, y)==FLOOR) {
                _mx=x-1;
                _my=y;
                return true;
            }
            if (CheckObstacles (x+1, y)==FLOOR) {
                _mx=x+1;
                _my=y;
                return true;
            }
            if (CheckObstacles (x-1, y+1)==FLOOR) {
                _mx=x-1;
                _my=y+1;
                return true;
            }
            if (CheckObstacles (x+1, y+1)==FLOOR) {
                _mx=x+1;
                _my=y+1;
                return true;
            }
        }

        /* X, Y-1 */
        if (CheckObstacles (x, y-1)==CACTUS_1) {
            if (CheckObstacles (x-1, y)==FLOOR) {
                _mx=x-1;
                _my=y;
                return true;
            }
            if (CheckObstacles (x+1, y)==FLOOR) {
                _mx =x+1;
                _my=y;
                return true;
            }
            if (CheckObstacles (x-1, y-1)==FLOOR) {
                _mx=x-1;
                _my=y-1;
                return true;
            }
            if (CheckObstacles (x+1, y-1)==FLOOR) {
                _mx=x+1;
                _my=y-1;
                return true;
            }
        }

        /* X+1, Y-1 */
        if (CheckObstacles (x+1, y-1)==CACTUS_1) {
            if (CheckObstacles (x, y-1)==FLOOR) {
                _mx=x;
                _my=y-1;
                return true;
            }
            if (CheckObstacles (x+1, y)==FLOOR) {
                _mx=x+1;
                _my=y;
                return true;
            }
        }

        /* X+1, Y+1 */
        if (CheckObstacles (x+1, y+1)==CACTUS_1) {
            if (CheckObstacles (x, y+1)==FLOOR) {
                _mx=x;
                _my=y+1;
                return true;
            }
            if (CheckObstacles (x+1, y)==FLOOR) {
                _mx=x+1;
                _my=y;
                return true;
            }
        }

        /* X-1, Y-1 */
        if (CheckObstacles (x-1, y-1)==CACTUS_1) {
            if (CheckObstacles (x, y-1)==FLOOR) {
                _mx=x;
                _my=y-1;
                return true;
            }
            if (CheckObstacles (x-1, y)==FLOOR) {
                _mx=x-1;
                _my=y;
                return true;
            }
        }

        /* X-1, Y+1 */
        if (CheckObstacles (x-1, y+1)==CACTUS_1) {
            if (CheckObstacles (x, y+1)==FLOOR) {
                _mx=x;
                _my=y+1;
                return true;
            }
            if (CheckObstacles (x-1, y)==FLOOR) {
                _mx=x-1;
                _my=y;
                return true;
            }
        }
    }

    return false;
}

uint32_t Game::CheckBounces (uint32_t x, uint32_t y) const {
    //if (x < 0) return OUT; // Always positive or null
    if (x > 27)
        return OUT;
    if (y < 3)
        return OUT;
    if (y > 20)
        return OUT;

    return (_board [y][x]);
}

uint32_t Game::CheckObstacles (uint32_t x, uint32_t y) const {
    //if (x < 0) return OUT;
    if (x > 27)
        return OUT;
    if (y < 3)
        return OUT;
    if (y > 20)
        return OUT;
    if (x >= 11 && x <= 17 && y >= 8 && y <= 14)
        return BLOCK_1;

    return (_board [y][x]);
}

bool Game::RemoveXYObject (ListOfObjects &liste, uint32_t x, uint32_t y) {
    for (ListOfObjectsIterator it=liste.begin (); it != liste.end (); ++it) {
        Object *aObject=*it;
        if (aObject->getX()==x && aObject->getY()==y) {
            liste.erase (it);
            delete aObject;
            return true; /* OK removed */
        }
    }
    return false; /* Nothing to remove */
}

void Game::ReplaceCactusByMonster (uint32_t x, uint32_t y) {
    ListOfPoints aList=ComputeThreeCactusList (x, y);
    if (aList.empty ())
        return; // No cactus to replace

    // Replacing the cactus by a monster
    RemoveXYObject (_mListOfCactus, x, y);

    for (ListOfPointsIterator it=aList.begin (); it != aList.end (); ++it) {
        FPoint & aPoint=*it;
        RemoveXYObject (_mListOfCactus, aPoint.getX(), aPoint.getY());
    }
    CreateMonster (MONSTER_1, x, y);
}

ListOfPoints Game::ComputeThreeCactusList (uint32_t x, uint32_t y) {
    /* Making a first list */
    ListOfPoints firstList;
    for (int32_t j=-1; j < 2; j++) { /* -1, 0, 1 */
        for (int32_t i=-1; i < 2; i++) { /* -1, 0, 1 */
            if ((j==0) && (i==0)) ;
            else {
                uint32_t check=CheckObstacles (x+i, y+j);
                if ( check==CACTUS_1 || check==CACTUS_2 ) {
                    firstList.push_back (FPoint (x+i, y+j));
                }
            }
        }
    }

    if (firstList.empty ()==true) {
        return firstList; /* Aucun voisin a (x,y) */
    }

    if (firstList.size () > 1) {/* On arrete les recherches, la liste contient au moins 3 cactus */
        return firstList;
    }
    /* Taking the only one elmt */
    FPoint aPoint;
    aPoint=firstList.front ();

    uint32_t xx, yy;
    aPoint.getXY(xx,yy);

    /* Making a second list */
    ListOfPoints secondList;
    for (int32_t j=-1; j < 2; j++) { /* -1, 0, 1 */
        for (int32_t i=-1; i < 2; i++) { /* -1, 0, 1 */
            uint32_t check=CheckObstacles (xx+i, yy+j);
            if ( check==CACTUS_1 || check==CACTUS_2) {
                secondList.push_back (FPoint (xx+i, yy+j));
            }
        }
    }

    /* The Truth */
    if (secondList.size ()==2) {
        secondList.clear (); /* Aucun voisin a (x,y). Empty list*/
    }

    return secondList;
}

/* Shake : melange 8 directions (0-7) */
void ShakeDirection (uint32_t tab_dir[]) {
    uint32_t tab_indices[8];

    /* prepare les tables de Shake-up */
    for (uint32_t i=0; i<8; ++i) {
        tab_dir[i]=0;
        tab_indices[i]=i;
    }

    /* melange des directions */
    for (uint32_t n_indices=8, i=0; i<8; ++i) {
        uint32_t index=rand() % n_indices;	/* prend un indice dans la */
        /* table des cases vides */
        tab_dir[i]=tab_indices[index] + 1;
        --n_indices;	/* 1 direction de moins */
        /* suppression de la direction dans table */
        for (uint32_t j=index; j<n_indices; ++j)
            tab_indices[j]=tab_indices[j+1];
    }
}

int32_t sign (int32_t val) {
    if (val < 0)
        return -1;
    if (val > 0)
        return 1;
    return 0;
}

// --------------------------
// MAIN
// --------------------------
int main (int argc, char *argv[]) {
    // Graphical init
    sf::RenderWindow window(sf::VideoMode(256, 192), "Tombstone City");

    // Init random generator
    struct timeval td;
    gettimeofday (&td, NULL);
    srand (td.tv_sec);

    Game game;

    game.playSound(3); // theme

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time time = clock.getElapsedTime();
        if (time.asMilliseconds() >= 20) { // 50 pfs
            clock.restart();

            bool fin = game.StateMachine();
            if (fin)
                window.close();
            else {
                game.Process();
                game.Display(window);
            }
        }
    }

    return 0;
}

bool Game::loadTexture (uint32_t tileNumber, const string &filename) {
    sf::Texture texture;
    string path = "./icons/";
    string fullname = path + filename + ".gif";
    bool result = texture.loadFromFile (fullname);
    if (result) {
        _mapOfTextures[tileNumber] = texture;
    }
    return result;
}

bool Game::loadTextures () {
    loadTexture (FLOOR,        "floor");
    loadTexture (CACTUS_1,     "cactus_1");
    loadTexture (CACTUS_2,     "cactus_2");
    loadTexture (CACTUS_2,     "cactus_2");
    loadTexture (PLAYER_1,     "player_1");
    loadTexture (PLAYER_2,     "player_2");
    loadTexture (PLAYER_3,     "player_3");
    loadTexture (PLAYER_4,     "player_4");
    loadTexture (MONSTER_1,    "monster_1");
    loadTexture (MONSTER_2,    "monster_2");
    loadTexture (TIR_1,        "tir_1");
    loadTexture (TIR_2,        "tir_2");
    loadTexture (SPLASH,       "splash");
    loadTexture (BUISSON_1,    "buisson_1");
    loadTexture (BUISSON_2,    "buisson_2");
    loadTexture (BLOCK_1,      "block_1");
    loadTexture (BLOCK_2,      "block_2");
    loadTexture (SCORE,        "score");
    loadTexture (BOARD,        "board");
    loadTexture (GAMEOVER,     "gameover");
    loadTexture (LOWER_SCREEN, "lower_screen");
    return true;
}

void Game::buildCharacterTextures () {
    for (uint32_t i = 0; i < 96; ++i) { // 96 Tiles
        sf::Image img;
        sf::Color c = sf::Color::Green;
        //c.a = 0;
        img.create(8, 8, c); // Transparent
        const char *s = TabDefaultCharPattern [i];
        uint32_t len = strlen (s);
        for (uint32_t y=0, idx=0; idx < len; idx+=2, ++y) {
            uint32_t val = 0;
            sscanf (&s[idx], "%2X", &val);
            for (uint32_t x = 0; x<8; ++x) {
                if (val & 0x80) {
                    img.setPixel(x, y, sf::Color::Black);
                }
                else {
                    sf::Color ct = sf::Color::Black;
                    ct.a = 0; // Transparent background
                    img.setPixel(x, y, ct);
                }
                val = val << 1;
            }
        }

        sf::Texture t;
        t.loadFromImage(img);
        _vectOfCaracters.push_back(t);
    }
}

uint32_t Game::GetPatternNumber (const char c) {
    // TI99 characters are NOT ASCII
    if (c>= '0' && c <= '9') return (c-'0') + 16;
    if (c>= 'A' && c <= 'Z') return (c-'A') + 33;
    if (c>= 'a' && c <= 'z') return (c-'a') + 65;
    return 0; // Not managed yet
}

void Game::DisplayValue (sf::RenderWindow &window, uint32_t value, uint32_t col, uint32_t row) {
    char buf [10];
    sprintf (buf, "%d", value);
    string s = buf;
    uint32_t l=s.size();
    uint32_t c = col - l + 1;
    DisplayString (window, s, c, row);
}

void Game::DisplayString (sf::RenderWindow &window, const string &s, uint32_t col, uint32_t row) {
    uint32_t l = s.size();
    for (uint32_t i=0; i<l; ++i) {
        sf::Sprite sp;
        uint32_t idx = GetPatternNumber (s.at(i));
        sp.setTexture(getCharacterTexture(idx));
        uint32_t x = (col+i)*8;
        uint32_t y = row*8;
        sp.setPosition (sf::Vector2f(x, y));
        window.draw(sp);
    }
}

void Game::Display (sf::RenderWindow &window) {

    window.clear(sf::Color::Black);

    // Display BOARD
    sf::Sprite bd;
    bd.setTexture(getTexture (BOARD));
    window.draw(bd);

   // Display list of Objects
    DisplayListOfObjects (window, _mListOfCactus);
    DisplayListOfObjects (window, _mListOfBuissons);
    DisplayListOfObjects (window, _mListOfMonsters);
    DisplayListOfObjects (window, _mListOfCages);

    DisplayObject (window, _player_Object);
    DisplayObject (window, _splash_Object);
    DisplayObject (window, _fire_Object);

    // Display messages
    switch (_gameState) {
    case Game::S_INIT: {
        DisplayString (window, "Press FIRE to start", 6, 16);
        DisplayString (window, "Press ESC to quit", 8, 18);
        DisplayString(window, "Press P to pause", 8, 20);
    }
    break;

    case Game::S_RUN:
        break;

    case Game::S_PAUSE: {
        DisplayString(window, "Press P to continue", 6, 16);
    }
    break;
    case Game::S_END: {
        DisplayString(window, "Press ENTER to restart", 6, 16);
    }
    break;

    default:
        break;
    }

    DisplayValue (window, _day, 4, 23); // Day
    DisplayValue (window, _score, 13, 23); // Population
    DisplayValue (window, _life, 24, 23); // Shooners
    window.display();
}

void Game::DisplayObject (sf::RenderWindow &window, Object *obj) {
    if (!obj) return;
    sf::Sprite so;
    uint32_t x,y;
    obj->getXY(x,y);
    x+=2; // To center the area
    so.setTexture(getTexture (obj->getTileNumber()));
    so.setScale (_scale, _scale);
    so.setPosition(sf::Vector2f(x*8*_scale, y*8*_scale));
    window.draw(so);
}

void Game::DisplayListOfObjects (sf::RenderWindow &window, const ListOfObjects &mListOfObjects) {
    for (list<Object*>::const_iterator it = mListOfObjects.begin(); it != mListOfObjects.end(); ++it) {
        DisplayObject (window, *it);
    }
}
