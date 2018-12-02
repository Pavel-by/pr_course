#ifndef TEXT

#include "text_structs.h"

Text *read_text();
void free_text(Text *text);
void free_sentence(Sentence *sentence);
void free_word(Word *word);
wchar_t *senttostr(Sentence *sentence);
int sent_eq(Sentence *sentence1, Sentence *sentence2);

#endif
#define TEXT 1