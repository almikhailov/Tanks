#include <iostream>
#include <windows.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#include "rlutil.h"


#define LIGHTRED 12
#define LIGHTGRAY 7
#define YELLOW 14
#define BLACK 0
#define WHITE 15
#define LIGHTBLUE 9


#define MOVE_UP   72
#define MOVE_DOWN 80
#define MOVE_LEFT 75
#define MOVE_RIGHT 77



#define SHOOT_ENEMY 255
#define SHOOT ' '


// BORDER SETTINGS
#define LIMIT_UP 1
#define LIMIT_DOWN 29
#define LIMIT_RIGHT 119
#define LIMIT_LEFT 1




#define MAX_LIVES 3
#define MAX_BULLETS 100
#define CHARACTER_BULLET 158


#define CHARACTER_TANK 219
#define CHARACTER_ENEMY 178



#define CHAR_WALL 254

std::vector <class WALL *> vecWall;
bool checkCoordsWall(std::vector<WALL *> tetmp,int tempX,int tempY);

void paint_bordes();

using namespace std;

class TANK;

class GAME_TIME {
private:
   int g_time;
public:
   GAME_TIME(int);
   void print_time();
   int get_time();
};

GAME_TIME::GAME_TIME(int g_time) { this->g_time = g_time; }

int GAME_TIME::get_time() { return g_time; }

void GAME_TIME::print_time(){
   gotoxy(60, 1); printf("%5d", g_time);
   g_time++;
}

class BULLET {
private:
	int x, y;
	int direccion;
	TANK *t;
	bool state;      // live or dead
public:
   BULLET(int, int, int);
	int getX();
	int getY();

	bool getState();
	void setState(bool);
    void die();
	void print();
	void mover();
	void del();
};

// At the time of its creation, the bullet has a predetermined address
BULLET::BULLET(int x, int y, int direccion) {
    this->x = x;
    this->y = y;
    this->direccion = direccion;
    this->state = true;
}



int BULLET::getX() { return x; }
int BULLET::getY() { return y; }

void BULLET::print() {
   if (!getState()) return;
   gotoxy(x, y); rlutil::setColor(LIGHTRED); printf("%c", CHARACTER_BULLET); rlutil::setColor(LIGHTGRAY);
}

void BULLET::del() {
   gotoxy(x, y); printf(" ");
}

void BULLET::mover() {
   del();

   if (direccion == MOVE_UP && y >= LIMIT_UP + 1) {
      if (y == LIMIT_UP + 1) {
         state = false;
      } else {
         y--;
      }
   }

   if (direccion == MOVE_DOWN && y <= LIMIT_DOWN - 1) {
      if (y == LIMIT_DOWN - 1) {
         state = false;
      } else {
         y++;
      }
   }

   if (direccion == MOVE_LEFT && x >= LIMIT_LEFT + 1) {
      if (x == LIMIT_LEFT + 1) {
         state = false;
      } else {
         x--;
      }
   }

   if (direccion == MOVE_RIGHT && x <= LIMIT_RIGHT - 1) {
      if (x == LIMIT_RIGHT - 1) {
         state = false;
      } else {
         x++;
      }
   }

   if (state) print();
}

bool BULLET::getState() {
   return state;
}

void BULLET::setState(bool state) {
   this->state = state;
}

class TANK {
private:
	int x, y;
   int direccion;
   int kills;
   int lives;
   int max_bullets;
public:
   vector <BULLET *> bullets;
	TANK(int, int);
	int getX();
	int getY();
	int getMax_bullets();
	int getLives();
	void setLives(int);
	int getKills();
	void setKills(int);
	void setX(int);
   void setY(int);
	void setDireccion(int);
	int getDireccion();
	void print();
	void printUp();
	void printDown();
	void printLeft();
	void printRight();
	void printLivesKills();
	void del();
	void mover();
	void mover_bullets();
	void die(BULLET *bullet);
};

TANK::TANK(int x, int y) {
   this->x = x;
   this->y = y;
   direccion = MOVE_UP;
   lives = MAX_LIVES;
   kills = 0;
   max_bullets = MAX_BULLETS;
}

