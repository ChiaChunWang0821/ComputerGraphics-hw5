unsigned int LoadTextureBMPWH(const char * filename, int height, int width);

void top_menu_func(int);

void x_axis_func();
void y_axis_func();
void z_axis_func();
void perspective_func();
void view_port_func();

void make_view(int);
void make_projection(int);
void draw_view();
void draw_view_volume();
void draw_floor();
void draw_axes();

void draw_scene(void);

void draw_obstacle();
void draw_stone();
void draw_coral();
void draw_grass1();
void draw_grass2();
void draw_grass3();
void draw_cube1();
void draw_cube2();

void draw_ROV();
void draw_body();
void draw_wheel();
void draw_camera();
void draw_top_blade();
void draw_back_blade();
void draw_arm();
void draw_ROV_light();

void make_texture_brick1();
void make_texture_brick2();
void make_texture_seabed();