#ifndef _BACKGROUND_
#define _BACKGROUND_

#include "globals.h"
#include "sprites.h"
#include "vec2.h"
#include "trigo.h"
#include "element.h"
#include "station.h"
#include "shot.h"

#define IMAGE_WIDTH 128
#define IMAGE_HEIGHT 64

#define SECTOR_COLUMNS 10
#define SECTOR_LINES  20

#define MAP_WIDTH SECTOR_COLUMNS*IMAGE_WIDTH
#define MAP_HEIGHT SECTOR_LINES*IMAGE_HEIGHT

#define RADAR_POSX 2
#define RADAR_POSY 54


Meteor met[NBMAX_METEOR];
Ennemis enn[NBMAX_ENNEMIS];
Shot ennShot;
Explosion xplo[NBMAX_EXPLOSION];
byte xploIt=0;
Gem gems[NBMAX_GEM];
/*
Station home=Station (vec2(300,300));
bool station_active=false;
 */
bool isOut(vec2 pos){
  if ((-128>pos.x)||pos.x>MAP_WIDTH||(-64>pos.y)||pos.y>MAP_HEIGHT)
    return true;
  else
    return false;
}
void putMeteor(vec2 pos, vec2 speed){  
  for (int i=0; i<NBMAX_METEOR; i++){
    if (!met[i].active){
      met[i].active=true;
      met[i].pos=pos;
      met[i].life=METEOR_LIFE;
      met[i].speed=speed;
      i=99;
    }
  }
}
/*
void putStation(void){
  station_active=true;
}
*/

void putEnnemis(vec2 pos, vec2 speed, byte type){

  for (int i=0; i<NBMAX_ENNEMIS; i++){
    if (!enn[i].active){   
      enn[i].reboot(pos,speed,type);
      i=NBMAX_ENNEMIS;
    }
  }
}

void explode(vec2 pos, byte type){
  xplo[xploIt].type=type;
  xplo[xploIt].pos=pos;
  xplo[xploIt].count=0;
  if (++xploIt>=NBMAX_EXPLOSION)
    xploIt=0;
}

void addGem(vec2 pos){
  for (int i=0; i<NBMAX_GEM; i++){
    if (!gems[i].active){
      gems[i].active=true;
      gems[i].pos=pos;
      i=99;
    }
  }
}
void mapCenter(){
  mapCoord.x=-(MAP_WIDTH/2-64);
  mapCoord.y=-(MAP_HEIGHT/2-32);
}

void drawStars(){
  //randomSeed(randSeed);
  byte temp=0;
  for (int i=0; i<SECTOR_COLUMNS; i++){
    for (int j=0; j< SECTOR_LINES; j++){
      if (++temp>3)
        temp=0;
      sprites.drawSelfMasked(mapCoord.x+i*IMAGE_WIDTH,mapCoord.y+j*IMAGE_HEIGHT,stars,temp);
    }
  }   
/*  for (int i=0; i<SECTOR_COLUMNS; i++){ 
    for (int j=0; j< SECTOR_LINES; j++){
      byte temp=random(15)+1;
      for (int k=0;k<4;k++){
        if (temp&(8>>k)==(8>>k))
          sprites.drawSelfMasked(mapCoord.x+i*IMAGE_WIDTH,mapCoord.y+j*IMAGE_HEIGHT,stars,k); //nice idea but no...
      } 
    }
  }  */  
}