int TANK::getX() {
	return x;
}

int TANK::getY() {
	return y;
}

int TANK::getMax_bullets() {
   return max_bullets;
}

void TANK::setX(int x) {
   this->x = x;
}

void TANK::setY(int y) {
   this->y = y;
}

int TANK::getKills() {
   return kills;
}

void TANK::setKills(int kills) {
   this->kills = (kills >= 0) ? kills : 0;
}

int TANK::getLives() {
   return lives;
}

void TANK::setLives(int lives) {
   this->lives = (lives >= 0) ? lives : 3;
}

void TANK::setDireccion(int direccion) {
   if (direccion == MOVE_UP || direccion == MOVE_DOWN || direccion == MOVE_LEFT || direccion == MOVE_RIGHT) {
      this->direccion = direccion;
   }
}

int TANK::getDireccion() {
    return direccion;
}

void TANK::printUp() {
	gotoxy(x, y); printf(" %c", CHARACTER_TANK);
	gotoxy(x, y + 1);	printf("%c%c%c", CHARACTER_TANK, CHARACTER_TANK, CHARACTER_TANK);
	gotoxy(x, y + 2);	printf("%c %c", CHARACTER_TANK, CHARACTER_TANK);
}

void TANK::printDown() {
	gotoxy(x, y);	printf("%c %c", CHARACTER_TANK, CHARACTER_TANK);
	gotoxy(x, y + 1); printf("%c%c%c", CHARACTER_TANK, CHARACTER_TANK, CHARACTER_TANK);
	gotoxy(x, y + 2); printf(" %c ", CHARACTER_TANK);
}

void TANK::printRight() {
   gotoxy(x, y); printf("%c%c", CHARACTER_TANK, CHARACTER_TANK);
	gotoxy(x, y + 1); printf(" %c%c", CHARACTER_TANK, CHARACTER_TANK);
	gotoxy(x, y + 2);	printf("%c%c", CHARACTER_TANK, CHARACTER_TANK);
}

void TANK::printLeft() {
	gotoxy(x, y); printf(" %c%c", CHARACTER_TANK, CHARACTER_TANK);
	gotoxy(x, y + 1); printf("%c%c", CHARACTER_TANK, CHARACTER_TANK);
	gotoxy(x, y + 2);	printf(" %c%c", CHARACTER_TANK, CHARACTER_TANK);
}

void TANK::printLivesKills() {
   gotoxy(1, 2); cout << "Lives: " << getLives();
   gotoxy(1, 3); cout << "Kills: " << getKills();
}

void TANK::print() {
   switch(direccion) {
      case MOVE_UP:
         printUp();
         break;
      case MOVE_DOWN:
         printDown();
         break;
      case MOVE_LEFT:
         printLeft();
         break;
      case MOVE_RIGHT:
         printRight();
         break;
   }
   /* Paint the heart of the tank of some color*/
   gotoxy(x + 1, y + 1);
   rlutil::setColor(LIGHTRED); rlutil::setBackgroundColor(YELLOW);
   printf("H");
   rlutil::setColor(LIGHTGRAY); rlutil::setBackgroundColor(BLACK);
   this->printLivesKills();
}

void TANK::del() {
   gotoxy(x, y); printf("   ");
   gotoxy(x, y + 1); printf("   ");
   gotoxy(x, y + 2); printf("   ");
}

