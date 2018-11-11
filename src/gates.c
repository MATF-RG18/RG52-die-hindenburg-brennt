
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <time.h>
#include <string.h>
#include "gates.h"

/*
 * Pozicija pemena (vrha kule koji je istovremeno i projektil koji se 
 * ispaljije u pravcu cepelina) dobijena iz ModelView matrice
 */
float pemen_x[7];
float pemen_y[7];
float pemen_z[7];

/*
 * Pozicija BOSS PEMENA (vrha reichstag-a koji je istovremeno i projektil koji se 
 * ispaljije u pravcu cepelina) dobijena iz ModelView matrice
 */
float boss_x;
float boss_y;
float boss_z;

/*
 * Matrica vratnica(kula) koja govori koliiko i gde se generisu kule koje cine svaki
 * od nivoa date igrice. Ona se generise slucajno
 */
int gate_matrix[20][7]; 

// Brojac koji broji do kog smo nivoa (kapije stigli)
int gate_counter = 0;

/*
 * Parametri koji sluze za animaciju kula, animaciju dima (koji se generise kada
 * se ispali projektil), animaciju ispaljivanja projektila i animaiju kretanja 
 * usta pemena (projektila)
 */
float gate_animation_parameter = -51;
float smoke_animation_parameter[8];
float pemen_bullet_animation_parameter[8];
float pemen_animation_parameter;

// niz koji cuva pozicije gde treba da se generise dim
float smoke_position[8];
// insikator koji govori da li treba da se isplai BOSS PEMEN
int fire_boss = 0;

/* Niz indikatora koji govore da li treba da se rotira pemen na vrhu svake kule
 * (za svaku kulu po jedan indikator), vrednosti uglova rotacije oko x i y ose,
 * i za koliko se inkrementiraju
 */
int rotation_indicator[7];
float alpha[7], beta[7];
float alpha_curr[7], beta_curr[7];
float delta_alpha = 6;
float delta_beta = 8;


/*
 * Funkcija za iscrtavanje jedne kule
 * Kao argument se prosledjuje koja kula se iscrtava
 */
