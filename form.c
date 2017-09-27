#include <gtk/gtk.h>
#include <form.h>
#include <draw.h>

GtkWidget *window, *d_window;
GtkWidget *grid;
GtkWidget *frame;
GtkWidget *paned;
GtkWidget *btn_box;

void gui(GtkWidget *window)

{

    //g_object_set(obj,"margin", 12, 0);
    paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    //gtk_paned_set_position(GTK_PANED(paned), 1000);
    gtk_container_add(GTK_CONTAINER(window), paned);

    frame = draw(window);
    gtk_paned_add1 (GTK_PANED (paned), frame);
    //gtk_widget_set_size_request (widgets.d_window, 500, 500 );

    btn_box = draw_buttons();
    gtk_paned_add2 (GTK_PANED(paned), btn_box);
    //grid = gtk_grid_new();
    //gtk_grid_set_row_homogeneous(GTK_GRID(grid), 3);
    //gtk_paned_pack2 (GTK_PANED (paned), grid, FALSE, FALSE);

    //gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);
    //gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1); 

}

void activate (GtkApplication *app, gpointer data)

{

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), 0);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_window_set_default_size (GTK_WINDOW(window), 1200, 1100);

    gui(window);
    notify_draw(window);
    
    gtk_widget_show_all (window);
}



