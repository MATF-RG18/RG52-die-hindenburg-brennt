
    
    

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


//static float metak_param = 0.1;

int vratnice[20][7]; //matrica vratnica pakla

static int brojac_vratnica = 0;

static float gate_animation_parameter = -51;
static float smoke_animation_parameter[8];
static float pemen_bullet_animation_parameter[7];
static float pemen_animation_parameter;

static int indikator_rotacije[7];
static float smoke_position[8];

static float phi, theta, omega;
static float delta_phi, delta_theta, delta_omega;
/*
 * alpha = rotacija pemena oko x_ose
 * beta = rotacija pemena oko y_ose
 */
static float alpha[7], beta[7];
static float alpha_curr[7], beta_curr[7];
static float delta_alpha = 6;
static float delta_beta = 8;

static float x_curr = 0;
static float y_curr = 0;

void iscrtaj_kulu(int i);
void set_normal_and_vertex(float u, float v);
void DrawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC);
void inicijalizacija_matrice_vratnica();
double aim_angle_init(double x1,  double y1, double x2, double y2);

void draw_smoke(int i); 

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
    
    
    inicijalizacija_matrice_vratnica();
    phi = theta = omega = 0;
    delta_phi = delta_theta = delta_omega= M_PI / 90;

    int i;
    for(i = 0; i<7; i++) {
      alpha_curr[i] = 0;
      beta_curr[i] = 0;
      alpha[i] = 0;
      beta[i] = 0;
      indikator_rotacije[i] = 1;
      smoke_animation_parameter[i] = 0;
    }
    
    smoke_animation_parameter[7] = 0;
    
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
    
    pemen_animation_parameter += 0.09;
    
    
    //gate_animation_parameter += 0.15;
    if(gate_animation_parameter >=1){
      gate_animation_parameter = -71;
      brojac_vratnica++;
      int i;
      for(i = 0; i<7; i++) {
        alpha_curr[i] = 0;
        beta_curr[i] = 0;
        indikator_rotacije[i] = 1;
        smoke_animation_parameter[i] = 0;
      }
    }
    if(gate_animation_parameter >= -50 && gate_animation_parameter < -35)
      gate_animation_parameter += 0.15;
    else if(gate_animation_parameter >= -35 && gate_animation_parameter <= -20)
      gate_animation_parameter += 0.1;
    else if(gate_animation_parameter >= -20)
      gate_animation_parameter += 0.05;
    else
      gate_animation_parameter += 0.05;
    
    
    
    if(gate_animation_parameter >-25 && gate_animation_parameter <-24 && indikator_rotacije[3] == 1) {
      indikator_rotacije[3] = 0;
      smoke_position[3] = gate_animation_parameter;
      pemen_bullet_animation_parameter[3] = gate_animation_parameter;
      printf("gate_animation_parameter: %f, pemen_bullet_animation_parameter[3] = %f\n", gate_animation_parameter, pemen_bullet_animation_parameter[3]);
    }
    else if(gate_animation_parameter >-15 && gate_animation_parameter <-14 && indikator_rotacije[1] == 1) {
      indikator_rotacije[1] = 0;
      smoke_position[1] = gate_animation_parameter;
      pemen_bullet_animation_parameter[1] = gate_animation_parameter;
     printf("gate_animation_parameter: %f, pemen_bullet_animation_parameter[1] = %f\n", gate_animation_parameter, pemen_bullet_animation_parameter[1]);
    }
    else if(gate_animation_parameter >-15 && gate_animation_parameter <-14 && indikator_rotacije[5] == 1) {
      indikator_rotacije[5] = 0;
      smoke_position[5] = gate_animation_parameter;
      pemen_bullet_animation_parameter[5] = gate_animation_parameter;
     printf("gate_animation_parameter: %f, pemen_bullet_animation_parameter[5] = %f\n", gate_animation_parameter, pemen_bullet_animation_parameter[5]);
    }
    else if(gate_animation_parameter >-5 && gate_animation_parameter <-4 && indikator_rotacije[0] == 1) {
      indikator_rotacije[0] = 0;
      smoke_position[0] = gate_animation_parameter;
      
      pemen_bullet_animation_parameter[0] = gate_animation_parameter;
      
     printf("gate_animation_parameter: %f, pemen_bullet_animation_parameter[0] = %f\n", gate_animation_parameter, pemen_bullet_animation_parameter[0]);
      
    }
    else if(gate_animation_parameter >-5 && gate_animation_parameter <-4 && indikator_rotacije[6] == 1) {
      indikator_rotacije[6] = 0;
      smoke_position[6] = gate_animation_parameter;
      pemen_bullet_animation_parameter[6] = gate_animation_parameter;
     printf("gate_animation_parameter: %f, pemen_bullet_animation_parameter[6] = %f\n", gate_animation_parameter, pemen_bullet_animation_parameter[6]);
    }
    else if(gate_animation_parameter >-2.5 && gate_animation_parameter <-2 && indikator_rotacije[2] == 1) {
      indikator_rotacije[2] = 0;
      smoke_position[2] = gate_animation_parameter;
      pemen_bullet_animation_parameter[2] = gate_animation_parameter;
      
     printf("gate_animation_parameter: %f, pemen_bullet_animation_parameter[2] = %f\n", gate_animation_parameter, pemen_bullet_animation_parameter[2]);
      
      
    }
    else if(gate_animation_parameter >-2.5 && gate_animation_parameter <-2 && indikator_rotacije[4] == 1) {
      indikator_rotacije[4] = 0;
      smoke_position[4] = gate_animation_parameter;
      pemen_bullet_animation_parameter[4] = gate_animation_parameter;
     printf("gate_animation_parameter: %f, pemen_bullet_animation_parameter[4] = %f\n", gate_animation_parameter, pemen_bullet_animation_parameter[4]);
    }
    
    
    
    int i;
    for(i = 0; i < 7; i++){
      pemen_bullet_animation_parameter[i] += 0.6;
      if(alpha_curr[i] < alpha[i] && indikator_rotacije[i] == 1) {
        alpha_curr[i] += delta_alpha;
        
      }
      else if(alpha_curr[i] > alpha[i] && indikator_rotacije[i] == 1) {
        alpha_curr[i] -= delta_alpha;
        
      }
      
      if(beta_curr[i] < beta[i] && indikator_rotacije[i] == 1){
        beta_curr[i] += delta_beta;
        
      }
      else if(beta_curr[i] > beta[i] && indikator_rotacije[i] == 1) {
        beta_curr[i] -= delta_beta;
        
      }
      
      if(indikator_rotacije[i] == 0) {
        smoke_animation_parameter[i] += 0.15;
      }
    }
    
    
    
    
    
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
    
   
    int i;
    
    for(i = -3; i<4; i++) {
     
      if(vratnice[brojac_vratnica][i+3] == 1){
        
        glPushMatrix();
            glTranslatef(i,-1,gate_animation_parameter);
            iscrtaj_kulu(i+3);
            //iscrtaj_loptu_test(i+3);
        glPopMatrix();
    
        alpha[i+3] = aim_angle_init(0,y_curr, gate_animation_parameter, 1);
        beta[i+3] = aim_angle_init( -gate_animation_parameter, i , 0,x_curr); 
        
      }
    }
    
    /*glPopMatrix();
    
        glTranslatef(-3, 1, gate_animation_parameter);
        glutSolidCube(2);
    
    glPopMatrix();
    */
    
    glutSwapBuffers();
   
}

