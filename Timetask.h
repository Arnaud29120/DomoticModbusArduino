
#ifndef Timetask_h
#define Timetask_h

#include "Arduino.h"

#define TSKMAX 8
#define NMAX 256 //2^8
typedef void (*ptrFunc) ();//to call a function which takes no arguments, and returns void
/*
ptrFunc foo;

void hello () {  Serial.print ("hello");  }  // end of hello

foo = hello;
*/
class Timetask
{
  public:
    Timetask( uint8_t period , bool loopinf = true );
    Timetask();
    void setTask( uint8_t lvl , ptrFunc f ); //nT = 2^0
    void Start( bool loopinf = true );//une fois ou en boucle
    void Stop();//arrete si mode boucle
    void Run();

  private:
 // const uint8_t TASKMAX = 8;
    int _period;
    bool _loop;
    bool _runonce;
    bool _IsRunning;
    uint16_t _CycleCount;
    uint16_t _n;
    bool _Task_isPtrset[TSKMAX];
    ptrFunc _Task_fnPtr[TSKMAX];
  	unsigned long _TimePrec, _TimeCur;
};


#endif

/*
projet
nous devons gerer une interruption de timer 
au lieu de faire une boucle while qui au finale bloquerait le systeme
meme si on peut ajouter des taches dans la boucle principale du programme arduino (loop)



*/

