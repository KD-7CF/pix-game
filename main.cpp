#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <vector>

/* *** constante de type define ***/

#define RUN  true
#define STOP !RUN

#define WIDTH  90 // Largeur par default de l'ecran 
#define HEIGHT 35 // Hauteur par default de l'ecran 

// *** DIRECTION SHOOT GUN ***
#define U_V 'U'   // UP_VERTICAL
#define D_V 'D'  //  DOWN_VERTICAL
#define L_H 'L' //   LEFT_HORIZONTAL
#define R_H 'R'//    RIGHT_HORIZONTAL
// ***************************


using namespace std;


// ************** VARIABLES GLOBALS  **************

bool etat_JEU = !RUN;
COORD ptShoot;  // position current de la balle > > > > >
char dirShoot; // direction de tire

mutex m; // synchronisation du thread

// *************************************************



void posXY(int x, int y){
   COORD coord;
   coord.X = x;
   coord.Y = y;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int getcurrentX(){
   CONSOLE_SCREEN_BUFFER_INFO info_SCREEN;
   if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info_SCREEN))
      return -1;
   return info_SCREEN.dwCursorPosition.X; 
}


int getcurrentY(){
   CONSOLE_SCREEN_BUFFER_INFO info_SCREEN;
   if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info_SCREEN))
      return -1;
   return info_SCREEN.dwCursorPosition.Y; 
}

void clearScreen(COORD *coord = nullptr){

   DWORD Nbr_chars; 
   CONSOLE_SCREEN_BUFFER_INFO info_SCREEN;

   if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info_SCREEN)) 
      return;

   if(!FillConsoleOutputCharacter(
            GetStdHandle(STD_OUTPUT_HANDLE),
            ' ', // remplire d'espace
            info_SCREEN.dwSize.X * info_SCREEN.dwSize.Y,
            (COORD) {0, 0},
            &Nbr_chars
            )) return;


   if(!FillConsoleOutputAttribute(
            GetStdHandle(STD_OUTPUT_HANDLE),
            info_SCREEN.wAttributes,
            info_SCREEN.dwSize.X * info_SCREEN.dwSize.Y,
            (COORD) {0, 0},
            &Nbr_chars
            )) return;

   if(coord != nullptr)
      *coord = {(SHORT) ((rand() % (WIDTH - 10)) + 6), (SHORT) ((rand() % (HEIGHT - 10)) + 6)};
}


BOOL hideCursor(BOOL bVisible){

   CONSOLE_CURSOR_INFO info_CURSOR_SCREEN;
   info_CURSOR_SCREEN.dwSize = 1;
   info_CURSOR_SCREEN.bVisible = bVisible;

   return SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info_CURSOR_SCREEN);
}

BOOL fontScreen(
      LPCWSTR FaceName   = L"Consolas",
      COORD   dwFontSize = {7, 15},
      UINT    FontFamily = FF_DONTCARE,
      UINT    FontWeight = FW_NORMAL){

   CONSOLE_FONT_INFOEX info_FONT_SCREEN;
   info_FONT_SCREEN.cbSize = sizeof(info_FONT_SCREEN);
   info_FONT_SCREEN.dwFontSize = dwFontSize; 
   info_FONT_SCREEN.FontFamily = FontFamily;
   info_FONT_SCREEN.FontWeight = FontWeight;
   wcscpy(info_FONT_SCREEN.FaceName, FaceName); // Copier LPCWSTR (WCHAR*) --> WCHAR[]

   return SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &info_FONT_SCREEN);
}

