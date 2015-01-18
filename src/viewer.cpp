#include "viewer.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include "draw.hpp"


void Viewer::input_handler(int type) {
  current_mode = type;
}


void Viewer::initialize_viewing_matrix() {
  vm[2][3] = 3;

  T1[0][0] = 0.5;
  T1[1][1] = 0.5;
  T1[2][2] = 0.5;
  T1[3][3] = 1;

  T1[0][3] = 0.5;
  T1[1][3] = 0.5;
  T1[2][3] = 0.5;

  T2[0][0] = 2;
  T2[1][1] = 2;
  T2[2][2] = 2;

  T2[0][3] = -1;
  T2[1][3] = -1;
  T2[2][3] = -1;
  T2[3][3] = 1;
}

void Viewer::initialize_viewport_matrix(double x0, double y0, double x1, double y1) {

  w = std::abs(x1 - x0);
  h = std::abs(y1 - y0);
  
  vpm[0][0] = w*1/2;
  vpm[0][3] = (w+std::min(x0,x1)*2)*1/2; 

  vpm[1][1] = h*1/2;
  vpm[1][3] = (h+std::min(y0,y1)*2)*1/2 ;

  vpm[2][2] = 1/2;
  vpm[2][3] = 1/2;

  vpm[3][3] = 1;
}

void Viewer::set_perspective_matrix(double fov, double ar, double f, double n) {
  if (ar > 0) {
    pm[0][0] = 1/(ar*tan((fov/2)*M_PI/180));
    pm[1][1] = 1/tan((fov/2)*M_PI/180);
  }
  else if (ar < 0) {
    pm[0][0] = 1/tan((fov/2)*M_PI/180);
    pm[1][1] = ar/tan((fov/2)*M_PI/180);
  }

  pm[2][2] = (f+n)/(f-n);
  pm[2][3] = (-2*f*n)/(f-n);
  pm[3][2] = -1;
  pm[3][3] = 0;
}

void Viewer::initialize_box() {

  p[0][0] = 1;
  p[0][1] = 1;
  p[0][2] = 1;
  p[0][3] = 1;
  
  p[1][0] = -1;
  p[1][1] = 1;
  p[1][2] = 1;
  p[1][3] = 1;

  p[2][0] = 1;
  p[2][1] = 1;
  p[2][2] = -1;
  p[2][3] = 1;

  p[3][0] = -1;
  p[3][1] = 1;
  p[3][2] = -1;
  p[3][3] = 1;

  p[4][0] = 1;
  p[4][1] = -1;
  p[4][2] = 1;
  p[4][3] = 1;

  p[5][0] = -1;
  p[5][1] = -1;
  p[5][2] = 1;
  p[5][3] = 1;

  p[6][0] = -1;
  p[6][1] = -1;
  p[6][2] = -1;
  p[6][3] = 1;

  p[7][0] = 1;
  p[7][1] = -1;
  p[7][2] = -1;
  p[7][3] = 1;

  wc[0][0] = 0;
  wc[0][1] = 0;
  wc[0][2] = 0;
  wc[0][3] = 1;

  wc[1][0] = 0.5;
  wc[1][1] = 0;
  wc[1][2] = 0;
  wc[1][3] = 1;

  wc[2][0] = 0;
  wc[2][1] = 0.5;
  wc[2][2] = 0;
  wc[2][3] = 1;

  wc[3][0] = 0;
  wc[3][1] = 0;
  wc[3][2] = -0.5;
  wc[3][3] = 1;

  oc[0][0] = 0;
  oc[0][1] = 0;
  oc[0][2] = 0;
  oc[0][3] = 1;

  oc[1][0] = 0.5;
  oc[1][1] = 0;
  oc[1][2] = 0;
  oc[1][3] = 1;

  oc[2][0] = 0;
  oc[2][1] = 0.5;
  oc[2][2] = 0;
  oc[2][3] = 1;

  oc[3][0] = 0;
  oc[3][1] = 0;
  oc[3][2] = -0.5;
  oc[3][3] = 1;
}

