#include<gtk/gtk.h>


static void app_activate(GApplication *app, gpointer user_data){
	g_print("we need a filename argument\n");
}


//app_open arguments
//application --- the application (usually GtkApplication)
//files --- an array of GFiles. [array length=n_files] [element-type GFile]
//n_files --- the number of the elements of files
//hint --- a hint provided by the calling instance (usually it can be ignored)
//user_data --- user data set when the signal handler was connected.

static void app_open(GApplication *app, GFile **files, gint n_files, gchar *hint, gpointer user_data){
	GtkWidget *window;
	GtkWidget *scr;
	GtkWidget *textView;
	GtkTextBuffer *textBuffer;

	//file contents in the form of array of char and array length - gsize
	char *contents;
	gsize length;
	char *filename;


	window = gtk_application_window_new(GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOWW(window),"FIle Viewer");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
	gtk_window_maximize(GTK_WINDOW(window));

	scr = gtk_scrolled_window_new();
	gtk_window_set_child(GTK_WINDOW(window), scr);

	textView = gtk_text_view_new();
	textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_WORD_CHAR);

	//make it non-editable
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textView), FALSE);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scr), textView);



	//file contents loading
	//The function g_file_load_contents loads the file contents into a buffer, which is automatically allocated and sets contents to point that buffer. The length of the buffer is set to length. It returns TRUE if the file's contents are successfully loaded and FALSE if an error occurs.
	//If this function succeeds, it inserts the contents into GtkTextBuffer, frees the buffer pointed by contents, sets the title of the window, frees the memories pointed by filename and then shows the window. If it fails, it outputs an error message and destroys the window, causing the program to quit.
	
	if(g_file_load_contents(files[0], NULL, &contents, &length, NULL, NULL))
	{
		gtk_text_buffer_set_text(textBuffer, contents, length);
		g_free(contents);

		//set window title same as the first word of the file
		if((filename = g_file_get_basename(files[0])) != NULL){
			gtk_window_set_title(GTK_WINDOW(window), filename);
			g_free(filename);
		}
		gtk_widget_show(window);
	}
	else{
		if((filename = g_file_get_path(files[0])) != NULL){
			g_print("No such file: %s\n", filename);
			g_free(filename);
		}
		gtk_window_destroy(GTK_WINDOW(window));
	}

}




int main(int argc, char **argv){
	GtkApplication *app;
	int status;

	//G_APPLICATION_HANDLES_OPEN   means one argument is allowed and arument can be any file_name and G_APLICATION_FLAGS_NONE means no argument is allowed
	app = gtk_application_new("org.gtk.example", G_APPLICATION_HANDLES_OPEN);
	//It just outputs the error message and the application quits immediately because no window is created
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
	

	//the main functionality is in the handler app_open
	//imp role - GtkApplicationWindow, GtkScrolledWindow, GtkTextView and GtkTextBuffer 
	//Sets wrap mode to GTK_WRAP_WORD_CHAR in GtktextView
	//Sets GtkTextView to non-editable because the program isn't an editor but only a viewer
	//reads the file and inserts the text into GtkTextBuffer
	//If the file is not opened then outputs an error message and destroys the window
	g_signal_connect (app, "open", G_CALLBACK(app_open), NULL);
	status =g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
}
