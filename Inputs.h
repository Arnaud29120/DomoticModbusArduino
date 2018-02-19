
#ifndef Inputs_h
#define Inputs_h

#include "Arduino.h"

/*
cette classe permettra la gestion des entrrees voirs des soties ??

elle filtre les rebonds
elle transorme un signal ana en plusieurs entree di ex lcd keypad
elle peut servir de gestionnaire d'event ex on utilise un pointeur sur une fonction up_raising_edge()

*/
#define AIMAX 1
//constante de conversion d ai vers di ex lcd keypad
#define AILVL 6
#define AILVL0 50
#define AILVL1 250
#define AILVL2 450
#define AILVL3 650
#define AILVL4 850
#define AILVL5 1000

#define DIMAX 16
//evolueront en classe
typedef struct { 
    String lbl;
    String unt;
    int aiarduino;
    unsigned int val;
    unsigned int vhh;
    unsigned int vl;
    unsigned int vh;
    unsigned int vll;
    bool ahh;
    bool ah;
    bool al;
    bool all;
 } AIARD;
// 6/01 ajout vhh , vh ,vl vll flag ahh ah al all


typedef struct { 
    String lbl;
     int diarduino;
     bool vir;
     bool val;
     byte aiidx;
     bool re;
     bool fe;
     bool nch;
     bool ncl;
     byte ton;
     byte toff;//pour la gestion pulse on s'arretera a 255    
 } DIARD;

#define DOMAX 8
//pour la gestion pulse on s'arretera a 255    
typedef struct { String lbl; int doarduino; bool inv; bool val; byte ton; byte toff; } DOARD;






class Inputs
{
  public:
    Inputs();
    void Add_AI( String lbl , String unt, int aiarduino );
    void DspAnaToDis(String lbl);
    void Add_DI( String lbl , int diarduino , bool vir , byte aiidx);
    void DbgSerialOutAis();
    void ReadAis();
     void DbgSerialOutDis();
    void ReadDis();
    bool DI_GetState( int DIIDX);
    byte DI_Count();

    void Add_DO( String lbl , int doa , bool inv);
    bool DO_GetState( int DOIDX);
    bool DO_SetState( int DOIDX , bool STATE );
    void DbgSerialOutDos();
    byte DO_Count();
  private:
 // const uint8_t TASKMAX = 8;
    AIARD _AIS[AIMAX];//nombre d'entree ana
    byte _AIS_idx;
    bool _AIS_set;
    int _AIS_Lvls[AILVL];//conversion ana di a revoir mais existe ;)
    int GetAiIdxFromLabel(String ailbl);
    DIARD _DIS[DIMAX];//nombre d'entree di virtuel comprise
    byte _DIS_idx;

    DOARD _DOS[DOMAX];
    byte _DOS_idx;
    
};


#endif

/*
projet
nous devons gerer une interruption de timer 
au lieu de faire une boucle while qui au finale bloquerait le systeme
meme si on peut ajouter des taches dans la boucle principale du programme arduino (loop)



*/

