#include <string.h>
#include "12-tfetextview.h"

//The public functions in 12-tfetextview.c are declared in 12-tfetextview.h file

//structure of TfeTextView
//The type of parent is GtkTextView which is C structure. It is declared in gtktextview.h. GtkTextView is the parent of TfeTextView.
//file is a pointer to GFile. It can be NULL if no file corresponds to the TfeTextView object.


//TfeTextView structure has GtkTextView type as the first member. In the same way, GtkTextView has its parent type (GtkWidget) as the first member. GtkWidget has its parent type (GtkInitiallyUnowned) as the first member. The structure of GtkInitiallyUnowned is the same as GObject. Therefore, TFeTextView includes GObject, GtkWidget and GtkTextView in itself.

//GObject -- GInitiallyUnowned -- GtkWidget -- GtkTextView -- TfeTextView


struct _TfeTextView {
  GtkTextView parent;
  GFile *file;
};

G_DEFINE_TYPE (TfeTextView, tfe_text_view, GTK_TYPE_TEXT_VIEW);

/* In TfeTextView, two signals are registered:- 
	"change-file" signal. This signal is emitted when tv->file is changed.
	"open-response" signal. tfe_text_view_open function is not able to return the status because it uses GtkFileChooserDialog. This signal is emitted instead of the return value of the function.
	Third part of anum is the array described in the next line
*/

enum {
  CHANGE_FILE,
  OPEN_RESPONSE,
  NUMBER_OF_SIGNALS
};

//A static variable or array is used to store the signal ID. A static array is used to register two or more signals.
static guint tfe_text_view_signals[NUMBER_OF_SIGNALS];


//In the destruction process of TfeTextView, the reference count of widgets related to TfeTextView is automatically decreased. But GFile pointed by tv->file needs to decrease its reference count by one. You must write the code in the dispose handler tfe_text_view_dispose.
//If tv->file points a GFile, decrease its reference count. g_clear_object decreases the reference count and assigns NULL to tv->file. In dispose handlers, we usually use g_clear_object rather than g_object_unref.

static void tfe_text_view_dispose (GObject *gobject) {
  TfeTextView *tv = TFE_TEXT_VIEW (gobject);

  if (G_IS_FILE (tv->file))
    g_clear_object (&tv->file);

  G_OBJECT_CLASS (tfe_text_view_parent_class)->dispose (gobject);
}

//Initializes TfeTextView part in TfeTextView instance. This function just initializes tv->file to be NULL.
static void tfe_text_view_init (TfeTextView *tv) {
  tv->file = NULL;
}

//In the disposing process, the object uses the pointer in its class to call the handler. Therefore, tfe_text_view_dispose needs to be registered in the class when the TfeTextView class is initialized. The function tfe_text_view_class_init is the class initialization function

/*Signals are registered in the class initialization function.
	- Registers "change-file" signal. g_signal_new function is used. The signal "change-file" has no default handler (object method handler). You usually don't need to set a default handler. If you need it, use g_signal_new_class_handler function. See GObject API Reference, g_signal_new_class_handler for further information.
	- The return value of g_signal_new is the signal id. The type of signal id is guint, which is the same as unsigned int. It is used in the function g_signal_emit.
	- Registers "open-response" signal. This signal has a parameter.
	- Number of the parameters. "open-response" signal has one parameter.
*/



static void tfe_text_view_class_init (TfeTextViewClass *class) {
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->dispose = tfe_text_view_dispose;
  tfe_text_view_signals[CHANGE_FILE] = g_signal_new ("change-file",
                                 G_TYPE_FROM_CLASS (class),
                                 G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                                 0 /* class offset */,
                                 NULL /* accumulator */,
                                 NULL /* accumulator data */,
                                 NULL /* C marshaller */,
                                 G_TYPE_NONE /* return_type */,
                                 0     /* n_params */
                                 );
  tfe_text_view_signals[OPEN_RESPONSE] = g_signal_new ("open-response",
                                 G_TYPE_FROM_CLASS (class),
                                 G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                                 0 /* class offset */,
                                 NULL /* accumulator */,
                                 NULL /* accumulator data */,
                                 NULL /* C marshaller */,
                                 G_TYPE_NONE /* return_type */,
                                 1     /* n_params */,
                                 G_TYPE_INT
                                 );
}


