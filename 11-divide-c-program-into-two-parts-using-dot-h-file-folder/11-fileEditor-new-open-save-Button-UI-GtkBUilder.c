#include<gtk/gtk.h>
#include "11-tfetextview.h"


static void app_activate(GApplication *app, gpointer user_data){
        g_print("we need to give a filename as an argument\n");
}


static void app_open (GApplication *app, GFile ** files, gint n_files, gchar *hint, gpointer user_data) {
	GtkWidget *win;
	GtkWidget *nb;
	GtkWidget *lab;
	GtkNotebookPage *nbp;
	GtkWidget *scr;
	GtkWidget *tv;
	GtkTextBuffer *tb;
	char *contents;
	gsize length;
	char *filename;
	int i;
	GtkBuilder *build;

	/*
	GtkWidget *boxv;
	GtkWidget *boxh;
	GtkWidget *dmy1;
	GtkWidget *dmy2;
	GtkWidget *dmy3;
	GtkWidget *btnn; // button for new 
	GtkWidget *btno; // button for open 
	GtkWidget *btns; // button for save 
	GtkWidget *btnc; // button for close 

	win = gtk_application_window_new (GTK_APPLICATION (app));
	gtk_window_set_title (GTK_WINDOW (win), "file editor");
	gtk_window_set_default_size (GTK_WINDOW (win), 600, 400);

	boxv = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_window_set_child (GTK_WINDOW (win), boxv);

	boxh = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_append (GTK_BOX (boxv), boxh);

	dmy1 = gtk_label_new(NULL); // dummy label for left space 
	gtk_label_set_width_chars (GTK_LABEL (dmy1), 10);
	dmy2 = gtk_label_new(NULL); // dummy label for center space 
	gtk_widget_set_hexpand (dmy2, TRUE);
	dmy3 = gtk_label_new(NULL); // dummy label for right space 
	gtk_label_set_width_chars (GTK_LABEL (dmy3), 10);
	btnn = gtk_button_new_with_label ("New");
	btno = gtk_button_new_with_label ("Open");
	btns = gtk_button_new_with_label ("Save");
	btnc = gtk_button_new_with_label ("Close");

	gtk_box_append (GTK_BOX (boxh), dmy1);
	gtk_box_append (GTK_BOX (boxh), btnn);
	gtk_box_append (GTK_BOX (boxh), btno);
	gtk_box_append (GTK_BOX (boxh), dmy2);
	gtk_box_append (GTK_BOX (boxh), btns);
	gtk_box_append (GTK_BOX (boxh), btnc);
	gtk_box_append (GTK_BOX (boxh), dmy3);

	nb = gtk_notebook_new ();
	gtk_widget_set_hexpand (nb, TRUE);
	gtk_widget_set_vexpand (nb, TRUE);
	gtk_box_append (GTK_BOX (boxv), nb);

	*/
	build = gtk_builder_new_from_file ("11-fileEditor-new-open-save-Button-UI-GtkBUilder.ui");
	win = GTK_WIDGET (gtk_builder_get_object (build, "win"));
	gtk_window_set_application (GTK_WINDOW (win), GTK_APPLICATION (app));
	nb = GTK_WIDGET (gtk_builder_get_object (build, "nb"));
	g_object_unref(build);

	for (i = 0; i < n_files; i++) {
		if (g_file_load_contents (files[i], NULL, &contents, &length, NULL, NULL)) {
			scr = gtk_scrolled_window_new ();
			tv = tfe_text_view_new ();
			tb = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv));
			gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv), GTK_WRAP_WORD_CHAR);
			gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scr), tv);

			tfe_text_view_set_file (TFE_TEXT_VIEW (tv),  g_file_dup (files[i]));
			gtk_text_buffer_set_text (tb, contents, length);
			g_free (contents);
			filename = g_file_get_basename (files[i]);
			lab = gtk_label_new (filename);
			gtk_notebook_append_page (GTK_NOTEBOOK (nb), scr, lab);
			nbp = gtk_notebook_get_page (GTK_NOTEBOOK (nb), scr);
			g_object_set (nbp, "tab-expand", TRUE, NULL);
			g_free (filename);
		} else if ((filename = g_file_get_path (files[i])) != NULL) {
			g_print ("No such file: %s.\n", filename);
			g_free (filename);
		} else
			g_print ("No valid file is given\n");
	}
	if (gtk_notebook_get_n_pages (GTK_NOTEBOOK (nb)) > 0) {
		gtk_widget_show (win);
	} else
		gtk_window_destroy (GTK_WINDOW (win));
}

int main(int argc, char **argv){
        GtkApplication *app;
        int status;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_HANDLES_OPEN);
        g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
        g_signal_connect (app, "open", G_CALLBACK(app_open), NULL);
        status =g_application_run(G_APPLICATION(app), argc, argv);
        g_object_unref(app);
        return status;
}


//Gtk provides GtkBuilder. It reads user interface (UI) data and builds a window. It reduces this cumbersome work.  
