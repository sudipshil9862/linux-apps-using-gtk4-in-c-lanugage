all: 13-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions

13-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions: 13-tfeapplication.o 13-tfenotebook.o 13-tfetextview.o 13-css.o
	gcc -o 13-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions 13-tfeapplication.o 13-tfetextview.o  13-tfenotebook.o 13-css.o `pkg-config --libs gtk4`


13-tfeapplication.o: 13-tfeapplication.c 13-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions.h
	gcc -c -o 13-tfeapplication.o `pkg-config --cflags gtk4` 13-tfeapplication.c

13-tfenotebook.o: 13-tfenotebook.c 13-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions.h
	gcc -c -o 13-tfenotebook.o `pkg-config --cflags gtk4` 13-tfenotebook.c 

13-tfetextview.o: 13-tfetextview.c 13-tfetextview.h
	gcc -c -o 13-tfetextview.o `pkg-config --cflags gtk4` 13-tfetextview.c

13-css.o: 13-css.c 13-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions.h
	gcc -c -o 13-css.o `pkg-config --cflags gtk4` 13-css.c

.Phony: clean

clean:
	rm -f 13-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions 13-tfeapplication.o 13-tfenotebook.o 13-tfetextview.o 13-css.o

