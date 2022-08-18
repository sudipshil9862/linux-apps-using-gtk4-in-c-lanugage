//GtkNotebook is a very important object in the text file editor tfe. It connects the application and TfeTextView objects. A set of public functions are declared in tfenotebook.h. The word "tfenotebook" is used only in filenames. There's no "TfeNotebook" object.
/*
 This header file describes the public functions in tfenotebook.c:-
	- notebook_page_save saves the current page to the file of which the name specified in the tab. If the name is untitled or untitled followed by digits, FileChooserDialog appears and a user can choose or specify a filename.
	- notebook_page_close closes the current page.
	- notebook_page_open shows a file chooser dialog and a user can choose a file. The file is inserted to a new page.
	- notebook_page_new_with_file creates a new page and the file given as an argument is read and inserted into the page.
	- notebook_page_new creates a new empty page.
Y
ou probably find that the functions except notebook_page_close are higher level functions of
	tfe_text_view_save
	tef_text_view_open
	tfe_text_view_new_with_file
	tfe_text_view_new
*/

void
notebook_page_save(GtkNotebook *nb);

void
notebook_page_close (GtkNotebook *nb);

void
notebook_page_open (GtkNotebook *nb);

void
notebook_page_new_with_file (GtkNotebook *nb, GFile *file);

void
notebook_page_new (GtkNotebook *nb);