void draw_tower(int i) 
{
 
    //Donja polovina kule
    glPushMatrix();
        glTranslatef(0, -0.76 , 0);
        glScalef(0.3,2,0.3);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
        glRotatef(45, 0,1,0);
        glTranslatef(0.17, -0.76 , 0.17);
        glScalef(0.15,2,0.15);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
        glRotatef(45, 0,1,0);
        glTranslatef(-0.17, -0.76 , 0.17);
        glScalef(0.15,2,0.15);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
        glRotatef(45, 0,1,0);
        glTranslatef(0.17, -0.76 , -0.17);
        glScalef(0.15,2,0.15);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
        glRotatef(45, 0,1,0);
        glTranslatef(-0.17, -0.76 , -0.17);
        glScalef(0.15,2,0.15);
        glutSolidCube(1);
    glPopMatrix();
    
    
    
    
    
    
    //Gornja polovina kule
    
    /*
     * Rotira se oko y ose za ugao beta
     */
    glRotatef(-beta_curr[i], 0,1,0);
        
    
    
    //Gornji noseci stub
    glPushMatrix();
        
        glTranslatef(0, 0.75 , 0);
        glScalef(0.15,2,0.15);
        glutSolidCube(1);
    glPopMatrix();
    
    //Lopta ispod Pemena
     glPushMatrix();
        glTranslatef(0, 1.8 , 0);
        glScalef(0.15,0.15,0.15);
        glutSolidSphere(0.724, 20, 20);
    glPopMatrix();  
    
    
    //Lopte na stubu Pemena
    
    glPushMatrix();
        glTranslatef(0, 1.65 , 0.1);
        glScalef(0.15,0.15,0.15);
        glutSolidSphere(0.524, 20, 20);
    glPopMatrix(); 
    
    
    glPushMatrix();
        glTranslatef(0, 1.35 , 0.1);
        glScalef(0.15,0.15,0.15);
        glutSolidSphere(0.524, 20, 20);
    glPopMatrix(); 
    
    glPushMatrix();
        glTranslatef(0, 1.05 , 0.1);
        glScalef(0.15,0.15,0.15);
        glutSolidSphere(0.524, 20, 20);
    glPopMatrix(); 
    
    glPushMatrix();
        glTranslatef(0, 0.75 , 0.1);
        glScalef(0.15,0.15,0.15);
        glutSolidSphere(0.524, 20, 20);
    glPopMatrix(); 
    
    glPushMatrix();
        glTranslatef(0, 0.45 , 0.1);
        glScalef(0.15,0.15,0.15);
        glutSolidSphere(0.524, 20, 20);
    glPopMatrix(); 
    
    
    
    
    
    
    
    /*Pemen */
    
    // Promena boje pemena
    GLfloat diffuse_coeffs_pemen[] = { 1, 1, 0.0, 1 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_pemen);
        
   
    /* 
     * Osnova Pemena: dve odsecene sfere koje se pokrecu (implementirano preko
     * ravni odsecanja) 
     */
    glPushMatrix();
        
        glTranslatef(0,2,0);
        glScalef(0.2,0.2,0.2);
        glScalef(0.5,0.5,0.5);
        
        // ako je omogucena rotacija, rotiraj pemen za dati ugao 
        if(rotation_indicator[i] == 1){
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
          
        }
        // ako nije, rotiraj za poslednji ugao u poziciju i ispali
        else {
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
          glTranslatef(0, 0, pemen_bullet_animation_parameter[i]);
          
        }
        
        glRotatef(20, 1, 0,0);
        
        double clip_plane_1[] = {0, -1, sin(pemen_animation_parameter), 0};
        double clip_plane_2[] = {0, sin(pemen_animation_parameter),-1,0};;
        
        glClipPlane(GL_CLIP_PLANE0, clip_plane_1);
        glClipPlane(GL_CLIP_PLANE1, clip_plane_2);
        
        
       
        glEnable(GL_CLIP_PLANE0);
       
        
        glutSolidSphere(1.5, 20, 20);
        
        
        /*
         * Ovde izvlacim poziciju pemena (projektila) u globalnom koordinantnom
         * sistemu kako bih mogao da vrsim detekciju kolizija sa cepelinom
         */ 
         
        GLfloat matrix[16]; 
        glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
        
        pemen_x[i] = matrix[12];
        pemen_y[i] = matrix[13];
        pemen_z[i] = matrix[14];
          
        
        
        glDisable(GL_CLIP_PLANE0);
        
       
        glEnable(GL_CLIP_PLANE1);
        
        glutSolidSphere(1.5, 20, 20);
        glDisable(GL_CLIP_PLANE1);
    glPopMatrix();
    
    
    /*
     * Osnova Pemena: jedna fiksna odsecena sfera kako pemen ne bi otvorio 
     * skroz usta
     */
    glPushMatrix();
        
        
        glTranslatef(0,2,0);
        glScalef(0.2,0.2,0.2);
        glScalef(0.5,0.5,0.5);
        
        // ako je omogucena rotacija, rotiraj pemen za dati ugao 
        if(rotation_indicator[i] == 1){
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
            
        }
        // ako nije, rotiraj za poslednji ugao u poziciju i ispali
        else {
          
          draw_smoke(i);
          
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
          glTranslatef(0,0,pemen_bullet_animation_parameter[i]);
          
        }
        
        glRotatef(20, 1, 0,0);
        
        double clip_plane_3[] = {0, 0,-1,0};
        glClipPlane(GL_CLIP_PLANE3, clip_plane_3);
        glEnable(GL_CLIP_PLANE3);
        glutSolidSphere(1.5, 20, 20);
        glDisable(GL_CLIP_PLANE3);
        
        
    glPopMatrix();

    /*
     * Oci, brkovi i obrve Pemena 
     */
    // Levo oko  
    glPushMatrix();
    
        GLfloat diffuse_coeffs[] = { 0.75, 0.75, 0.75, 1 };
    
        glTranslatef(0,2,0);
        glScalef(0.2,0.2,0.2);
        glScalef(0.5,0.5,0.5);
        
        // ako je omogucena rotacija, rotiraj pemen za dati ugao 
        if(rotation_indicator[i] == 1){
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
           
        }
        // ako nije, rotiraj za poslednji ugao u poziciju i ispali
        else {
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
          glTranslatef(0,0,pemen_bullet_animation_parameter[i]);
          
        }
        
        
        glTranslatef(-1.25, 1.25, 0);
        // Menjam boju oka u crveno
        GLfloat diffuse_coeffs_eye[] = { 0.9, 0.0, 0.0, 1 };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_eye);
            
        glutSolidSphere(0.3, 20, 20);
    
        // Vracam prvobitne parametre osvetljenja
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
            
    glPopMatrix();

    // Desno oko
    glPushMatrix();

            
    
        glTranslatef(0,2,0);
        glScalef(0.2,0.2,0.2);
        glScalef(0.5,0.5,0.5);
        
        // ako je omogucena rotacija, rotiraj pemen za dati ugao 
        if(rotation_indicator[i] == 1){
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
 
        }
        // ako nije, rotiraj za poslednji ugao u poziciju i ispali
        else {
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
          glTranslatef(0,0,pemen_bullet_animation_parameter[i]);
          
        }
        glTranslatef(1.25, 1.25, 0);
        
        
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_eye);
            
        
        glutSolidSphere(0.3, 20, 20);
        
        
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
            
    
    glPopMatrix();

    // Desna obrva
    glPushMatrix();
    
        
        glTranslatef(0,2,0);
        glScalef(0.2,0.2,0.2);
        glScalef(0.5,0.5,0.5);
    
        glTranslatef(1.2, 2, 0.8);
        
        // ako je omogucena rotacija, rotiraj pemen za dati ugao 
        if(rotation_indicator[i] == 1){ 
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
            
        }
        // ako nije, rotiraj za poslednji ugao u poziciju i ispali
        else {
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
          glTranslatef(0,0,pemen_bullet_animation_parameter[i]);
          
        }
        
        glRotatef(30, 0,0,1);
        glScalef(1.2, 0.3,0.3);
        glutSolidCube(1);
    
    glPopMatrix();
    
    // Leva obrva
    glPushMatrix();
    
        
        glTranslatef(0,2,0);
        glScalef(0.2,0.2,0.2);
        glScalef(0.5,0.5,0.5);
    
        glTranslatef(-1.2, 2, 0.8);
        
        // ako je omogucena rotacija, rotiraj pemen za dati ugao 
        if(rotation_indicator[i] == 1){
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
            
        }
        // ako nije, rotiraj za poslednji ugao u poziciju i ispali
        else {
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
          glTranslatef(0,0,pemen_bullet_animation_parameter[i]);
          
        }
        
        glRotatef(-30, 0,0,1);
        glScalef(1.2, 0.3,0.3);
        glutSolidCube(1);
    
    glPopMatrix();

    // Brkovi
    glPushMatrix();
       
        
        glTranslatef(0,2.05,0);
        glScalef(0.2,0.2,0.2);
        glScalef(0.5,0.5,0.5);
        
        // ako je omogucena rotacija, rotiraj pemen za dati ugao 
        if(rotation_indicator[i] == 1){
            glRotatef(-alpha_curr[i] , 1, 0, 0);
            
        }
        // ako nije, rotiraj za poslednji ugao u poziciju i ispali
        else {
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
          glTranslatef(0,0,pemen_bullet_animation_parameter[i]);
          
        }
        glTranslatef(0,0.4,1.6);
        glScalef(0.3,0.3,0.1);
        glutSolidCube(1);
    
    glPopMatrix();
    
}