void iscrtaj_kulu(int i) 
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
    
    
   
    /* Osnova Pemena: dve odsecene sfere koje se pokrecu*/
    glPushMatrix();
        
        glTranslatef(0,2,0);
        glScalef(0.2,0.2,0.2);
        glScalef(0.5,0.5,0.5);
        
        
        if(indikator_rotacije[i] == 1){
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
          
        }
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
    
        
        glDisable(GL_CLIP_PLANE0);
        
       
        glEnable(GL_CLIP_PLANE1);
        
        glutSolidSphere(1.5, 20, 20);
        glDisable(GL_CLIP_PLANE1);
    glPopMatrix();
    
    
    /* Osnova Pemena: jedna fiksna odsecena sfera kako pemen ne bi otvorio 
     * skroz usta
     */
    glPushMatrix();
        
        
        glTranslatef(0,2,0);
        glScalef(0.2,0.2,0.2);
        glScalef(0.5,0.5,0.5);
        
        if(indikator_rotacije[i] == 1){
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
            
        }
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

    /* Oci, brkovi i obrve Pemena */
    // Levo oko  
    glPushMatrix();
    
        
        glTranslatef(0,2,0);
        glScalef(0.2,0.2,0.2);
        glScalef(0.5,0.5,0.5);
        
        if(indikator_rotacije[i] == 1){
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
           
        }
        else {
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
          glTranslatef(0,0,pemen_bullet_animation_parameter[i]);
          
        }
        
        
        glTranslatef(-1.25, 1.25, 0);
        glutSolidSphere(0.3, 20, 20);
    
    glPopMatrix();

    // Desno oko
    glPushMatrix();

        
        glTranslatef(0,2,0);
        glScalef(0.2,0.2,0.2);
        glScalef(0.5,0.5,0.5);
        
        if(indikator_rotacije[i] == 1){
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
 
        }
        else {
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
          glTranslatef(0,0,pemen_bullet_animation_parameter[i]);
          
        }
        glTranslatef(1.25, 1.25, 0);
        glutSolidSphere(0.3, 20, 20);
    
    glPopMatrix();

    // Desna obrva
    glPushMatrix();
    
        
        glTranslatef(0,2,0);
        glScalef(0.2,0.2,0.2);
        glScalef(0.5,0.5,0.5);
    
        glTranslatef(1.2, 2, 0.8);
        
        if(indikator_rotacije[i] == 1){ 
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
            
        }
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
        
        if(indikator_rotacije[i] == 1){
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
            
        }
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
        
        if(indikator_rotacije[i] == 1){
            glRotatef(-alpha_curr[i] , 1, 0, 0);
            
        }
        else {
          glRotatef(-alpha_curr[i] + 10, 1, 0, 0);
          glTranslatef(0,0,pemen_bullet_animation_parameter[i]);
          
        }
        glTranslatef(0,0.4,1.6);
        glScalef(0.3,0.3,0.1);
        glutSolidCube(1);
    
    glPopMatrix();
    
}


