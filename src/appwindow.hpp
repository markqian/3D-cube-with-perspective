#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <gtkmm.h>
#include "viewer.hpp"
#include <string>

class AppWindow : public Gtk::Window {
public:
  AppWindow();
  void update_label(int mode);
  std::string getMessageText(std::string mode,std::string trans);
protected:
  virtual bool on_key_press_event(GdkEventKey* event);
  virtual bool on_key_release_event(GdkEventKey* event);

private:
  // A "vertical box" which holds everything in our window
  Gtk::VBox m_vbox;
  Gtk::Label label;
  // The menubar, with all the menus at the top of the window
  Gtk::MenuBar m_menubar;
  // Each menu itself
  Gtk::Menu m_menu_app;
  Gtk::Menu m_menu_mode;
  Gtk::Menu m_menu_reset;
  //Sub-items in the menu
  Gtk::MenuItem m_reset_item;
  Gtk::RadioButtonGroup m_group;
  Gtk::RadioMenuItem *m_model_item, *m_view_item, *m_port_item;
  // The main OpenGL area
  Viewer m_viewer;
  
};

#endif
