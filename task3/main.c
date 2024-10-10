#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Функція-обробник для завершення програми
void my_function(int status, void *arg) {
    int handler_number = *(int *)arg;
    sleep(1);  
    printf("Обробник %d викликаний з кодом статусу: %d\n", handler_number, status);
}

// Обробник сигналу SIGINT (Ctrl+C)
void handle_sigint(int sig) {
    printf("\nОтримано сигнал SIGINT, програма завершиться.\n");
    exit(0);  
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Введіть кількість обробників завершення процесу\n");
        return EXIT_FAILURE;
    }

    int num_handlers = atoi(argv[1]);
    if (num_handlers <= 0) {
        fprintf(stderr, "Кількість обробників має бути додатнім числом\n");
        return EXIT_FAILURE;
    }

    // Масив для зберігання номерів обробників
    int *handlers = malloc(num_handlers * sizeof(int));

    // Реєструємо обробники
    for (int i = 0; i < num_handlers; i++) {
        handlers[i] = i + 1;  
        if (on_exit(my_function, &handlers[i]) != 0) {
            fprintf(stderr, "Не вдалося зареєструвати обробник %d\n", i);
            return EXIT_FAILURE;
        }
    }

    // Реєструємо обробник для сигналу SIGINT (Ctrl+C)
    signal(SIGINT, handle_sigint);

    printf("Програма працює. Для завершення натисніть Ctrl+C або закрийте термінал.\n");

    while (1) {
        pause();  
    }

    free(handlers);

    return EXIT_SUCCESS;
}

