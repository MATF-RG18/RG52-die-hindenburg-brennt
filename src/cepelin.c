
#include <GL/glut.h>
#include <math.h>

#include "cepelin.h"

/*
 * Trenutne  koordinate cepelina u xy ravni (z je uvek 0)
 */
float x_curr, y_curr;
/*
 * Vektori za koliko se u svakom taktu pomera cepelin po x i y osi
 */
float v_x, v_y;

/*
 * Indikatori koji odredjuju da li se cepelin krece po x ili y osi 
 */
int movement_indicator_cx = 0;
int movement_indicator_cy = 0;


/*
 * Indikatori koji odredjuju da li je cepelin dosao dovoljno blizu ivice
 * ekrana kako bi zapoceo rotaciju nazad u pocetni polozaj 
 */
int border_rotation_indicator_x = 0;
int border_rotation_indicator_y = 0;

/*
 * Idikator o tome da li je cepelin ziv 
 */
int cepelin_alive = 1;
/*
 * Indikator o tome da li je cepelin zapaljen 
 */
int cepelin_on_fire = 0;
/*
 * Parametar koji broji koliko je cepelinu ostalo da zivi pre nego sto izgori
 */
float cepelin_life_parameter = 0;
/*
 * Koordinate cepelina koje sam dobio iy ModelView matrice
 */
float cepelin_x;
float cepelin_y;
float cepelin_z;
/*
 * Indikator koji govori o tome da li je zapoceta nagla promena smera kretanja
 * cepelina.
 */
int direction_change_indicator[4];

/*
 * Parametar koji odgovara brzini eksplozije cepelina
 */
float explosion_animation_parameter = 0;

/*
 * Parametar koji odgovara brzini pusenja cepelina
 */
float smoke_expansion_parameter = 0;

/*
 * Parametar koji govori o kretanju dima oko cepelina
 */
float smoke_movement_parameter = 0;

/*
 * Indikator koji govori o tome da li treba da se zapocne rotacija nazad u pocetni
 * polozaj, vrednosti tih uglova rotacije oko x-ose(yang) i y-ose(xang) i 
 * za koliko se u svakom taktu inkrementira ili dekrementira data vrednost ugla
 */
int angle_compensation_indicator[4];
float xang, yang;
float delta_xang, delta_yang;


/*
 * Funkcija za iscrtavanje elipsoida koje je potreban za cepelin 
 * Kod je preuzet sa: https://www.gamedev.net/forums/topic/126624-generating-an-ellipsoid-in-opengl/
 * Mogao sam i da odradim prosto skaliranje po z-osi
 */
void DrawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC)
{
    float tStep = (M_PI) / (float)uiSlices;
    float sStep = (M_PI) / (float)uiStacks;
   
    float t;
    for(t = -M_PI/2; t <= (M_PI/2)+.0001; t += tStep)
    {
        glBegin(GL_TRIANGLE_STRIP);
        float s;
        for(s = -M_PI; s <= M_PI+.0001; s += sStep)
        {
          // Koristim normalne vektore kao kod sfere, posto korisnik ne zna 
          // koja je razlika
          
           glNormal3f(
            sin(t) * sin(s),
            cos(t),
            sin(t) * cos(s)
            );
            glVertex3f(fA * cos(t) * cos(s), fB * cos(t) * sin(s), fC * sin(t));
            glVertex3f(fA * cos(t+tStep) * cos(s), fB * cos(t+tStep) * sin(s), fC * sin(t+tStep));
        }
        glEnd();
    }
}


/*
 * Funkcija za iscrtavanje samog cepelina
 */