void TANK::mover() {
    int oldX;
    int oldY;
    oldX=x;
    oldY=y;

   if (kbhit()) {
      char kbchar = getch();

      del();


      if (kbchar == MOVE_LEFT) {
         if (direccion == MOVE_LEFT)
            if (x > LIMIT_LEFT + 1) x--;
      }

      if (kbchar == MOVE_RIGHT) {
         if (direccion == MOVE_RIGHT)
            if ((x + 2) < LIMIT_RIGHT - 1) x++;
      }

      if (kbchar == MOVE_DOWN) {
         if (direccion == MOVE_DOWN)
            if ((y + 2) < LIMIT_DOWN - 1)y++;
      }

      if (kbchar == MOVE_UP) {
         if (direccion == MOVE_UP)
            if (y > LIMIT_UP + 1) y--;
      }

      bool tempfl=true;
      for (int tempxx=0; tempxx<=2; tempxx++){
      for (int tempyy=0; tempyy<=2; tempyy++){
      if (!checkCoordsWall(vecWall, x+tempxx ,y+tempyy)) tempfl=false;
      }};

       if ( !tempfl )
       {
        x=oldX;
        y=oldY;
       };



      if (kbchar == SHOOT) {


         if (bullets.size() >= max_bullets) return;

         BULLET *b;
         do {
            b = new BULLET(this->x + 1, this->y + 1, this->direccion);
            //
            if (b == NULL) {

               gotoxy(x, y); cout << "IT WAS NOT POSSIBLE TO OBTAIN MEMORY";
               system("pause>nul");
               system("pause>nul");
               system("pause>nul");
               system("pause>nul");
            }
            //
         } while (b ==NULL);
         bullets.push_back(b);


         int i = bullets.size();

      } else {
         setDireccion(kbchar);
      }

   }
}

//Move all the bullets and also remove them from memory

void TANK::mover_bullets() {
   // You have to print the tank again
   this->print();

     for (int i = 0; i < bullets.size(); ++i) {
      bullets[i]->mover();
      this->print();
      if (bullets[i]->getState() == false) {
         bullets[i]->del();
         delete (bullets[i]);
         bullets[i] = NULL;
      }
   }

   int i = 0;
   while (i < bullets.size()) {
      if (bullets[i] == NULL) {
         bullets.erase(bullets.begin() + i);
      } else {
         ++i;
      }
   }
}

void TANK::die(BULLET *bullet) {
   del();
   cout << "\a";

   del();
   rlutil::setColor(WHITE);
   gotoxy(x, y);     printf("* *");
   gotoxy(x, y + 1); printf(" *");
   gotoxy(x, y + 2); printf("* *");
   Sleep(100);


   del();
   rlutil::setColor(LIGHTGRAY);
   gotoxy(x, y);     printf(" * ");
   gotoxy(x, y + 1); printf("***");
   gotoxy(x, y + 2); printf(" * ");
   Sleep(100);

   del();
   rlutil::setColor(WHITE);
   gotoxy(x, y);     printf("* *");
   gotoxy(x, y + 1); printf(" *");
   gotoxy(x, y + 2); printf("* *");
   Sleep(100);

   del();
   rlutil::setColor(LIGHTGRAY);
   gotoxy(x, y);     printf(" * ");
   gotoxy(x, y + 1); printf("***");
   gotoxy(x, y + 2); printf(" * ");
   Sleep(100);

   del();
   rlutil::setColor(WHITE);
   gotoxy(x, y);     printf(" * ");
   gotoxy(x, y + 1); printf("***");
   gotoxy(x, y + 2); printf(" * ");
   Sleep(100);

   del();
   rlutil::setColor(LIGHTGRAY);
   bullet->setState(false);

   this->setLives(this->getLives() - 1);
}



class TANK_ENEMY {
private:
   int x, y;
   int direccion;
   bool state;
   bool multibullet;

public:
   vector <BULLET *> bullets;
	TANK_ENEMY(int, int, bool );
	int getX();
	int getY();
	void setDireccion(int);
	int getDireccion();
	void print();
	void printUp();
	void printDown();
	void printLeft();
	void printRight();
	void del();
	void mover();
	void mover_bullets();
	void die(BULLET *);
};

TANK_ENEMY::TANK_ENEMY(int x, int y, bool multibullet) {
   this->x = x;
   this->y = y;
   direccion = MOVE_UP;
   state = true;
   this->multibullet = multibullet;
}

int TANK_ENEMY::getX() {
	return x;
}

int TANK_ENEMY::getY() {
	return y;
}