//gtk_text_view_get_file is a simple function
//The important thing is to duplicate tv->file. Otherwise, if the caller frees the GFile object, tv->file is no more guaranteed to point the GFile. Another reason to use g_file_dup is that GFile isn't thread-safe. If you use GFile in the different thread, the duplication is necessary.

GFile *tfe_text_view_get_file (TfeTextView *tv) {
  g_return_val_if_fail (TFE_IS_TEXT_VIEW (tv), NULL);

  if (G_IS_FILE (tv->file))
    return g_file_dup (tv->file);
  else
    return NULL;
}


/*
 save_file function:- 
 	This function is called from saveas_dialog_response and tfe_text_view_save. This function saves the contents of the buffer to the file given as an argument. If error happens, it displays an error message. The class of this function is static. Therefore, only functions in this file (tfeTetview.c) call this function. Such static functions usally don't have g_return_val_if_fail function.
	
	- Gets the text contents from the buffer.
	- Saves the contents to the file. If no error happens, set the modified flag to be FALSE. This means that the buffer is not modified since it has been saved. And set the return status stat to be TRUE.
	- If it fails to save the contents, displays an error message.
	- Creates a message dialog with the error message.
	- Connects the "response" signal to gtk_window_destroy, so that the dialog disappears when a user clicked on the button.
	- Shows the window, frees err and set stat to be FLASE.
	- Frees contents.
	- Returns to the caller.
	- saveas_dialog_response function. This is a signal handler for the "response" signal on GtkFileChooserDialog instance created by tfe_text_view_saveas function. This handler analyzes the response and determines whether to save the contents.
	- If the response is GTK_RESPONSE_ACCEPT, the user has clicked on the Save button. So, it tries to save.
	- Gets the GFile file from GtkFileChooserDialog.
	- If it doesn't point GFile, it outputs an error message to the log.
	- Otherwise, it calls save_file to save the contents to the file.
	- If save_file has successfully saved the contents, tv->file is updated. If the old GFile pointed by tv->file exists, it is freed in advance. Emits "change-file" signal.
	- Unrefs file.
	- destroys the file chooser dialog.
	- tfe_text_view_save function.
	- tfe_text_view_save is public, i.e. it is open to the other files. So, it doesn't have static class. Public functions should check the parameter type with g_return_if_fail function. If tv is not a pointer to a TfeTextView instance, then it logs an error message and immediately returns. This function is similar to g_return_val_if_fail, but no value is returned because tfe_text_view_save doesn't return a value.
	- Gets GtkTextBuffer instance and GtkWidget instance and assignes them to tb andwin respectively.
	- If the buffer hasn't modified, then it doesn't need to save it. So the function returns.
	- If tv->file is NULL, no file has given yet. It calls tfe_text_view_saveas which prompts a user to select a file or specify a new file to save.
	- If tv->file doesn't point GFile, somethig bad has happened. Logs an error message.
	- Calls save_file to save the contents to the file.
	- tfe_text_view_saveas function. It shows GtkFileChooserDialog and prompts the user to choose a file.
	- Creates GtkFileChooserDialog. The title is "Save file". Transient parent of the dialog is win, which is the top-level window. The action is save mode. The buttons are Cancel and Save.
	- connects the "response" signal of the dialog and saveas_dialog_response handler.
	- Shows the dialog.
*/
//Generate FileChooserDialog -> User Chooser afile (GTkFileChooserDialog) ->[signal]-> save the file(saveas_dialog_response)

static gboolean save_file (GFile *file, GtkTextBuffer *tb, GtkWindow *win) {
  GtkTextIter start_iter;
  GtkTextIter end_iter;
  gchar *contents;
  gboolean stat;
  GtkWidget *message_dialog;
  GError *err = NULL;

  gtk_text_buffer_get_bounds (tb, &start_iter, &end_iter);
  contents = gtk_text_buffer_get_text (tb, &start_iter, &end_iter, FALSE);
  if (g_file_replace_contents (file, contents, strlen (contents), NULL, TRUE, G_FILE_CREATE_NONE, NULL, NULL, &err)) {
    gtk_text_buffer_set_modified (tb, FALSE);
    stat = TRUE;
  } else {
    message_dialog = gtk_message_dialog_new (win, GTK_DIALOG_MODAL,
                                             GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
                                            "%s.\n", err->message);
    g_signal_connect (message_dialog, "response", G_CALLBACK (gtk_window_destroy), NULL);
    gtk_widget_show (message_dialog);
    g_error_free (err);
    stat = FALSE;
  }
  g_free (contents);
  return stat;
}

