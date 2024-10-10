#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

int main(int argc, char *argv[]) {
    // Перевірка, чи було передано команду
    if (argc < 2) {
        fprintf(stderr, "Введіть команду або команди, розділені крапкою з комою (;)\n");
        return EXIT_FAILURE;
    }

    // Об'єднання всіх аргументів в одну строку
    char command[1024] = "";  
    for (int i = 1; i < argc; i++) {
        strcat(command, argv[i]);
        if (i < argc - 1) {
            strcat(command, " ");  /
        }
    }

    // Виконання команди
    int returnCode = system(command);

    // Виведення коду завершення
    printf("Код завершення: %d\n", returnCode);

    return returnCode == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

