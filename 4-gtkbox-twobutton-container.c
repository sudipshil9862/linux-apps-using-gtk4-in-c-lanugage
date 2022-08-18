//GtkWindow and GtkApplicationWindow can have only one child. If you want to add two or more widgets in a window, you need a container widget. GtkBox is one of the containers. It arranges two or more child widgets into a single row or column. The following procedure shows the way to add two buttons in a window.

//	Create a GtkApplicationWindow instance.
//	Create a GtkBox instance and add it to the GtkApplicationWindow as a child.
//	Create a GtkButton instance and append it to the GtkBox.
//	Create another GtkButton instance and append it to the GtkBox.


//multiple button and multiple text label using GtkBox

#include<gtk/gtk.h>



static void clicked_button(GtkButton *button1, gpointer user_data){
	//get text of button
	const gchar *s;
	s = gtk_button_get_label(button1);
	
	//string comparison
	if(g_strcmp0(s,"click me") == 0)
		//changed button text
		gtk_button_set_label(button1, "text-changed");
	else
		//back to same button text
		gtk_button_set_label(button1, "click me");
}


static void quit_button(GtkButton *button2, gpointer user_data){
	//destroy window
	GtkWindow *window = GTK_WINDOW(user_data);
	gtk_window_destroy(window);
}


static void app_activate(GApplication *app, gpointer user_data){
	GtkWidget *window; //window
	GtkWidget *box;   //GtkBox means container
	GtkWidget *button1;
	GtkWidget *button2;

	//creating window
	window = gtk_application_window_new(GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(window), "gtkbox-two-button-container");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);


	//creating box
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_set_homogeneous(GTK_BOX(box), TRUE);
	gtk_window_set_child(GTK_WINDOW(window), box);

	//button1
	button1 = gtk_button_new_with_label("click me");
	g_signal_connect(button1, "clicked",G_CALLBACK(clicked_button),NULL);

	//button2
	button2 = gtk_button_new_with_label("quit");
	g_signal_connect(button2, "clicked", G_CALLBACK(quit_button), window);


	GtkWidget *label;
	label = gtk_label_new("text label");

	gtk_box_append(GTK_BOX(box), button1);
	gtk_box_append(GTK_BOX(box), button2);
	gtk_box_append(GTK_BOX(box), label);

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
