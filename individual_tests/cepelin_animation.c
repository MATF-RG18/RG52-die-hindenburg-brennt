#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <math.h>

#include <stdio.h>


#define TIMER_ID 0
#define TIMER_INTERVAL 20


static int window_width;
static int window_height;

//static int keystates[256];
static int keystates[4];

const static float size = 0.5;  /* Sirina i visina cepelina. */
static float x_curr, y_curr;    /* Tekuce koordinate centra kvadrata. */
static float v_x, v_y;          /* Komponente vektora brzine kretanja
* kvadrata. */
static int animation_ongoing;   /* Fleg koji odredjuje da li je
                                 * animacija u toku. */
static int indikator_kretanja_cx = 0;
static int indikator_kretanja_cy = 0;

static int granicni_indikator_rotacije_x = 0;
static int granicni_indikator_rotacije_y = 0;

static int indikator_promene_smera[4];

static int indikator_nadoknade[4];

static float xang, yang;
static float delta_xang, delta_yang;


/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);
static void on_display(void);
static void on_reshape(int width, int height);
static void on_keyboardUp(unsigned char key, int x, int y);

void DrawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC);

int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(700, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    /* Registruju se funkcije za obradu dogadjaja. */
    glutKeyboardFunc(on_keyboard);
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardUpFunc(on_keyboardUp);
    glutIgnoreKeyRepeat(0);
    
    srand(time(NULL));

    x_curr = y_curr = 0;

    xang = yang = 0;
    delta_xang = delta_yang = 0.5;
   
    
    v_x = 0.05;
    v_y = 0.05;
    animation_ongoing = 0;
    
    keystates[0] = 0;
    keystates[1] = 0;
    keystates[2] = 0;
    keystates[3] = 0;
    
    indikator_nadoknade[0] = 0;
    indikator_nadoknade[1] = 0;
    indikator_nadoknade[2] = 0;
    indikator_nadoknade[3] = 0;

    indikator_promene_smera[0] = 0;
    indikator_promene_smera[1] = 0;
    indikator_promene_smera[2] = 0;
    indikator_promene_smera[3] = 0;
    
    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0.8, 0.8, 0.8, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    /* Ulazi se u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;

    case 'g':
    case 'G':
        /* Pokrece se animacija. */
        if (!animation_ongoing) {
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            animation_ongoing = 1;
        }
        
        break;

    case 'A':
    case 'a':
      if(indikator_nadoknade[1] == 0){
      if(animation_ongoing && v_x > 0 && indikator_promene_smera[1] == 0) {
        v_x = v_x * (-1);
        if(delta_xang < 0) {
          delta_xang = delta_xang * (-1);
        }
      }
      keystates[0] = 1;
      indikator_kretanja_cx = 1;
      glutPostRedisplay();
      }
    break;
    
    case 'D':
    case 'd':
      if(indikator_nadoknade[0] == 0) {
      if(animation_ongoing && v_x < 0 && indikator_promene_smera[0] == 0) {
        v_x = v_x * (-1);
        
        if(delta_xang > 0) {
          delta_xang = delta_xang * (-1);
        }
      }
      indikator_kretanja_cx = 1;
      keystates[1] = 1;
      glutPostRedisplay();
      }
    break;

    case 'W':
    case 'w':
      if(indikator_nadoknade[3] == 0) {
      if(animation_ongoing && v_y < 0 && indikator_promene_smera[3] == 0) {
        v_y = v_y * (-1);
        
        if(delta_yang < 0) {
          delta_yang = delta_yang * (-1);
        }
      }
      keystates[2] = 1;
      indikator_kretanja_cy = 1;
      glutPostRedisplay();
      }
    break;
    
    case 'S':
    case 's':
      if(indikator_nadoknade[2] == 0) {
      if(animation_ongoing && v_y > 0 && indikator_promene_smera[2] == 0) {
        v_y = v_y * (-1);
        
        if(delta_yang > 0) {
          delta_yang = delta_yang * (-1);
        }
      }
      keystates[3] = 1;
      indikator_kretanja_cy = 1;
      glutPostRedisplay();
      }
    break;
    
    case 'h':
    case 'H':
        /* Zaustavlja se animacija. */
        animation_ongoing = 0;
        break;
    }
}

