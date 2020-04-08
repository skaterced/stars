/* Contains Explosion, Element -> Meteor, Ennemis ->...
 *  
 */

#ifndef _ELEMENT_
#define _ELEMENT_

#include "globals.h"
#include "vec2.h"

#define NBMAX_METEOR 10
#define METEOR_LIFE 10
#define NBMAX_ENNEMIS 10
#define NBMAX_EXPLOSION 5

#define EXPLOSION_SMALL 1
#define EXPLOSION_MEDIUM 2
#define EXPLOSION_BIG 3

#define ENNEMIS_SPACEINVADER 1
#define ENNEMIS_BIGEYEMONSTER 2

class Explosion {
  public:
    vec2 pos;
    byte type;
    byte count;
    Explosion(){
      type=0;
      pos=vec2(0,0);
      count=50; //means inactive
    }
    Explosion::explode(vec2 pos_, int type_){
      type=type_;
      pos=pos_;    
      count=0;
    }
    Explosion::update(){
      if (count<18){
        count++;
        
        switch(type){
        case EXPLOSION_BIG:
          if (count<6){ //big
            ab.fillCircle(pos.x+mapCoord.x,pos.y+mapCoord.y,2+count*2);
          }
          else if (count<18){
            ab.fillCircle(pos.x+mapCoord.x,pos.y+mapCoord.y,13);
            ab.fillCircle(pos.x+mapCoord.x,pos.y+mapCoord.y,count-6,0);      
          }
        break;
        case EXPLOSION_MEDIUM:    
          if (count<6){ //medium
            ab.fillCircle(pos.x+mapCoord.x,pos.y+mapCoord.y,count+1);
          }
          else if (count<17){
            ab.fillCircle(pos.x+mapCoord.x,pos.y+mapCoord.y,7);
            ab.fillCircle(pos.x+mapCoord.x,pos.y+mapCoord.y,count-6,0);      
          }
        break;
        case EXPLOSION_SMALL:
          if (count<4){ //small
            ab.fillCircle(pos.x+mapCoord.x,pos.y+mapCoord.y,count);
          }
          else if (count<13){
            ab.fillCircle(pos.x+mapCoord.x,pos.y+mapCoord.y,4);
            ab.fillCircle(pos.x+mapCoord.x,pos.y+mapCoord.y,count-4,0);
          }
        break;
        default:
        break;
        }  
      }
    }
};

class Element {  
  public:
    vec2 pos;
    vec2 speed;
    vec2 reste;
    //byte dir; //will add it if needed
    int life;
    int active;
    Element() {
      pos=vec2(0,0);
      speed=vec2(0,0);
      reste=vec2(0,0);
      //dir=0;
      int life=METEOR_LIFE; 
      active=false;
    }  
    void Element::draw();
};
void Element::draw(){
  //ab.setCursor(pos.x+mapCoord.x,pos.y+mapCoord.y);
  //ab.print(F("Est-ce que ca marche?"));
}

class Meteor : public Element {
  public:
    void Meteor::draw() {
      this->pos+=(this->reste+this->speed)/SPEED_DIVISOR;
      this->reste=(this->reste+this->speed)%SPEED_DIVISOR;
      //ab.fillCircle(pos.x+mapCoord.x-6, pos.y+mapCoord.y-6,6,0);
      sprites.drawExternalMask(pos.x+mapCoord.x-6, pos.y+mapCoord.y-6, this->life<(METEOR_LIFE/2)? meteor_dmg:meteor, meteor_mask, 0,0);
      //sprites.drawSelfMasked(pos.x+mapCoord.x-6, pos.y+mapCoord.y-6, this->life<50? meteor_dmg:meteor,0);
    }
};

class Ennemis : public Element {
  public:
    byte frame;
    byte type;
    Ennemis::update(void){
      switch (type){
        case ENNEMIS_SPACEINVADER: default:
          this->pos+=(this->reste+this->speed)/SPEED_DIVISOR;
          this->reste=(this->reste+this->speed)%SPEED_DIVISOR;
          sprites.drawSelfMasked(pos.x+mapCoord.x-5, pos.y+mapCoord.y-4, spaceInvader_sprite, frame);
          if (ab.everyXFrames(5))
            frame=frame==0? 1:0;        
        break;      
        case ENNEMIS_BIGEYEMONSTER:
          this->pos+=(this->reste+this->speed)/SPEED_DIVISOR;
          this->reste=(this->reste+this->speed)%SPEED_DIVISOR;
          if (ab.everyXFrames(10)){
            frame++;   
            if (frame>4)
              frame=0;  
          }    
          //int temp=trigoInv(pos+mapCoord,vec2(64,32));
          sprites.drawSelfMasked(pos.x+mapCoord.x-8,pos.y+mapCoord.y-7,monster,frame);
          vec2 pointD=trigoVec(trigoInv(pos+mapCoord,vec2(64,32)),4,pos)+mapCoord;
          ab.drawRect(pointD.x-1,pointD.y,3,2,0);
          ab.drawPixel(pointD.x,pointD.y);
        break;
      }
    }
  
};


#endif
