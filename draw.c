#include <stdlib.h>
//#include <stdbool.h>
#include <gtk/gtk.h>
#include <draw.h>

cairo_surface_t *surface;
widgets_t drawing;
gboolean **matrix;
GdkRGBA color;

int m_rows, m_columns;
double scale;
const double min_scale = 8.0;
const double max_scale = 20.0;

static void clear_surface (GtkWidget *widget, gpointer data)

{
    cairo_t *cr;
    cr = cairo_create (surface);

    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_paint (cr);

    cairo_destroy (cr);
}

static void net_draw (GtkWidget *widget, int sign, double x, double y)

{

    int i, rows, columns;
    double step,height, width;

    cairo_t *cr;

    height = gtk_widget_get_allocated_height (widget);
    width = gtk_widget_get_allocated_width (widget);

    //int del_i = m_rows - (height / scale) ;
    //int del_j = m_columns - (width / scale) ;

    //printf("%d %d\n", x1, y1);

    cr = cairo_create (surface);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_set_line_width (cr, 0.6);

    step = 0.0;

    while (width >= step)
    {
	cairo_move_to (cr, step, 0);
	cairo_line_to (cr, step, height);
	cairo_stroke (cr);
	step+=scale;
    }

    step = 0.0;

    while (height >= step)
    {
	cairo_move_to (cr, 0, step);
	cairo_line_to (cr, width, step);
	cairo_stroke (cr);
	step+=scale;
    }

    cairo_set_source_rgba (cr, color.red, color.green, color.blue, color.alpha);

    rows = height / scale;
    columns = width / scale;

    int coef_rows =  rows / m_rows;	//int x0 = width / 4 / min_scale;
    int coef_col = columns / m_columns;	//int y0 = height / 4 / min_scale;

    double y1 = height / scale;
    double x1 = width / scale;

    double y0 = height / 2.0;
    double x0 = width / 2.0;

    //int del_i = ;
    //int del_j = m_columns - columns;

/*
    if (x > x0 && y > y0) { del_i = -del_i; del_j = -del_j;  } //4

    if (x < x0 && y < y0) { del_i = del_i; del_j = del_j; } // 2

    if (x < x0 && y > y0) { del_i = del_i; del_j = -del_j; } // 1

    if (x > x0 && y < y0) { del_i = -del_i; del_j = del_j; } // 3
*/
    
    int i0 = y / scale; int j0 = x / scale;

    printf ("%d %d\n", i0, j0);

    for (int i = 0; i < m_rows; i++)
    {
	for (int j = 0; j < m_columns; j++)
	{
	    if (matrix[i][j] == TRUE)
    	    {
		
		if (i0 > i && j0 > j)  //4
		{
		
		cairo_rectangle (cr, (i-scale)*scale, (j-scale)*scale, scale, scale);
		cairo_fill (cr);
		}
	        if (i0 < i && j0 < j)  // 2
		{
		
		cairo_rectangle (cr, (i+scale)*scale, (j+scale)*scale, scale, scale);
		cairo_fill (cr);
		}
	        if (i0 < i && j0 > j)  // 1
		{
		
		cairo_rectangle (cr, (i+scale)*scale, (j-scale)*scale, scale, scale);
		cairo_fill (cr);
		}
	        if (i0 > i && j0 < j) // 3
		{
		
		cairo_rectangle (cr, (i-scale)*scale, (j+scale)*scale, scale, scale);
		cairo_fill (cr);
		}
	    }
	}
    }

    cairo_destroy (cr);
    gtk_widget_queue_draw (widget);
}


/*

    another part of the free space

*/

static gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer data)

{
    if (surface) cairo_surface_destroy (surface);
    surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                               CAIRO_CONTENT_COLOR,
                                               gtk_widget_get_allocated_width (widget),
                                               gtk_widget_get_allocated_height (widget));

    if (matrix == NULL) init_matrix (widget);

    clear_surface(widget, 0);
    net_draw (widget, 0, 0, 0);

    return TRUE;
}

/*

    another part of the free space

*/

static gboolean draw_cb (GtkWidget *widget, cairo_t *cr, gpointer data)

{
    cairo_set_source_surface (cr, surface, 0, 0);
    cairo_paint (cr);

    return TRUE;
}

/*

    another part of the free space

*/

static void draw_brush (GtkWidget *widget, double x, double y)

{
    int i, j;

    i = x / min_scale; 
    j = y / min_scale;

    if ( i >= 0 && i <= m_rows && j >= 0 && j <= m_columns)

    {
	cairo_t *cr;

	matrix[i][j] = TRUE;

	i = x / scale; 
	j = y / scale;

        cr = cairo_create (surface);

	cairo_rectangle(cr, i*scale, j*scale, scale, scale);
	cairo_set_source_rgba (cr, color.red, color.green, color.blue, color.alpha);
	cairo_fill (cr);

	cairo_destroy (cr);
	gtk_widget_queue_draw_area (widget, i*scale, j*scale, scale, scale);

    }
}

/*

    another part of the free space

*/

static gboolean motion_notify_event_cb (GtkWidget *widget, GdkEventMotion *event, gpointer data)

{
    if (surface == NULL)  return FALSE;

    if (event->state & GDK_BUTTON1_MASK) draw_brush (widget, event->x, event->y);

    return TRUE;
}

/*

    another part of the free space CAIRO_CLIP()

*/

static gboolean scroll_event_cb (GtkWidget *widget, GdkEventScroll *event, gpointer data)

