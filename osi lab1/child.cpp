#include <iostream>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <string>

int main(int argc, char* argv[]) {
    char c;
    std::cout << "start programm" << std::endl;
    FILE *fptr;
    fptr = fopen(argv[1], "w");
    std::string line;
    while (getline(std::cin, line)) {
        size_t length = sizeof(line) / sizeof(char);
        if (line[length - 1] == '.' || line[length - 1] == ';'){
            fprintf(fptr, "%s", line);
        } else {
            std::cout << "Alarm у нас траблы" << std::endl;
        }
    }
    fclose(fptr); 
    return 0;
}