void drawRadar(){
  volatile bool blinking;
  volatile bool slowBlinking;
  volatile bool fastBlinking;
  if(ab.everyXFrames(5))
    blinking=!blinking;
  if(ab.everyXFrames(10))
    slowBlinking=!slowBlinking;
  if(ab.everyXFrames(3))
    fastBlinking=!fastBlinking;    
  ab.fillRect (RADAR_POSX,RADAR_POSY,11,9);//(2,54,21,9);
  ab.drawPixel(RADAR_POSX+5,RADAR_POSY+4,0);
  ab.drawLine(RADAR_POSX-1,RADAR_POSY-1,RADAR_POSX-1,RADAR_POSY+1); // corners
  ab.drawLine(RADAR_POSX-1,RADAR_POSY-1,RADAR_POSX+1,RADAR_POSY-1);
  ab.drawLine(RADAR_POSX+11,RADAR_POSY-1,RADAR_POSX+11,RADAR_POSY+1);
  ab.drawLine(RADAR_POSX+11,RADAR_POSY-1,RADAR_POSX+9,RADAR_POSY-1);
  ab.drawLine(RADAR_POSX-1,RADAR_POSY+9,RADAR_POSX-1,RADAR_POSY+7);
  ab.drawLine(RADAR_POSX-1,RADAR_POSY+9,RADAR_POSX+1,RADAR_POSY+9);  
  ab.drawLine(RADAR_POSX+11,RADAR_POSY+9,RADAR_POSX+11,RADAR_POSY+7);
  ab.drawLine(RADAR_POSX+11,RADAR_POSY+9,RADAR_POSX+9,RADAR_POSY+9);

  //SECTOR BORDER
  int temp=(mapCoord.x-64)/IMAGE_WIDTH;
  int temp2=(mapCoord.y-32)/IMAGE_HEIGHT;
  if (temp>-5){
    ab.fillRect(RADAR_POSX,RADAR_POSY,temp+5,9,0);
  }
  else if (temp<-(SECTOR_COLUMNS-6)){
    temp+=(SECTOR_COLUMNS-6);
    ab.fillRect(RADAR_POSX+temp+11,RADAR_POSY,-temp,9,0);
  }
  if (temp2>-4){
    ab.fillRect(RADAR_POSX,RADAR_POSY,11,temp2+4,0);
  }
  else if (temp2<-(SECTOR_LINES-5)){
    temp2+=(SECTOR_LINES-5);
    ab.fillRect(RADAR_POSX,RADAR_POSY+temp2+9,11,-temp2,0);
  }
      
  for (int i=0; i<NBMAX_METEOR; i++){
    if (met[i].active){
      temp=(mapCoord.x+met[i].pos.x-29)/IMAGE_WIDTH; //29: IMAGE_WIDTH/4-meteor_image_width/4
      temp2=(mapCoord.y+met[i].pos.y-13)/IMAGE_HEIGHT;
      
      if ((temp<6&&temp>-6)&&(temp2<5&&temp2>-5)){
        ab.drawPixel(RADAR_POSX+temp+5,4+RADAR_POSY+temp2,slowBlinking? 0:1);
      }
    }
  }
  for (int i=0; i<NBMAX_ENNEMIS; i++){
    if (enn[i].active){
      temp=(mapCoord.x+enn[i].pos.x-29)/IMAGE_WIDTH; //pos adjust. from meteor but...
      temp2=(mapCoord.y+enn[i].pos.y-13)/IMAGE_HEIGHT;
      
      if ((temp<6&&temp>-6)&&(temp2<5&&temp2>-5)){
        ab.drawPixel(RADAR_POSX+temp+5,4+RADAR_POSY+temp2,fastBlinking? 0:1);
      }
    }
  }  
  /*
  if (station_active){ //not in radar anymore because it will be in a cut scene
    temp=(mapCoord.x+home.pos.x-49)/IMAGE_WIDTH; //58: IMAGE_WIDTH/2-station_image_width/2= -64 +15
    temp2=(mapCoord.y+home.pos.y+3)/IMAGE_HEIGHT; //  - 32  +35
    if ((temp<6&&temp>-6)&&(temp2<5&&temp2>-5)){
      ab.drawPixel(RADAR_POSX+temp+5,4+RADAR_POSY+temp2,slowBlinking? 0:1);
    }
  }
  */
}

