#include "12-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions.h"

/* The returned string should be freed with g_free() when no longer needed. */

/*
 get_untitled function:-
	Static variable c is initialized at the first call of this function. After that c keeps its value unless it is changed explicitly.
	Increases c by one and if it is zero then it returns "Untitled". If it is a positive integer then it returns "Untitled<the integer>", for example, "Untitled1", "Untitled2", and so on. The function g_strdup_printf creates a string and it should be freed by g_free when it becomes useless. The caller of get_untitled is in charge of freeing the string.
	calls notebook_page_build to build the contents of the page.
*/

static gchar*
get_untitled () {
  static int c = -1;
  if (++c == 0) 
    return g_strdup_printf("Untitled");
  else
    return g_strdup_printf ("Untitled%u", c);
}


/*
get_current_textview:-
   This function gets the TfeTextView object belongs to the current page.
	- Gets the page number of the current page.
	- Gets the child widget scr, which is a GtkScrolledWindow instance, of the current page.
	- Gets the child widget of scr, which is a TfeTextView instance, and returns it.
*/
static TfeTextView *
get_current_textview (GtkNotebook *nb) {
  int i;
  GtkWidget *scr;
  GtkWidget *tv;

  i = gtk_notebook_get_current_page (nb);
  scr = gtk_notebook_get_nth_page (nb, i);
  tv = gtk_scrolled_window_get_child (GTK_SCROLLED_WINDOW (scr));
  return TFE_TEXT_VIEW (tv);
}


/*
 Gets the GFile instance from tv.
	- Gets the GkScrolledWindow instance which is the parent widget of tv.
	- If file points GFile, then assigns the filename of the GFile into filename. Then, unref the GFile object file.
	- Otherwise (file is NULL), assigns untitled string to filename.
	- Creates a GtkLabel instance label with the filename and set the label of the GtkNotebookPage with label. 
*/

static void
file_changed_cb (TfeTextView *tv, GtkNotebook *nb) {
  GtkWidget *scr;
  GtkWidget *label;
  GFile *file;
  char *filename;

  file = tfe_text_view_get_file (tv);
  scr = gtk_widget_get_parent (GTK_WIDGET (tv));
  if (G_IS_FILE (file)) {
    filename = g_file_get_basename (file);
    g_object_unref (file);
  } else
    filename = get_untitled ();
  label = gtk_label_new (filename);
  g_free (filename);
  gtk_notebook_set_tab_label (nb, scr, label);
}


//notebook_page_save:- Gets TfeTextView belongs to the current page. Calls tfe_text_view_save.

void
notebook_page_save (GtkNotebook *nb) {
  g_return_if_fail(GTK_IS_NOTEBOOK (nb));

  TfeTextView *tv;

  tv = get_current_textview (nb);
  tfe_text_view_save (TFE_TEXT_VIEW (tv));
}



/*
 This function closes the current page. If the page is the only page the notebook has, then the function destroys the top-level window and quits the application.
	If the page is the only page the notebook has, it calls gtk_window_destroy to destroys the top-level window.
	Otherwise, removes the current page.
*/

void notebook_page_close (GtkNotebook *nb) {
  g_return_if_fail(GTK_IS_NOTEBOOK (nb));

  GtkWidget *win;
  int i;

  if (gtk_notebook_get_n_pages (nb) == 1) {
    win = gtk_widget_get_ancestor (GTK_WIDGET (nb), GTK_TYPE_WINDOW);
    gtk_window_destroy(GTK_WINDOW (win));
  } else {
    i = gtk_notebook_get_current_page (nb);
    gtk_notebook_remove_page (GTK_NOTEBOOK (nb), i);
  }
}


/*
 notebook_page_build function:-
	- Creates GtkScrolledWindow.
	- Sets the wrap mode of tv to GTK_WRAP_WORD_CHAR so that lines are broken between words or graphemes.
	- Inserts tv to GtkscrolledWindow as a child.
	- Creates GtkLabel, then appends scr and lab to the GtkNotebook instance nb.
	- Sets "tab-expand" property to TRUE. The function g_object_set sets properties on an object. The object is any object derived from GObject. In many cases, an object has its own function to set its properties, but sometimes not. In that case, use g_object_set to set the property.
	- Sets the current page of nb to the newly created page.
	- Connects "change-file" signal and file_changed_cb handler.
*/

