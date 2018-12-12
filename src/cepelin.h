#ifndef CEPELIN_H
#define CEPELIN_H


void draw_cepelin();
void draw_explosion();
void draw_cepelin_smoke();

const float size;

extern float x_curr, y_curr;
extern float v_x, v_y;

extern float explosion_animation_parameter;

extern float smoke_expansion_parameter;
extern float smoke_movement_parameter;

extern int movement_indicator_cx;
extern int movement_indicator_cy;

int border_rotation_indicator_x;
int border_rotation_indicator_y;

extern int cepelin_alive;
extern int cepelin_on_fire;
extern float cepelin_life_parameter;
extern float cepelin_x;
extern float cepelin_y;
extern float cepelin_z;

extern int direction_change_indicator[4];

extern int angle_compensation_indicator[4];

extern float xang, yang;
extern float delta_xang, delta_yang;

#endif