void TANK_ENEMY::setDireccion(int direccion) {
   if (direccion == MOVE_UP || direccion == MOVE_DOWN || direccion == MOVE_LEFT || direccion == MOVE_RIGHT) {
      this->direccion = direccion;
   }
}

int TANK_ENEMY::getDireccion() {
    return direccion;
}

void TANK_ENEMY::printUp() {
	gotoxy(x, y); printf(" %c", CHARACTER_ENEMY);
	gotoxy(x, y + 1);	printf("%c%c%c", CHARACTER_ENEMY, CHARACTER_ENEMY, CHARACTER_ENEMY);
	gotoxy(x, y + 2);	printf("%c %c", CHARACTER_ENEMY, CHARACTER_ENEMY);
}

void TANK_ENEMY::printDown() {
	gotoxy(x, y);	printf("%c %c", CHARACTER_ENEMY, CHARACTER_ENEMY);
	gotoxy(x, y + 1); printf("%c%c%c", CHARACTER_ENEMY, CHARACTER_ENEMY, CHARACTER_ENEMY);
	gotoxy(x, y + 2); printf(" %c ", CHARACTER_ENEMY);
}

void TANK_ENEMY::printRight() {
   gotoxy(x, y); printf("%c%c", CHARACTER_ENEMY, CHARACTER_ENEMY);
	gotoxy(x, y + 1); printf(" %c%c", CHARACTER_ENEMY, CHARACTER_ENEMY);
	gotoxy(x, y + 2);	printf("%c%c", CHARACTER_ENEMY, CHARACTER_ENEMY);
}

void TANK_ENEMY::printLeft() {
	gotoxy(x, y); printf(" %c%c", CHARACTER_ENEMY, CHARACTER_ENEMY);
	gotoxy(x, y + 1); printf("%c%c", CHARACTER_ENEMY, CHARACTER_ENEMY);
	gotoxy(x, y + 2);	printf(" %c%c", CHARACTER_ENEMY, CHARACTER_ENEMY);
}

void TANK_ENEMY::print() {
   switch(direccion) {
      case MOVE_UP:
         printUp();
         break;
      case MOVE_DOWN:
         printDown();
         break;
      case MOVE_LEFT:
         printLeft();
         break;
      case MOVE_RIGHT:
         printRight();
         break;
   }

   gotoxy(x + 1, y + 1);
   rlutil::setColor(LIGHTRED); rlutil::setBackgroundColor(LIGHTBLUE);
   printf("E");
   rlutil::setColor(LIGHTGRAY); rlutil::setBackgroundColor(BLACK);
}

void TANK_ENEMY::del() {
   gotoxy(x, y); printf("   ");
   gotoxy(x, y + 1); printf("   ");
   gotoxy(x, y + 2); printf("   ");
}

void TANK_ENEMY::mover() {
static int kbchar;

    int oldX;
    int oldY;
    oldX=x;
    oldY=y;

if ( (rand() % 10 + 1 ) > 5 ) {

   kbchar = rand() % 5 + 1;

   switch(kbchar)
{
    case 1:
        kbchar=MOVE_LEFT;
        break;
    case 2:
        kbchar=MOVE_RIGHT;
        break;
    case 3:
        kbchar=MOVE_DOWN;
        break;
    case 4:
        kbchar=MOVE_UP;
        break;
    case 5:
        kbchar=SHOOT_ENEMY;
        break;
}

};

      del();

      if (kbchar == MOVE_LEFT) {
         if (direccion == MOVE_LEFT)
            if (x > LIMIT_LEFT + 1) x--;
      }

      if (kbchar == MOVE_RIGHT) {
         if (direccion == MOVE_RIGHT)
            if ((x + 2) < LIMIT_RIGHT - 1) x++;
      }

      if (kbchar == MOVE_DOWN) {
         if (direccion == MOVE_DOWN)
            if ((y + 2) < LIMIT_DOWN - 1)y++;
      }

      if (kbchar == MOVE_UP) {
         if (direccion == MOVE_UP)
            if (y > LIMIT_UP + 1) y--;
      }

      bool tempfl=true;
      for (int tempxx=0; tempxx<=2; tempxx++){
      for (int tempyy=0; tempyy<=2; tempyy++){
      if (!checkCoordsWall(vecWall, x+tempxx ,y+tempyy)) tempfl=false;
      }};

       if ( !tempfl )
       {
        x=oldX;
        y=oldY;
       };


      if (kbchar == SHOOT_ENEMY) {
            if (multibullet == false && bullets.size() > 0)
            return;

         bullets.push_back(new BULLET(this->x + 1, this->y + 1, this->direccion));
      }

      setDireccion(kbchar);
      print();

}