void drawBackground(){//, int RandSeed){  //-------------------------------------- Draw Background --------------------------

 byte temp=0;
  for (int i=0; i<NBMAX_METEOR; i++){
    if (met[i].active){
      if (!isOut(met[i].pos)){
        met[i].draw();
        temp++;
      }
      else {met[i].active=false;}
    }
  }  
  if (0==temp){
    putMeteor(vec2(0,random(1000)), vec2(random(10)+1,random(2)-1));
  }
  temp=0;
  for (int i=0; i<NBMAX_ENNEMIS; i++){
    if (enn[i].active){
      if (!isOut(enn[i].pos)){
        if(enn[i].update()){
          if (0==ennShot.active){
            ennShot.active=SHOT_DURATION;
            int temp=trigoInv(enn[i].pos+mapCoord,vec2(64,32));// <-aiming the ship (if it's not in the border...)
            ennShot.pos=trigoVec(temp,10,enn[i].pos+mapCoord);
            ennShot.dir=temp;            
            ennShot.speed=trigoVec(temp,6,vec2(0,0));             
          }
        }
        temp++;
      }
      else {enn[i].active=false;}
    }
  }
  if (ennShot.active){
    ennShot.draw();
  }
  if (0==temp){
    putEnnemis(vec2(1280,1280),vec2(0,0),ENNEMIS_FLYINGSAUCER);      
  }  
  for (int i=0;i<NBMAX_EXPLOSION;i++){
    xplo[i].update();
  }
  for (int i=0;i<NBMAX_GEM;i++){
    if (gems[i].active)
      gems[i].draw();
  }  
  drawRadar();
}

//todo: add Dmg to both side depending on the speed difference
vec2 elementCollision(vec2 objPos, int radius, int force, int dmg){ //Circular collision check. objPos must be previously centered. 
  //vec2 temp;
  for (int i=0; i<NBMAX_METEOR; i++){
    if (met[i].active){
      
      if ((magn(objPos-met[i].pos-mapCoord)!=-1)&&(magn(objPos-met[i].pos-mapCoord)<(radius+6))){
      //if ((temp.x-7<shipPos.x&&shipPos.x<temp.x+19)&&(temp.y-7<shipPos.y&&shipPos.y<temp.y+19)){ // 22 = 12(image width/heigth)+10(ship radius)
        met[i].life-=dmg;
        if (met[i].life<=0){
          met[i].active=false;
          explode(met[i].pos,EXPLOSION_MEDIUM);
          addGem(met[i].pos); //todo add random ?
        }
        if (force>0){
          met[i].speed-=(objPos-met[i].pos-mapCoord)*force/10;
        }
        return objPos-met[i].pos-mapCoord;
      }
    }
  }
  for (int i=0; i<NBMAX_ENNEMIS; i++){
    if (enn[i].active){
      byte temp;
      if (ENNEMIS_BIGEYEMONSTER==enn[i].type){
        temp=8;
      }
      else
        temp=5;
      if ((magn(objPos-enn[i].pos-mapCoord)!=-1)&&(magn(objPos-enn[i].pos-mapCoord)<(radius+temp))){      
        enn[i].life-=dmg; 
        if (enn[i].life<=0){
          enn[i].active=false;
          explode(enn[i].pos, EXPLOSION_MEDIUM);
          addGem(enn[i].pos); //todo add random ?
        }
        if (force>0){
          enn[i].speed-=(objPos-enn[i].pos-mapCoord)*force/10;
        }
        return objPos-enn[i].pos-mapCoord;
      }
    }
  }
  if (ennShot.active){
      if ((magn(objPos-ennShot.pos)!=-1)&&(magn(objPos-ennShot.pos)<(radius+1))){
        ennShot.active=false;
        return vec2(99,2); // x 99 means hit by an ennemi shot -> 2 is the dmg inflicted
      }
  }
  for (int i=0; i<NBMAX_GEM;i++){
    if (gems[i].active){
      if ((magn(objPos-gems[i].pos-mapCoord)!=-1)&&(magn(objPos-gems[i].pos-mapCoord)<(radius+2))){
        gems[i].active=false;
        return vec2(98,2); // x 98 means coin collected
      }
    }
  }
  
  
  return vec2(0,0);
}


#endif
