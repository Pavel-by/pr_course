#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <time.h>
#include "text.h"

#define DATE_ERA 10
#define FALSE 0
#define TRUE 1
#define MASK_DATE_1 L"dd/dd/dddd"
#define MASK_DATE_2 L"dddd-dd-dd"

typedef struct {
    int day;
    int mounth;
    int year;
} Date;

int days[] = {
    31,
    28,
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31
};

int *mounths[] = {
    L"Jan",
    L"Feb",
    L"Mar",
    L"Apr",
    L"May",
    L"Jun",
    L"Jul",
    L"Aug",
    L"Sep",
    L"Oct",
    L"Nov",
    L"Dec",
};

void remove_similar(Text *text) {
    for (int i = 0; i < text->len - 1; i++) {
        for (int j = i + 1; j < text->len; j++) {
            if (sent_eq(text->sentences[i], text->sentences[j])) {
                free_sentence(text->sentences[j]);
                for (int k = j; k < text->len - 1; k++) {
                    text->sentences[k] = text->sentences[k+1];
                }
                text->len--;
                j--;
            }
        }
    }
}

void replace_highnoon(Text *text) {
    Sentence *sentence;
    Word *w1, *w2;
    for (int i = 0; i < text->len; i++) {
        sentence = text->sentences[i];
        w1 = sentence->first;
        w2 = w1->next;
        while (w2 != NULL) {
            if (w1->delimeter == L' ' && !wcscmp(w1->pointer, L"high") && !wcscmp(w2->pointer, L"noon")) {
                w1->next = w2->next;
                if (w2->next != NULL) {
                    w2->next->prev = w1;
                }
                w1->delimeter = w2->delimeter;
                free_word(w2);
                free(w1->pointer);
                w1->pointer = malloc(sizeof(wchar_t) * 8);
                wcscpy(w1->pointer, L"полдень");
            } else {
                w1 = w1->next;
            }
            w2 = w1->next;
        }
    }
}

void replace_midnight(Text *text) {
    Sentence *sentence;
    Word *w1;
    for (int i = 0; i < text->len; i++) {
        sentence = text->sentences[i];
        w1 = sentence->first;
        while (w1 != NULL) {
            if (!wcscmp(w1->pointer, L"полночь")) {
                free(w1->pointer);
                w1->pointer = malloc(sizeof(wchar_t) * 10);
                wcscpy(w1->pointer, L"midnight");
            } 
            w1 = w1->next;
        }
    }
}

int mask_cmp(const wchar_t *source, const wchar_t *mask) {
    int len_mask = wcslen(mask), len = wcslen(source);
    if (len_mask != len) return FALSE;
    for (int i = 0; i < len; i++) {
        switch (mask[i]) {
            case L'd': {
                if (!iswdigit(source[i])) return FALSE;
                break;
            }
            case L'S': {
                if (!iswalpha(source[i])) return FALSE;
                break;
            }
            default: {
                if (source[i] != mask[i]) return FALSE;
            }
        }
    }
    return TRUE;
}

int iswcsdigit(wchar_t *str) {
    int len = wcslen(str);
    for (int i = 0; i < len; i++) {
        if (!iswdigit(str[i])) return FALSE;
    }
    if (len == 0) return FALSE;
    return TRUE;
}

int strtomounth(wchar_t *str) {
    for (int i = 0; i < 12; i++) {
        if (!wcscmp(mounths[i], str)) return i + 1;
    }
    return -1;
}

int is_date_correct(Date *date) {
    if (date->day <= 0 || date->mounth <= 0) return FALSE;
    if (date->mounth > 12) return FALSE;
    if (date->mounth == 2 && date->year % 4 == 0 && date->day > 29) return FALSE;
    if (date->day > days[date->mounth - 1]) return FALSE;
    return TRUE;
}

int date_cmp(const void *d1, const void *d2) {
    return (((Date*)d1)->day + ((Date*)d1)->mounth * 50 + ((Date*)d1)->year * 20 * 50) - (((Date*)d2)->day + ((Date*)d2)->mounth * 50 + ((Date*)d2)->year * 20 * 50);
}

