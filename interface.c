#include <stdio.h>
#include <wchar.h>
#include "course_utils.h"
#include "colors.h"

void show_help() {
    wprintf(
        L"%lsДоступные команды:%ls\n"
        L"\t1 - Заменить в тексте все подстроки <high noon> на <полдень> и <полночь> на <midnight>\n"
        L"\t2 - Найти и вывести все даты в тексте заданные в виде < DD/MM/YYYY > или < YYYY-MM-DD > в порядке возрастания этих дат\n"
        L"\t3 - Удалить все предложения, которые начинаются и заканчиваются на одно и то же слово\n"
        L"\t4 - Для всех вхождений дат в тексте вида <<day> <month> <year>>, вывести эти даты в виде <DD.MM.YYYY> и строку <Happened> если эта дата была до текущей и <Not Happened> в противном случае. Например, для даты <03 Jan 1666> вывести <03.01.1666 Happened>\n"
        L"\tp - Вывести текст в текущем состоянии\n"
        L"\th - Вывести доступные команды\n"
        L"\t0 - Выйти из программы\n",
        COLOR_BLUE,
        COLOR_NORMAL
    );
}

wchar_t read_action() {
    wprintf(L"%lsВведите действие: %ls", COLOR_GREEN, COLOR_NORMAL);
    wchar_t c = fgetwc(stdin);
    wchar_t temp = c;
    while (temp != L'\n') {
        temp = fgetwc(stdin);
    }
    return c;
}