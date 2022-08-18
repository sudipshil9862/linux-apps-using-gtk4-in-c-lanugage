all: 11-fileEditor-new-open-save-Button-UI-GtkBUilder

11-fileEditor-new-open-save-Button-UI-GtkBUilder: 11-fileEditor-new-open-save-Button-UI-GtkBUilder.o 11-tfetextview.o
	gcc -o 11-fileEditor-new-open-save-Button-UI-GtkBUilder 11-fileEditor-new-open-save-Button-UI-GtkBUilder.o 11-tfetextview.o `pkg-config --libs gtk4`

11-fileEditor-new-open-save-Button-UI-GtkBUilder.o: 11-fileEditor-new-open-save-Button-UI-GtkBUilder.c 11-tfetextview.h
	gcc -c -o 11-fileEditor-new-open-save-Button-UI-GtkBUilder.o `pkg-config --cflags gtk4` 11-fileEditor-new-open-save-Button-UI-GtkBUilder.c
11-tfetextview.o: 11-tfetextview.c 11-tfetextview.h
	gcc -c -o 11-tfetextview.o `pkg-config --cflags gtk4` 11-tfetextview.c


.Phony: clean

clean:
	rm -f 11-fileEditor-new-open-save-Button-UI-GtkBUilder 11-fileEditor-new-open-save-Button-UI-GtkBUilder.o 11-tfetextview.o

