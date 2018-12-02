#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include "text_structs.h"
#include "colors.h"

#define STRING_ERA 10
#define TEXT_ERA 5
#define WORD_ERA 5

void free_word(Word *word)
{
    free(word->pointer);
    free(word);
}

void free_sentence(Sentence *sentence)
{
    Word *word = sentence->first, *lastWord = NULL;
    while (word != NULL)
    {
        if (lastWord != NULL)
        {
            free(lastWord);
            lastWord = NULL;
        }
        free(word->pointer);
        lastWord = word;
        word = lastWord->next;
    }
    if (lastWord != NULL)
        free(lastWord);
    free(sentence);
}

void free_text(Text *text)
{
    for (int i = 0; i < (*text).len; i++)
    {
        free_sentence((*text).sentences[i]);
    }
    free((*text).sentences);
    free(text);
}

int is_sentence_end(wchar_t c)
{
    return c == L'.' || c == L'\0' || c == L'\n' ? 1 : 0;
}

int is_text_end(wchar_t c)
{
    return c == L'\n' ? 1 : 0;
}

int is_word_delimeter(wchar_t c)
{
    return c == L',' || c == L' ' ? 1 : 0;
}

Text *read_text()
{
    wprintf(L"%lsВведите текст: \n%ls", COLOR_GREEN, COLOR_NORMAL);

    int textMaxLen = TEXT_ERA;
    Text *text = malloc(sizeof(Text));
    (*text).len = 0;
    (*text).sentences = malloc(sizeof(Sentence *) * textMaxLen);

    Word *word = NULL, *lastWord = NULL;
    int wordMaxLen, wordLen;

    Sentence *sentence = NULL;

    Word *tempWord;
    Sentence *tempSentence;
    Sentence **tempSentenceArray;
    wchar_t *tempString;

    wchar_t c = fgetwc(stdin);

    /*
        Считываем в цикле посимвольно из входного потока. 
        Если word == null - создаем новое слово. Это может быть в 3-х случаях:
        1. Первый заход
        2. Следующее слово в предложении (тут надо установить зависимости между словами)
        3. Первое слово в предложении: просто создаем слово
        Если sencence == null - создаем предложение. Дальше все просто.
    */
    while (!is_text_end(c))
    {

        if (word == NULL)
        {
            //Если нужно создать новое слово
            wordLen = 0;
            wordMaxLen = WORD_ERA;

            tempWord = malloc(sizeof(Word)); //Выделяем память под слово
            if (tempWord == NULL)
            {
                free_text(text);
                return NULL;
            }
            word = tempWord;
            tempString = malloc(sizeof(wchar_t) * wordMaxLen); //выделяем память под строку в слове
            if (tempString == NULL)
            {
                free_text(text);
                return NULL;
            }
            word->pointer = tempString;
            word->delimeter = 0;
            (*word).prev = NULL;
            word->next = NULL;
            if (lastWord != NULL)
            {
                lastWord->next = word;
                word->prev = lastWord;
            }
        }

        if (sentence == NULL)
        {
            //Создаем новое предложение
            sentence = malloc(sizeof(Sentence));
            (*sentence).first = word;
            (*sentence).delimeter = 0;
            (*text).sentences[(*text).len++] = sentence;
        }

        if (is_sentence_end(c))
        {
            //если встретили конец предложения
            (*word).pointer[wordLen++] = L'\0';

            word = NULL;
            lastWord = NULL;

            if (!is_text_end(c))
                (*sentence).delimeter = c;
            sentence = NULL; // Обнуляем указатель, чтобы создать новое предложение на следующем шаге
        }
        else if (is_word_delimeter(c))
        {
            (*word).delimeter = c;
            (*word).pointer[wordLen++] = L'\0';
            lastWord = word;
            word = NULL;
        }
        else
        {
            ((*word).pointer)[wordLen++] = c;
            (word->pointer)[wordLen] = L'\0';
        }

        if (wordLen == wordMaxLen - 1 && word != NULL)
        {
            wordMaxLen += WORD_ERA;
            tempString = realloc((*word).pointer, sizeof(wchar_t) * wordMaxLen);
            if (tempString == NULL)
            {
                free_text(text);
                return NULL;
            }
            (*word).pointer = tempString;
        }
        if ((*text).len == textMaxLen && sentence != NULL)
        {
            textMaxLen += TEXT_ERA;
            tempSentenceArray = realloc((*text).sentences, sizeof(Sentence *) * textMaxLen);
            if (tempSentenceArray == NULL)
            {
                free_text(text);
                return NULL;
            }
            (*text).sentences = tempSentenceArray;
        }
        c = fgetwc(stdin);
    }
    return text;
}

wchar_t *senttostr(Sentence *sentence)
{
    int max = STRING_ERA;
    wchar_t *text = malloc(sizeof(wchar_t) * max);
    if (text == NULL)
        return NULL;
    text[0] = L'\0';
    void *temp;
    Word *word = sentence->first;
    while (word != NULL)
    {
        int len = wcslen(text);
        int wordLen = wcslen(word->pointer);
        if (word->delimeter != 0)
            wordLen++;
        if (len + wordLen >= max - 1)
        {
            while (len + wordLen >= max - 1)
            {
                max += STRING_ERA;
            }
            temp = realloc(text, sizeof(wchar_t) * max);
            if (temp == NULL)
            {
                free(text);
                return NULL;
            }
            text = temp;
        }
        wcscat(text, (*word).pointer);
        if ((*word).delimeter != 0)
        {
            len = wcslen(text);
            text[len++] = (*word).delimeter;
            text[len] = L'\0';
        }
        word = word->next;
    }
    if (sentence->delimeter != 0)
    {
        int len = wcslen(text);
        if (len >= max - 1)
        {
            temp = realloc(text, sizeof(wchar_t) * ++max);
            if (temp == NULL)
            {
                free(text);
                return NULL;
            }
            text = temp;
        }
        text[len] = sentence->delimeter;
        text[len + 1] = L'\0';
    }
    return text;
}

wchar_t *wcslwr(wchar_t *s)
{
    int len = wcslen(s);
    wchar_t *result = malloc(sizeof(wchar_t) * (len + 1));
    for (int i = 0; i < len; i++)
    {
        result[i] = towlower(s[i]);
    }
    result[len] = '\0';
    return result;
}

int sent_eq(Sentence *sent1, Sentence *sent2)
{
    Word *w1, *w2;
    w1 = sent1->first;
    w2 = sent2->first;
    while (w1 != NULL && w2 != NULL)
    {
        wchar_t *temp1 = wcslwr(w1->pointer), *temp2 = wcslwr(w2->pointer);
        if (wcscmp(temp1, temp2) != 0 || w1->delimeter != w2->delimeter)
        {
            free(temp1);
            free(temp2);
            temp1 = senttostr(sent1);
            temp2 = senttostr(sent2);
            free(temp1);
            free(temp2);
            return 0;
        }
        free(temp1);
        free(temp2);
        w1 = w1->next;
        w2 = w2->next;
    }
    if (w1 != w2)// || sent1->delimeter != sent2->delimeter)
        return 0;
    return 1;
}

/*wchar_t *wclwr(wchar_t c) {
    //английские
    if (c >= 65 && c <= 90) c += 32;
    //русские
    if (c >= 1040 && c <= 1071) c += 32;
}*/
