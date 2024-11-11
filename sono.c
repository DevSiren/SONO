// sono.c

#include "sono.h"
#include <stdio.h>
#include <string.h>

Note notes[MAX_NOTES];
int note_count = 0;

void save_notes() {
    FILE *file = fopen("notes.csv", "w");
    if (file == NULL) {
        perror("Error");
        return;
    }
    fprintf(file, "id,title,content,tags\n");
    for (int i = 0; i < note_count; i++) {
        fprintf(file, "%d,%s,%s,%s\n", notes[i].id, notes[i].title, notes[i].content, notes[i].tags);
    }
    fclose(file);
}

void load_notes() {
    FILE *file = fopen("notes.csv", "r");
    if (file == NULL) {
        perror("Error");
        return;
    }
    char buffer[1024];
    fgets(buffer, sizeof(buffer), file);
    note_count = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%d,%99[^,],%499[^,],%99[^\n]", &notes[note_count].id, notes[note_count].title, notes[note_count].content, notes[note_count].tags);
        note_count++;
    }
    fclose(file);
}

void export_to_json() {
    json_object *jarray = json_object_new_array();
    for (int i = 0; i < note_count; i++) {
        json_object *jnote = json_object_new_object();
        json_object_object_add(jnote, "id", json_object_new_int(notes[i].id));
        json_object_object_add(jnote, "title", json_object_new_string(notes[i].title));
        json_object_object_add(jnote, "content", json_object_new_string(notes[i].content));
        json_object_object_add(jnote, "tags", json_object_new_string(notes[i].tags));
        json_object_array_add(jarray, jnote);
    }
    FILE *file = fopen("notes.json", "w");
    fprintf(file, "%s\n", json_object_to_json_string(jarray));
    fclose(file);
    json_object_put(jarray);
}

void on_save_menu_item_activate(GtkMenuItem *menuitem, gpointer data) {
    save_notes();
}

void on_load_menu_item_activate(GtkMenuItem *menuitem, gpointer data) {
    load_notes();
}

void on_export_menu_item_activate(GtkMenuItem *menuitem, gpointer data) {
    export_to_json();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "SONO");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *menu_bar = gtk_menu_bar_new();
    GtkWidget *file_menu = gtk_menu_new();
    GtkWidget *save_menu_item = gtk_menu_item_new_with_label("Save");
    GtkWidget *load_menu_item = gtk_menu_item_new_with_label("Import");
    GtkWidget *export_menu_item = gtk_menu_item_new_with_label("Export to JSON");

    g_signal_connect(save_menu_item, "activate", G_CALLBACK(on_save_menu_item_activate), NULL);
    g_signal_connect(load_menu_item, "activate", G_CALLBACK(on_load_menu_item_activate), NULL);
    g_signal_connect(export_menu_item, "activate", G_CALLBACK(on_export_menu_item_activate), NULL);

    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), load_menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), export_menu_item);

    GtkWidget *file_menu_item = gtk_menu_item_new_with_label("Файл");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu_item);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
