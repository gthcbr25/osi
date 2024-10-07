#include <iostream>
#include <string>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    std::string name;
    std::cout << "Введите имя файла:" << std::endl;
    std::cin >> name;


    int fd1[2], fd2[2];
    if (pipe(fd1) == -1) {
        std::cout << "An error occured with creating a pipe1" << std::endl;
        return 1;
    }
    if (pipe(fd2) == -1) {
        std::cout << "An error occured with creating a pipe2" << std::endl;
        return 1;
    }

    int read1 = fd1[0], write1 = fd1[1];
    int read2 = fd2[0], write2 = fd2[1];
    switch (fork()) {
        case -1:
            std::cout << "An error occured with creating a child process" << std::endl;
            return 1;
        case 0:
            close(write1);
            close(read2);
            if (dup2(read1, STDIN_FILENO) == -1) {
                std::cout << "error with redirecting input" << std::endl;
                return 1;
            }
            if (dup2(write2, STDOUT_FILENO) == -1) {
                std::cout << "error with redirecting output" << std::endl;
                return 1;
            }
            execl("child", "child", name.c_str(), NULL);
            exit(EXIT_FAILURE);
        default:
            close(write2);
            write(write1, name.c_str(), sizeof(name));
            std::string line;
            std::cin >> line;
            while(line != "***"){
                write(write1, line.c_str(), sizeof(line)+1);
                std::cin >> line;
            }
            
            close(write1);
            close(read2);
            close(read1);
            wait(nullptr);
    }
    return 0;
}