static void saveas_dialog_response (GtkWidget *dialog, gint response, TfeTextView *tv) {
  GtkTextBuffer *tb = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv));
  GFile *file;
  GtkWidget *win = gtk_widget_get_ancestor (GTK_WIDGET (tv), GTK_TYPE_WINDOW);

  if (response == GTK_RESPONSE_ACCEPT) {
    file = gtk_file_chooser_get_file (GTK_FILE_CHOOSER (dialog));
    if (! G_IS_FILE (file))
      g_warning ("TfeTextView: gtk_file_chooser_get_file returns non GFile.\n");
    else if (save_file(file, tb, GTK_WINDOW (win))) {
      if (G_IS_FILE (tv->file))
        g_object_unref (tv->file);
      tv->file = file;
      /*
	g_signal_emit:-
		- The first argument is the instance on which the signal is emitted.
		- The second argument is the signal id.
		- The third argument is the detail of the signal. "change-file" signal and "open-response" signal doesn't have details and the argument is zero when no details.
		- "change-file" signal doesn't have parameter, so there's no fourth parameter.
		- "open-response" signal has one parameter. The fourth parameter is the parameter.
      */
      g_signal_emit (tv, tfe_text_view_signals[CHANGE_FILE], 0);
    } else
      g_object_unref (file);
  }
  gtk_window_destroy (GTK_WINDOW (dialog));
}


//Save and saveas functions write the contents in the GtkTextBuffer to a file to a file specified by tv->file. If tv->file is NULL, then it shows GtkFileChooserDialog and prompts the user to choose a file to save.

void tfe_text_view_save (TfeTextView *tv) {
  g_return_if_fail (TFE_IS_TEXT_VIEW (tv));

  GtkTextBuffer *tb = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv));
  GtkWidget *win = gtk_widget_get_ancestor (GTK_WIDGET (tv), GTK_TYPE_WINDOW);

  if (! gtk_text_buffer_get_modified (tb))
    return; /* no need to save it */
  else if (tv->file == NULL)
    tfe_text_view_saveas (tv);
  else if (! G_IS_FILE (tv->file))
    g_error ("TfeTextView: The pointer tv->file isn't NULL nor GFile.\n");
  else
    save_file (tv->file, tb, GTK_WINDOW (win));
}

//After tfe_text_view_save, it saves the contents to the file and sets tv->file to point the GFile instance for the file
//The function saveas uses GtkFileChooserDialog and prompts the user to select a existed file or specify a new file to save. Then, the function changes tv->file and save the contents to the specified file. If an error occurs, it is shown to the user through the message dialog. The error is managed only in the TfeTextView and no information is notified to the caller.

//working of GtkFileChooser:- When you use GtkFileChooserDialog, you need to divide the program into two parts. One is a function which creates GtkFileChooserDialog and the other is a signal handler. The function just creates and shows GtkFileChooserDialog. The rest is done by the handler. It gets Gfile from GtkFileChooserDialog and saves the buffer to the file by calling save_file.

void tfe_text_view_saveas (TfeTextView *tv) {
  g_return_if_fail (TFE_IS_TEXT_VIEW (tv));

  GtkWidget *dialog;
  GtkWidget *win = gtk_widget_get_ancestor (GTK_WIDGET (tv), GTK_TYPE_WINDOW);

  dialog = gtk_file_chooser_dialog_new ("Save file", GTK_WINDOW (win), GTK_FILE_CHOOSER_ACTION_SAVE,
                                      "Cancel", GTK_RESPONSE_CANCEL,
                                      "Save", GTK_RESPONSE_ACCEPT,
                                      NULL);
  g_signal_connect (dialog, "response", G_CALLBACK (saveas_dialog_response), tv);
  gtk_widget_show (dialog);
}

//tfe_text_view_new just creates a new TfeTextView instance and returns the pointer to the new instance
//tfe_text_view_new_with_file is given a Gfile object as an argument and it loads the file into the GtkTextBuffer instance, then returns the pointer to the new instance. If an error occurs during the creation process, NULL is returned

