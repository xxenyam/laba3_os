#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define DEFAULT_FILENAME "output"  

void generate_random_numbers(const char *filename, int number) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Помилка відкриття файлу");
        exit(EXIT_FAILURE);
    }

    // Генерація випадкових чисел і запис їх у файл
    for (int i = 0; i < number; i++) {
        fprintf(file, "%.6f\n", (double)rand() / RAND_MAX);
    }

    fclose(file);
    printf("Файл %s збережено\n", filename);
}

int main(int argc, char *argv[]) {
    char *file_name = NULL;
    int num_children = 0;

    // Обробка опцій командного рядка
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
            if (i + 1 < argc) {
                file_name = argv[++i];  
                setenv("FILE_NAME", file_name, 1);  
                printf("Змінна середовища FILE_NAME встановлена: %s\n", getenv("FILE_NAME"));
            } else {
                fprintf(stderr, "Помилка: не вказано ім'я файлу після опції -f\n");
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--number") == 0) {
            if (i + 1 < argc) {
                num_children = atoi(argv[++i]);  
                if (num_children <= 0) {
                    fprintf(stderr, "Помилка: кількість дочірніх програм має бути додатнім числом\n");
                    return EXIT_FAILURE;
                }
            } else {
                fprintf(stderr, "Помилка: не вказано кількість після опції -n\n");
                return EXIT_FAILURE;
            }
        }
    }

    // Перевірка, чи встановлена змінна середовища FILE_NAME
    if (getenv("FILE_NAME") == NULL) {
        file_name = DEFAULT_FILENAME;  
    } else {
        file_name = getenv("FILE_NAME");
    }

    printf("Поточне ім'я файлу: %s\n", file_name);  

    // Запускаємо дочірні програми
    srand(time(NULL));  
    for (int i = 1; i <= num_children; i++) {
        pid_t pid = fork();
        if (pid == 0) {  
            char child_file_name[256];
            snprintf(child_file_name, sizeof(child_file_name), "%s_%d.txt", file_name, i); 
            generate_random_numbers(child_file_name, i);  
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            perror("Помилка при створенні процесу");
            exit(EXIT_FAILURE);
        }
    }

    // Очікуємо завершення всіх дочірніх процесів
    for (int i = 0; i < num_children; i++) {
        wait(NULL);
    }

    printf("Всі дочірні програми завершені.\n");

    return EXIT_SUCCESS;
}