{
    double x, y, height, width;
    cairo_t *cr;

    int sign = 0;

    x = event->x; y = event->y;

    cr = cairo_create (surface);

	if (scale < max_scale && event->direction == GDK_SCROLL_UP)
	    {
		scale += 1.0;
		clear_surface(widget, 0);
		net_draw(widget, sign, x, y);
		gtk_widget_queue_draw(widget);
	    }
	
	if (scale > min_scale && event->direction == GDK_SCROLL_DOWN)
	    {
		sign = -1;
		scale -= 1.0;
		clear_surface(widget, 0);
		net_draw(widget, sign, x, y);
		gtk_widget_queue_draw(widget);
	    }
    

    cairo_destroy (cr);

    return TRUE;
}

static void close_window ()

{

    if (surface) cairo_surface_destroy (surface);

}

/*

    another part of the free space

*/

GtkWidget* draw_buttons ()

{
    drawing.btn_box = gtk_button_box_new (GTK_ORIENTATION_VERTICAL);

    gtk_container_set_border_width (GTK_CONTAINER (drawing.btn_box), 10);
    gtk_button_box_set_layout (GTK_BUTTON_BOX(drawing.btn_box), GTK_BUTTONBOX_CENTER);

    drawing.create = gtk_button_new_with_label("Create");
    gtk_container_add(GTK_CONTAINER(drawing.btn_box), drawing.create);

    drawing.color = gtk_button_new_with_label("Color");
    gtk_container_add(GTK_CONTAINER(drawing.btn_box), drawing.color);

    drawing.clear = gtk_button_new_with_label("Clear");
    gtk_container_add (GTK_CONTAINER(drawing.btn_box), drawing.clear);

    return drawing.btn_box;
}

static gboolean create_pressed (GtkWidget *widget, gpointer data)

{
    GtkWidget *new_window;
    GtkWidget *draw;

    new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    draw = gtk_drawing_area_new();

    gtk_container_add (GTK_CONTAINER(new_window), draw);

    gtk_widget_show (new_window);
}

static gboolean color_pressed (GtkWidget *widget, gpointer data)

{
    GtkWidget *color_dialog = gtk_color_chooser_dialog_new ("Colors", 0);

    int status = gtk_dialog_run (GTK_DIALOG(color_dialog));

    if (status == -5)
	{
	    gtk_color_chooser_get_rgba (GTK_COLOR_CHOOSER (color_dialog), &color);
	    gtk_widget_destroy (color_dialog);
	}
    else if (status == -6) gtk_widget_destroy (color_dialog);
}


static gboolean button_press_event_cb (GtkWidget *widget, GdkEventButton *event, gpointer data)

{
    if (surface == NULL) return FALSE;

    if (event->button == GDK_BUTTON_PRIMARY)  draw_brush (widget, event->x, event->y);

    return TRUE;
}

static gboolean clear_pressed (GtkWidget *widget, gpointer data)

{

    clear_surface (widget, 0);

    for(int i = 0; i < m_rows; ++i) 
	for (int j = 0; j < m_columns; j++) 
	    {
		matrix[i][j] =  FALSE;
	    }

    net_draw(data, 0, 0, 0);
    gtk_widget_queue_draw(data);

    return TRUE;
}

/*

    another part of free space

*/

static gboolean init_matrix (GtkWidget *widget)
{

	m_rows = gtk_widget_get_allocated_height (widget) / min_scale;
	m_columns = gtk_widget_get_allocated_width (widget) / min_scale;

	//intf("%d %d\n", m_rows, m_columns);

	matrix = g_malloc_n (m_rows, sizeof(gboolean*));
	for(int i = 0; i < m_rows; i++) matrix[i] = g_malloc_n (m_columns, sizeof(gboolean));

	for(int i = 0; i < m_rows; ++i) 
	    for (int j = 0; j < m_columns; j++) matrix[i][j] =  FALSE;

    return TRUE;
}

void notify_draw()

{
    g_signal_connect (drawing.drawing_area, "draw", G_CALLBACK (draw_cb), 0);
    g_signal_connect (drawing.drawing_area, "configure-event", G_CALLBACK (configure_event_cb), 0);
    g_signal_connect (drawing.drawing_area, "motion-notify-event", G_CALLBACK (motion_notify_event_cb), 0);
    g_signal_connect (drawing.drawing_area, "button-press-event", G_CALLBACK (button_press_event_cb),0);
    g_signal_connect (drawing.window, "destoy", G_CALLBACK(close_window), 0);
    g_signal_connect (drawing.drawing_area, "scroll-event", G_CALLBACK (scroll_event_cb), 0);

    g_signal_connect (drawing.clear, "clicked", G_CALLBACK(clear_pressed), drawing.drawing_area);
    g_signal_connect (drawing.color, "clicked", G_CALLBACK(color_pressed), 0);
    g_signal_connect (drawing.create, "clicked", G_CALLBACK(create_pressed),drawing.drawing_area);

    gtk_widget_set_events (drawing.drawing_area, gtk_widget_get_events (drawing.drawing_area)
					| GDK_BUTTON_PRESS_MASK
					| GDK_POINTER_MOTION_MASK
					| GDK_SCROLL_MASK
					| GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK );
}

static void do_some_fugly ()

{
    surface = 0;
    scale = min_scale;

    color.red = 0.0;
    color.green = 0.0;
    color.blue = 0.0;
    color.alpha = 1.0;

}

GtkWidget* draw (GtkWidget *window)

{

    do_some_fugly ();

    drawing.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    drawing.drawing_area = gtk_drawing_area_new();

    drawing.frame = gtk_frame_new (0);
    gtk_frame_set_shadow_type (GTK_FRAME (drawing.frame), GTK_SHADOW_IN);
    gtk_widget_set_size_request (GTK_WIDGET(drawing.frame), 1000, 1100);

    gtk_container_add (GTK_CONTAINER(drawing.frame), drawing.drawing_area);

    return drawing.frame;
}

/*

    another part of the free space

*/