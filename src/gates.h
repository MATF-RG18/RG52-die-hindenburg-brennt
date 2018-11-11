#ifndef GATES_H
#define GATES_H

void draw_tower(int i);
void draw_reichstag(); 
void draw_half_tower();
void gate_matrix_init();
double aim_angle_init(double x1,  double y1, double x2, double y2);
void draw_smoke(int i); 

extern float pemen_x[7];
extern float pemen_y[7];
extern float pemen_z[7];

extern float boss_x;
extern float boss_y;
extern float boss_z;

extern int gate_matrix[20][7]; //matrica vratnica pakla

extern int gate_counter;

extern float gate_animation_parameter;
extern float smoke_animation_parameter[8];
extern float pemen_bullet_animation_parameter[8];
extern float pemen_animation_parameter;


extern int rotation_indicator[7];
extern float smoke_position[8];
extern int fire_boss;

extern float alpha[7], beta[7];
extern float alpha_curr[7], beta_curr[7];
extern float delta_alpha;
extern float delta_beta;


#endif