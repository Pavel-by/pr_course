#include <stdio.h>
#include <locale.h>
#include "text.h"
#include "text_structs.h"
#include "course_utils.h"
#include "interface.h"

#define STRING_ERA 10
#define TEXT_ERA 5
#define WORD_ERA 5

int main() {

    setlocale(LC_ALL, "");

    Text *text = read_text();
    wprintf(L"PS: при вводе команды учитывается только первый введенный символ, поэтому строки <1> и <1234> идентичны.\n");
    
    if (text == NULL) {
        wprintf(L"Memory error\n");
    } else {
        remove_similar(text);
        show_help();
        wchar_t action = read_action();

        while (action != L'q') {
            switch (action) {
                case L'1': {
                    task1(text);
                    break;
                }
                case L'2': {
                    task2(text);
                    break;
                }
                case L'3': {
                    task3(text);
                    break;
                }
                case L'4': {
                    task4(text);
                    break;
                }
                case L'p': {
                    for (int i = 0; i < text->len; i++) {
                        wprintf(L"%ls", senttostr(text->sentences[i]));
                    }
                    wprintf(L"\n");
                    break;
                }
                case L'h': {
                    show_help();
                    break;
                }
                default: {
                    wprintf(L"Неопознанное действие. Аккуратнее, есть же меню помощи. Вводить фигню - признак -*- *-* ** *-- -*-- ****    *--* *- *-** -**- -*-* * *--\n");
                    break;
                }
            }
            action = read_action();
        }
        wprintf(L"До встречи. Не ломайте программу.\n");
    }


    return 0;
}