#include<gtk/gtk.h>


//GtkTextView has a feature for editing multiple line. 
//add two things to the file viewer:
//1. Memory to store a pointer to the GFile instance.
//2. A function to write/save the file.
//There are a couple of ways to store the details of GFile. 1. Use global variables; 2. Make a child object, which can extend the instance memory for the GFile object. 
//Defining a sufficient size array of pointers to GFile. GFile *file[20];
//we will define TfeTextView as a child object of GtkTextView. It has everything that GtkTextView has. Specifically, TfeTextView has a GtkTextbuffer which corresponds to the GtkTextView inside TfeTextView. The additional important thing is that TfeTextView can also keep an additional pointer to GFile.

//TfeTextView is divided into two parts. Tfe and TextView. Tfe is called the prefix, namespace or module. TextView is called the object.
//There are three differnet identifier patterns. TfeTextView (camel case), tfe_text_view (this is used to write functions) and TFE_TEXT_VIEW (This is used to cast a pointer to point TfeTextView type).


//declaration of TfeTextView
//Defining TfeTextView widget which is the child object of GtkTextView


//define TFE_TYPE_TEXT_VIEW as tfe_text_view_get_type() 

#define TFE_TYPE_TEXT_VIEW tfe_text_view_get_type ()

//G_DECLARE_FINAL_TYPE macro. The arguments are the child object name in camel case, lower case with underscore, prefix (upper case), object (upper case with underscore) and parent object name (camel case)
G_DECLARE_FINAL_TYPE (TfeTextView, tfe_text_view, TFE, TEXT_VIEW, GtkTextView)


//Declare the structure _TfeTextView. The underscore is necessary. The first member is the parent object. Notice this is not a pointer but the object itself. The second member and after are members of the child object. TfeTextView structure has a pointer to a GFile instance as a member.
struct _TfeTextView
{
	GtkTextView parent;
	GFile *file;
};

//G_DEFINE_TYPE macro
G_DEFINE_TYPE (TfeTextView, tfe_text_view, GTK_TYPE_TEXT_VIEW);

//efine instance init function (tfe_text_view_init)
static void tfe_text_view_init (TfeTextView *tv) {}

//Define class init function (tfe_text_view_class_init)
static void tfe_text_view_class_init (TfeTextViewClass *class) {}

//two functions 1. tfe_text_view_set_file and 2. tfe_text_view_get_file
// textView is a pointer to the TfeTextView object instance which is a C-structure '_TfeTextView'. So, the structure has a member file as a pointer to a GFile instance. textView->file = file is an assignment of file to a member file of the structure pointed by textView. This is an example how to use the extended memory in a child widget.
void tfe_text_view_set_file (TfeTextView *tv, GFile *f) {
	tv -> file = f;
}

GFile *tfe_text_view_get_file (TfeTextView *tv) {
	return tv -> file;
}



//create an instance. Its name is (prefix)_(object)_new. If the parent object function needs parameters, this function also need them. You sometimes might want to add some parameters. It's your choice. Use g_object_new function to create the instance. The arguments are (prefix)_TYPE_(object), a list to initialize properties and NULL. In this code no property needs to be initialized. And the return value is casted to GtkWidget.
GtkWidget *tfe_text_view_new (void) {
	return GTK_WIDGET (g_object_new (TFE_TYPE_TEXT_VIEW, NULL));
}

//------------------defining TfeTextView is completed---------------------



//save notebook before-close
static gboolean before_close(GtkWindow *window, gpointer user_data){
	GtkWidget *nb = GTK_WIDGET(user_data);
	GtkWidget *scr;
	GtkWidget *textView;
	GFile *file;
	char *pathname;
	GtkTextBuffer *textBuffer;
	GtkTextIter start_iter;
	GtkTextIter end_iter;
	char *contents;
	unsigned int n;
	unsigned int i;

	//get the number of pages notebook has
	n = gtk_notebook_get_n_pages(GTK_NOTEBOOK(nb));
	//for loop to iterate each pages
	for(i=0; i<n; ++i){
		//get gtkScrolledWindow, TfeTextView and a pointer to GFile
		scr = gtk_notebook_get_nth_page(GTK_NOTEBOOK(nb), i);
		textView = gtk_scrolled_window_get_child(GTK_SCROLLED_WINDOW(scr));
		file = tfe_text_view_get_file(TFE_TEXT_VIEW(textView));
		
		//get GtkTextBuffer and contents. start_iter and end_iter are iterators of the buffer.
		textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
		gtk_text_buffer_get_bounds(textBuffer, &start_iter, &end_iter);
		contents = gtk_text_buffer_get_text(textBuffer, &start_iter, &end_iter, FALSE);
		//g_file_replace_contents - this function writes/saves the contents to the file. If it fails then it outputs a error msg
		if(! g_file_replace_contents(file, contents, strlen(contents), NULL, TRUE, G_FILE_CREATE_NONE, NULL, NULL, NULL)){
			pathname = g_file_get_path(file);
			g_print("ERROR: Can't save %s\n", pathname);
			g_free(pathname);
		}
		//free contents
		g_free(contents);
	}
	return FALSE;
}



