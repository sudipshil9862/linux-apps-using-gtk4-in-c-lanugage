#include<gtk/gtk.h>


static void click_cb(GtkButton *button, gpointer user_data){
	g_print("clicked...\n");

	//destroy winodw after click
	GtkWindow *window = GTK_WINDOW(user_data);
	gtk_window_destroy(window);
	
}

static void app_activate(GApplication *app, gpointer user_data){
	GtkWidget *window;
	GtkWidget *button;
	
	//new window
	g_print("window creating...\n");
	window = gtk_application_window_new(GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(window), "button");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

	//new button
	g_print("button creating...\n");
	button = gtk_button_new_with_label("Quit");
	gtk_window_set_child(GTK_WINDOW(window),button);
	g_signal_connect(button, "clicked", G_CALLBACK(click_cb),window);

	gtk_widget_show(window);
}


int main(int argc, char **argv){
	GtkApplication *app;
	int status;
	app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_print("app creating\n");
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;

}
