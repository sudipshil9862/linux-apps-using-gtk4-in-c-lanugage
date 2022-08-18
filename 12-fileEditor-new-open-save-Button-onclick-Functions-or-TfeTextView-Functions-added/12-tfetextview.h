#ifndef __TFE_TEXT_VIEW_H__
#define __TFE_TEXT_VIEW_H__

#include<gtk/gtk.h>

#define TFE_TYPE_TEXT_VIEW tfe_text_view_get_type ()
G_DECLARE_FINAL_TYPE (TfeTextView, tfe_text_view, TFE, TEXT_VIEW, GtkTextView)


//"open-response" signal response
//These values are public that's why we wrote these here
//The parameter is set to TFE_OPEN_RESPONSE_SUCCESS when tfe_text_view_open has successfully opened a file and read it
//The parameter is set to TFE_OPEN_RESPONSE_CANCEL when the user has canceled.
//The parameter is set to TFE_OPEN_RESPONSE_ERROR when an error has occurred.

enum TfeTextViewOpenResponseType
{
  TFE_OPEN_RESPONSE_SUCCESS,
  TFE_OPEN_RESPONSE_CANCEL,
  TFE_OPEN_RESPONSE_ERROR
};

GFile * tfe_text_view_get_file (TfeTextView *tv);

void tfe_text_view_open (TfeTextView *tv, GtkWindow *win);

void tfe_text_view_save (TfeTextView *tv);

void tfe_text_view_saveas (TfeTextView *tv);

GtkWidget *tfe_text_view_new_with_file (GFile *file);

GtkWidget *tfe_text_view_new (void);

#endif /* __TFE_TEXT_VIEW_H__ */                                     
