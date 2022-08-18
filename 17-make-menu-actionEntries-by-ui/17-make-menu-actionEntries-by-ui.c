#include <gtk/gtk.h>

static void new_activated (GSimpleAction *action, GVariant *parameter, gpointer win) {
}

static void open_activated (GSimpleAction *action, GVariant *parameter, gpointer win) {
}

static void save_activated (GSimpleAction *action, GVariant *parameter, gpointer win) {
}

static void saveas_activated (GSimpleAction *action, GVariant *parameter, gpointer win) {
}

static void close_activated (GSimpleAction *action, GVariant *parameter, gpointer win) {
}

static void cut_activated (GSimpleAction *action, GVariant *parameter, gpointer win) {
}

static void copy_activated (GSimpleAction *action, GVariant *parameter, gpointer win) {
}

static void paste_activated (GSimpleAction *action, GVariant *parameter, gpointer win) {
}

static void selectall_activated (GSimpleAction *action, GVariant *parameter, gpointer win) {
}

static void fullscreen_changed (GSimpleAction *action, GVariant *state, gpointer win) {
  if (g_variant_get_boolean (state))
    gtk_window_maximize (GTK_WINDOW (win));
  else
    gtk_window_unmaximize (GTK_WINDOW (win));
  g_simple_action_set_state (action, state);
}

static void quit_activated (GSimpleAction *action, GVariant *parameter, gpointer app)
{
  g_application_quit (G_APPLICATION(app));
}

static void app_activate (GApplication *app, gpointer user_data) {
  GtkWidget *win = gtk_application_window_new (GTK_APPLICATION (app));

  const GActionEntry win_entries[] = {
    { "new", new_activated, NULL, NULL, NULL },
    { "open", open_activated, NULL, NULL, NULL },
    { "save", save_activated, NULL, NULL, NULL },
    { "saveas", saveas_activated, NULL, NULL, NULL },
    { "close", close_activated, NULL, NULL, NULL },
    { "cut", cut_activated, NULL, NULL, NULL },
    { "copy", copy_activated, NULL, NULL, NULL },
    { "paste", paste_activated, NULL, NULL, NULL },
    { "selectall", selectall_activated, NULL, NULL, NULL },
    { "fullscreen", NULL, NULL, "false", fullscreen_changed }
  };
  g_action_map_add_action_entries (G_ACTION_MAP (win), win_entries, G_N_ELEMENTS (win_entries), win);

  gtk_application_window_set_show_menubar (GTK_APPLICATION_WINDOW (win), TRUE);

  gtk_window_set_title (GTK_WINDOW (win), "menu3");
  gtk_window_set_default_size (GTK_WINDOW (win), 400, 300);
  gtk_widget_show (win);
}

static void app_startup (GApplication *app, gpointer user_data) {
  GtkBuilder *builder = gtk_builder_new_from_file ("17-make-menu-actionEntries-by-ui.ui");
  GMenuModel *menubar = G_MENU_MODEL (gtk_builder_get_object (builder, "menubar"));

  gtk_application_set_menubar (GTK_APPLICATION (app), menubar);
  g_object_unref (builder);

  const GActionEntry app_entries[] = {
    { "quit", quit_activated, NULL, NULL, NULL }
  };
  g_action_map_add_action_entries (G_ACTION_MAP (app), app_entries, G_N_ELEMENTS (app_entries), app);
}

#define APPLICATION_ID "org.gtk.example"

int main (int argc, char **argv) {
  GtkApplication *app;
  int stat;

  app = gtk_application_new (APPLICATION_ID, G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "startup", G_CALLBACK (app_startup), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (app_activate), NULL);

  stat =g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  return stat;
}