void draw_cepelin()
{   
    /*
     * Vrsi se rotacija oko x i y ose za odredjeni ugao (sve u cilju da kretanje
     * cepelina bude sto reallisticnije) i translira se cepelin za x_curr i 
     * y_curr 
     */
    glRotatef(xang, 0,1,0);
    glRotatef(yang, 1,0,0);
    glTranslatef(x_curr - size / 2, y_curr - size / 2, 0.5);
    
    
    //Iscrtavanje dima
    glPushMatrix();
    
      glRotatef(-xang, 0,1,0);
      glRotatef(-yang, 1,0,0);
      glTranslatef(smoke_movement_parameter,0,0);
      draw_cepelin_smoke();
      
      
    
    glPopMatrix();
    
    glPushMatrix();
    
      glRotatef(-xang, 0,1,0);
      glRotatef(-yang, 1,0,0);
      glTranslatef(0.9,0,0);
      draw_cepelin_smoke();
      
    glPopMatrix();
    
    glPushMatrix();
    
      glRotatef(-xang, 0,1,0);
      glRotatef(-yang, 1,0,0);
      glTranslatef(-smoke_movement_parameter,0,0);
      draw_cepelin_smoke();
      
    glPopMatrix();
    // Iscrtavanje elipsoida
    DrawEllipsoid(10, 10, 0.5,0.5,1);
    // Iscrtavanje putnickog dela
    glPushMatrix();
        glTranslatef(0, -0.5 , 0);
        glScalef(0.5,0.2,0.5);
        glutSolidCube(1);
    glPopMatrix();
    /*
     * Iscrtavanje peraja na rubu cepelina
     */
    glPushMatrix();
        glTranslatef(-0.2, -0.3 , 0.6);
        glRotatef(30,0,0,1);
        glScalef(0.7,0.04,0.3);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.2, -0.3 , 0.6);
        glRotatef(-30,0,0,1);
        glScalef(0.7,0.04,0.3);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, 0.3 , 0.6);
        glScalef(0.04,0.7,0.2);
        glutSolidCube(1);
    glPopMatrix();
    
}

/*
 * Funckija za iscrtavanje eksplozije kada cepelin izgori (bilo usled kolizije 
 * ili kada izgori usled projektila)
 */
void draw_explosion() 
{
  glPushMatrix();
    /*
     * Eksplozija se transllira na poziciju cepelina, skalira se u skladu 
     * sa parametrom za enimaciju eksplozije i menja se boja eksplozije
     */
    glTranslatef(x_curr, y_curr, 0);
    glScalef(explosion_animation_parameter, explosion_animation_parameter, explosion_animation_parameter);
    GLfloat diffuse_coeffs_explosion[] = { 1, 0.64, 0.0, 1 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_explosion);
        
    glutSolidSphere(1, 20, 20);
    
    glPushMatrix();
    glTranslatef(0.4, 0.4, 0.4);
    glutSolidSphere(0.7, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-1, -0.4, 0.4);
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-1.5, 0.4, 0);
    glutSolidSphere(0.6, 20, 20);
    glPopMatrix();
    
    
    GLfloat diffuse_coeffs[] = { 0.75, 0.75, 0.75, 1 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    
  glPopMatrix();
}

void draw_cepelin_smoke() 
{
  //Iskljucujem osvetljenje posto za dim koristim boje bez osvetljenja
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
        
    //Iscrtavanje dima koji izlazi iz cepelina
        glPushMatrix();
            
            glColor3f(1,1,1);
            
            glTranslatef(cepelin_x-0.7, cepelin_y - 0.5 + smoke_movement_parameter, cepelin_z+0.2);
            glScalef(smoke_expansion_parameter/2, smoke_expansion_parameter/3, smoke_expansion_parameter/0.5);
            
            glutSolidSphere(0.5,10,10);
            
        glPopMatrix();
        
        
        glPushMatrix();
            
            glColor3f(1,1,1);
            
            glTranslatef(cepelin_x, cepelin_y - 0.5 + smoke_movement_parameter, cepelin_z+0.2);
            glScalef(smoke_expansion_parameter, smoke_expansion_parameter, smoke_expansion_parameter);
            
            glutWireSphere(0.5,10,10);
            
        glPopMatrix();
        
        glPushMatrix();
            
            glColor3f(1,1,1);
            
            glTranslatef(cepelin_x, cepelin_y - 0.5 + smoke_movement_parameter, cepelin_z+0.2);
            glScalef(smoke_expansion_parameter, smoke_expansion_parameter, smoke_expansion_parameter);
            
            glScalef(0.4,0.8,0.1);
            glutWireCube(0.4);
            
        glPopMatrix();
    //Ukljucujem svetlo za dalja moguca iscrtavanja    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}