/*
 * Funkcija za iscrtavanje srusene kule
 */
void draw_half_tower()
{
    glPushMatrix();
        glTranslatef(0, -0.76 , 0);
        glScalef(0.3,2,0.3);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
        glRotatef(45, 0,1,0);
        glTranslatef(0.17, -0.76 , 0.17);
        glScalef(0.15,2,0.15);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
        glRotatef(45, 0,1,0);
        glTranslatef(-0.17, -0.76 , 0.17);
        glScalef(0.15,2,0.15);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
        glRotatef(45, 0,1,0);
        glTranslatef(0.17, -0.76 , -0.17);
        glScalef(0.15,2,0.15);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
        glRotatef(45, 0,1,0);
        glTranslatef(-0.17, -0.76 , -0.17);
        glScalef(0.15,2,0.15);
        glutSolidCube(1);
    glPopMatrix();
    
}

// Funkcija za iscrtavanje Reichstag-a sa sve PEMEN BOSS-om
void draw_reichstag() 
{
  
  glPushMatrix();
    glTranslatef(0,-0.76, 0);
    glScalef(6, 1, 2);
    glutSolidCube(1);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(-3,-0.76, 1);
    glScalef(1, 2, 0.5);
    glutSolidCube(1);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(3,-0.76, 1);
    glScalef(1, 2, 0.5);
    glutSolidCube(1);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(-3,-0.76, -1);
    glScalef(1, 2, 0.5);
    glutSolidCube(1);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(3,-0.76, -1);
    glScalef(1, 2, 0.5);
    glutSolidCube(1);
  glPopMatrix();
  
  GLfloat diffuse_coeffs_reicstag_front[] = { 0.0, 0.75, 0.75, 1 };
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_reicstag_front);
    
  glPushMatrix();
    glTranslatef(0,-0.76, 1.25);
    glScalef(1.5, 1.2, 0.25);
    glutSolidCube(1);
  glPopMatrix();
  
  GLfloat diffuse_coeffs_pillars[] = { 0.75, 0.0, 0.25, 1 };
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_pillars);
    
  
  glPushMatrix();
    glTranslatef(-0.75,-0.76, 1.25);
    glScalef(0.25, 1.7, 0.25);
    glutSolidCube(1);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.75,-0.76, 1.25);
    glScalef(0.25, 1.7, 0.25);
    glutSolidCube(1);
  glPopMatrix();
  
  GLfloat diffuse_coeffs[] = { 0.75, 0.75, 0.75, 1 };
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    
  
  
  
  
  
  /*BOSS PEMEN*/
  
  GLfloat diffuse_coeffs_pemen[] = { 1, 1, 0.0, 1 };
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_pemen);
  
  glPushMatrix();
  glTranslatef(0,1,0);
  glScalef(8,8,8);
    glPushMatrix();
          
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
          
          /*
           * Ako je ukljucen indikator za ispaljivanje BOSS-a, zapocni paljbu
           * (translacija po z osi)
           */ 
           
          if(fire_boss == 1) {
            glTranslatef(0,0,pemen_bullet_animation_parameter[7]);
          }
          
          glRotatef(45, 1, 0,0);
          double clip_plane_1[] = {0, -1, sin(pemen_animation_parameter), 0};
          double clip_plane_2[] = {0, sin(pemen_animation_parameter),-1,0};;
          
          glClipPlane(GL_CLIP_PLANE0, clip_plane_1);
          glClipPlane(GL_CLIP_PLANE1, clip_plane_2);
          
          
          
        
          glEnable(GL_CLIP_PLANE0);
        
          
          glutSolidSphere(1.5, 20, 20);
          
          /*
           * Ovde se izvlace koordinate BOSS PEMEN-a iz ModelView matrice
           * koje se kasnije koriste za detekciju kolizija sa cepelinom
           */
          GLfloat matrix[16]; 
          glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
          
          boss_x = matrix[12];
          boss_y = matrix[13];
          boss_z = matrix[14];
          
          
          glDisable(GL_CLIP_PLANE0);
          
        
          glEnable(GL_CLIP_PLANE1);
          
          glutSolidSphere(1.5, 20, 20);
          glDisable(GL_CLIP_PLANE1);
      glPopMatrix();
      
      
      
      /* 
       * Osnova Pemena: jedna fiksna odsecena sfera kako pemen ne bi otvorio 
       * skroz usta
       */
      glPushMatrix();
          
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
          
          /*
           * Ako je ispaljen BOSS, crtaj dim i 
           * zapocni paljbu (translacija po z osi)
           */ 
           
          if(fire_boss == 1) {
            
            draw_smoke(7);
            
            glTranslatef(0,0,pemen_bullet_animation_parameter[7]);
           
          }
          
          glRotatef(45, 1, 0,0);
          double clip_plane_3[] = {0, 0,-1,0};
          glClipPlane(GL_CLIP_PLANE3, clip_plane_3);
          glEnable(GL_CLIP_PLANE3);
          glutSolidSphere(1.5, 20, 20);
          glDisable(GL_CLIP_PLANE3);
          
          
      glPopMatrix();

      /* 
       * Oci, brkovi i obrve Pemena 
       */
      // Levo oko  
      glPushMatrix();
      
          
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
          
          /*
           * Ako je ukljucen indikator za ispaljivanje BOSS-a, zapocni paljbu
           * (translacija po z osi)
           */ 
           
          if(fire_boss == 1) {
            glTranslatef(0,0,pemen_bullet_animation_parameter[7]);
          }
          
          glTranslatef(-1.25, 1.25, 0);
          
          GLfloat diffuse_coeffs_eye[] = { 0.9, 0.0, 0.0, 1 };
          glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_eye);
        
          
          glutSolidSphere(0.3, 20, 20);
      
          glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
          
      glPopMatrix();

      // Desno oko
      glPushMatrix();

      
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
          
          /*
           * Ako je ukljucen indikator za ispaljivanje BOSS-a, zapocni paljbu
           * (translacija po z osi)
           */ 
           
          if(fire_boss == 1) {
            glTranslatef(0,0,pemen_bullet_animation_parameter[7]);
          }
          
          glTranslatef(1.25, 1.25, 0);
          
          glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_eye);
          glutSolidSphere(0.3, 20, 20);
          glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
          
      glPopMatrix();

      // Desna obrva
      glPushMatrix();
      
      
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
      
          /*
           * Ako je ukljucen indikator za ispaljivanje BOSS-a, zapocni paljbu
           * (translacija po z osi)
           */ 
           
          if(fire_boss == 1) {
            glTranslatef(0,0,pemen_bullet_animation_parameter[7]);
          }
          
          glTranslatef(1.2, 2, 0.8);
          glRotatef(30, 0,0,1);
          glScalef(1.2, 0.3,0.3);
          glutSolidCube(1);
      
      glPopMatrix();
      
      // Leva obrva
      glPushMatrix();
      
          
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
      
          /*
           * Ako je ukljucen indikator za ispaljivanje BOSS-a, zapocni paljbu
           * (translacija po z osi)
           */ 
           
          if(fire_boss == 1) {
            glTranslatef(0,0,pemen_bullet_animation_parameter[7]);
          }
          
          glTranslatef(-1.2, 2, 0.8);
          glRotatef(-30, 0,0,1);
          glScalef(1.2, 0.3,0.3);
          glutSolidCube(1);
      
      glPopMatrix();

      // Brkovi
      glPushMatrix();
        
          
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
          
          /*
           * Ako je ukljucen indikator za ispaljivanje BOSS-a, zapocni paljbu
           * (translacija po z osi)
           */ 
           
          if(fire_boss == 1) {
            glTranslatef(0,0,pemen_bullet_animation_parameter[7]);
          }
          
          glTranslatef(0,0.4,1.6);
          glScalef(0.3,0.3,0.1);
          glutSolidCube(1);
      
      glPopMatrix();
    glPopMatrix();
 
}


