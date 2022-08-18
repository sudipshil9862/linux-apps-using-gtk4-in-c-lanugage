#include<gtk/gtk.h>


static void click_cb(GtkButton *button, gpointer user_data){
        GtkWindow *window = GTK_WINDOW(user_data);
        gtk_window_destroy(window);

}


static void click_btn2(GtkButton *button, gpointer user_data){
	//GtkFontChooserDialog
	        
}


static void app_activate(GApplication *app, gpointer user_data){
	GtkWidget *win;
	GtkBuilder *build;
	GtkWidget *label;
	GtkButton *quitbtn;
	GtkDropDown *dropdown;
	GtkButton *btn2;
	static GtkCssProvider *provider;

	build = gtk_builder_new_from_file ("18-font-rendering-ui-fontchooser.ui");
	win = GTK_WIDGET (gtk_builder_get_object (build, "win"));
	gtk_window_set_application (GTK_WINDOW (win), GTK_APPLICATION (app));
	
	label = GTK_WIDGET (gtk_builder_get_object (build, "label2"));
	gtk_widget_set_name (label, "lb");
	provider = gtk_css_provider_new ();
  	GdkDisplay *display = gtk_widget_get_display (GTK_WIDGET (win));
  	gtk_css_provider_load_from_data (provider, "label#lb {background-color: red; color:white;font-family: 'Lohit Devanagari';font-size: 30px;}", -1);
  	gtk_style_context_add_provider_for_display (display, GTK_STYLE_PROVIDER (provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
	//gtk_widget_add_css_class (label, "title-4");

	//g_signal_connect (dropdown, "notify::selected-item", G_CALLBACK (selected_item_changed), item);
        //selected_item_changed (dropdown, NULL, item);


	btn2 = GTK_BUTTON (gtk_builder_get_object (build, "btn2"));
	g_signal_connect(btn2, "clicked", G_CALLBACK(click_btn2),label);
	
	
	quitbtn = GTK_BUTTON (gtk_builder_get_object (build, "btnc"));
	g_signal_connect(quitbtn, "clicked", G_CALLBACK(click_cb),win);
	g_object_unref(build);

	/*
	//textview last
	GtkWidget *textView;
	GtkTextBuffer *textBuffer;
	GdkDisplay *display;
	GdkDisplay *screen;
	GtkCssProvider *provider;
	GError *error;

	textView = gtk_text_view_new();
    	textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
    	display = gdk_display_get_default ();
    	screen = gdk_display_get_default_screen (display);
    	provider = gtk_css_provider_new();
    	gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
   	error = NULL;
    	gtk_css_provider_load_from_file (provider, g_file_new_for_path("my.css"), &error);
	gtk_container_add(GTK_CONTAINER(window), textView);
	*/
	

	gtk_widget_show (win);

}



int main(int argc, char **argv){
        GtkApplication *app;
        int status;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_HANDLES_OPEN);
        g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
        status =g_application_run(G_APPLICATION(app), argc, argv);
        g_object_unref(app);
        return status;
}



