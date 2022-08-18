#include<gtk/gtk.h>

int main(int argc, char *argv[]) {

	//gtk_init method will do all the needed configuration before your application starts and render on the screen
	gtk_init();

	//right now done is a boolean variable with a value 'false' and in the next part we have a loop for this reason
	gboolean done = FALSE;



	//everything u see is a widget. window, button, level are widget. [informations written there are LEVEL]  level is a widget.
	GtkWidget *window;
	//gtk_window_new creates top-level window
	window = gtk_window_new();
	//draw this window on screen using gtk_widget_show
	gtk_widget_show(window);


	//g_main_context_iteration is a method to continuously run the window until user explicitly close the application and null is context for now.    here done variable is always false
	while (!done) {
		g_main_context_iteration(NULL, TRUE);
	}

	return 0;
}
