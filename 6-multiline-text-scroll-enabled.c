#include<gtk/gtk.h>

//Problem :- If you add more characters beyond the limit of the window, the height increases and the window extends. If the height gets bigger than the height of the display screen, you won't be able to control the size of the window

//solution :- GtkScrolledWindow
//Create a GtkScrolledWindow and insert it as a child of the GtkApplicationWindow; and
//Insert the GtkTextView widget to the GtkScrolledWindow as a child


static void app_activate(GApplication *app, gpointer user_data){
	GtkWidget *window;
	//GtkScrolledWindow
	GtkWidget *scr;
	
	GtkWidget *textView;
	GtkTextBuffer *textBuffer;
	gchar *text;

	text = "Sudip\n"
		"Associate Software Engineer\n"
		"Manager: Jens Ulrik Petersen\n"
		"Waiting for upcoming project\n"
		"Jens is suffering from COVID\n"
		"write something here :- "
	;
	
	//create window
	window = gtk_application_window_new(GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(window), "Multiple-Line");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);


	//creating GtkScrolledWIndow
	scr = gtk_scrolled_window_new();
	gtk_window_set_child(GTK_WINDOW(window), scr);


	//create textview
	textView = gtk_text_view_new();

	//create textbuffer connected to textview
	textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
	gtk_text_buffer_set_text(textBuffer, text, -1);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_WORD_CHAR);
	//gtk_window_set_child(GTK_WINDOW(window), textView);
	//now we don't need this line as we have to insert textview to gtkscrolledwindow
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scr), textView);
	gtk_widget_show(window);
}



int main(int argc, char **argv){
	GtkApplication *app;
	int status;
	app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
}