void Screen(){

   SMALL_RECT rect {0, 0, WIDTH - 1, HEIGHT - 1}; // la taille de l'ecran console
   COORD coord;
   coord.X = WIDTH;
   coord.Y = HEIGHT;
   SetConsoleTitle("War Survivor");
   SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coord); // definir le domaine de mon tampon necessaire ~ à la taille de l'ecran 
   SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rect); // dimensionner l'ecran

   hideCursor(FALSE); // Affiché ou Caché le curseur
   fontScreen(L"NSimSun", {7, 15}); // Definir le thème Police de la console

   /*
    *
    ************* DRAWING SCREEN  *************
    *
    *              SCHEMA CADRE
    *
    *                 WIDTH
    *          |+5 .....^..... +5|
    *          |.
    *          |.
    * HEIGHT < |.
    *          |.
    *          |.
    *          |+5
    ide de la souri
    */

   posXY(0, 0);
   for(int i=0; i<=WIDTH - 10; i++){
      posXY(getcurrentX() + (5+i), 5); cout << "\u2580\n";
   }

   posXY(5, 5);
   for(int i=0; i<=HEIGHT - 10; i++){
      posXY(4, getcurrentY()); cout << "\u2590\n";
      posXY(WIDTH - 5, getcurrentY() - 1); cout << "\u2590\n";
   }

   posXY(0, getcurrentY());
   for(int i=0; i<=WIDTH - 10; i++){
      posXY(getcurrentX() + (5+i), getcurrentY()); cout << "\u2580\n"; 
      posXY(getcurrentX(), getcurrentY() - 1);
   }

}


void __Put(const char* objet, COORD* coord = nullptr){
   if(coord != nullptr)
      posXY(coord->X, coord->Y);
   cout << objet;
}


void shootGun(COORD pt, char direction){

   switch(direction) {

      case U_V:
         while(pt.Y > 6 ){
            pt.Y --; 
            __Put("*\n", &pt);
            this_thread::sleep_for(30ms);
            __Put(" \n", &pt);

            ptShoot.Y = pt.Y;
         }

         break;

      case D_V:
         while(pt.Y < HEIGHT - 5 ){
            pt.Y ++; 
            __Put("*\n", &pt);
            this_thread::sleep_for(30ms);
            __Put(" \n", &pt);

            ptShoot.Y = pt.Y;
         }

         break;

      case L_H:
         while(pt.X > 5 ){
            pt.X --; 
            __Put("*\n", &pt);
            this_thread::sleep_for(10ms);
            __Put(" \n", &pt);

            ptShoot.X = pt.X;
         }

         break;

      case R_H:
         while(pt.X < WIDTH - 7 ){
            pt.X ++; 
            __Put("*\n", &pt);
            this_thread::sleep_for(10ms);
            __Put(" \n", &pt);

            ptShoot.X = pt.X;
         }

         break;

      default:
         break;
   }
}

void initJEU(COORD* coord){

   srand((unsigned int) time(NULL)); // initialiser le random à une variable aleatoire au premier lancement 

   /*
    * Coordonnées avant:   COORD[0]
    * Coordonnées current: COORD[1]
    */

   coord[1].X = (rand() % (WIDTH  - 12)) + 6; 
   coord[1].Y = (rand() % (HEIGHT - 10)) + 6;
   coord[0] = coord[1]; // coordonnées avant initialisation

   etat_JEU = RUN;

   const char* alldirPut[] = {"\u25B2\n", "\u25BC\n", "\u25C0\n", "\u25B6\n"}; // Toutes les directions possibles à afficher
   __Put(alldirPut[rand() %  (sizeof(alldirPut) / sizeof(char*))], &coord[1]); // Position aléatoire de la direction
}

