#include <LiquidCrystal.h>

#include <ModbusIP.h>
#include <Modbus.h>
#include <SPI.h>
#include <Ethernet.h>

#include <Wire.h>


#include "Timetask.h"
#include "Inputs.h"

unsigned long TMS0,TMS1;

ModbusIP mb;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {192, 168, 1, 120};

//Timetask TASKER = new Timetask(5,false);
//arduino oblige a initialiser les objets ici 
//Timetask TASKER; delecnche erreur il faudrait un cosntructeur par default voir surcharge en c++
//Timetask TASKER;//ok ave constucteur surcharge on fera une surcharge par defaut au cas ou mais
//on initialisera directement puisqu arduino fctt ainsi
Timetask TASKER(200,false);

//Inputs INP();//declenche erreur
Inputs INP;



typedef union
{
 float f;
 word w[2];
} uFW_t;

//FLOATUNION_t myFloat;

// select the pins used on the LCD panel
LiquidCrystal lcd(34, 35, 30, 31, 32, 33);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
//-----------------------------------------------------------------------------------------
// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(A0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  
 return btnNONE;  // when all others fail, return this...
}


//----------------------------------------------------------------------
//              SETUP
//----------------------------------------------------------------------
void setup() {
  // initialize serial:
  Serial.begin(115200);// Open serial communications and wait for port to open:
 
  lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("Modbus TCP Mega"); // print a simple message
 lcd.setCursor(0,1);
 lcd.print("Version 0.1"); // print a simple message
   
  mb.config (mac, ip);
//  mb.setSlaveId(1);  //ne fct pas pour modbustcp
  mbSetupCoil();

  TMS1 = millis();
  TMS1 = TMS0; //realtime before gps synchro

  
//TASKER = new Timetask(200,false);
TASKER.setTask(0,task0);
TASKER.setTask(1,task1);
TASKER.setTask(2,task2);
TASKER.setTask(3,task3);
TASKER.setTask(4,task4);
TASKER.setTask(5,task5);
TASKER.setTask(6,task6);
TASKER.setTask(7,task7);
TASKER.Start(true);

//configuretion des entrees et sorites vioir le module input
INP.Add_AI("A0","lsb",A0); //entree analogique du lcd keypad
//entre digitale de 49 a 42
//voir blog 42  PL7 DI_7 à  49  PL0 / ICP4 DI_0
INP.Add_DI( "DI_0" , 49, false ,0);
INP.Add_DI( "DI_1" , 48, false ,0);
INP.Add_DI( "DI_2" , 47, false ,0);
INP.Add_DI( "DI_3" , 46, false ,0);
INP.Add_DI( "DI_4" , 45, false ,0);
INP.Add_DI( "DI_5" , 44, false ,0);
INP.Add_DI( "DI_6" , 43, false ,0);
INP.Add_DI( "DI_7" , 42, false ,0);
//conversion entree ana en di virtuel
INP.DspAnaToDis("A0");
//sortie inverse on a 1 la sorite es ta 0 mais c transaprent pour suer
//par defaut totue sles sorties sotn inactive, si elel sont inverse la sortie est a 1 ex module 8 relay
// String lbl , int doa , bool inv)
INP.Add_DO("DO_0" , 22, true);
INP.Add_DO("DO_1" , 23, true);
INP.Add_DO("DO_2" , 24, true);
INP.Add_DO("DO_3" , 25, true);
INP.Add_DO("DO_4" , 26, true);
INP.Add_DO("DO_5" , 27, true);
INP.Add_DO("DO_6" , 28, true);
INP.Add_DO("DO_7" , 29, true);


}
//----------------------------------------------------------------------------------------------------------------
void task0(){ 
  //Serial.println("TASKER - task0");
  }
//----------------------------------------------------------------------------------------------------------------
void task1()
{ 
  //Serial.println("TASKER - task1");
  }
//----------------------------------------------------------------------------------------------------------------
void task2(){ 
  //Serial.println("TASKER - task2");
  }
//----------------------------------------------------------------------------------------------------------------
void task3(){ 
  //Serial.println("TASKER - task3");
  }
