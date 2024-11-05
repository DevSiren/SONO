#ifndef SONO_H
#define SONO_H

#include <gtk/gtk.h>

#define MAX_NOTES 100

typedef struct {
    int id;
    char title[100];
    char content[500];
    char tags[100];
} Note;

void save_notes();
void load_notes();
void export_to_json();
void on_save_menu_item_activate(GtkMenuItem *menuitem, gpointer data);
void on_load_menu_item_activate(GtkMenuItem *menuitem, gpointer data);
void on_export_menu_item_activate(GtkMenuItem *menuitem, gpointer data);

#endif // SONO_H