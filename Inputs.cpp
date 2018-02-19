#include "Arduino.h"
#include "Inputs.h"
//-------------------------------------------------------------------------------------------------------------

Inputs::Inputs()
{
  //assignation des seuils de conversion ai vers di
_AIS_Lvls[0] = AILVL0;
_AIS_Lvls[1] = AILVL1;
_AIS_Lvls[2] = AILVL2;
_AIS_Lvls[3] = AILVL3;
_AIS_Lvls[4] = AILVL4;
_AIS_Lvls[5] = AILVL5;
//index deu tableau entree ana
_AIS_idx=0;

} 
//-------------------------------------------------------------------------------------------------------------
//
//
//
//
//-------------------------------------------------------------------------------------------------------------
/*
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

*/    
//DIARD _DIS[DIMAX];//tableau d'entree di
    //le type entree di typedef struct { String lbl; int diarduino;bool vir;byte aiidx;} DIARD;
//  Ajout d'une entre analogique//bool vir pour virtuel ex entree ana vers di n est pas associe a une entree arduino

void Inputs::Add_DI( String lbl , int diarduino , bool vir , byte aiidx )
{
 if ( (_DIS_idx >=0) && (_DIS_idx < DIMAX))
  {

    _DIS[_DIS_idx].lbl = lbl;
    //controle a ajouter si virutel -1 pour diarduino et reciproque
    _DIS[_DIS_idx].diarduino = diarduino;
    if ( diarduino != -1 )
      {
      //ajout 17/02 pour config des entrees de arduino
      //comment savoir si une pin existe en config matereille ??
      //teste a prevoir !!!
        pinMode(diarduino, INPUT); 
      }
    _DIS[_DIS_idx].vir = vir;
    _DIS[_DIS_idx].aiidx = aiidx;//permet de lier entree ana et di
    _DIS_idx++;
  }
}
//-------------------------------------------------------------------------------------------------------------
//typedef struct { String lbl; int doarduino; bool inv; bool val; byte ton; byte toff; } DOARD;
void Inputs::Add_DO( String lbl , int doa , bool inv)
{
 if ( (_DOS_idx >=0) && (_DOS_idx < DOMAX))
  {
  _DOS[_DOS_idx].lbl = lbl;
  _DOS[_DOS_idx].doarduino = doa;
  _DOS[_DOS_idx].inv = inv;
  _DOS[_DOS_idx].val = false;
//configuration amterielel prevoir uen controel des pin sinon crash assuréee
//les sorties sont configuree selon l'etat on les place a 0 par defaut pour des raisosn de securite
//si une entre est inverse elle est donc mise e 1 poru desactiver realsi
  pinMode(doa, OUTPUT);

  if ( _DOS[_DOS_idx].inv == true )
  {
    digitalWrite(_DOS[_DOS_idx].doarduino, HIGH);
  }
  else
  {
    digitalWrite(_DOS[_DOS_idx].doarduino, LOW);
  }
  
  _DOS_idx++;
  }
}

void Inputs::DbgSerialOutDos()
{
byte i=0;
//_DIS[i] buffer des entree anaDIMAX
    for( i = 0 ; i < _DOS_idx ; i++ )
    {
      Serial.print("Inputs::ReadDos => ");
      Serial.print(_DOS[i].lbl);
      Serial.print(" ");
      Serial.println(_DOS[i].val);
    }
}

bool Inputs::DO_SetState( int DOIDX , bool STATE )
{

    if ( ( DOIDX >= 0 ) && ( DOIDX <= DOMAX ) )
    {  Serial.print("Inputs::DO_SetState => DOIDX ");
  Serial.println(DOIDX);
      _DOS[DOIDX].val = STATE;
      if ( (_DOS[DOIDX].val == true) && (_DOS[DOIDX].inv == true) ){ digitalWrite(_DOS[DOIDX].doarduino, LOW);Serial.println("LOW"); }
      if ( (_DOS[DOIDX].val == false) && (_DOS[DOIDX].inv == true) ){ digitalWrite(_DOS[DOIDX].doarduino, HIGH); Serial.println("HIGH");}
      if ( (_DOS[DOIDX].val == true) && (_DOS[DOIDX].inv == false) ){ digitalWrite(_DOS[DOIDX].doarduino, HIGH); Serial.println("HIGH");}
      if ( (_DOS[DOIDX].val == false) && (_DOS[DOIDX].inv == false) ){ digitalWrite(_DOS[DOIDX].doarduino, LOW);Serial.println("LOW"); }
      return true;
    }
  else{
    return false;
    }
}


bool Inputs::DO_GetState( int DOIDX)
{
    if ( ( DOIDX >= 0 ) && ( DOIDX <= DOMAX ) )
    {
      return _DOS[DOIDX].val;
    }
  else{
    return false;
    }
}

byte Inputs::DO_Count()
{
  return _DOS_idx;
}
//-------------------------------------------------------------------------------------------------------------
//  Ajout d'une entre analogique
//  paremtre : lbl          type : String       Emploi : Desigantion de entree
//  paremtre : unt          type : String       Emploi : unite d ela mesure si necessaire
//  paremtre : aiarduino    type : int          Emploi : entree de la carte arduino mega 
//
//------------------------------------------------------------------------------------------------------------
// doit etre modifie pour gere vhh,vh,vl,vll
// on peut prevoir un facteur d'echelle voir uen courbe pour le ssodnes log etc...
    //le type entree ana typedef struct { String lbl; String unt;int aiarduino;} AIARD;
    /*
    typedef struct { 
    String lbl;
    String unt;
    int aiarduino;
    uint val;
    uint vhh;
    uint vl;
    uint vh;
    uint vll;
    bool ahh;
    bool ah;
    bool al;
    bool all;
 } AIARD;
    
    */
    //AIARD _AIS[AIMAX];//tableau d'entree ana