Viewer::Viewer()
{

  Glib::RefPtr<Gdk::GL::Config> glconfig;

  // Ask for an OpenGL Setup with
  //  - red, green and blue component colour
  //  - a depth buffer to avoid things overlapping wrongly
  //  - double-buffered rendering to avoid tearing/flickering
  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB |
                                     Gdk::GL::MODE_DEPTH |
                                     Gdk::GL::MODE_DOUBLE);
  if (glconfig == 0) {
    // If we can't get this configuration, die
    std::cerr << "Unable to setup OpenGL Configuration!" << std::endl;
    abort();
  }

  // Accept the configuration
  set_gl_capability(glconfig);

  // // Register the fact that we want to receive these events
  add_events(Gdk::BUTTON1_MOTION_MASK    |
             Gdk::BUTTON2_MOTION_MASK    |
             Gdk::BUTTON3_MOTION_MASK    |
             Gdk::BUTTON_PRESS_MASK      | 
             Gdk::BUTTON_RELEASE_MASK    |
  	     Gdk::VISIBILITY_NOTIFY_MASK 
        );
}

Viewer::~Viewer()
{
  // Nothing to do here right now.
}

void Viewer::invalidate()
{
  // Force a rerender
  Gtk::Allocation allocation = get_allocation();
  get_window()->invalidate_rect( allocation, false);
}

void Viewer::reset_view()
{
  w = get_width();
  h = get_height();

  initialize_box();

  fov = 80;
  ar = w/h;
  far = -20;
  near = -1;

  set_perspective_matrix(fov, ar, far, near);
  vx = 0;
  vy = 0;
  initialize_viewport_matrix(15,15,w-15,h-15);
  buttonpressed[0] = false;
  buttonpressed[1] = false;
  buttonpressed[2] = false;
  
  for (int i = 0; i<6; i++)
    keypress[i] = '\0';

  x = 0;
  y = 0;

  mm = identity;
  vm = identity;
  initialize_viewing_matrix();

  invalidate();
}

void Viewer::on_realize()
{
  // Do some OpenGL setup.
  // First, let the base class do whatever it needs to
  Gtk::GL::DrawingArea::on_realize();
  
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();
  
  if (!gldrawable)
    return;

  if (!gldrawable->gl_begin(get_gl_context()))
    return;

  gldrawable->gl_end();
  current_mode = MODEL;

  reset_view();
 
  std::cout<<"height is: "<<get_height()<<std::endl;
  std::cout<<"width is: "<<get_width()<<std::endl;
}


int calculateBC(Vector4D p, double BC[6]) {
  int kode = 0;
  int mask = 1;
  double w = p[3];

  BC[0] = p[0];
  BC[1] = w - p[0];
  BC[2] = p[1];
  BC[3] = w - p[1];
  BC[4] = p[2];
  BC[5] = w - p[2];

  for (int i = 0; i<6; i++) {
    if (BC[i] < 0) {
      kode = kode | mask;
    }
    mask <<= 1;
  }
  return kode;
}

void Viewer::clip_segment(Vector4D p0, Vector4D p1) {
  int Kode0, Kode1,Klip,Mask;
  double alpha0,alpha1,alpha;
  double BC0[6], BC1[6];
  Vector4D p,pp0,pp1;
  
  Kode0 = calculateBC(p0, BC0); //signs of boundaries represented in bits for p0;
  Kode1 = calculateBC(p1, BC1);
    
  if ((Kode0 & Kode1) != 0) {
    return; 
  }
  if ((Kode0 | Kode1) != 0) {
    Klip = Kode0 | Kode1;
    alpha0 = 0.0;
    alpha1 = 1.0;
    Mask = 1;
  
   for (int i = 0; i<6; i++) {
     if ((Klip & Mask) !=0) { 
       alpha = BC0[i] / (BC0[i]-BC1[i]);
       if ((Kode0 & Mask) != 0)
	 alpha0 = std::max(alpha0, alpha);
       else
	 alpha1 = std::min(alpha1, alpha);
       if (alpha1 < alpha0) return;
     }
     Mask <<= 1;
   }

   if (Kode0 != 0) {
     pp0 = p0 + alpha0*(p1 - p0);
   }
   else {
     pp0 = p0;
   }

   if (Kode1 != 0) {
     pp1 = p0 + alpha1*(p1 - p0);
   }
   else {
     pp1 = p1;
   }

   //draw
   pp0 = T2 * pp0;
   pp1 = T2 * pp1;
   
   pp0[0] /= pp0[3];
   pp0[1] /= pp0[3];
   pp0[2] /= pp0[3];
   pp0[3] /= pp0[3];
   
   pp1[0] /= pp1[3];
   pp1[1] /= pp1[3];
   pp1[2] /= pp1[3];
   pp1[3] /= pp1[3];
   
   pp0 = vpm * pp0; 
   pp1 = vpm * pp1;
   
   draw_line(Point2D(pp0[0], pp0[1]),
	     Point2D(pp1[0], pp1[1]));    
   
  }
  else {
    //draw
    p0 = T2 * p0;   //T2 is inverse of T1 
    p1 = T2 * p1;
    
    p0[0] /= p0[3];
    p0[1] /= p0[3];
    p0[2] /= p0[3];
    p0[3] /= p0[3];
    
    p1[0] /= p1[3];
    p1[1] /= p1[3];
    p1[2] /= p1[3];
    p1[3] /= p1[3];
    
    p0 =vpm * p0; 
    p1 =vpm * p1;
    
    draw_line(Point2D(p0[0], p0[1]),    
	      Point2D(p1[0], p1[1]));     
  }
}