static void
notebook_page_build (GtkNotebook *nb, GtkWidget *tv, char *filename) {
  GtkWidget *scr = gtk_scrolled_window_new ();
  GtkNotebookPage *nbp;
  GtkWidget *lab;
  int i;

  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv), GTK_WRAP_WORD_CHAR);
  gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scr), tv);
  lab = gtk_label_new (filename);
  i = gtk_notebook_append_page (nb, scr, lab);
  nbp = gtk_notebook_get_page (nb, scr);
  g_object_set (nbp, "tab-expand", TRUE, NULL);
  gtk_notebook_set_current_page (nb, i);
  //signal connection
  g_signal_connect (GTK_TEXT_VIEW (tv), "change-file", G_CALLBACK (file_changed_cb), nb);
}


/*
 open_response handler:-
	- If the response code is NOT TFE_OPEN_RESPONSE_SUCCESS or tfe_text_view_get_file doesn't return the pointer to a GFile, it has failed to open and read a new file. Then, what notebook_page_open did in advance need to be canceled. The instance tv hasn't been a child widget of GtkScrolledWindow yet. Such instance has floating reference. Floating reference will be explained later in this subsection. You need to call g_object_ref_sink first. Then the floating reference is converted into an ordinary reference. Now you call g_object_unref to decrease the reference count by one.
	- Otherwise, everything is okay. Gets the filename, builds the contents of the page.
*/

static void open_response (TfeTextView *tv, int response, GtkNotebook *nb) {
  GFile *file;
  char *filename;

  if (response != TFE_OPEN_RESPONSE_SUCCESS || ! G_IS_FILE (file = tfe_text_view_get_file (tv))) {
    g_object_ref_sink (tv);
    g_object_unref (tv);
  }else {
    filename = g_file_get_basename (file);
    g_object_unref (file);
    notebook_page_build (nb, GTK_WIDGET (tv), filename);
  }
}


/*
 notebook_page_open function:-
	- Creates TfeTextView object. If NULL is returned, an error has happened. Then, it returns to the caller.
	- Connects the signal "open-response" and the handler open_response.
	- Calls tfe_text_view_open. The "open-response" signal will be emitted later to inform the result of opening and reading a file.
*/
void notebook_page_open (GtkNotebook *nb) {
  g_return_if_fail(GTK_IS_NOTEBOOK (nb));

  GtkWidget *tv;

  if ((tv = tfe_text_view_new ()) == NULL)
    return;
  //signal connection
  g_signal_connect (TFE_TEXT_VIEW (tv), "open-response", G_CALLBACK (open_response), nb);
  tfe_text_view_open (TFE_TEXT_VIEW (tv), GTK_WINDOW (gtk_widget_get_ancestor (GTK_WIDGET (nb), GTK_TYPE_WINDOW)));
}


//Calls tfe_text_view_new_with_file. If the function returns NULL, an error has happend. Then, it does nothing and returns.
//Gets the filename and builds the contents of the page.

void notebook_page_new_with_file (GtkNotebook *nb, GFile *file) {
  g_return_if_fail(GTK_IS_NOTEBOOK (nb));
  g_return_if_fail(G_IS_FILE (file));

  GtkWidget *tv;
  char *filename;

  if ((tv = tfe_text_view_new_with_file (file)) == NULL)
    return; /* read error */
  filename = g_file_get_basename (file);
  notebook_page_build (nb, tv, filename);
}

/*
 notebook_page_new function:-
	- g_return_if_fail is used to check the argument.
	- Creates TfeTextView object. If it fails, it returns to the caller.
	- Creates filename, which is "Untitled", "Untitled1", ... .
*/

void
notebook_page_new (GtkNotebook *nb) {
  g_return_if_fail(GTK_IS_NOTEBOOK (nb));

  GtkWidget *tv;
  char *filename;

  if ((tv = tfe_text_view_new ()) == NULL)
    return;
  filename = get_untitled ();
  notebook_page_build (nb, tv, filename);
}

