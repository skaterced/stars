#ifndef _BACKGROUND_
#define _BACKGROUND_

#include "globals.h"
#include "sprites.h"
#include "vec2.h"
#include "element.h"

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

void putMeteor(vec2 pos){

  for (int i=0; i<NBMAX_METEOR; i++){
    if (!met[i].active){
      met[i].active=true;
      met[i].pos=pos;  //ok, maybe not rand... 
      //met[i].pos.y=50;
      met[i].life=100;
      met[i].speed=vec2(1,0);
      return 0;
    }
  }
}
void putEnnemis(vec2 pos, vec2 speed){

  for (int i=0; i<NBMAX_ENNEMIS; i++){
    if (!enn[i].active){
      enn[i].active=true;
      enn[i].pos=pos;        
      enn[i].life=100;
      enn[i].speed=speed;
      return 0;
    }
  }
}

void mapCenter(){
  mapCoord.x=-(MAP_WIDTH/2-64);
  mapCoord.y=-(MAP_HEIGHT/2-32);
}

//void drawStars(int x, int y, int RandSeed){ //camera X,Y
void drawStars(int randSeed){ //int x, int y, int RandSeed){
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
  if(ab.everyXFrames(5))
    blinking=!blinking;
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
      temp=(mapCoord.x+met[i].pos.x-58)/IMAGE_WIDTH; //58: IMAGE_WIDTH/2-meteor_image_width/2
      temp2=(mapCoord.y+met[i].pos.y-26)/IMAGE_HEIGHT;
      
      if ((temp<6&&temp>-6)&&(temp2<5&&temp2>-5)){
        ab.drawPixel(RADAR_POSX+temp+5,4+RADAR_POSY+temp2,blinking? 0:1);
      }
    }
  }
}

void drawBackground(int x, int y, int RandSeed){
  drawStars(RandSeed);//x, y, RandSeed);
  //vec2 UperLeftCorner=vec2(1,1);
  //ab.everyXFrames(2)
  //ab.drawRect(UperLeftCorner.x,UperLeftCorner.y,MAP_WIDTH,MAP_HEIGHT);
  for (int i=0; i<NBMAX_METEOR; i++){
    if (met[i].active){
      met[i].draw();
    }
  }
  for (int i=0; i<NBMAX_ENNEMIS; i++){
    if (enn[i].active){
      enn[i].draw();
    }
  }  

  drawRadar();
}

vec2 Metcollision(vec2 objPos, int radius, int force, int dmg){ //Circular collision check. objPos must be previously centered. 
  vec2 temp;
  for (int i=0; i<NBMAX_METEOR; i++){
    if (met[i].active){
      temp=met[i].pos+mapCoord+vec2(6,6);
      if ((magn(objPos-temp)!=-1)&&(magn(objPos-temp)<(radius+6))){
      //if ((temp.x-7<shipPos.x&&shipPos.x<temp.x+19)&&(temp.y-7<shipPos.y&&shipPos.y<temp.y+19)){ // 22 = 12(image width/heigth)+10(ship radius)
        met[i].life-=dmg; //todo check if alive and draw lifeBar
        if (force>0){
          met[i].speed-=(objPos-temp)*force/10;
        }
        return objPos-temp;
      }
    }
  }
  temp=vec2(0,0);
  return temp;
}


/*
void drawMeteor(vec2 pos, bool dmg){  // to erase...
  sprites.drawExternalMask(pos.x, pos.y, dmg? meteor_dmg:meteor, meteor_mask, 0,0);
}
*/
void drawVecLine(vec2 A, vec2 B){
  ab.drawLine(A.x,A.y,B.x,B.y);
}

#endif