void Inputs::Add_AI( String lbl , String unt, int aiarduino )
{
  if ( (_AIS_idx >=0) && (_AIS_idx < AIMAX))
  {
    _AIS[_AIS_idx].lbl = lbl;
    _AIS[_AIS_idx].unt = unt;
    _AIS[_AIS_idx].aiarduino = aiarduino;
    _AIS_idx++;
  }
}

//-------------------------------------------------------------------------------------------------------------	
int Inputs::GetAiIdxFromLabel(String ailbl)
{
    byte i =0;  
    int idf = -1;
    
    for( i = 0 ; i < AIMAX ; i++ )
    {
      if  ( _AIS[i].lbl == ailbl ) { idf = (int) i; }
    }
    return idf;
}
//-----------------------------------------------------------



//convertit une entree ana en plusieurs di
void Inputs::DspAnaToDis( String ailbl )
{
byte i =0;
bool aiused;//determine si ai est deja utilise ??pour futur
bool dispace = false; //assez de place pour ajout
String lblcomp; //libelle calclule pour la di
int aiidx;//index de ai a convertrt

  //on verifie que l'on a assez de di enreserve
  i = _AIS_idx + AILVL; //index courant des entree di + nb de di virtuel / ai le nombre de di depend du nombre de seuil ou niveua lvl
  // i 0 et AILVL <6 on cree di 0 a 5 soit 6 entree
  if ( i <= DIMAX ){dispace = true;}

//recherche du idx de ai en f de ailbl passé en argument
aiidx = GetAiIdxFromLabel(ailbl);
  //traitement
  if (  (aiidx != -1) && dispace )
  {
      for(i =0 ; i<AILVL; i++)
      {
        lblcomp = "DI:"+ String(_DIS_idx)+";V:1;AI:"+ailbl;
        Add_DI( lblcomp , -1 , true , (byte) aiidx);
      }
    
  }
 
}

//------------------------------------------------------
/*
lit toutes les entrees ana et gere les etat
selon vhh , vh ,vl vll flag ahh ah al all
*/
void Inputs::ReadAis()
{
byte i=0;
//_AIS[i] buffer des entree ana
    for( i = 0 ; i < _AIS_idx ; i++ )
    {
      _AIS[i].val = analogRead(_AIS[i].aiarduino);
    }
}
//-----------------------------------------------------
void Inputs::DbgSerialOutAis()
{
byte i=0;
//_AIS[i] buffer des entree ana
    for( i = 0 ; i < _AIS_idx ; i++ )
    {
      Serial.print("Inputs::ReadAis => ");
      Serial.print(_AIS[i].lbl);
      Serial.print(" ");
      Serial.println(_AIS[i].val);
    }
}
//-----------------------------------------------------
bool Inputs::DI_GetState( int DIIDX)
{
    if ( ( DIIDX >= 0 ) && ( DIIDX <= DIMAX ) )
    {
      return _DIS[DIIDX].val;
    }
  else{
    return false;
    }
}

byte Inputs::DI_Count()
{
  return _DIS_idx;
}


//--------------------------------------------------------
void Inputs::ReadDis()
{

byte i=0;
byte j=0;
unsigned int k =0;
byte idxt=0;
int idxai=0;


for( i = 0 ; i < DIMAX ; i++ )
 {
    if ( ( _DIS[i].vir == true ) && ( _DIS[i].diarduino == -1 ) )
    {
//entree virtuelle
      idxt = _DIS[i].aiidx;
      idxai = _AIS[idxt].aiarduino;
      k= analogRead(idxai);
      for(j =0 ; j<AILVL; j++)
      {
        if ( j ==0 ) //premier seuil
        {
          if (( k >= 0 ) && ( k <= _AIS_Lvls[0])) { _DIS[i].val = true; }
        }
        else // seuils suivant
        {
          if (( k >= _AIS_Lvls[j-1] ) && ( k <= _AIS_Lvls[j])) { _DIS[i+j].val = true; }
        }

       }//fin for
      i = i + AILVL;
      //on a 5 bouton 6 possibilite donc 6 entree virtuelle de i=5 j 0 <6 =>5max i+j =10 max => la prochaine entrre est a i+ 6
    }//fin test entree virtuelle
    else
    {
     //entree reelle
      if( digitalRead( _DIS[i].diarduino ) == HIGH ){ _DIS[i].val = true; }else { _DIS[i].val = false; }
      
      //a implementer plus tard
      //on devra cree un efonction addDi qui configurera la broche du mcu
    }
    
  }//for parcours des entrees
}
//-----------------------------------------------------
void Inputs::DbgSerialOutDis()
{
byte i=0;
//_DIS[i] buffer des entree anaDIMAX
    for( i = 0 ; i < _DIS_idx ; i++ )
    {
      Serial.print("Inputs::ReadDis => ");
      Serial.print(_DIS[i].lbl);
      Serial.print(" ");
      Serial.println(_DIS[i].val);
    }
}


