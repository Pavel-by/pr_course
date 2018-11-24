#include <wchar.h>

#ifndef TEXT_STRUCTS

typedef struct
{
    wchar_t *pointer; /* Указатель на слово */
    wchar_t delimeter /* Разделитель, стоящий после слова. NULL, если следующий после слова символ был символом конца предложения или всего текста */
} Word;

typedef struct
{
    Word *words;      /* Слова, содержащиеся в предложении */
    int len;          /* Количество слов в предложении */
    wchar_t delimeter /* Разделитель предложения - символ, на который предложение заканчивается. Точка (потенциально могут появиться еще и вопрос с воскл. знаком) */
} Sentence;

typedef struct
{
    Sentence *sentences; /* Указатель на список предложений, которые есть в тексте */
    int len              /* Количество предложений */
} Text;

#endif
#define TEXT_STRUCTS 1