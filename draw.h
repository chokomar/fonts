#ifndef __DRAW_H__
#define __DRAW_H__

typedef struct widgets
{
    GtkWidget *create, *clear, *word, *color;
    GtkWidget *window;
    GtkWidget *paned;
    GtkWidget *frame;
    GtkWidget *drawing_area;
    GtkWidget *btn_box;
    //gboolean **matrix;

} widgets_t;

extern widgets_t drawing;

static void clear_surface (GtkWidget *widget, gpointer data);

static gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer data);

static gboolean draw_cb (GtkWidget *widget, cairo_t *cr, gpointer data);

static void draw_brush (GtkWidget *widget, double x, double y);

GtkWidget* draw_buttons ();

static gboolean init_matrix (GtkWidget *widget);

static void net_draw (GtkWidget *widget, int sign, double x, double y);

static gboolean scroll_event_cb (GtkWidget* widget, GdkEventScroll *event, gpointer data);

static gboolean create_pressed (GtkWidget* widget, gpointer data);

static gboolean color_pressed (GtkWidget *widget,  gpointer data);

static gboolean color_activated (GtkColorChooser *color_chooser, gpointer data);

static gboolean clear_pressed (GtkWidget *widget, gpointer data);

static gboolean button_press_event_cb (GtkWidget *widget, GdkEventButton *event, gpointer data);

static gboolean motion_notify_event_cb (GtkWidget *widget,  GdkEventMotion *event, gpointer data);

void notify_draw ();

static void close_window (void);

GtkWidget*  draw(GtkWidget *widget);

static void do_some_fugly ();

#endif // __DRAW_H__