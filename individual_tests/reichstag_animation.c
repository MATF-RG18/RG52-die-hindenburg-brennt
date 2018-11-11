
    
    

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <time.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 10

static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int);
void reichstag();

//static float metak_param = 0.1;

int vratnice[20][7]; //matrica vratnica pakla

static int brojac_vratnica = 0;

static float gate_animation_parameter = -51;
static float animation_parameter = 0;

static float phi, theta, omega;
static float delta_phi, delta_theta, delta_omega;

static float alpha, beta;

static float x_curr = 0;
static float y_curr = 0;

void iscrtaj_kulu(void);
void set_normal_and_vertex(float u, float v);
void DrawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC);


int main(int argc, char** argv){
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    glutInitWindowSize(800, 500);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor(0,0,0,0);
    
    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    
    
    
    phi = theta = omega = 0;
    delta_phi = delta_theta = delta_omega= M_PI / 90;

    
    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    
    glutMainLoop();
    
    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
  double pi = M_PI;
  
    switch(key){
        case 27:
            exit(0);
            break;
            
       case 'p':
        /* Dekrementira se ugao phi i ponovo iscrtava scena. */
        phi -= delta_phi;
        if (phi > 2 * pi) {
            phi -= 2 * pi;
        } else if (phi < 0) {
            phi += 2 * pi;
        }
        glutPostRedisplay();
        break;

    case 'P':
        /* Inkrementira se ugao phi i ponovo iscrtava scena. */
        phi += delta_phi;
        if (phi > 2 * pi) {
            phi -= 2 * pi;
        } else if (phi < 0) {
            phi += 2 * pi;
        }
        glutPostRedisplay();
        break;

    case 'r':
    case 'R':
        /* Resetuju se uglovi phi i theta na pocetne vrednosti. */
        phi = theta = 0;
        glutPostRedisplay();
        break;

    case 't':
        /*
         * Dekrementira se ugao theta i ponovo iscrtava scena. Ovaj
         * ugao se odrzava u intervalu [-89,89] stepeni.
         */
        theta -= delta_theta;
        if (theta < -(pi / 2 - pi / 180)) {
            theta = -(pi / 2 - pi / 180);
        }
        glutPostRedisplay();
        break;

    case 'T':
        /*
         * Inkrementira se ugao theta i ponovo iscrtava scena. Ovaj
         * ugao se odrzava u intervalu [-89,89] stepeni.
         */
        theta += delta_theta;
        if (theta > pi / 2 - pi / 180) {
            theta = pi / 2 - pi / 180;
        }
        glutPostRedisplay();
        break;
        
     case 'o':
        /*
         * Dekrementira se ugao theta i ponovo iscrtava scena. Ovaj
         * ugao se odrzava u intervalu [-89,89] stepeni.
         */
        omega -= delta_omega;
        if (omega < -(pi / 2 - pi / 180)) {
            omega = -(pi / 2 - pi / 180);
        }
        glutPostRedisplay();
        break;

    case 'O':
        /*
         * Inkrementira se ugao theta i ponovo iscrtava scena. Ovaj
         * ugao se odrzava u intervalu [-89,89] stepeni.
         */
        omega += delta_theta;
        if (omega > pi / 2 - pi / 180) {
            omega = pi / 2 - pi / 180;
        }
        glutPostRedisplay();
        break;
            
    }
}

static void on_reshape(int width, int height)
{
    glViewport(0,0,width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)width/height, 1, 70);

    
}

static void on_timer(int value)
{
    if(value != TIMER_ID) return;
    
    animation_parameter += 0.09;
    
    
    gate_animation_parameter += 0.15;
    if(gate_animation_parameter >=1){
      gate_animation_parameter = -71;
      brojac_vratnica++;
    }
    if(gate_animation_parameter >= -50 && gate_animation_parameter < -35)
      gate_animation_parameter += 0.009;
    else if(gate_animation_parameter >= -35 && gate_animation_parameter <= -20)
      gate_animation_parameter += 0.005;
    else if(gate_animation_parameter >= -20)
      gate_animation_parameter += 0.0035;
    else
      gate_animation_parameter += 0.15;
    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    
    glutPostRedisplay();
}





