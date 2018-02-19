#include "Arduino.h"
#include "Timetask.h"
//-------------------------------------------------------------------------------------------------------------

Timetask::Timetask()
{
  _period = 20;
  _runonce = false;
  _loop = true; //par defaut pour test 
  _IsRunning = false;
  _n=0;
  _CycleCount =0;
} 

//-------------------------------------------------------------------------------------------------------------

Timetask::Timetask(uint8_t period , bool loopinf)
{
	_period = period;
	_runonce = false;
	_loop = loopinf; //par defaut pour test 
	_IsRunning = false;
  _n=0;
  _CycleCount =0;
}

//-------------------------------------------------------------------------------------------------------------	

void Timetask::Start( bool loopinf )
{
	_loop = loopinf;
	_TimePrec = millis();
	_TimeCur = _TimePrec;
	//_CycleCount =0;
	_IsRunning = true;
  _runonce = false;
  _n=0;
}

//-------------------------------------------------------------------------------------------------------------

void Timetask::setTask( uint8_t lvl , ptrFunc f )
{
  if ( ( lvl >= 0 ) && (lvl < TSKMAX ) && ( f != NULL ) )
  {
   _Task_isPtrset[lvl] = true;
   _Task_fnPtr[lvl] = f;
  
  }

}

//-------------------------------------------------------------------------------------------------------------

void Timetask::Stop()
{
  _IsRunning = false;
}

//-------------------------------------------------------------------------------------------------------------

void Timetask::Run()
{
  uint8_t i=0;
  uint8_t j=0;

  if ( ( _loop && _IsRunning ) || ( !_loop && !_runonce) )//soit on boucle soit on execute une fois
  {
    _TimeCur = millis();
  
    if ( ( _TimeCur - _TimePrec) >= _period )
    {



      for( i=0; i < TSKMAX; i++)
      {
          //Serial.print("_n: ");Serial.print(_n);Serial.print(" i ");Serial.println(i);
          j= (uint8_t) ( 1 << i );
          //Serial.print(" 2^i ");Serial.println(j);
          j =(uint8_t)  ( _n % j );
          if ( j == 0 )
        {
          if ( _Task_isPtrset[i] ) { _Task_fnPtr[i]();}
        }

      }

      _n++;
      _TimePrec = _TimeCur;
      
      //atteint limite comptage pour niveau tache
      if ( _n >= NMAX )
      {  
        _n=0;
        _CycleCount++;
        if ( !_loop ){ _IsRunning = false; _runonce = true; }
      }
//  
    }
  }

}