static void on_keyboardUp(unsigned char key, int x, int y)
{
  switch(key) {
    case 'a':
    case 'A':
      keystates[0] = 0;
      if(indikator_nadoknade[1] == 0)
        indikator_nadoknade[0] = 1;
    break;
    
    case 'D':
    case 'd':
      keystates[1] = 0;
      if(indikator_nadoknade[0] == 0)
        indikator_nadoknade[1] = 1;
    break;
    
    case 'W':
    case 'w':
      keystates[2] = 0;
      if(indikator_nadoknade[3] == 0)
        indikator_nadoknade[2] = 1;
    break;
    
    case 'S':
    case 's':
      keystates[3] = 0;
      if(indikator_nadoknade[2] == 0)
        indikator_nadoknade[3] = 1;
    break;
  }
}

static void on_timer(int value)
{
    /*
     * Proverava se da li callback dolazi od odgovarajuceg tajmera.
     */
    if (value != TIMER_ID)
        return;

    
    
    /* Azuriraju se koordinate centra cepelina. */
    if(indikator_kretanja_cx){

        x_curr += v_x;
        
        //printf("Usao je u kretanje po x\n");
        
        if(granicni_indikator_rotacije_x == 0){
         // printf("Indikator rotacije je 0 pa bi trebao da rotira\n");
            xang += delta_xang;
        }
        indikator_kretanja_cx = 0;
    }
    if(indikator_kretanja_cy){
        y_curr += v_y;
        if(granicni_indikator_rotacije_y == 0){
            yang += delta_yang;
        }
        indikator_kretanja_cy = 0;
    }
    
    
    
    
    if (x_curr <= -(window_width/150 - size / 2)) {
        x_curr = -(window_width/150 - size / 2);
    }
    if (x_curr >= window_width/150 - size / 2) {
        x_curr = window_width/150 - size / 2;
    }

    if (y_curr <= -(window_height/150 - size / 2)) {
        
        y_curr = -(window_height/150 - size / 2);
    }

    if (y_curr >= window_height/150 - size / 2) {
        y_curr = window_height/150 - size / 2;
    }
    
    
    
    
    //PROVERA BLIZINE GRANICE (PRESTANAK ROTACIJE)
    
    if((x_curr <= (window_width/150) * (3.0/4)) && (x_curr >= -(window_width/150) * (3.0/4))) {
      granicni_indikator_rotacije_x = 0;
      /*
      printf("Sama granica je: %f\n", (float)(window_width/150*4.0/5));
      printf("Resetovao je granicni_indikator_rotacije_x\n");
      */
    }
    
    if((y_curr <= (window_height/150) * (3.0/4)) && (y_curr >= -(window_height/150) * (3.0/4))) {
      granicni_indikator_rotacije_y = 0;
    }
    
    if((x_curr >= (window_width/150) * (3.0/4)) || (x_curr <= -(window_width/150) * (3.0/4))) {
      granicni_indikator_rotacije_x = 1;
     // printf("Setovoa je granicni idnikator rotacije x\n");
    }
    
    if((y_curr >= (window_height/150) * (3.0/4)) || (y_curr <= -(window_height/150) * (3.0/4))) {
      granicni_indikator_rotacije_y = 1;
    }

    
    //ZADAVANJE GRANICE ROTACIJE CEPELINA
    
    if(xang <= (-30)) {
      xang = (-30);
    }
    
    if(xang >= (30)) {
      xang = (30);
    }
    
    if(yang >= (30)) {
      yang = (30);
    }
    
    if(yang <= (-30)) {
      yang = (-30);
    }

    
    //VRACANJE U 0
    
    if((keystates[0] == 0) || granicni_indikator_rotacije_x == 1) {
      if(xang != 0 && indikator_nadoknade[1] == 0 && indikator_nadoknade[0] == 1) {
        indikator_promene_smera[0] = 1;
        xang -= (delta_xang);
      }
      else {
        indikator_promene_smera[0] = 0;
        indikator_nadoknade[0] = 0;
      }
    }
    
     if((keystates[1] == 0) || granicni_indikator_rotacije_x == 1) {
      if(xang != 0 && indikator_nadoknade[0] == 0 && indikator_nadoknade[1] == 1) {
        indikator_promene_smera[1] = 1;
        xang -= (delta_xang);
      }
      else {
        indikator_promene_smera[1] = 0;
        indikator_nadoknade[1] = 0;
      }
    }
    
    if((keystates[2] == 0) || granicni_indikator_rotacije_y == 1) {
      if(yang != 0 && indikator_nadoknade[3] == 0 && indikator_nadoknade[2] == 1) {
        indikator_promene_smera[2] = 1;
        yang -= (delta_yang);
      }
      else {
        indikator_nadoknade[2] = 0;
        indikator_promene_smera[2] = 0;
      }
    }
    
    if((keystates[3] == 0) || granicni_indikator_rotacije_y == 1) {
      if(yang != 0 && indikator_nadoknade[2] == 0 && indikator_nadoknade[3] == 1) {
        indikator_promene_smera[3] = 1;
        yang -= (delta_yang);
      }
      else {
        indikator_nadoknade[3] = 0;
        indikator_promene_smera[3] = 0;
      }
    }
    
    /* Forsira se ponovno iscrtavanje prozora. */
    glutPostRedisplay();

    /* Po potrebi se ponovo postavlja tajmer. */
    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
}

