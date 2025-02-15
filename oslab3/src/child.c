#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void write_lines_ending_with_dot(const char *input_data, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    const char *current_line = input_data;

    while (*current_line) {
        const char *line_end = strchr(current_line, '\n');
        if (!line_end) {
            line_end = current_line + strlen(current_line);
        }

        if ((line_end > current_line && *(line_end - 1) == '.') | (line_end > current_line && *(line_end - 1) == ';')) {
            fwrite(current_line, 1, line_end - current_line, file);
            fputc('\n', file);
        }

        current_line = line_end;
        if (*current_line == '\n') {
            current_line++;
        }
    }

    fclose(file);
}

int main() {
    const char* back_name = "Lab3.back";
    unsigned perms = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;

    int map_fd = shm_open(back_name, O_RDWR, perms);
    if (map_fd < 0) {
        perror("SHM_OPEN");
        exit(EXIT_FAILURE);
    }

    struct stat statbuf;
    if (fstat(map_fd, &statbuf) == -1) {
        perror("FSTAT");
        close(map_fd);
        exit(EXIT_FAILURE);
    }

    size_t map_size = statbuf.st_size;

    char* memptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, map_fd, 0);
    if (memptr == MAP_FAILED) {
        perror("MMAP");
        close(map_fd);
        exit(EXIT_FAILURE);
    }

    close(map_fd);

    char filename[20] = {0};
    size_t readed_data_id = 0;

    // Разделяем имя файла и данные
    for (size_t i = 0; i < map_size; i++) {
        if (memptr[i] != '|') {
            filename[i] = memptr[i];
        } else {
            readed_data_id = i + 1;
            break;
        }
    }

    char* input_data = memptr + readed_data_id;

    write_lines_ending_with_dot(input_data, filename);

    munmap(memptr, map_size);
    return 0;
}