#include "appwindow.hpp"

std::string AppWindow::getMessageText(std::string mode, std::string transformation) {
  std::ostringstream s;
  std::string m;
  std::string mod;

  s <<mode<<" - "<<transformation<<" FOV: "<<m_viewer.fov<<"  "<<"Near:  "<<m_viewer.near<<"  "<<"Far: "<<m_viewer.far;

  m = s.str();
  return m;
}

void AppWindow::update_label(int mode) {

  if (mode == Viewer::VIEWPORT) {
    label.set_text(getMessageText("Viewport", ""));
  }
  if (mode == Viewer::MODEL) {
    label.set_text(getMessageText("Model","Rotate"));
  }
  if (mode == Viewer::VIEW) {
    label.set_text(getMessageText("View","Rotate"));
  }
}

AppWindow::AppWindow()
{
  set_title("Cube");

  // A utility class for constructing things that go into menus, which
  // we'll set up next.
  using Gtk::Menu_Helpers::MenuElem;
  
  // Set up the application menu
  // The slot we use here just causes AppWindow::hide() on this,
  // which shuts down the application.
  m_menu_app.items().push_back(MenuElem("Quit",
    sigc::mem_fun(*this, &AppWindow::hide)));

  m_model_item = new Gtk::RadioMenuItem(m_group,"model");
  m_view_item = new Gtk::RadioMenuItem(m_group,"view");
  m_port_item = new Gtk::RadioMenuItem(m_group,"viewport");

  m_menu_mode.items().push_back(*m_port_item);
  m_port_item->signal_activate().connect(sigc::bind(sigc::mem_fun(m_viewer,&Viewer::input_handler),Viewer::VIEWPORT));
  m_port_item->signal_activate().connect(sigc::bind(sigc::mem_fun(*this,&AppWindow::update_label),Viewer::VIEWPORT));


  m_menu_mode.items().push_back(*m_model_item);
  m_model_item->signal_activate().connect(sigc::bind(sigc::mem_fun(m_viewer,&Viewer::input_handler),Viewer::MODEL));
  m_model_item->signal_activate().connect(sigc::bind(sigc::mem_fun(*this,&AppWindow::update_label),Viewer::MODEL));

  m_menu_mode.items().push_back(*m_view_item);
  m_view_item->signal_activate().connect(sigc::bind(sigc::mem_fun(m_viewer,&Viewer::input_handler),Viewer::VIEW));
  m_view_item->signal_activate().connect(sigc::bind(sigc::mem_fun(*this,&AppWindow::update_label),Viewer::VIEW));

  m_menu_reset.items().push_back(m_reset_item);
  m_reset_item.signal_activate().connect(sigc::mem_fun(m_viewer, &Viewer::reset_view));
  m_reset_item.add_label("Reset");
  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));

  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Modes", m_menu_mode));

  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Reset", m_menu_reset));

  label.set_text("Model - Rotate  FOV: 80  Near: -1  Far -20");
  m_vbox.add(label);
  // First add the vertical box as our single "top" widget
  add(m_vbox);

  // Put the menubar on the top, and make it as small as possible
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

  // Put the viewer below the menubar. pack_start "grows" the widget
  // by default, so it'll take up the rest of the window.
  m_viewer.set_size_request(300, 300);

  m_vbox.pack_start(m_viewer);

  add_events(Gdk::KEY_PRESS_MASK | 
	     Gdk::KEY_RELEASE_MASK);
		    
  show_all();
}


bool AppWindow::on_key_press_event(GdkEventKey* event) {
  if (event->keyval == GDK_KEY_o) {
    if (m_viewer.current_mode == Viewer::VIEW)
      label.set_text(getMessageText("View", "Rotate"));
    m_viewer.keypress[0] = 'o';
  }
  if (event->keyval == GDK_KEY_n) {
    if (m_viewer.current_mode == Viewer::VIEW)
      label.set_text(getMessageText("View", "Translate"));
    m_viewer.keypress[1] = 'n';
  }
  if (event->keyval == GDK_KEY_p) {
    if (m_viewer.current_mode == Viewer::VIEW)
      label.set_text(getMessageText("View", "Fov/Near/Far"));
    m_viewer.keypress[2] = 'p';
  }
  if (event->keyval == GDK_KEY_r) {
    if (m_viewer.current_mode == Viewer::MODEL)
      label.set_text(getMessageText("Model", "Rotate"));
    m_viewer.keypress[3] = 'r';
  }
  if (event->keyval == GDK_KEY_t) {
    if (m_viewer.current_mode == Viewer::MODEL)
      label.set_text(getMessageText("Model", "Translate"));
    m_viewer.keypress[4] = 't';
  }
  if (event->keyval == GDK_KEY_s) {
    if (m_viewer.current_mode == Viewer::MODEL)
      label.set_text(getMessageText("Model", "Scale"));
    m_viewer.keypress[5] = 's';
  }
  if (event->keyval == GDK_KEY_q) {
    hide();
  }
  if (event->keyval == GDK_KEY_a) {
    label.set_text(getMessageText("Model", "Rotate"));
    m_viewer.reset_view();
  }

  std::cout<<"pressing key"<<std::endl;

  return true;
}

bool AppWindow::on_key_release_event(GdkEventKey* event) {
  if (event->keyval == GDK_KEY_o) {
    m_viewer.keypress[0] = '\0';
  }
  if (event->keyval == GDK_KEY_n) {
    m_viewer.keypress[1] = '\0';
  }
  if (event->keyval == GDK_KEY_p) {
    m_viewer.keypress[2] = '\0';
  }
  if (event->keyval == GDK_KEY_r) {
    m_viewer.keypress[3] = '\0';
  }
  if (event->keyval == GDK_KEY_t) {
    m_viewer.keypress[4] = '\0';
  }
  if (event->keyval == GDK_KEY_s) {
    m_viewer.keypress[5] = '\0';
  }
  
  std::cout<<"releasing key"<<std::endl;
  return true;
}
