//this library will be taken care by pkg-config
#include <gtk/gtk.h>


//static function -- function is scopped/stciked to this file
//we don't have any user_data
static void activate (GtkApplication* app, gpointer user_date){
	
	//widget pointer -- we should have a window 
	GtkWidget *window;

	//pass application pointer to this function
	window = gtk_application_window_new(app);

	//setting title for the window - wrapping the window to GTK_WINDOW - cause widget can be anything maybe a button or something that's why for window recognition use GTK_WINDOW
	gtk_window_set_title(GTK_WINDOW(window), "just-a-window-beginner");

	//window size - pass window and size
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

	//tell which widget to show now
	gtk_widget_show(window);	

}


//main function for command line argument - gcc + package_file_name + -o + object_file_name + file_name + again_package_file_name
int main(int argc, char** argv){
	
	//gtk application object pointer
	GtkApplication *app;
	int status;

	//creating an gtk application -- first parameter is server(first time keep it same for now, 2nd parameter flag_none as this is nto c++ that's why we need to put something over here)
	app = gtk_application_new ("org.gtk.exampleapp", G_APPLICATION_FLAGS_NONE);

	//activator of an application -- what will happen when a item is clicked
	//activation function -- activate a window
	g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);

	//running an application by passing some arguments
	status = g_application_run (G_APPLICATION (app), argc, argv);
  	
	
	g_object_unref (app);

  	return status;

}


