#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <time.h>
#include <string.h>

#include "world.h"
#include "gates.h"
#include "cepelin.h"

#define TIMER_ID 0
#define TIMER_INTERVAL 5

// Niz koji cuva stanja tastera za kretanje
static int keystates[4];
// Indikator koji odredjuje da li je animacija pauzirana
static int animation_ongoing;
// Indikator koji govori da li se nastavlja igra nakon smrti cepelina
static int continue_playing_param = 0;

/*
 * Funkcija za inicijalizaciju pocetnih vrednosti
 */
void global_param_init();


static void on_keyboardUp(unsigned char key, int x, int y);
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int);


int main(int argc, char** argv){
   
  
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    glutInitWindowSize(800, 500);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor(0.18,0.31,0.31,0);
    
    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutKeyboardUpFunc(on_keyboardUp);
    glutIgnoreKeyRepeat(0);
    
    gate_matrix_init();
    
    global_param_init();
    
    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    
    glutMainLoop();
    
    return 0;
}

void global_param_init()
{
  
    tile_animation_parameter = 0;  
    world_blocks_animation_parameter = 0;
  
    smoke_expansion_parameter = 0;
    smoke_movement_parameter = 0;
  
    explosion_animation_parameter = 0;
    gate_counter = 0;
  
    cepelin_life_parameter = 0;
    cepelin_on_fire = 0;
  
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
    
    angle_compensation_indicator[0] = 0;
    angle_compensation_indicator[1] = 0;
    angle_compensation_indicator[2] = 0;
    angle_compensation_indicator[3] = 0;

    direction_change_indicator[0] = 0;
    direction_change_indicator[1] = 0;
    direction_change_indicator[2] = 0;
    direction_change_indicator[3] = 0;
   
    
    
    
    int i;
    for(i = 0; i<7; i++) {
      alpha_curr[i] = 0;
      beta_curr[i] = 0;
      alpha[i] = 0;
      beta[i] = 0;
      rotation_indicator[i] = 1;
      smoke_animation_parameter[i] = 0;
    }
    
    smoke_animation_parameter[7] = 0;
    
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
        if (!animation_ongoing && cepelin_alive) {
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            animation_ongoing = 1;
        }
        
        break;
    /*
     * Ukoliko se stisne jedan od tastera WASDwasd,provera da li je doslo 
     * do promene smera, azuriranje ugla rotacije cepelina (oko svoje ose), 
     * hvata se stanje stisnutog tastera, postavlja se indikator kretanja i to 
     * sve pod uslovom da nije zapoceto vracanje cepelina u pocetno stanje i 
     * da animacija nije pauzirana
     */
    case 'A':
    case 'a':
      if(angle_compensation_indicator[1] == 0){
        if(animation_ongoing && v_x > 0 && direction_change_indicator[1] == 0) {
          v_x = v_x * (-1);
          if(delta_xang < 0) {
            delta_xang = delta_xang * (-1);
          }
        }
        keystates[0] = 1;
        movement_indicator_cx = 1;
        glutPostRedisplay();
      }
    break;
    
    case 'D':
    case 'd':
      if(angle_compensation_indicator[0] == 0) {
        if(animation_ongoing && v_x < 0 && direction_change_indicator[0] == 0) {
          v_x = v_x * (-1);
          
          if(delta_xang > 0) {
            delta_xang = delta_xang * (-1);
          }
        }
        movement_indicator_cx = 1;
        keystates[1] = 1;
        glutPostRedisplay();
      }
    break;

    case 'W':
    case 'w':
      if(angle_compensation_indicator[3] == 0) {
        if(animation_ongoing && v_y < 0 && direction_change_indicator[3] == 0) {
          v_y = v_y * (-1);
          
          if(delta_yang < 0) {
            delta_yang = delta_yang * (-1);
          }
        }
        keystates[2] = 1;
        movement_indicator_cy = 1;
        glutPostRedisplay();
      }
    break;
    
    case 'S':
    case 's':
      if(angle_compensation_indicator[2] == 0) {
        if(animation_ongoing && v_y > 0 && direction_change_indicator[2] == 0) {
          v_y = v_y * (-1);
          
          if(delta_yang > 0) {
            delta_yang = delta_yang * (-1);
          }
        }
        keystates[3] = 1;
        movement_indicator_cy = 1;
        glutPostRedisplay();
        }
    break;
    
    case 'h':
    case 'H':
        /* Zaustavlja se animacija. */
        if(cepelin_alive == 1){
            animation_ongoing = 0;
        }
        break;
    
      /*
       * Ovaj taster moze da se stisne samo kada je cepelin eksplodirao
       * i on zapocinje igru ispocetka
       */
    case 'C':
    case 'c':
      if(!animation_ongoing && !cepelin_alive){
        
        continue_playing_param = 1;
        animation_ongoing = 1;
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);

      }
    break;
    }
    
    
}

