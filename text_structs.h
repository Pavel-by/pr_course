#include <wchar.h>

#ifndef TEXT_STRUCTS

typedef struct WordProt Word;
typedef struct SentenceProt Sentence;
typedef struct TextProt Text;

struct WordProt
{
    wchar_t delimeter; /* Разделитель, стоящий после слова. NULL, если следующий после слова символ был символом конца предложения или всего текста */

    Word *next;
    Word *prev;
    wchar_t *pointer;  /* Указатель на слово */
};

struct SentenceProt
{
    Word *first; /* Указатель на первое слово */
    wchar_t delimeter;  /* Разделитель предложения - символ, на который предложение заканчивается. Точка (потенциально могут появиться еще и вопрос с воскл. знаком) */
};

struct TextProt
{
    Sentence **sentences; /* Указатель на список предложений, которые есть в тексте */
    int len;                     /* Количество предложений */
};


#endif
#define TEXT_STRUCTS 1