#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "algebra.hpp"
#include "a2.hpp"
#include <string>


// The "main" OpenGL widget
class Viewer : public Gtk::GL::DrawingArea {
public:
  Viewer();
  virtual ~Viewer();
  void input_handler(int type);
  double fov, ar, far, near,w,h,vx,vy;
  int current_mode;

  enum {
    MODEL,
    VIEW,
    VIEWPORT,
  };

  // A useful function that forces this widget to rerender. If you
  // want to render a new frame, do not call on_expose_event
  // directly. Instead call this, which will cause an on_expose_event
  // call when the time is right.
  void invalidate();

  // *** Fill in these functions (in viewer.cpp) ***

  // Restore all the transforms and perspective parameters to their
  // original state. Set the viewport to its initial size.
  void reset_view();
  char keypress[6];
protected:

  // Events we implement
  // Note that we could use gtkmm's "signals and slots" mechanism
  // instead, but for many classes there's a convenient member
  // function one just needs to define that'll be called with the
  // event.

  // Called when GL is first initialized
  virtual void on_realize();
  // Called when our window needs to be redrawn
  virtual bool on_expose_event(GdkEventExpose* event);
  // Called when the window is resized
  virtual bool on_configure_event(GdkEventConfigure* event);
  // Called when a mouse button is pressed
  virtual bool on_button_press_event(GdkEventButton* event);
  // Called when a mouse button is released
  virtual bool on_button_release_event(GdkEventButton* event);
  // Called when the mouse moves
  virtual bool on_motion_notify_event(GdkEventMotion* event);

private:
  // You will want to declare some more matrices here
  Matrix4x4 identity;
  Matrix4x4 pm; //perspective matrix
  Matrix4x4 vpm; //viewport matrix
  Matrix4x4 vm; //viewing matrix
  Matrix4x4 mm; //modelling matrix
  Matrix4x4 T1,T2; //T2 will initialized to be the inverse of T1
  Vector4D p[8]; //box points
  Vector4D pp[8]; //projection points
  Vector4D wc[4];
  Vector4D oc[4];
  Vector4D wcc[4];
  Vector4D occ[4];

  bool buttonpressed[3];
  double x, y;
  

  void initialize_viewing_matrix();
  void initialize_viewport_matrix(double w, double h,double vx, double vy);
  void set_perspective_matrix(double fov, double ar, double f, double n);
  void initialize_box();
  void initialize_np();
  void clip_segment(Vector4D p0, Vector4D p1);
  void draw_borders();
};

#endif
