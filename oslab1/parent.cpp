#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/wait.h>
#include <fcntl.h>
#include <vector>

int main() {
    std::string file_name1;
    std::string file_name2;
    char c = 1;
    write(STDOUT_FILENO, "Enter file name 1: ", 20);
    while (c != '\n') {
        read(STDIN_FILENO, &c, sizeof(char));
        if (c != '\n') {
            file_name1 += c;
        }  
    }
    c = 1;

    write(STDOUT_FILENO, "Enter file name 2: ", 20);
    while (c != '\n') {
        read(STDIN_FILENO, &c, sizeof(char));
        if (c != '\n') {
            file_name2 += c;
        }      
    }


    int fd1[2], fd2[2];
    int tmp = pipe(fd1);
    if (tmp == -1) {
        write(STDERR_FILENO, "An error occured with creating pipe1", 37);
        return 1;
    }
    tmp = pipe(fd2);
    if (tmp == -1) {
        write(STDERR_FILENO, "An error occured with creating pipe2", 37);
        return 1;
    }


    pid_t process_id1 = fork();
    pid_t process_id2 = fork();
    if (process_id1 == -1) {
        write(STDERR_FILENO, "An error occured with creating child process 1", 47);
        return 1;
    }
    if (process_id2 == -1) {
        write(STDERR_FILENO, "An error occured with creating child process 2", 47);
        return 1;
    }
    if (process_id1 == 0 && process_id2 > 0) {  //child process 1
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        tmp = dup2(fd1[0], STDIN_FILENO);
        if (tmp == -1) {
            write(STDERR_FILENO, "An error occured with redirecting input 1", 40);
            return 1;
        }

        tmp = execl("child1", "child1", file_name1.c_str(), NULL);
        if (tmp == -1) {
            write(STDERR_FILENO, "An error occured with runing program from a child process 1", 59);
            return 1;
        }
        exit(EXIT_FAILURE);
    }

    if (process_id1 > 0 && process_id2 == 0) {  //child process 2
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[1]);
        tmp = dup2(fd2[0], STDIN_FILENO);
        if (tmp == -1) {
            write(STDERR_FILENO, "An error occured with redirecting input 2", 40);
            return 1;
        }

        tmp = execl("child2", "child2", file_name2.c_str(), NULL);
        if (tmp == -1) {
            write(STDERR_FILENO, "An error occured with runing program from a child process 2", 58);
            return 1;
        }
        exit(EXIT_FAILURE);
    }

    if (process_id1 > 0 && process_id2 > 0) {   //parent process
        close(fd1[0]);
        close(fd2[0]);
        char c;
        char prev = '?';
        std::vector<char> vec;
        while(read(STDIN_FILENO, &c, 1)) {
            vec.push_back(c);
            if (c == '\n') {
                if (vec.size() > 11){
                    for (int i = 0; i < vec.size(); ++i) {
                        write(fd2[1], &vec[i], 1);
                    }
                } else {
                    for (int i = 0; i < vec.size(); ++i) {
                        write(fd1[1], &vec[i], 1);
                    }
                }
                vec.clear();
            }
            prev = c;
        }
        close(fd1[1]);
        close(fd2[1]);
        wait(nullptr);
    }
    return 0;
}