static void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,0,0,0,0,1,0);
    /*gluLookAt(5 * cos(theta) * cos(phi),
              5 * cos(theta) * sin(phi),
              5 * sin(theta) ,
              0, 0, 0, 0, 0, 1);
   */
    /*GLfloat light_position[] = {5,10,5,0};
    GLfloat light_ambient[] = {0.2,0.2,0.2,1};
    GLfloat light_diffuse[] = {0.5,0.5,0.5,1};
    GLfloat light_specular[] = {0.8,0.8,0.8,1};
    
    GLfloat ambient_coefs[] = {0.2,0.2,0.2,1};
    GLfloat diffuse_coefs[] = {0.5,0.0,0.0,1};
    GLfloat specular_coefs[] = {0.8,0.8,0.8,1};
    GLfloat shininess = 100;
    */
    
     GLfloat light_position[] = { 1, 1, 1, 0 };

    /* Ambijentalna boja svetla. */
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1 };

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 1.0, 0.1, 0.1, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.0, 0.0, 0.8, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 20;
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    
    
    /*
    glPushMatrix();
        glTranslatef(0,0,0);
        iscrtaj_kulu();
    glPopMatrix();
   */
    glTranslatef(0,-1,gate_animation_parameter);
    reichstag();
    
   
    
    
    glutSwapBuffers();
   
}

void reichstag()
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
  
  glPushMatrix();
    glTranslatef(0,-0.76, 1.25);
    glScalef(1.5, 1.2, 0.25);
    glutSolidCube(1);
  glPopMatrix();
  
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
  
  
  
  
  
  
  
  
  glPushMatrix();
  glTranslatef(0,1,0);
  glScalef(8,8,8);
    glPushMatrix();
          
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
          
          glRotatef(45, 1, 0,0);
          double clip_plane_1[] = {0, -1, sin(animation_parameter), 0};
          double clip_plane_2[] = {0, sin(animation_parameter),-1,0};;
          
          glClipPlane(GL_CLIP_PLANE0, clip_plane_1);
          glClipPlane(GL_CLIP_PLANE1, clip_plane_2);
          
          
        
          glEnable(GL_CLIP_PLANE0);
        
          
          glutSolidSphere(1.5, 20, 20);
      
          
          glDisable(GL_CLIP_PLANE0);
          
        
          glEnable(GL_CLIP_PLANE1);
          
          glutSolidSphere(1.5, 20, 20);
          glDisable(GL_CLIP_PLANE1);
      glPopMatrix();
      
      
      /* Osnova Pemena: jedna fiksna odsecena sfera kako pemen ne bi otvorio 
      * skroz usta
      */
      glPushMatrix();
          
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
          
          glRotatef(45, 1, 0,0);
          double clip_plane_3[] = {0, 0,-1,0};
          glClipPlane(GL_CLIP_PLANE3, clip_plane_3);
          glEnable(GL_CLIP_PLANE3);
          glutSolidSphere(1.5, 20, 20);
          glDisable(GL_CLIP_PLANE3);
          
          
      glPopMatrix();

      /* Oci, brkovi i obrve Pemena */
      // Levo oko  
      glPushMatrix();
      
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
          
          glTranslatef(-1.25, 1.25, 0);
          glutSolidSphere(0.3, 20, 20);
      
      glPopMatrix();

      // Desno oko
      glPushMatrix();

      
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
          
          glTranslatef(1.25, 1.25, 0);
          glutSolidSphere(0.3, 20, 20);
      
      glPopMatrix();

      // Desna obrva
      glPushMatrix();
      
      
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
      
          glTranslatef(1.2, 2, 0.8);
          glRotatef(30, 0,0,1);
          glScalef(1.2, 0.3,0.3);
          glutSolidCube(1);
      
      glPopMatrix();
      
      // Leva obrva
      glPushMatrix();
      
          
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
      
          glTranslatef(-1.2, 2, 0.8);
          glRotatef(-30, 0,0,1);
          glScalef(1.2, 0.3,0.3);
          glutSolidCube(1);
      
      glPopMatrix();

      // Brkovi
      glPushMatrix();
        
          
          
          glScalef(0.2,0.2,0.2);
          glScalef(0.5,0.5,0.5);
          
          glTranslatef(0,0.4,1.6);
          glScalef(0.3,0.3,0.1);
          glutSolidCube(1);
      
      glPopMatrix();
    glPopMatrix();
  
  
}

