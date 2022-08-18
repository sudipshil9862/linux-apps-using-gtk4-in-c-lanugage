all: 12-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions

12-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions: 12-tfeapplication.o 12-tfenotebook.o 12-tfetextview.o
	gcc -o 12-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions 12-tfeapplication.o 12-tfetextview.o  12-tfenotebook.o `pkg-config --libs gtk4`


12-tfeapplication.o: 12-tfeapplication.c 12-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions.h
	gcc -c -o 12-tfeapplication.o `pkg-config --cflags gtk4` 12-tfeapplication.c

12-tfenotebook.o: 12-tfenotebook.c 12-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions.h
	gcc -c -o 12-tfenotebook.o `pkg-config --cflags gtk4` 12-tfenotebook.c 

12-tfetextview.o: 12-tfetextview.c 12-tfetextview.h
	gcc -c -o 12-tfetextview.o `pkg-config --cflags gtk4` 12-tfetextview.c


.Phony: clean

clean:
	rm -f 12-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions 12-fileEditor-new-open-save-Button-onclick-Functions-in-TfeTextView-Functions.o

