#include <stdio.h>
#include <locale.h>
#include "text.h"

int main() {

    setlocale(LC_ALL, "");

    Text *text = read_text();
    wprintf(L"Text is: %ls", (*text).pointer);

    return 0;
}