void Viewer::draw_borders() {
  Vector4D 
    b1(-1,1,-1,1),
    b2(1,1,-1,1),
    b3(-1,-1,-1,1),
    b4(1,-1,-1,1);
  
  b1 = vpm * b1;
  b2 = vpm * b2;
  b3 = vpm * b3;
  b4 = vpm * b4;

  draw_line(Point2D(b1[0],b1[1]), Point2D(b2[0],b2[1]));
  draw_line(Point2D(b1[0],b1[1]), Point2D(b3[0],b3[1]));
  draw_line(Point2D(b2[0],b2[1]), Point2D(b4[0],b4[1]));
  draw_line(Point2D(b3[0],b3[1]), Point2D(b4[0],b4[1]));
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Here is where your drawing code should go.
  
  draw_init(get_width(), get_height());
  
  /* A few of lines are drawn below to show how it's done. */

  set_colour(Colour(0.1, 0.1, 0.1));

  for (int i =0; i<8; i++) {
    //apply model transformations
    pp[i] = mm * p[i];
    //apply view transformations
    pp[i] = vm.invert()* pp[i];
    //homogeneous space transformation
    pp[i] = pm*pp[i];
    //move and scale pp so that 0<= x, y, z <= w
    pp[i] = T1 * pp[i]; 
  }

  for (int i =0; i<4; i++) {
    occ[i] = mm * oc[i];    
    occ[i] = vm.invert() * occ[i];
    occ[i] = pm*occ[i];
    occ[i] = T1 * occ[i]; 

    wcc[i] = vm.invert() * wc[i];
    wcc[i] = pm*wcc[i]; 
    wcc[i] = T1 * wcc[i]; 
  }

  //draw object axis
  clip_segment(occ[0], occ[1]);
  clip_segment(occ[0], occ[2]);
  clip_segment(occ[0], occ[3]);

  //draw world axis
  clip_segment(wcc[0], wcc[1]);
  clip_segment(wcc[0], wcc[2]);
  clip_segment(wcc[0], wcc[3]);

  //draw top face
  clip_segment(pp[0], pp[2]); // (1,1,1) right 
                                       // (1,1,-1)
  clip_segment(pp[0], pp[1]); // (1,1,1) back
                                       // (-1,1,1)
  clip_segment(pp[1], pp[3]); // (-1,1,1) left
                                       // (-1,1,-1)
  clip_segment(pp[2], pp[3]); // (1,1,-1) front
                                       // (-1,1,-1)

  //// draw middle lines
  clip_segment(pp[0], pp[4]); // (1,1,1) back right
                                        // (1,-1,1)
  clip_segment(pp[1], pp[5]); // (-1,1,1) back left
                                        // (-1,-1,1)
  clip_segment(pp[3], pp[6]); // (-1,1,-1) front left
                                       // (-1,-1,-1)
  clip_segment(pp[2], pp[7]); // (1,1,-1) front right
                                       // (1,-1,-1)

  // //draw bottom face
  clip_segment(pp[5], pp[6]); //(-1,-1,1)     left
                                        //(-1,-1,-1)
  clip_segment(pp[4], pp[5]); //(1,-1,1)     top
                                        //(-1,-1,1)
  clip_segment(pp[4], pp[7]); //(1,-1,1)     right
                                       //(1,-1,-1)
  clip_segment(pp[6], pp[7]); //(-1,-1,-1)  bottom
                                       //(1,-1,-1)

  draw_borders();
  draw_complete();
            
  // Swap the contents of the front and back buffers so we see what we
  // just drew. This should only be done if double buffering is enabled.
  gldrawable->swap_buffers();

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_configure_event(GdkEventConfigure* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;
  
  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_button_press_event(GdkEventButton* event)
{
  x = event->x;
  y = event->y;

  if (event->button == 1) {
    if (current_mode == VIEWPORT) {
      vx = event->x;
      vy = event->y;
    }
    buttonpressed[0] = true;
  }
  if (event->button == 2) {
    buttonpressed[1] = true;
  }
  if (event->button == 3) {
    buttonpressed[2] = true;
  }  
  invalidate();
  std::cerr << "Stub: Button " << event->button << " pressed" << std::endl;
  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  if (event->button == 1) {
    if (current_mode == VIEWPORT) {
      initialize_viewport_matrix(vx,vy,event->x,event->y);
    }
    buttonpressed[0] = false;
  }
  if (event->button == 2) {
    buttonpressed[1] = false;
  }
  if (event->button == 3) {
    buttonpressed[2] = false;
  }  
  invalidate();

  std::cerr << "Stub: Button " << event->button << " released" << std::endl;
  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  double dx,dy;
  dx = event->x - x;
  dy = event->y - y;
  if (buttonpressed[0] == true) {
    if (current_mode == VIEW && keypress[0] == 'o')  
      vm =  vm * rotation(dy, 'x');
    if (current_mode == VIEW && keypress[1] == 'n')  
      vm =  vm *translation(Vector3D(dx/100,0,0));
    if (current_mode == VIEW && keypress[2] == 'p') {
      fov += dy/20;
      if (fov <5) fov = 5;
      if (fov >160) fov = 160;
      set_perspective_matrix(fov, ar, far, near);
    }
    if (current_mode == MODEL && keypress[3] == 'r')  
      mm =  mm * rotation(dy, 'x');
    if (current_mode == MODEL && keypress[4] == 't')  
      mm = translation(Vector3D(dx/100,0,0)) * mm;
    if (current_mode == MODEL && keypress[5] == 's') 
      mm = scaling(Vector3D(event->x/x,1,1)) * mm;    
  }

  if (buttonpressed[1] == true) {
    if (current_mode == VIEW && keypress[0] == 'o')  
      vm = rotation(dy, 'y') * vm;
    if (current_mode == VIEW && keypress[1] == 'n')  
      vm = translation(Vector3D(0,dx/100,0)) * vm;
    if (current_mode == VIEW && keypress[2] == 'p') {
      near += dy/20;
      set_perspective_matrix(fov, ar, far, near);
    }
    
    if (current_mode == MODEL && keypress[3] == 'r')  
      mm = rotation(dy, 'y') * mm;
    if (current_mode == MODEL && keypress[4] == 't')  
      mm = translation(Vector3D(0,dx/100,0)) * mm;
    if (current_mode == MODEL && keypress[5] == 's') 
      mm = scaling(Vector3D(1,event->y/y,1)) * mm;    
  }

  if (buttonpressed[2] == true) {
    if (current_mode == VIEW && keypress[0] == 'o')  
      vm = rotation(dy, 'z') * vm;
    if (current_mode == VIEW && keypress[1] == 'n')  
      vm = translation(Vector3D(0,0,dx/100)) * vm;
    if (current_mode == VIEW && keypress[2] == 'p') {
      far += dy/20;
      set_perspective_matrix(fov, ar, far, near);
    }
    if (current_mode == MODEL && keypress[3] == 'r')  
      mm = rotation(dy, 'z') * mm;
    if (current_mode == MODEL && keypress[4] == 't')  
      mm = translation(Vector3D(0,0,dx/100)) * mm;
    if (current_mode == MODEL && keypress[5] == 's') 
      mm = scaling(Vector3D(1,1,event->x/x)) * mm;    
  }

  x = event->x;
  y = event->y;

  invalidate();
  std::cerr << "Stub: Motion at " << event->x << ", " << event->y << std::endl;
  return true;
}