//----------------------------------------------------------------------------------------------------------------
void task4(){ 
  //Serial.println("TASKER - task4");
  }
//----------------------------------------------------------------------------------------------------------------
void task5(){ Serial.println("TASKER - task5");INP.ReadAis();INP.ReadDis();}
//----------------------------------------------------------------------------------------------------------------
void task6(){ 
  Serial.println("TASKER - task6");
  INP.DbgSerialOutAis();
  INP.DbgSerialOutDis();
  INP.DbgSerialOutDos();  
  logic_MAIN();
  }
//----------------------------------------------------------------------------------------------------------------
void task7(){ Serial.println("TASKER - task7");LcdTest_DisplayDi();}
//----------------------------------------------------------------------------------------------------------------
void loop() {
// TAche modbus TCP 
  mb.task();

TASKER.Run();


//gestion tache rtc basse priorite
TMS1 = millis();
  
  if ( (TMS1 - TMS0) > 30000 )  
  { 
    mbDisplayCoils();
    TMS0 = TMS1;
  }

}
//---------------------------------------------------------------------------------------
void LcdTest_DisplayDi()
{
  byte Len = INP.DI_Count();
 byte i =0;
 String LcdLine1 = " Digital Input ";
 String LcdLine2="";
  for (i=0;i< Len;i++)
 {
  if ( INP.DI_GetState(i) ) 
  {  
    LcdLine2 = LcdLine2 + "1";
  }
  else
  {
    LcdLine2 = LcdLine2 + "0";
  }
 }
 
lcd.setCursor(0,0);
 lcd.print(LcdLine1); // print a simple message
 lcd.setCursor(0,1);
 lcd.print(LcdLine2); // print a simple message


 
}

//-------------------------------------------------------------------------------------------
void logic_MAIN()
{
bool a,b,c;
a = INP.DI_GetState(0);b = INP.DI_GetState(1);c = a & b;INP.DO_SetState(0,c);
a = INP.DI_GetState(2);b = INP.DI_GetState(3);c = a & b;INP.DO_SetState(1,c);

}



 
//---------------------------------------------------------------------------------------
// Gestion modbus
//
//
//----------------------------------------------------------------------------------------------------------------
bool abCoilBank1[16];//un mot/word c'est 16 bit
word wBaseAdr1;//adreese de base
word wLenAdr1;//nombre de bits ici 16

void mbSetupCoil()
{
  byte bB0;
  word wW0;//offset index dans la bank
  wBaseAdr1= 0;
  wLenAdr1 = 16;
  //abCoilBank1[0];

  for ( bB0 = 0; bB0 < wLenAdr1; bB0++)
  {
    wW0 = wBaseAdr1+(word)bB0;
    if ( (wW0 %2 == 0 ) || (wW0 == 0))
    {
      mb.addCoil(wW0,true);
      abCoilBank1[wW0] = true;
      Serial.print("Ajout Coil ad ");
      Serial.print(wW0);
      Serial.println(" initial set FALSE ");
    }
    else
    {
      mb.addCoil(wW0,false);
      abCoilBank1[wW0] = false;
      Serial.print("Ajout Coil ad ");
      Serial.print(wW0);
      Serial.println(" initial set FALSE ");
    }
  
  }
//void addCoil (offset word, bool value)
}
//-----------------------------------------------------------------------------------------------------
void mbDisplayCoils()
{
  byte iB0;
  bool bB0;
  word wW0;//offset index dans la bank
  Serial.println("----- COILS ---------");
  Serial.println("ad \t val \t ch");
 for ( iB0 = 0; iB0 < wLenAdr1; iB0++)
  {
    wW0 = wBaseAdr1+(word)iB0;
    bB0 = mb.Coil(wW0);
    
    Serial.print(wW0);Serial.print('\t');
    Serial.print(bB0);Serial.print('\t');
    
    if ( bB0!= abCoilBank1[wW0] )
    {
    Serial.println(" EDGE ");
    abCoilBank1[wW0]=bB0;
    }
    else
    {
    Serial.println(" SAME ");
    }
      

  }



}