/*
 * Description of tfe_text_view_new_with_file:-
 	- tfe_text_view_new function. Just returns the value from the function g_object_new but casts it to the pointer to GtkWidget. Initialization is done in tfe_text_view_init which is called in the process of g_object_new function.
	- tfe_text_view_new_with_file function.
	- g_return_val_if_fail is described in GLib API Reference, g_return_val_if_fail. And also GLib API Reference, Message Logging. It tests whether the argument file is a pointer to GFile. If it's true, then the program goes on to the next line. If it's false, then it returns NULL (the second argument) immediately. And at the same time it logs out the error message (usually the log is outputted to stderr or stdout). This function is used to check the programmer's error. If an error occurs, the solution is usually to change the (caller) program and fix the bug. You need to distinguish programmer's errors and runtime errors. You shouldn't use this function to find runtime errors.
	- If an error occurs when reading the file, then the function returns NULL.
	- Calls the function tfe_text_view_new. The function creates TfeTextView instance and returns the pointer to the instance. If an error happens in tfe_text_view_new, it returns NULL.
	- Gets the pointer to GtkTextBuffer corresponds to tv. The pointer is assigned to tb
	- Assigns the contents read from the file to GtkTextBuffer pointed by tb.
	- Duplicates file and sets tv->file to point it.
	- The function gtk_text_buffer_set_modified (tb, FALSE) sets the modification flag of tb to FALSE. The modification flag indicates that the contents of the buffer is modified. It is used when the contents are saved. If the modification flag is FALSE, it doesn't need to save the contents.
	- Frees the memories pointed by contents.
	- Returns tv, which is a pointer to the newly created TfeTextView instance. If an error happens, NULL is returned
*/

GtkWidget *tfe_text_view_new_with_file (GFile *file) {
  g_return_val_if_fail (G_IS_FILE (file), NULL);

  GtkWidget *tv;
  GtkTextBuffer *tb;
  char *contents;
  gsize length;

  if (! g_file_load_contents (file, NULL, &contents, &length, NULL, NULL)) /* read error */
    return NULL;

  if ((tv = tfe_text_view_new()) != NULL) {
    tb = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv));
    gtk_text_buffer_set_text (tb, contents, length);
    TFE_TEXT_VIEW (tv)->file = g_file_dup (file);
    gtk_text_buffer_set_modified (tb, FALSE);
  }
  g_free (contents);
  return tv;
}

static void open_dialog_response(GtkWidget *dialog, gint response, TfeTextView *tv) {
  GtkTextBuffer *tb = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv));
  GFile *file;
  char *contents;
  gsize length;
  GtkWidget *message_dialog;
  GError *err = NULL;
 	/*
	g_signal_emit:-
		- The first argument is the instance on which the signal is emitted.
		- The second argument is the signal id.
		- The third argument is the detail of the signal. "change-file" signal and "open-response" signal doesn't have details and the argument is zero when no details.
		- "change-file" signal doesn't have parameter, so there's no fourth parameter.
		- "open-response" signal has one parameter. The fourth parameter is the parameter.
      	*/

  if (response != GTK_RESPONSE_ACCEPT)
    g_signal_emit (tv, tfe_text_view_signals[OPEN_RESPONSE], 0, TFE_OPEN_RESPONSE_CANCEL);
  else if (! G_IS_FILE (file = gtk_file_chooser_get_file (GTK_FILE_CHOOSER (dialog)))) {
    g_warning ("TfeTextView: gtk_file_chooser_get_file returns non GFile.\n");
    g_signal_emit (tv, tfe_text_view_signals[OPEN_RESPONSE], 0, TFE_OPEN_RESPONSE_ERROR);
  } else if (! g_file_load_contents (file, NULL, &contents, &length, NULL, &err)) { /* read error */
    g_object_unref (file);
    message_dialog = gtk_message_dialog_new (GTK_WINDOW (dialog), GTK_DIALOG_MODAL,
                                             GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
                                            "%s.\n", err->message);
    g_signal_connect (message_dialog, "response", G_CALLBACK (gtk_window_destroy), NULL);
    gtk_widget_show (message_dialog);
    g_error_free (err);
    g_signal_emit (tv, tfe_text_view_signals[OPEN_RESPONSE], 0, TFE_OPEN_RESPONSE_ERROR);
  } else {
    gtk_text_buffer_set_text (tb, contents, length);
    g_free (contents);
    if (G_IS_FILE (tv->file))
      g_object_unref (tv->file);
    tv->file = file;
    gtk_text_buffer_set_modified (tb, FALSE);
    g_signal_emit (tv, tfe_text_view_signals[OPEN_RESPONSE], 0, TFE_OPEN_RESPONSE_SUCCESS);
    g_signal_emit (tv, tfe_text_view_signals[CHANGE_FILE], 0);
  }
  gtk_window_destroy (GTK_WINDOW (dialog));
}



