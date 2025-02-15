#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

#define INITIAL_SIZE 128

char *read_lines() {
    char *line = malloc(INITIAL_SIZE);
    if (!line) {
        perror("Allocation error");
        exit(EXIT_FAILURE);
    }

    size_t size = INITIAL_SIZE;
    size_t length = 0;
    int ch;

    while ((ch = getchar()) != EOF) {
        if (length + 1 >= size) {
            size *= 2;
            char *new_line = realloc(line, size);
            if (!new_line) {
                perror("Reallocation error");
                free(line);
                exit(EXIT_FAILURE);
            }
            line = new_line;
        }
        line[length++] = ch;
    }

    if (length == 0 && ch == EOF) {
        free(line);
        return NULL;
    }

    line[length] = '\0';
    return line;
}

int main(int argc, char* argv[]) {
    const char* back_name = "Lab3.back";
    unsigned perms = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;

    char* input_data = NULL;
    char* file_name = NULL;

    size_t size = 0;
    printf("Enter filename: ");
    getline(&file_name, &size, stdin);

    printf("Enter strings: ");
    input_data = read_lines();

    size_t input_len = strlen(input_data);
    if (input_data[input_len - 1] == '\n') {
        input_data[input_len - 1] = '\0';
        input_len--;
    }

    size_t map_size = strlen(file_name) + 1 + input_len + 1;

    int fd = shm_open(back_name, O_RDWR | O_CREAT, perms);
    if (fd == -1) {
        perror("SHM_OPEN");
        free(input_data);
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, map_size) == -1) {
        perror("FTRUNCATE");
        close(fd);
        free(input_data);
        exit(EXIT_FAILURE);
    }

    char* memptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memptr == MAP_FAILED) {
        perror("MMAP");
        close(fd);
        free(input_data);
        exit(EXIT_FAILURE);
    }

    snprintf(memptr, map_size, "%s|%s", file_name, input_data);

    pid_t cpid = fork();

    if (cpid == -1) {
        perror("FORK");
        munmap(memptr, map_size);
        close(fd);
        free(input_data);
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        munmap(memptr, map_size);
        close(fd);
        execl("./build/child", "child", NULL);
        perror("EXECL");
        exit(EXIT_FAILURE);
    } else {
        int status;
        wait(&status);
        free(input_data);
        munmap(memptr, map_size);
        close(fd);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Child process completed successfully.\n");
            exit(EXIT_SUCCESS);
        } else {
            fprintf(stderr, "Child process failed.\n");
            exit(EXIT_FAILURE);
        }
    }
}