void draw_smoke(int i) 
{
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
        
        glPushMatrix();
            
            glColor3f(1,1,1);
            
            if(i != 7) {
                glTranslatef(i, 1, smoke_position[i]);
            
            }
            printf("%f\n", smoke_animation_parameter[i]);
            
            if(smoke_animation_parameter[i] < 8){
                glScalef(smoke_animation_parameter[i], smoke_animation_parameter[i], smoke_animation_parameter[i]);
                glutWireCube(1);
                
                glTranslatef(-0.4, 0.3, 0);
                glScalef(2, 0, 1.5);
                glutWireCube(1);
                
                glTranslatef(0.3, 0.4, 0);
                glScalef(1, 3, 2.2);
                glutWireCube(1);
                
                glTranslatef(-0.2, 0, 0);
                glScalef(2.2, 3, 0);
                glutWireCube(1);
                

                glTranslatef(-1.4, 0.5, 0.5);
                glScalef(0.3, 0.3, 0.3);
                glutWireCube(1);
                
                glTranslatef(0.45, -0.3, 0);
                glScalef(0.5, 0.5, 1.5);
                glutWireCube(1);
                
                glTranslatef(-0.4, 0, 0.2);
                glScalef(0.4, 0.2, 0.5);
                glutWireCube(1);
            }
            
        
        glPopMatrix();
        
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}


void inicijalizacija_matrice_vratnica()
{
  
  printf("Ulaz u vratnice\n");
  
    int i;
    //Ovo su prve vratnice pakla sa dva stuba na krajevima
    vratnice[0][0] = 1;
    vratnice[0][6] = 1;
   
    
    
    srand(time(NULL));
    
    int r1, r2, r3, r4;
    
    //Ovde se inicijalizuju ostale vratnice
    for (i = 1; i < 20; i++) {
      
      r1 = r2 = r3 = r4 = 0;

      //Prvih 5 vratnica imaju samo po jedan stub
      if(i < 5) {
        printf("Prvih pet vratnica\n");
        r1 = rand() % 7;
        vratnice[i][r1] = 1;
      }
      // Drugih 5 imaju 2 stuba
      else if(i < 10 && i >=5) {
        
        printf("Drugih pet vratnica\n");

        r1 = rand() % 7;
        r2 = rand() % 7;
        if(r1 == r2) {
          r2 = (r2 + 1) % 7;
        }
        vratnice[i][r1] = 1;
        vratnice[i][r2] = 1;
      }
      // Trecih 5 imaju 3 stuba
      else if(i < 15 && i >= 10) {
        
        printf("Trecih pet vratnica\n");

        r1 = rand() % 7;
        r2 = rand() % 7;
        r3 = rand() % 7;
        while (r1 == r2) {
          r2 = rand() % 7;
        }
        while (r3 == r1 || r3 == r2) {
          r3 = rand() % 7;
        }
        vratnice[i][r1] = 1;
        vratnice[i][r2] = 1;
        vratnice[i][r3] = 1;
      
      }
      // Poslednjih 5 imaju 4 stuba
      else if(i < 20 && i >= 15) {
        
        printf("Poslednjih pet ratnica\n");

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
        
        vratnice[i][r1] = 1;
        vratnice[i][r2] = 1;
        vratnice[i][r3] = 1;
        vratnice[i][r4] = 1;
      
      }  
    }
}
 
double aim_angle_init(double x1,  double y1, double x2, double y2)
{
  
  double k = (y2-y1) / (x2 - x1);
  double ugao = atan(k)*180/M_PI;
  if(ugao >= 90)
    return ugao - 90;
  else
    return ugao;
}