static void on_reshape(int width, int height)
{
    window_height = height;
    window_width = width;
    glViewport(0,0,width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)width/height, 1, 1000);
    glutPostRedisplay();
    //glFrustum(0, width, 0, height, 1, 500);
    
}


void DrawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC)
{
    float tStep = (M_PI) / (float)uiSlices;
    float sStep = (M_PI) / (float)uiStacks;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    float t;
    for(t = -M_PI/2; t <= (M_PI/2)+.0001; t += tStep)
    {
        glBegin(GL_TRIANGLE_STRIP);
        float s;
        for(s = -M_PI; s <= M_PI+.0001; s += sStep)
        {
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


static void on_display(void)
{
  
   
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0,0,5,0,0,0,0,1,0);
  
  GLfloat light_position[] = {1,1,1,0};
  GLfloat light_ambient[] = {0.0,0.0,0.0,1};
  GLfloat light_diffuse[] = {0.7,0.7,0.7,1};
  GLfloat light_specular[] = {0.8,0.8,0.8,1};
  
  GLfloat ambient_coefs[] = {0.7,0.2,0.2,1};
  GLfloat diffuse_coefs[] = {1,0.0,0.0,1};
  GLfloat specular_coefs[] = {1,0,0,1};
  GLfloat shininess = 30;
  
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coefs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coefs);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coefs);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
  
  
  
    /* Postavlja se boja svih piksela na zadatu boju pozadine. */

    /* Crta se kvadrat na odgovarajucoj poziciji. */
   /* glBegin(GL_POLYGON);
        glVertex3f(x_curr - size / 2, y_curr - size / 2, 0);
        glVertex3f(x_curr + size / 2, y_curr - size / 2, 0);
        glVertex3f(x_curr + size / 2, y_curr + size / 2, 0);
        glVertex3f(x_curr - size / 2, y_curr + size / 2, 0);
    glEnd();
*/
   /*
      glPushMatrix();
        glTranslatef(x_curr - size / 2, y_curr - size / 2, -2);
        glScalef(0.5,0.6,3);
        glutSolidCube(1);
    glPopMatrix();
    */
   
   
   
   
   
   
   
   
   
    glRotatef(xang, 0,1,0);
    glRotatef(yang, 1,0,0);
    glTranslatef(x_curr - size / 2, y_curr - size / 2, -2);
    DrawEllipsoid(10, 10, 0.5,0.5,1);
    glPushMatrix();
        glTranslatef(0, -0.5 , 0);
        glScalef(0.5,0.2,0.5);
        glutSolidCube(1);
    glPopMatrix();
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
    
   
   
   
    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}
