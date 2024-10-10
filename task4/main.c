#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#define DEFAULT_BUFFER_SIZE 127

// Функція для виведення довідкової інформації
void print_help() {
    printf("Спрощений командний процесор\n");
    printf("Опції:\n");
    printf("  -h, --help    Вивести довідкову інформацію\n");
    printf("  -b, --buffer  Вказати розмір буфера для команд\n");
    printf("Команда 'stop' завершує роботу процесора\n");
}

// Основна програма командного процесора
int main(int argc, char *argv[]) {
    int buffer_size = DEFAULT_BUFFER_SIZE;  
    char *buffer;
    
    // Обробка аргументів командного рядка
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help();
            return EXIT_SUCCESS;
        } else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--buffer") == 0) {
            if (i + 1 < argc) {
                buffer_size = atoi(argv[++i]);
                if (buffer_size <= 0) {
                    fprintf(stderr, "Неправильний розмір буфера\n");
                    return EXIT_FAILURE;
                }
            } else {
                fprintf(stderr, "Не вказано розмір буфера\n");
                return EXIT_FAILURE;
            }
        }
    }

    // Виділяємо пам'ять під буфер
    buffer = malloc(buffer_size * sizeof(char));
    if (buffer == NULL) {
        perror("Помилка виділення пам'яті");
        return EXIT_FAILURE;
    }

    // Отримуємо ім'я користувача
    struct passwd *pw = getpwuid(getuid());
    if (pw == NULL) {
        perror("Помилка отримання інформації про користувача");
        free(buffer);
        return EXIT_FAILURE;
    }
    char *username = pw->pw_name;

    // Основний цикл командного процесора
    while (1) {
        printf("[%s]$ ", username);
        if (fgets(buffer, buffer_size, stdin) == NULL) {
            printf("Помилка зчитування команди\n");
            continue;
        }

        // Видаляємо символ нового рядка
        buffer[strcspn(buffer, "\n")] = 0;

        // Якщо команда — 'stop', завершуємо програму
        if (strcmp(buffer, "stop") == 0) {
            printf("Wait 3 seconds...\n");
            sleep(3);  
            system("clear");  
            break;
        }

        // Виконуємо команду за допомогою system()
        int status = system(buffer);
        if (status == -1) {
            perror("Помилка виконання команди");
        } else if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            printf("Команда завершилась з кодом: %d\n", exit_status);
        }
    }

    free(buffer);
    return EXIT_SUCCESS;
}