static void app_activate(GApplication *app, gpointer user_data){
	g_print("we need to give a filename as an argument\n");
}


//app_open arguments
//application --- the application (usually GtkApplication)
//files --- an array of GFiles. [array length=n_files] [element-type GFile]
//n_files --- the number of the elements of files
//hint --- a hint provided by the calling instance (usually it can be ignored)
//user_data --- user data set when the signal handler was connected.

static void app_open(GApplication *app, GFile **files, gint n_files, gchar *hint, gpointer user_data){
	GtkWidget *window;
	
	//GtkNotebook
	GtkWidget *nb;

	//GtkLabel Text to store filename
	GtkWidget *label;

	GtkNotebookPage *nbp;

	GtkWidget *scr;
	GtkWidget *textView;
	GtkTextBuffer *textBuffer;

	//file contents in the form of array of char and array length - gsize
	char *contents;
	gsize length;
	char *filename;

	//Now multiple files can be displayed like file editor of linux (non-editable) - suppose 2nd time I tried to open a file then I have to manually press tab o that cuurent file can be opened unless previous file is tend to display
	int i;

	window = gtk_application_window_new(GTK_APPLICATION(app));
	//window title set manually
	gtk_window_set_title(GTK_WINDOW(window),"FIle Editor - edit and save");
	//gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
	//file viewer application size is set to maximum which is appropriate for file viewer
	gtk_window_maximize(GTK_WINDOW(window));

	//gtkapplication -> GtkNotebook -> GtkNotebookPage -> GtkScrolledWindow -> GtkTexView
	//notebook created and inserted to gtkapplication window as a child
	nb = gtk_notebook_new();
	gtk_window_set_child(GTK_WINDOW(window), nb);

	
	
	//file contents loading
	//The function g_file_load_contents loads the file contents into a buffer, which is automatically allocated and sets contents to point that buffer. The length of the buffer is set to length. It returns TRUE if the file's contents are successfully loaded and FALSE if an error occurs.
	//If this function succeeds, it inserts the contents into GtkTextBuffer, frees the buffer pointed by contents, sets the title of the window, frees the memories pointed by filename and then shows the window. If it fails, it outputs an error message and destroys the window, causing the program to quit.
	

	//each file gets their own widget and so they are created inside the for-loop
	//here i means ith argument that can be inserted as command-line-argument - means more files can be viewed simultaneously
	for(i=0; i<n_files; i++){
		if(g_file_load_contents(files[i], NULL, &contents, &length, NULL, NULL))
		{
			//scrolled window
			scr = gtk_scrolled_window_new();
			
			//textview and textbuffer in for loop
			textView = tfe_text_view_new();
			textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
			gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_WORD_CHAR);
			//editable line has been deleted
			gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scr), textView);

		
			//imp part
			tfe_text_view_set_file(TFE_TEXT_VIEW(textView), g_file_dup(files[i]));

			//insert content of the file into GtkTextBuffer and free the memory pointer by contents
			gtk_text_buffer_set_text(textBuffer, contents, length);
			g_free(contents);

			//remove if-else that was written here
			filename = g_file_get_basename(files[i]);
			label = gtk_label_new(filename);

			//appends scrolledWindow and GtkLabel into GtkNotebookPage
			gtk_notebook_append_page(GTK_NOTEBOOK(nb),scr,label);
			//gtkScrolledWindow widget is now connected to GtkNoteBookPage
			nbp = gtk_notebook_get_page(GTK_NOTEBOOK(nb), scr);
			//this g_object_set means setting properties of gtknotebookpage - like if tab-expend is truw then every file tab expands horizontally as long as possible. if FALSE then width of the tab is determined by the size of the filename/label. 
			g_object_set(nbp, "tab-expand", TRUE, NULL);

			g_free(filename);
		}
		else if((filename = g_file_get_path(files[i])) != NULL){
			//if filename cannot be read
			g_print("No such file: %s\n", filename);
			g_free(filename);
		}
		else{
			//if no filename has given in the command-line-argument
			g_print("NO valid file is given\n");
		}
	}


	//if no of filename that read is greater than 0 means at least 1 filename is given then it shows thw window otherwise destroy the window
	
	if(gtk_notebook_get_n_pages(GTK_NOTEBOOK(nb)) > 0){
		//Imagine that you are using this editor. First, you run the editor with arguments. The arguments are filenames. The editor reads the files and shows the window with the text of files in it. Then you edit the text. After you finish editing, you exit the editor. The editor updates files just before the window closes.
		//GtkWindow emits the "close-request" signal before it closes. We connect the signal and the handler before_close. A handler is a C function. When a function is connected to a certain signal, we call it a handler. The function before_close is invoked when the signal "close-request" is emitted.
		g_signal_connect(window, "close-request", G_CALLBACK(before_close), nb);
		gtk_widget_show(window);
	}
	else{
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