/*
 * Funkcija za iscrtavanje dima prilikom ispaljivanja projektila
 * Kao argument se prosledjuje kod koje kule se crta dim
 */
void draw_smoke(int i) 
{   
    //Iskljucujem osvetljenje posto za dim koristim boje bez osvetljenja
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
        
        glPushMatrix();
            
            glColor3f(1,1,1);
            
            if(i != 7) { //iscrtavanje dima kada se ispali prijektil sa neke od kula
                glTranslatef(i, 1, smoke_position[i]);
            
            }
            else { //iscrtavanje dima kada se ispali BOSS PEMEN
                glTranslatef(0, 1, smoke_position[7]);
                
            }
            
            // Iscrtavanje samog dima
            if(smoke_animation_parameter[i] < 8){
                glScalef(smoke_animation_parameter[i], smoke_animation_parameter[i], smoke_animation_parameter[i]);
                glutWireCube(1);
               
                glPushMatrix();
                glTranslatef(-0.4, -0.5, -0.5);
                
                glutWireSphere(0.3, 10, 10);
                glPopMatrix();
                
                glPushMatrix();
                glTranslatef(0.4, -0.5, -0.5);
                glScalef(0.8, 0.8, 0.8);
                glutWireSphere(0.3, 10, 10);
                glPopMatrix();
                
                glPushMatrix();
                glTranslatef(0.8, 0, -0.5);
                glScalef(0.8, 0.8, 0.8);
                glutWireSphere(0.3, 10, 10);
                glPopMatrix();
                
                
                glPushMatrix();
                glTranslatef(-0.4, 0.3, 0);
                glScalef(2, 0, 1.5);
                glutWireCube(1);
                glPopMatrix();
                
                glPushMatrix();
                glTranslatef(0.3, 0.05, 0);
                glScalef(1, 0.3, 2.2);
                glutWireCube(1);
                glPopMatrix();
                
                glPushMatrix();
                glTranslatef(-0.2, 0, 0);
                glScalef(2.2, 0.1, 1.2);
                glutWireCube(1);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(-1.4, 0.4, 0.5);
                glScalef(0.3, 0.3, 0.3);
                glutWireCube(1);
                glPopMatrix();
                
                glPushMatrix();
                glTranslatef(0.45, -0.3, 0);
                glScalef(0.5, 0.1, 1.5);
                glutWireCube(1);
                glPopMatrix();
                
                glPushMatrix();
                glTranslatef(-0.4, 0, 0.2);
                glScalef(0.4, 0.2, 0.5);
                glutWireCube(1);
                glPopMatrix();
               
            }
            
        
        glPopMatrix();
    //Ukljucujem svetlo za dalja moguca iscrtavanja    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}


/*
 * Funkcija koja na osnovu prosledjenih koordinata, kao argumenata, racuna 
 * ugao izmedju dve tacke
 */
double aim_angle_init(double x1,  double y1, double x2, double y2)
{
  
  double k = (y2-y1) / (x2 - x1);
  double ugao = atan(k)*180/M_PI;
  if(ugao >= 90)
    return ugao - 90;
  else
    return ugao;
}


/* 
 * Funkcija koja inicijallizuje matricu kula (nasumicno rasporedjuje kule po
 * nivoima)
 */
void gate_matrix_init()
{
  
  
  
    int i;
    //Prvi nivo su dve kule koje su van domasaja cepelina
    gate_matrix[0][0] = 1;
    gate_matrix[0][6] = 1;
   
    
    
    srand(time(NULL));
    
    int r1, r2, r3, r4;
    
    //Ovde se inicijalizuju ostale vratnice
    for (i = 1; i < 20; i++) {
      
      r1 = r2 = r3 = r4 = 0;

      //Prvih 5 vratnica imaju samo po jedan stub
      if(i < 5) {
        
        r1 = rand() % 7;
        gate_matrix[i][r1] = 1;
      }
      // Drugih 5 imaju 2 stuba
      else if(i < 10 && i >=5) {
        
        

        r1 = rand() % 7;
        r2 = rand() % 7;
        if(r1 == r2) {
          r2 = (r2 + 1) % 7;
        }
        gate_matrix[i][r1] = 1;
        gate_matrix[i][r2] = 1;
      }
      // Trecih 5 imaju 3 stuba
      else if(i < 15 && i >= 10) {
        
        

        r1 = rand() % 7;
        r2 = rand() % 7;
        r3 = rand() % 7;
        while (r1 == r2) {
          r2 = rand() % 7;
        }
        while (r3 == r1 || r3 == r2) {
          r3 = rand() % 7;
        }
        gate_matrix[i][r1] = 1;
        gate_matrix[i][r2] = 1;
        gate_matrix[i][r3] = 1;
      
      }
      // Poslednjih 5 imaju 4 stuba
      else if(i < 20 && i >= 15) {
        
        

        r1 = rand() % 7;
        r2 = rand() % 7;
        r3 = rand() % 7;
        r4 = rand() % 7;
        while (r1 == r2) {
          r2 = rand() % 7;
        }
        while (r3 == r1 || r3 == r2) {
          r3 = rand() % 7;
        }
        
        while (r4 == r1 || r4 == r2 || r4 == r3) {
          r4 = rand() % 7;
        }
        
        gate_matrix[i][r1] = 1;
        gate_matrix[i][r2] = 1;
        gate_matrix[i][r3] = 1;
        gate_matrix[i][r4] = 1;
      
      }  
    }
    
}