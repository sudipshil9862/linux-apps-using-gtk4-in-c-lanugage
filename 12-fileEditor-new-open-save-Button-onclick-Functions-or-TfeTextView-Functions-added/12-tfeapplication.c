#include "12-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions.h"


//the main file is 12-tfeappplication.c and other c files are 12-tfenotebook.c, 12-tfetextview.c and header files are also there
//12-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions.h is top header file and it includes gtk.h means #include<gtk/gtk.h> has written over there  and 12-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions.h is included in 12-tfeapplication.c and 12-tfenotebook.c


//As in previous 11 section, the 11-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions.c was a big c file, so in this 12 section, I divided this file into 12-tfeapplication.c and 12-tfenotebook.c file

/*
 tfeapplication.c includes all the code other than tfetxtview.c and tfenotebook.c. It does:
	- Application support, mainly handling command line arguments.
	- Builds widgets using ui file.
	- Connects button signals and their handlers.
	- Manages CSS.
*/


//a series of handlers correspond to the button signals

static void
open_cb (GtkNotebook *nb) {
  notebook_page_open (nb);
}

static void
new_cb (GtkNotebook *nb) {
  notebook_page_new (nb);
}

static void
save_cb (GtkNotebook *nb) {
  notebook_page_save (nb);
}

static void
close_cb (GtkNotebook *nb) {
  notebook_page_close (GTK_NOTEBOOK (nb));
}


//creates a new page and shows the window.

static void app_activate (GApplication *application) {
  GtkApplication *app = GTK_APPLICATION (application);
  GtkWidget *win = GTK_WIDGET (gtk_application_get_active_window (app));
  GtkWidget *boxv = gtk_window_get_child (GTK_WINDOW (win));
  GtkNotebook *nb = GTK_NOTEBOOK (gtk_widget_get_last_child (boxv));

  notebook_page_new (nb);
  gtk_widget_show (GTK_WIDGET (win));
}

/*
app_open:-
	- Creates notebook pages with files.
	- If no page has created, maybe because of read error, then it creates an empty page.
	- Shows the window.
These codes have become really simple thanks to tfenotebook.c and tfetextview.c.
*/

static void app_open (GApplication *application, GFile ** files, gint n_files, const gchar *hint) {
  GtkApplication *app = GTK_APPLICATION (application);
  GtkWidget *win = GTK_WIDGET (gtk_application_get_active_window (app));
  GtkWidget *boxv = gtk_window_get_child (GTK_WINDOW (win));
  GtkNotebook *nb = GTK_NOTEBOOK (gtk_widget_get_last_child (boxv));
  int i;

  for (i = 0; i < n_files; i++)
    notebook_page_new_with_file (nb, files[i]);
  if (gtk_notebook_get_n_pages (nb) == 0)
    notebook_page_new (nb);
  gtk_widget_show (win);
}


/*
 Startup signal is emitted just after the GtkApplication instance is initialized. What the signal handler needs to do is initialization of the application.
	- Builds the widgets using ui file.
	- Connects button signals and their handlers.
	- Sets CSS.

    Builds widgets using ui file. Connects the top-level window and the application with gtk_window_set_application.
	- 2Gets buttons and connects their signals and handlers.
	- Releases the reference to GtkBuilder.
	- Sets CSS. CSS in Gtk is similar to CSS in HTML. You can set margin, border, padding, color, font and so on with CSS. In this program CSS is in line 30. It sets padding, font-family and font size of GtkTextView.
	- GdkDisplay is used to set CSS. CSS will be explained in the next subsection.
*/



static void app_startup (GApplication *application) {
  GtkApplication *app = GTK_APPLICATION (application);
  GtkBuilder *build;
  GtkApplicationWindow *win;
  GtkNotebook *nb;
  GtkButton *btno;
  GtkButton *btnn;
  GtkButton *btns;
  GtkButton *btnc;

  build = gtk_builder_new_from_file ("12-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions.ui");
  win = GTK_APPLICATION_WINDOW (gtk_builder_get_object (build, "win"));
  nb = GTK_NOTEBOOK (gtk_builder_get_object (build, "nb"));
  gtk_window_set_application (GTK_WINDOW (win), app);
  btno = GTK_BUTTON (gtk_builder_get_object (build, "btno"));
  btnn = GTK_BUTTON (gtk_builder_get_object (build, "btnn"));
  btns = GTK_BUTTON (gtk_builder_get_object (build, "btns"));
  btnc = GTK_BUTTON (gtk_builder_get_object (build, "btnc"));
  g_signal_connect_swapped (btno, "clicked", G_CALLBACK (open_cb), nb);
  g_signal_connect_swapped (btnn, "clicked", G_CALLBACK (new_cb), nb);
  g_signal_connect_swapped (btns, "clicked", G_CALLBACK (save_cb), nb);
  g_signal_connect_swapped (btnc, "clicked", G_CALLBACK (close_cb), nb);
  g_object_unref(build);

GdkDisplay *display;

  display = gtk_widget_get_display (GTK_WIDGET (win));
  GtkCssProvider *provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_data (provider, "textview {padding: 10px; font-family: 'Arial'; font-size: 25pt;}", -1);
  gtk_style_context_add_provider_for_display (display, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}


/*
  Defines the application id. It is easy to find the application id, and better than the id is embedded in gtk_application_new.
	- Creates GtkApplication object.
	- Connects "startup", "activate" and "open" signals to their handlers.
	- Runs the application.
	- releases the reference to the application and returns the status.
*/


#define APPLICATION_ID "org.gtk.example"

int main (int argc, char **argv) {
  GtkApplication *app;
  int stat;

  app = gtk_application_new (APPLICATION_ID, G_APPLICATION_HANDLES_OPEN);

  g_signal_connect (app, "startup", G_CALLBACK (app_startup), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (app_activate), NULL);
  g_signal_connect (app, "open", G_CALLBACK (app_open), NULL);

  stat =g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  return stat;
}

