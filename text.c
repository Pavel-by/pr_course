#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "text_structs.h"

#define STRING_ERA 100
#define SENTENCE_ERA 5
#define WORD_ERA 10

Text *read_text() {
    int max = STRING_ERA; //Сюда мы потом запихнем и количество предложений, и количество слов #useful_variable

    //Считываем текст
    Text *text = malloc(sizeof(text));
    (*text).pointer = malloc(sizeof(wchar_t) * max);
    (*text).len = 0;
    wchar_t c = fgetwc(stdin);
    while (c != L'\n') {
        (*text).pointer[(*text).len++] = c;
        if ((*text).len == max - 1) {
            max += STRING_ERA;
            (*text).pointer = realloc((*text).pointer, sizeof(wchar_t) * max);
        }
        c = fgetwc(stdin);
    }
    (*text).pointer[(*text).len] = L'\0';

    //Разбиваем на предложения
    int text_len = (*text).len;
    int index = 0;
    max = SENTENCE_ERA;
    (*text).sentences = malloc(sizeof(Sentence) * max);
    (*text).sentencesCount = 0;
    Sentence sentence;
    while (index < text_len) {
        sentence.pointer = (*text).pointer[index];
        sentence.len = parse_sentence_len(sentence.pointer);
        index += sentence.len;
        (*text).sentences[(*text).sentencesCount++] = sentence;

    }

    return text;
}

int parse_sentence_len(wchar_t pointer) {
    wchar_t end = wcspbrk(pointer, L".");
    if (end == NULL) return wcslen(pointer);
    return end - pointer;
}

int is_sentence_end(wchar_t c) {
    return c == L'.' || c == L'\0' ? 1 : 0;
}

int is_word_delimeter(wchar_t c) {
    return c == L',' || c == L' ' ? 1 : 0;
}
