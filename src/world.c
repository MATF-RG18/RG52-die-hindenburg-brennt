#include <GL/glut.h>


#include "world.h"

// sirina i visina prozora
int window_width;
int window_height;

// parametar animacije poda
double tile_animation_parameter;

//parametar animacije ukrasa sa strane
double world_blocks_animation_parameter;

/* 
 * Fukncija za iscrtavanje sveta
 */
void draw_world()
{
  int i;
  
  // Iskljucujem svetlo
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glPushMatrix();
  
    //Iscrtavaja se donja ravan
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_POLYGON);
        glVertex3f(7, -3, -70);
        glVertex3f(-7, -3, -70);
        glVertex3f(-7, -3, 4);
        glVertex3f(7, -3, 4);
    glEnd();
    
  

    //Iscrtavaju se uzduzne linije
    glColor3f(0.9, 0.32, 0.69);
    for(i = -7; i<8; i++){
      glBegin(GL_POLYGON);
          glVertex3f(i+0.05, -2.98, -70);
          glVertex3f(i-0.05, -2.98, -70);
          glVertex3f(i-0.05, -2.98, 1);
          glVertex3f(i+0.05, -2.98, 1);
      glEnd();
    }
    //Iscrtavaju se poprecne linije
    for(i = -70; i<3; i+=2){
      glBegin(GL_POLYGON);
          glVertex3f(7, -2.98, tile_animation_parameter+i-0.05);
          glVertex3f(-7, -2.98, tile_animation_parameter+i-0.05);
          glVertex3f(-7, -2.98, tile_animation_parameter+i+0.05);
          glVertex3f(7, -2.98, tile_animation_parameter+i+0.05);
      glEnd();
    }
  glPopMatrix();
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  /*
   * Iscrtavaju se ukrasi sa leve i desne strane
   */
  
  //leva strana
  for(i = -70; i<3; i+=2){
    glPushMatrix();
      glTranslatef(-6.5, 2.7, world_blocks_animation_parameter+i);
      glScalef(0.5,0.5,1);
      glutSolidCube(1);
    glPopMatrix();
  }
  
  for(i = -69; i<3; i+=2){
    glPushMatrix();
      glTranslatef(-6.5, 1.7, world_blocks_animation_parameter+i);
      glScalef(0.5,0.5,1);
      glutSolidCube(1);
    glPopMatrix();
  }
  
  for(i = -70; i<3; i+=2){
    glPushMatrix();
      glTranslatef(-6.5, 0.7, world_blocks_animation_parameter+i);
      glScalef(0.5,0.5,1);
      glutSolidCube(1);
    glPopMatrix();
  }
  
  // desna strana
  for(i = -70; i<3; i+=2){
    glPushMatrix();
      glTranslatef(6.5, 2.7, world_blocks_animation_parameter+i);
      glScalef(0.5,0.5,1);
      glutSolidCube(1);
    glPopMatrix();
  }
  
  for(i = -69; i<3; i+=2){
    glPushMatrix();
      glTranslatef(6.5, 1.7, world_blocks_animation_parameter+i);
      glScalef(0.5,0.5,1);
      glutSolidCube(1);
    glPopMatrix();
  }
  
  for(i = -70; i<3; i+=2){
    glPushMatrix();
      glTranslatef(6.5, 0.7, world_blocks_animation_parameter+i);
      glScalef(0.5,0.5,1);
      glutSolidCube(1);
    glPopMatrix();
  }
 
 /*
  * Iscrtavaju se bankine sa leve i desne strane na donjoj ravni
  */
  for(i = -70; i<3; i++){
    glPushMatrix();
      glTranslatef(6.5, -2.98, i);
      glScalef(0.5,1,1);
      glutSolidCube(1);
    glPopMatrix();
  }
  
  for(i = -70; i<3; i++){
    glPushMatrix();
      glTranslatef(-6.5, -2.98, i);
      glScalef(0.5,1,1);
      glutSolidCube(1);
    glPopMatrix();
  }
}