void TANK_ENEMY::mover_bullets() {

   this->print();

   // Move all the bullets
   for (int i = 0; i < bullets.size(); ++i) {
      bullets[i]->mover();
      this->print();
      if (!bullets[i]->getState()) {
         bullets[i]->del();
         delete (bullets[i]);
         bullets[i] = NULL;
      }
   }

   int i = 0;
   while (i < bullets.size()) {
      if (bullets[i] == NULL) {
         bullets.erase(bullets.begin() + i);
      } else {
         ++i;
      }
   }
}

void TANK_ENEMY::die(BULLET *bullet) {
   state = false;

   cout << "\a";

   // Delete bullets from the screen
   for (int i = 0; i < bullets.size(); ++i) {
      bullets[i]->del();
   }

   for (int i = 0; i < bullets.size(); ++i) {
      delete(bullets[i]);
   }
   this->bullets.clear();

   del();
   gotoxy(x, y);     printf("* *");
   gotoxy(x, y + 1); printf(" *");
   gotoxy(x, y + 2); printf("* *");
   Sleep(50);


   del();
   gotoxy(x, y);     printf(" * ");
   gotoxy(x, y + 1); printf("***");
   gotoxy(x, y + 2); printf(" * ");
   Sleep(50);

   del();
   gotoxy(x, y);     printf(" * ");
   gotoxy(x, y + 1); printf("***");
   gotoxy(x, y + 2); printf(" * ");
   Sleep(50);

   del();

   // Set dead to the bullet that killed him
   bullet->setState(false);
}


void paint_bordes()
{

   for (int i = LIMIT_LEFT; i <= LIMIT_RIGHT; ++i) {
      gotoxy(i, LIMIT_UP); printf("%c", 205);
      gotoxy(LIMIT_RIGHT - i + LIMIT_LEFT, LIMIT_DOWN); printf("%c", 205);
      Sleep(10);
   }

   for (int i = LIMIT_UP; i <= LIMIT_DOWN; ++i) {
      gotoxy(LIMIT_LEFT, i); printf("%c", 186);
      gotoxy(LIMIT_RIGHT, LIMIT_DOWN - i + LIMIT_UP); printf("%c", 186);
      Sleep(10);
   }

   // corners
   gotoxy(LIMIT_LEFT, LIMIT_UP); printf("%c", 201);
   gotoxy(LIMIT_RIGHT, LIMIT_UP); printf("%c", 187);
   gotoxy(LIMIT_LEFT, LIMIT_DOWN); printf("%c", 200);
   gotoxy(LIMIT_RIGHT, LIMIT_DOWN); printf("%c", 188);
}




class WALL {
private:
	int x, y;

public:
	WALL(int, int);
	int getX();
	int getY();
	void print();
	};

WALL::WALL(int x, int y) {
   this->x = x;
   this->y = y;
   }

int WALL::getX() {
	return x;
}

int WALL::getY() {
	return y;
}
void WALL::print() {
   gotoxy(x, y); printf("%c", CHAR_WALL);
}

bool checkCoords(vector<TANK_ENEMY *> tetmp,int tempX,int tempY){
int curY;
int curX;
bool mFlag;

for(size_t i = 0; i < tetmp.size(); ++i){
curY = tetmp[i]->getY();
curX= tetmp[i]->getX();

mFlag=(tempX < (curX + 2 +3)) && (tempX > (curX - 2 -3)) && (tempY < (curY + 2 +3)) && (tempY > (curY - 2 -3));
if (mFlag) return(false);
};

return(true);
};