/* 
 * Funkcija koja hvata dogadjaje koji su se desili kada se odpusti neko dugme
 */
static void on_keyboardUp(unsigned char key, int x, int y)
{
  /*
   * Ako je odpusten neki od sledecih tastera, postavlja se indikator za 
   * vracanje cepelina u prvobitno stanje
   */
  switch(key) {
    case 'a':
    case 'A':
      keystates[0] = 0;
      if(angle_compensation_indicator[1] == 0)
        angle_compensation_indicator[0] = 1;
    break;
    
    case 'D':
    case 'd':
      keystates[1] = 0;
      if(angle_compensation_indicator[0] == 0)
        angle_compensation_indicator[1] = 1;
    break;
    
    case 'W':
    case 'w':
      keystates[2] = 0;
      if(angle_compensation_indicator[3] == 0)
        angle_compensation_indicator[2] = 1;
    break;
    
    case 'S':
    case 's':
      keystates[3] = 0;
      if(angle_compensation_indicator[2] == 0)
        angle_compensation_indicator[3] = 1;
    break;
  }
}


static void on_reshape(int width, int height)
{
  
    window_height = height;
    window_width = width;
    
    glViewport(0,0,width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)width/height, 1, 70);

    glutPostRedisplay();
    
}

static void on_timer(int value)
{
    if(value != TIMER_ID) return;
    
    
    /*
     * Animacija cepelina
     */
    
    /*
     * Ako je postavljen indikator za nastavak igre i ako igra nije pauzirana
     * reinicijalizuju se date promenljive, kako bi se zapocela igra izpocetka
     */
    if(continue_playing_param == 1 && animation_ongoing == 1) {
      
      global_param_init();
      cepelin_alive = 1;
      
      continue_playing_param = 0;
      gate_counter = 0;
      gate_animation_parameter = -71;
        
    }
    
    
    /*
     * Ako je cepelin ziv i ako je postavljen indikator za kredanje po x osi
     * onda se azurira pozicija cepelina
     */
    if(movement_indicator_cx && cepelin_alive == 1){

        x_curr += v_x;
        
       
       // Ako je granicni indikator rotacije postavljen na 0 onda se azurira 
       // ugao rotacije oko y ose
        if(border_rotation_indicator_x == 0){
       
            xang += delta_xang;
        }
        movement_indicator_cx = 0;
    }
    
    /*
     * Ako je cepelin ziv i ako je postavljen indikator za kredanje po y osi
     * onda se azurira pozicija cepelina
     */
    if(movement_indicator_cy && cepelin_alive == 1){
        y_curr += v_y;
        
       // Ako je granicni indikator rotacije postavljen na 0 onda se azurira 
       // ugao rotacije oko x ose
        if(border_rotation_indicator_y == 0){
            yang += delta_yang;
        }
        movement_indicator_cy = 0;
    }
      
      
      
    /*
     * Postavljanje granica kretanja cepelina po ekranu
     */  
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
      
      
      
    /*  
     * PROVERA BLIZINE GRANICE (PRESTANAK ROTACIJE)
     */  
    if((x_curr <= (window_width/150) * (3.0/4)) && (x_curr >= -(window_width/150) * (3.0/4))) {
      border_rotation_indicator_x = 0;
        
    }
      
    if((y_curr <= (window_height/150) * (3.0/4)) && (y_curr >= -(window_height/150) * (3.0/4))) {
      border_rotation_indicator_y = 0;
    }
      
    if((x_curr >= (window_width/150) * (3.0/4)) || (x_curr <= -(window_width/150) * (3.0/4))) {
      border_rotation_indicator_x = 1;
   
    }
      
    if((y_curr >= (window_height/150) * (3.0/4)) || (y_curr <= -(window_height/150) * (3.0/4))) {
      border_rotation_indicator_y = 1;
    }

    /* 
     * ZADAVANJE GRANICE ROTACIJE CEPELINA
     */
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

    /*   
     * VRACANJE U CEPELINA U POCETNO STANJE (ne odnosi se na translacije
     * nego samo na ugao rotacije cepelina oko svoje ose)
     */  
    
    /*
     * Ako nije stisnut taster A ili ako nije vec poceo vracanje cepelina u 
     * pocetno stanje, onda proverava da li je ugao rotacije oko y ose nije jednak
     * nuli, da li je indikator nadoknade ugla za pozitivan smer na x osi
     * iskljucen i da li je indikator nadoknade ugla za negativan smer na x osi
     * ukljucen. Ako to vazi, zapoceinje vracanje cepelina u pocetni polozaj.
     * Inace se indikator za promenu smera za taj smer postavlja na nulu
     * i indikator za kompenzaciju ugla se postavlja na nulu.
     * 
     * Analogno vazi i za ostala stanja tastera.
     */
    if((keystates[0] == 0) || border_rotation_indicator_x == 1) {
      if(xang != 0 && angle_compensation_indicator[1] == 0 && angle_compensation_indicator[0] == 1) {
        direction_change_indicator[0] = 1;
        xang -= (delta_xang);
      }
      else {
        direction_change_indicator[0] = 0;
        angle_compensation_indicator[0] = 0;
      }
    }
      
    if((keystates[1] == 0) || border_rotation_indicator_x == 1) {
      if(xang != 0 && angle_compensation_indicator[0] == 0 && angle_compensation_indicator[1] == 1) {
        direction_change_indicator[1] = 1;
        xang -= (delta_xang);
      }
      else {
        direction_change_indicator[1] = 0;
        angle_compensation_indicator[1] = 0;
      }
    }
      
    if((keystates[2] == 0) || border_rotation_indicator_y == 1) {
      if(yang != 0 && angle_compensation_indicator[3] == 0 && angle_compensation_indicator[2] == 1) {
        direction_change_indicator[2] = 1;
        yang -= (delta_yang);
      }
      else {
        angle_compensation_indicator[2] = 0;
        direction_change_indicator[2] = 0;
      }
    }
      
    if((keystates[3] == 0) || border_rotation_indicator_y == 1) {
      if(yang != 0 && angle_compensation_indicator[2] == 0 && angle_compensation_indicator[3] == 1) {
        direction_change_indicator[3] = 1;
        yang -= (delta_yang);
      }
      else {
        angle_compensation_indicator[3] = 0;
        direction_change_indicator[3] = 0;
      }
    }
      
      
      
      
    /*
     *Kolizija cepelina sa kulom
     */
    int i;
    for(i = -3; i<4; i++) {
      // Kolizija sa gornjim delom kule
      if(gate_animation_parameter >= -0.5 && gate_animation_parameter <= 0.5) {
        if(y_curr >= -1.05 && y_curr <= 2.55) {
          if(x_curr >= (i*2-0.8) && x_curr <= (i*2+0.8)){
            if(gate_matrix[gate_counter][i+3] == 1){
              // Ako dodje do kolizije, cepelin umire 
              cepelin_alive = 0;
              // Ako dodje do kolizije, postavlja se vrednost u matrici kula
              // na -1, sto ukazuje kasnije da se crta srusena kula
              gate_matrix[gate_counter][i+3] = -1;
            }
          }
        }
      }
      
      // Kolizija sa donjim delom kule
      if(gate_animation_parameter >= -0.5 && gate_animation_parameter <= 0.5) {
        if(y_curr >=  -2.75 && y_curr <-1.05) {
          if(x_curr >= (i*2-0.8) && x_curr <= (i*2+0.8)){
            if(gate_matrix[gate_counter][i+3] == 1 || gate_matrix[gate_counter][i+3] == -1){
              // Ako dodje do kolizije, cepelin umire 
              cepelin_alive = 0;
              
               
            }
          }
        }
      }
        
      
      // Kolizija sa Reichstag-om
      if(gate_animation_parameter >= -1.3 && gate_animation_parameter <= 1.3) {
        if(gate_counter == 20) {
          if(y_curr < -1.5) {
            if(x_curr >= -5 && x_curr <= 5) {
   
              cepelin_alive = 0;
             
            }
          }
        }
      }
      
      // Kolizija sa projektilom sa kule
      if(rotation_indicator[i+3] == 0 && gate_matrix[gate_counter][i+3] == 1) {
        if(pemen_z[i+3] >= cepelin_z-0.5 && pemen_z[i+3] <= cepelin_z+0.5) {
          if(pemen_y[i+3] >=  cepelin_y - 0.5 && pemen_y[i+3] <= cepelin_y + 0.5) {
            if(pemen_x[i+3] >= cepelin_x-0.5 && pemen_x[i+3] <= cepelin_x+0.5){
              if(gate_matrix[gate_counter][i+3] == 1 || gate_matrix[gate_counter][i+3] == -1){
                // Ako dodje do kolizije cepelin se zapali 
                cepelin_on_fire = 1; 
                
              }
            }
          }
        }
      }  
    }
      
   //Ako je doslo do kraja igre a igrac nije unisito Reichstag   
   if(cepelin_alive == 1 && gate_counter == 20) {
     if(gate_animation_parameter > 6) {
       cepelin_alive = 0;
    }
  }
   // Kolizija sa BOSS PEMEN-om   
   if(fire_boss == 1 && gate_counter == 20) {
     if(boss_z >= -3.5 && boss_z <= 2) {
       if(boss_y >= cepelin_y - 2 && boss_y <= cepelin_y + 2){
         if(boss_x >= cepelin_x - 2 && boss_x <= cepelin_x + 2) {
           // Ako dodje do kolizije cepelin odmah eksplodira
           cepelin_alive = 0;
           fire_boss = 0;
         }
       } 
     }
   }
   
   // Ako eksploszija dodje do odredjene vrednosti, zaustavlja se animacija
   if(explosion_animation_parameter >= 3) {
     animation_ongoing = 0;
   }
   
   // Ako je parametar za kretanje dima prebacio 3, resetuj vrednosti 
   if(smoke_movement_parameter >= 3) {
     smoke_movement_parameter = 0;
     smoke_expansion_parameter = 0;
   }
   
   
   
   // Ako je cepelin ziv i ako animacija i dalje traje, uveceava se parametar 
   // eksplozije
   if(cepelin_alive == 0 && animation_ongoing == 1) {
     explosion_animation_parameter += 0.05;
     
   }
   
   // Ako parametar koji broji koliko je cepelinu ostalo da zivi pre nego sto 
   // izgori predje vrednost od 300, cepelin umire
   if(cepelin_life_parameter >= 300) {
     cepelin_alive = 0;
   }
   
   // Ako cepelin gori, uvecaj parametar koji broji koliko je cepelinu ostalo 
   // da zivi pre nego sto izgori, parametar koji odredjuje smer kretanja dima
   // koji potice iz cepelina, kao i parametar za skaliranje velicine oblaka dima
   if(cepelin_on_fire && cepelin_alive) {
     
     cepelin_life_parameter += 0.15;
     smoke_movement_parameter += 0.09;
     smoke_expansion_parameter += 0.05;
     
     
   }  
      
      
      
      
    /*
     * Azuriranje vrednosti parametra animacije vratnica
     */
    
    // Ako je cepelin ziv, azuriraj animacioni paramtar pemena
    if(cepelin_alive){  
        pemen_animation_parameter += 0.09;
    }
   
    // Azuriraj parametre vratnica (nivoa) ako ona prodje iza cepelina
    // Prelazak na sledeci nivo
    if(gate_animation_parameter >=1 && gate_counter != 20){
      gate_animation_parameter = -71;
      gate_counter++;
      int i;
      for(i = 0; i<7; i++) {
        alpha_curr[i] = 0;
        beta_curr[i] = 0;
        rotation_indicator[i] = 1;
        smoke_animation_parameter[i] = 0;
      }
    }
    
    /*
     * Kontrola brzine priblizavanja kula u zavisnosti od udaljenosti od cepelina
     */
    if(cepelin_alive){
      if(gate_animation_parameter >= -50 && gate_animation_parameter < -35)
        gate_animation_parameter += 0.15;
      else if(gate_animation_parameter >= -35 && gate_animation_parameter <= -20)
        gate_animation_parameter += 0.1;
      else if(gate_animation_parameter >= -20)
        gate_animation_parameter += 0.05;
      else
        gate_animation_parameter += 0.05;
        
    }  
    
    /*
     * Ukoliko je kapija dosla do odjedjene razdaljine, ispaljuj projektil
     * (analogno za sve ostale kule)
     */
    if(gate_animation_parameter >-25 && gate_animation_parameter <-24 && rotation_indicator[3] == 1) {
      rotation_indicator[3] = 0;
      smoke_position[3] = gate_animation_parameter;
      pemen_bullet_animation_parameter[3] = gate_animation_parameter;
   
    }
    else if(gate_animation_parameter >-15 && gate_animation_parameter <-14 && rotation_indicator[1] == 1) {
      rotation_indicator[1] = 0;
      smoke_position[1] = gate_animation_parameter;
      pemen_bullet_animation_parameter[1] = gate_animation_parameter;
   
    }
    else if(gate_animation_parameter >-15 && gate_animation_parameter <-14 && rotation_indicator[5] == 1) {
      rotation_indicator[5] = 0;
      smoke_position[5] = gate_animation_parameter;
      pemen_bullet_animation_parameter[5] = gate_animation_parameter;
   
    }
    else if(gate_animation_parameter >-5 && gate_animation_parameter <-4 && rotation_indicator[0] == 1) {
      rotation_indicator[0] = 0;
      smoke_position[0] = gate_animation_parameter;
      pemen_bullet_animation_parameter[0] = gate_animation_parameter;
    }
    else if(gate_animation_parameter >-5 && gate_animation_parameter <-4 && rotation_indicator[6] == 1) {
      rotation_indicator[6] = 0;
      smoke_position[6] = gate_animation_parameter;
      pemen_bullet_animation_parameter[6] = gate_animation_parameter;
    }
    else if(gate_animation_parameter >-2.5 && gate_animation_parameter <-2 && rotation_indicator[2] == 1) {
      rotation_indicator[2] = 0;
      smoke_position[2] = gate_animation_parameter;
      pemen_bullet_animation_parameter[2] = gate_animation_parameter;     
    }
    else if(gate_animation_parameter >-2.5 && gate_animation_parameter <-2 && rotation_indicator[4] == 1) {
      rotation_indicator[4] = 0;
      smoke_position[4] = gate_animation_parameter;
      pemen_bullet_animation_parameter[4] = gate_animation_parameter;
    }
      
    // Ovde se vrsi ispaljivanje PEMEN BOSS-a na odredjenoj udaljenosti od cepelina  
    if(fire_boss == 0 && gate_animation_parameter >-30 && gate_animation_parameter <= -29.5 && gate_counter == 20) {
      fire_boss = 1;
      smoke_position[7] = gate_animation_parameter;
      pemen_bullet_animation_parameter[7] = gate_animation_parameter;
    }  
    
    /*
     * Ako je cepelin ziv, za svaki projektil se azurira vrednost njegove animacije
     * ispaljivanja. Ako je ukljucen indikator rotacije za datu kulu i ako 
     * kula nije zarotirana do date vrednosti, azuriraj trenutnu vrednost rotacije.
     * Ako je indikator rotacije iskljucen, azuriraj vrednost dima.
     * Ako je smo dosli do BOSS PEMEN-a, uvecaj njegov parametar isapljivanja 
     */
    if(cepelin_alive){  
      for(i = 0; i < 7; i++){
        pemen_bullet_animation_parameter[i] += 0.6;
        if(alpha_curr[i] < alpha[i] && rotation_indicator[i] == 1) {
          alpha_curr[i] += delta_alpha;
          
        }
        else if(alpha_curr[i] > alpha[i] && rotation_indicator[i] == 1) {
          alpha_curr[i] -= delta_alpha;
          
        }
          
        if(beta_curr[i] < beta[i] && rotation_indicator[i] == 1){
          beta_curr[i] += delta_beta;
            
        }
        else if(beta_curr[i] > beta[i] && rotation_indicator[i] == 1) {
          beta_curr[i] -= delta_beta;
            
        }
        
        if(rotation_indicator[i] == 0) {
          smoke_animation_parameter[i] += 0.15;
        }
      }
      if(gate_counter == 20 && fire_boss == 1){
        pemen_bullet_animation_parameter[7] += 0.6;  
      }
    }
    
    
    /*
     * Ako je cepelin ziv, azuriraj parametre za animaciju poda i ukrasa sa strane
     */
    
    if(cepelin_alive) {
      
      // Azuriranje parametara animacije poda
      tile_animation_parameter += 0.08;
      
      // Resetovanje parametra ako je linija otisla van scene
      if(tile_animation_parameter >= 2) {
        tile_animation_parameter = 0;
      }
      
      // Azuriranje parametara animacije ukrasa sa strane
      world_blocks_animation_parameter += 0.08;
      
      // Resetovanje parametra ako je ukras otisao van scene
      if(world_blocks_animation_parameter >= 2) {
        world_blocks_animation_parameter = 0;
      }
      
    }
    
    if(fire_boss == 1 && gate_counter == 20) {
      smoke_animation_parameter[7] += 0.15;
      
    }
    
    
      
    glutPostRedisplay();
    if(animation_ongoing){  
      glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
    
}


static void on_display(void)
{
  
  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,0,0,0,0,1,0);
   
    
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
    GLfloat diffuse_coeffs[] = { 0.75, 0.75, 0.75, 1 };

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
     * Iscrtavanje sveta
     */
    glPushMatrix();
        draw_world();
    glPopMatrix();
    
    /*
     * Iscrtavanje cepelina
     */
    glPushMatrix();
        glScalef(0.5,0.5,0.5);
        
        GLfloat diffuse_coeffs_cepelin[] = { 0.0, 0.3, 0.6, 1 };
            
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_cepelin);
            
        // Ako cepelin gori, promeni mu boju
        if(cepelin_on_fire == 1){
            GLfloat diffuse_coeffs[] = { 0.75, 0.05, 0.05, 1 };
            
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
            
            
            
    
        }
        draw_cepelin();
        
        
        
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
        
        
        /*
         * Ovde izvlacim koordinate cepelina, kako bi se kasnije koristile za 
         * detekciju kolizije za projektilima
         */
        GLfloat matrix[16]; 
        glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
        
        cepelin_x = matrix[12];
        cepelin_y = matrix[13];
        cepelin_z = matrix[14];
        
        
        
    glPopMatrix();
    
    
    
    /*
     * Ako je smo dosli do poslednjeg nivoa, iscrtaj Reichstag
     */
    if(gate_counter == 20) {
      glPushMatrix();
        GLfloat diffuse_coeffs_tower[] = { 0.9, 0.3, 0.3, 1 };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_tower);
        glTranslatef(0,-1,gate_animation_parameter);    
        draw_reichstag();
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
        
      glPopMatrix();
    }
    
    int i;
    
    /*
     * Iscrtaj vratnice
     */
    for(i = -3; i<4; i++) {
     
      if(gate_matrix[gate_counter][i+3] == 1){ // ako je vrednost 1, celu kulu
        
        glPushMatrix();
            GLfloat diffuse_coeffs_tower[] = { 0.9, 0.3, 0.3, 1 };
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_tower);
            glTranslatef(i,-1,gate_animation_parameter);
            draw_tower(i+3);
           
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
        glPopMatrix();
    
        /*
         * Racunaju se uglovi rotacije pemena (projektila) kako bi pogledom
         * pratili kretanje cepelina
         */
        alpha[i+3] = aim_angle_init(0,y_curr, gate_animation_parameter, 1);
        beta[i+3] = aim_angle_init( -gate_animation_parameter, i , 0,x_curr); 
        
      }
      else if(gate_matrix[gate_counter][i+3] == -1){ // inace, srusenu kulu
        glPushMatrix();
            glTranslatef(i,-1,gate_animation_parameter);
            GLfloat diffuse_coeffs_tower[] = { 0.9, 0.3, 0.3, 1 };
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_tower);
            
            draw_half_tower();
            
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
        glPopMatrix();
      }
    }
    
    /*
     * Ako je cepelin mrtav, crtaj eksploziju
     */
    if(cepelin_alive == 0) {
      glPushMatrix();
      
        draw_explosion();  
        
      glPopMatrix();
    }
  
    
    
    glutSwapBuffers();
   
}