//Open function shows GtkFileChooserDialog to users and prompts them to choose a file. Then it reads the file and puts the text into GtkTextBuffer.
//The parameter win is the top-level window. It will be a transient parent window of GtkFileChooserDialog when the dialog is created. This allows window managers to keep the dialog on top of the parent window, or center the dialog over the parent window. It is possible to give no parent window to the dialog. However, it is encouraged to give a parent window to dialog. This function might be called just after tv has been created. In that case, tv has not been incorporated into the widget hierarchy. Therefore it is impossible to get the top-level window from tv. That's why the function needs win parameter.

//This function is usually called when the buffer of tv is empty. However, even if the buffer is not empty, tfe_text_view_open doesn't treat it as an error. If you want to revert the buffer, calling this function is appropriate. Otherwise probably bad things will happen.

/*
 tfe_text_view_open function:-
	- Creates GtkFileChooserDialog. The title is "Open file". Transient parent window is the top-level window of the application, which is given by the caller. The action is open mode. The buttons are Cancel and Open.
	- connects the "response" signal of the dialog and open_dialog_response signal handler.
	- Shows the dialog.
	- open_dialog_response signal handler.
	- If the response from GtkFileChooserDialog is not GTK_RESPONSE_ACCEPT, the user has clicked on the "Cancel" button or close button on the header bar. Then, "open-response" signal is emitted. The parameter of the signal is TFE_OPEN_RESPONSE_CANCEL.
	- Gets the pointer to the Gfile by gtk_file_chooser_get_file. If it doesn't point GFile, maybe an error has occurred. Then it emits "open-response" signal with the parameter TFE_OPEN_RESPONSE_ERROR.
	- If an error occurs at file reading, then it decreases the reference count of the Gfile, shows a message dialog to report the error to the user and emits "open-response" signal with the parameter TFE_OPEN_RESPONSE_ERROR.
	- If the file has successfully been read, then the text is inserted to GtkTextBuffer, frees the temporary buffer pointed by contents and sets tv->file to point the file (no duplication is not necessary). Then, it emits "open-response" signal with the parameter TFE_OPEN_RESPONSE_SUCCESS and emits "change-file" signal.
	- destroys GtkFileCooserDialog.
*/


void tfe_text_view_open (TfeTextView *tv, GtkWindow *win) {
  g_return_if_fail (TFE_IS_TEXT_VIEW (tv));
  g_return_if_fail (GTK_IS_WINDOW (win));

  GtkWidget *dialog;

  dialog = gtk_file_chooser_dialog_new ("Open file", win, GTK_FILE_CHOOSER_ACTION_OPEN,
                                        "Cancel", GTK_RESPONSE_CANCEL,
                                        "Open", GTK_RESPONSE_ACCEPT,
                                        NULL);
  g_signal_connect (dialog, "response", G_CALLBACK (open_dialog_response), tv);
  gtk_widget_show (dialog);
}



//The memory allocated with tfe_text_view_new is an instance of TfeTextView object. Therefore, There can be multiple TfeTextView instances if tfe_text_view_new is called multiple times.
//The function tfe_text_view_new creates a new TfeTextView instance.
/*Initialization process :- 
   - initiates GObject(GInitiallyUnowned) part in TfeTextView instance
   - Initializes GtkWidget part in TfeTextView instance.
   - Initializes GtkTextView part in TfeTextView instance.
   - Initializes TfeTextView part in TfeTextView instance.
  [The step one through three is done by g_object_init, gtk_widget_init and gtk_text_view_init. They are called by the system automatically and you don't need to care about them. Step four is done by the function tfe_text_view_init in tfetextview.c.]
*/  
//summary :- TfeTextView -> GTkTextView -> GtkWidget -> GInitiallyUnowned -> GObject -> [GTYpeInstance, guint, GData, GtkWidgetPrivate, GTkTextViewPrivate, GFile]
 
//A TfeTextView instance is created with tfe_text_view_new or tfe_text_view_new_with_file

GtkWidget *tfe_text_view_new (void) {
  return GTK_WIDGET (g_object_new (TFE_TYPE_TEXT_VIEW, NULL));
}