bool checkCoordsWall(vector<WALL *> tetmp,int tempX,int tempY){
int curY;
int curX;
bool mFlag;

for(size_t i = 0; i < tetmp.size(); ++i){
curY = tetmp[i]->getY();
curX= tetmp[i]->getX();

mFlag=( (tempX==curX) && (tempY==curY) );
if (mFlag) return(false);
};

return(true);
};

int main()
{
    srand(time(NULL));


TANK tank1( (LIMIT_RIGHT + LIMIT_LEFT) /2 , (LIMIT_DOWN + LIMIT_UP) -4 );

   paint_bordes();

   GAME_TIME game_time(0);

	tank1.print();


   int tempX;
   int tempY;

   vector <TANK_ENEMY *> te;

   TANK_ENEMY tank_enemy1( (LIMIT_RIGHT + LIMIT_LEFT) /2 + 1 + rand() % 30 + 1 , (LIMIT_DOWN + LIMIT_UP) / 2 + rand() % 10 + 1 , false);
   te.push_back(&tank_enemy1);

   for (int tempI=2 ; tempI<=7 ; tempI++){
   do{
   tempX=(LIMIT_RIGHT + LIMIT_LEFT) /2  + (rand() % (LIMIT_RIGHT/2-3) + 1);
   tempY=(LIMIT_DOWN + LIMIT_UP) / 2 + (rand() % (LIMIT_DOWN/2-3) + 1);
   }while(!checkCoords(te,tempX,tempY));
   TANK_ENEMY* tank_enemyt = new TANK_ENEMY(tempX,tempY , false);
   te.push_back(tank_enemyt);
};


   for (int tempI=2 ; tempI<=7 ; tempI++){
   do{
   tempX=(LIMIT_LEFT+LIMIT_RIGHT) /2  - (rand() % (LIMIT_RIGHT/2-3) + 1);
   tempY=(LIMIT_DOWN + LIMIT_UP) / 2 - (rand() % (LIMIT_DOWN/2-3) + 1);
   }while(!checkCoords(te,tempX,tempY) && !checkCoordsWall(vecWall,tempX,tempY));

   WALL* wall_t = new WALL(tempX,tempY);
   vecWall.push_back(wall_t);
   wall_t->print();

   bool xyflag = rand() % 2;
   wall_t = new WALL(tempX+xyflag,tempY+(1-xyflag));
   vecWall.push_back(wall_t);
   wall_t->print();
   wall_t = new WALL(tempX+xyflag*2,tempY+(1-xyflag)*2);
   vecWall.push_back(wall_t);
   wall_t->print();
};

   game_time.print_time();

	while (true) {

      tank1.mover();
      tank1.mover_bullets();


      //enemy moves
      for (int i = 0; i < te.size(); ++i) {
         te[i]->mover();
         te[i]->mover_bullets();
      }


      // determine hit
      for (int i = 0; i < te.size(); ++i) {
         for (int j = 0; j < te[i]->bullets.size(); ++j) {

            int x = te[i]->bullets[j]->getX();
            int y = te[i]->bullets[j]->getY();

            switch(tank1.getDireccion()) {
               case MOVE_RIGHT:
                  if (y == tank1.getY() && (x == tank1.getX() || x == tank1.getX() + 1)
                      || y == tank1.getY() + 1 && (x == tank1.getX() + 1 || x == tank1.getX() + 2)
                      || y == tank1.getY() + 2 && (x == tank1.getX() || x == tank1.getX() + 1)) {
                     // collision
                  tank1.die(te[i]->bullets[j]);
                  }
                  break;

               case MOVE_LEFT:
                  if (y == tank1.getY() && (x == tank1.getX() + 1 || x == tank1.getX() + 2)
                      || y == tank1.getY() + 1 && (x == tank1.getX() || x == tank1.getX() + 1)
                      || y == tank1.getY() + 2 && (x == tank1.getX() + 1 || x == tank1.getX() + 2)) {
                   tank1.die(te[i]->bullets[j]);
                  }
                  break;

               case MOVE_UP:
                  if (y == tank1.getY() && x == tank1.getX() + 1
                      || y == tank1.getY() + 1 && (x >= tank1.getX() && x <= tank1.getX() + 2)
                      || y == tank1.getY() + 2 && (x == tank1.getX() || x == tank1.getX() + 2)) {
                   tank1.die(te[i]->bullets[j]);
                  }
                  break;

               case MOVE_DOWN:
                  if (y == tank1.getY() && (x == tank1.getX() || x == tank1.getX() + 2)
                      || y == tank1.getY() + 1 && (x >= tank1.getX() && x <= tank1.getX() + 2)
                      || y == tank1.getY() + 2 && x == tank1.getX() + 1) {
                   tank1.die(te[i]->bullets[j]);
                  }
                  break;
            }
         }
      }



      for (int j = 0; j < te.size(); ++j) {
         int direccion_tank_enemy = te[j]->getDireccion();

         for (int i = 0; i < tank1.bullets.size(); ++i) {
            int x = tank1.bullets[i]->getX(); // bullet x
            int y = tank1.bullets[i]->getY(); //

            switch (direccion_tank_enemy) {
               case MOVE_RIGHT:
                  if (y == te[j]->getY() && (x == te[j]->getX() || x == te[j]->getX() + 1)
                      || y == te[j]->getY() + 1 && (x == te[j]->getX() + 1 || x == te[j]->getX() + 2)
                      || y == te[j]->getY() + 2 && (x == te[j]->getX() || x == te[j]->getX() + 1)) {
                     // collision
                     te[j]->die(tank1.bullets[i]);
                     te.erase(te.begin() + j);
                     tank1.setKills(tank1.getKills() + 1);

                     continue;
                  }
                  break;


               case MOVE_LEFT:
                  if (y == te[j]->getY() && (x == te[j]->getX() + 1 || x == te[j]->getX() + 2)
                      || y == te[j]->getY() + 1 && (x == te[j]->getX() || x == te[j]->getX() + 1)
                      || y == te[j]->getY() + 2 && (x == te[j]->getX() + 1 || x == te[j]->getX() + 2)) {
                   te[j]->die(tank1.bullets[i]);
                   te.erase(te.begin() + j);
                   tank1.setKills(tank1.getKills() + 1);

                     continue;
                  }
                  break;

               case MOVE_UP:
                  if (y == te[j]->getY() && x == te[j]->getX() + 1
                      || y == te[j]->getY() + 1 && (x >= te[j]->getX() && x <= te[j]->getX() + 2)
                      || y == te[j]->getY() + 2 && (x == te[j]->getX() || x == te[j]->getX() + 2)) {
                     te[j]->die(tank1.bullets[i]);
                     te.erase(te.begin() + j);
                     tank1.setKills(tank1.getKills() + 1);

                     continue;
                  }
                  break;

               case MOVE_DOWN:
                  if (y == te[j]->getY() && (x == te[j]->getX() || x == te[j]->getX() + 2)
                      || y == te[j]->getY() + 1 && (x >= te[j]->getX() && x <= te[j]->getX() + 2)
                      || y == te[j]->getY() + 2 && x == te[j]->getX() + 1) {
                     te[j]->die(tank1.bullets[i]);
                     te.erase(te.begin() + j);
                     tank1.setKills(tank1.getKills() + 1);

                       continue;
                  }
                  break;

            }

         }
      }


      if (tank1.getLives() == 0) {
         gotoxy(1, 6); cout << "You loose!" << endl;
         cout << "\a\a\a" << endl;
         system("pause>nul");
         return 0;
      }

      if (te.size() == 0) {
         gotoxy(1, 4); cout << "You win!" << endl;
         cout << "\a\a\a" << endl;
         system("pause>nul");
         return 0;
      }



      Sleep(10);
      game_time.print_time();
	}


	return 0;
}