void Event() {

   COORD coord[2];
   initJEU(coord);

   while(etat_JEU){


      // *************************  ATTENTE D'EVENEMENT *************************

      if(GetAsyncKeyState(VK_ESCAPE)){
         if(MessageBox(
                  NULL, 
                  TEXT("Voulez - vous mettre en pause le jeu ?"), 
                  TEXT(" ? "), 
                  MB_YESNO |MB_ICONQUESTION
                  ) == IDNO){
            etat_JEU = STOP; 
         } else {
            bool PAUSE = true;
            while(PAUSE){
               if(
                     GetAsyncKeyState(VK_PAUSE) ||
                     GetAsyncKeyState(VK_SPACE) ||
                     GetAsyncKeyState(VK_SHIFT) ||
                     GetAsyncKeyState(VK_BACK)
                 ){
                  PAUSE = false;
               }

               COORD coord_PAUSE = { WIDTH / 2, HEIGHT / 2 };
               __Put("PAUSE\n", &coord_PAUSE);
               this_thread::sleep_for(800ms);
               __Put("     \n", &coord_PAUSE);
               this_thread::sleep_for(800ms);
            }
         }
      }

      if(GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_CONTROL)){
         ptShoot = coord[1]; // position de tire
         shootGun((COORD) {ptShoot.X, ptShoot.Y}, dirShoot);
      }


      // *************************  MOUVEMENT *************************

      if(GetAsyncKeyState(VK_UP)){
         if(coord[1].Y <= 6)
            coord[1].Y = HEIGHT - 5;
         else
            coord[1].Y --;

         __Put("  \n", &coord[0]); 
         __Put("\u25B2\n", &coord[1]);
         coord[0] = coord[1]; // copiés les currentes coordonées dans l'historique (coordonées avant) 

         dirShoot = U_V; // direction UP_VERTICAL
      }

      if(GetAsyncKeyState(VK_DOWN)){

         if(coord[1].Y >= HEIGHT - 5)
            coord[1].Y = 6;
         else
            coord[1].Y ++;

         __Put("  \n", &coord[0]);
         __Put("\u25BC\n", &coord[1]);
         coord[0] = coord[1]; // copiés les currentes XY .... l'historique  

         dirShoot = D_V; // direction DOWN_VERTICAL
      }

      if(GetAsyncKeyState(VK_LEFT)){

         if(coord[1].X <= 5)
            coord[1].X = WIDTH - 7;
         else
            coord[1].X -= 2;

         __Put("  \n", &coord[0]);
         __Put("\u25C0\n", &coord[1]);
         coord[0] = coord[1]; //copiés les currentes XY .... l'historique 

         dirShoot = L_H; // direction LEFT_HORIZONTAL
      }

      if(GetAsyncKeyState(VK_RIGHT)){

         if(coord[1].X >= WIDTH - 7)
            coord[1].X = 5;
         else
            coord[1].X += 2;

         __Put("  \n", &coord[0]);
         __Put("\u25B6\n", &coord[1]); 
         coord[0] = coord[1]; // copiés les currentes XY .... l'historique 

         dirShoot = R_H; // direction RIGHT_HORIZONTAL 
      }

      // **************************************************************************

      Sleep(100);
   }

}


void CombatContreMechantsA(){
   vector <COORD> MechantsA (20, {25, 25}); // creation de mechants de type A
   for(COORD Ax : MechantsA){
//      Ax = {(SHORT) ((rand() % (WIDTH  - 12)) + 6), (SHORT) ((rand() % (HEIGHT - 10)) + 6)}; // positionné chaque mechant A
      // __Put("\u26C7\n", &Ax);
   }

   char direction[] = {U_V, D_V, L_H, R_H};

   while(etat_JEU || !MechantsA.empty()){
      //switch(direction[rand() % (sizeof(direction)/sizeof(char))]) // direction aleatoire 
      switch(U_V)
      {
         case U_V:
            for(int i=0; i<MechantsA.size()/2; i++){

               __Put("  \n", &MechantsA[i]); // effacer la precedent position du mechant
               if(MechantsA[i].Y <= 6)
                  MechantsA[i].Y = HEIGHT - 5;
               else
                  MechantsA[i].Y --;

               __Put("\u26C7\n", &MechantsA[i]);
            }
            break;

         case D_V:
            for(int i=0; i<MechantsA.size(); i++){

               __Put("  \n", &MechantsA[i]); // effacer la precedent position du mechant
               if(MechantsA[i].Y >= HEIGHT - 5)
                  MechantsA[i].Y = 6;
               else
                  MechantsA[i].Y ++;
               __Put("\u26C7\n", &MechantsA[i]);
            }

            break;
         case L_H:
            break;
         case R_H:
            break;
         default:
            break;
      }

      this_thread::sleep_for(1000ms);   
   }
}

void CombatContreMechantsB(){
}
void CombatContreMechantsC(){
}
void CombatFinal(){
}

void Action(){

   if(!etat_JEU)
      this_thread::sleep_for(1000ms);

   while(etat_JEU){
      CombatContreMechantsA();
      CombatContreMechantsB();
      CombatContreMechantsC();
      CombatFinal();
   }
}

int main(int argc, char* argv[])
{
   Screen();

   thread taskEvent;
   thread taskAction;
   taskEvent  = thread(&Event);
   taskAction = thread(&Action);
   taskEvent.join();
   taskAction.join();
   return 0;
}








































































