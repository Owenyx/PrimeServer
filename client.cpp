#include <iostream>

int main() {
    int fd;

    // FIFO file path
	char * myfifo = "/tmp/myfifo";

     int count = 0; // Number of bytes (characters) read

     count = read(fd, buffer, size);

        if (count == 0)
            continue; // Nothing was read, read again    
}