void print_date1(Text *text) {
    Sentence *sentence;
    Word *w;
    int count = 0, max = DATE_ERA;
    Date *dates = malloc(sizeof(Date) * max), *temp;
    for (int i = 0; i < text->len; i++) {
        sentence = text->sentences[i];
        w = sentence->first;
        while (w != NULL) {
            if (w->prev == NULL) {
                //Это нужно, чтобы выяснить, есть ли слева пробел. Не, ну а что, такое задание
                w = w->next;
                continue;
            } 
            if (w->prev->delimeter == L' ' && mask_cmp(w->pointer, MASK_DATE_1) && w->delimeter == L' ') {
                //Даты в первом формате
                Date date;
                date.day = (w->pointer[0] - L'0') * 10 + (w->pointer[1] - L'0');
                date.mounth = (w->pointer[3] - L'0') * 10 + (w->pointer[4] - L'0');
                date.year = (w->pointer[6] - L'0') * 1000 + (w->pointer[7] - L'0') * 100 + (w->pointer[8] - L'0') * 10 + (w->pointer[9] - L'0');
                if (is_date_correct(&date)) {
                    if (count == max) {
                        max += DATE_ERA;
                        temp = realloc(dates, sizeof(Date) * max);
                        if (temp == NULL) {
                            wprintf(L"Ошибка при выделении памяти. Прерывание задания\n");
                            free(dates);
                            return;
                        }
                        dates = temp;
                    }
                    dates[count++] = date;
                }
            }
            if (w->prev->delimeter == L' ' && mask_cmp(w->pointer, MASK_DATE_2) && w->delimeter == L' ') {
                //Даты во втором формате
                Date date;
                date.day = (w->pointer[8] - L'0') * 10 + (w->pointer[9] - L'0');
                date.mounth = (w->pointer[5] - L'0') * 10 + (w->pointer[6] - L'0');
                date.year = (w->pointer[0] - L'0') * 1000 + (w->pointer[1] - L'0') * 100 + (w->pointer[2] - L'0') * 10 + (w->pointer[3] - L'0');
                if (is_date_correct(&date)) {
                    if (count == max) {
                        max += DATE_ERA;
                        temp = realloc(dates, sizeof(Date) * max);
                        if (temp == NULL) {
                            wprintf(L"Ошибка при выделении памяти. Прерывание задания\n");
                            free(dates);
                            return;
                        }
                        dates = temp;
                    }
                    dates[count++] = date;
                }
            }
            w = w->next;
        }
    }
    qsort(dates, count, sizeof(Date), date_cmp);
    for (int i = 0; i < count; i++) {
        wprintf(L"%04d-%02d-%02d\n", dates[i].year, dates[i].mounth, dates[i].day);
    }
}

void remove_similar_by_ends(Text *text) {
    Sentence *sent;
    Word *start, *end, *temp;
    for (int i = 0; i < text->len; i++) {
        sent = text->sentences[i];
        temp = sent->first;
        while (temp != NULL && wcslen(temp->pointer) == 0) {
            temp = temp->next;
        }
        if (temp == NULL) continue;
        start = temp;
        while (temp != NULL) {
            if (wcslen(temp->pointer) > 0) end = temp;
            temp = temp->next;
        }
        if (!wcscmp(start->pointer, end->pointer)) {
            free_sentence(sent);
            text->len--;
            for (int j = i; j < text->len; j++) {
                text->sentences[j] = text->sentences[j + 1];
            }
        }
    }
}

void print_date2(Text *text) {
    time_t mTime;
    time(&mTime);
    struct tm mTm = *localtime(&mTime);
    Date curDate = {mTm.tm_mday, mTm.tm_mon + 1, mTm.tm_year + 1900};
    Sentence *sent;
    Word *w1, *w2, *w3;
    for (int i = 0; i < text->len; i++) {
        sent = text->sentences[i];
        if (sent->first->next == NULL) continue;
        w1 = sent->first;
        w2 = w1->next;
        w3 = w2->next;
        while (w3 != NULL) {
            if (wcslen(w1->pointer) == 2 && wcslen(w2->pointer) == 3 && wcslen(w3->pointer) == 4 && iswcsdigit(w1->pointer) && iswcsdigit(w3->pointer) && w1->delimeter == L' ' && w2->delimeter == L' ' ) {
                Date date;
                date.day = (w1->pointer[0] - L'0') * 10 + (w1->pointer[1] - L'0');
                date.mounth = strtomounth(w2->pointer);
                date.year = (w3->pointer[0] - L'0') * 1000 + (w3->pointer[1] - L'0') * 100 + (w3->pointer[2] - L'0') * 10 + (w3->pointer[3] - L'0');
                if (is_date_correct(&date)) {
                    if (date_cmp(&curDate, &date) >= 0) {
                        wprintf(L"%02d.%02d.%04d Happened\n", date.day, date.mounth, date.year);
                    } else {
                        wprintf(L"%02d.%02d.%04d Not Happened\n", date.day, date.mounth, date.year);
                    }
                }
            }
            w1 = w1->next;
            w2 = w1->next;
            w3 = w2->next;
        }
    }
}

void task1(Text *text) {
    replace_highnoon(text);
    replace_midnight(text);
}

void task2(Text *text) {
    print_date1(text);
}

void task3(Text *text) {
    remove_similar_by_ends(text);
}

void task4(Text *text) {
    print_date2(text);
}