#include <iostream>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <string>

int main(int argc, char* argv[]) {
    
    int file = open(argv[1], O_CREAT | O_WRONLY, S_IRWXU);
    if (file == -1) {
        write(STDERR_FILENO, "An error occured with opening file", 35);
        return 1;
    }
    int temp = ftruncate(file, 0);
    if (temp == -1) {
        write(STDERR_FILENO, "An error occured with clearing file", 36);
        return 1;
    }

    int tmp = dup2(file, STDOUT_FILENO);
    if (tmp == -1) {
        write(STDERR_FILENO, "An error occured with redirecting stdout to file", 48);
        close(file);
        return 1;
    }

    std::vector<char> vec;
    char c;
    while (read(STDIN_FILENO, &c, 1)) {
        vec.push_back(c);
        if (c == '\n') {
                for (int i = vec.size() - 2; i >= 0; --i) {
                    write(STDOUT_FILENO, &vec[i], 1);
                }
                write(STDOUT_FILENO, &vec.back(), 1);
                vec.clear();
            }
    }
    close(file); 
    return 0;
}