#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    int fd;
    const char * myfifo = "/tmp/myfifo";
    string s;
    char arr[2048];
    {
        // Open FIFO for write only
        fd = open(myfifo, O_WRONLY);
        if (argc < 3)
            getline(cin, s);
        else {
            s = string("RANGE ") +
                string(argv[1]) +
                string(" ") +
                string(argv[2]);
        }
        write(fd, s.c_str(), s.size());
        close(fd);

        // Open FIFO for Read only
        fd = open(myfifo, O_RDONLY);

        // Read from FIFO
        read(fd, arr, sizeof(arr));

        // Print the read message
        printf("Server said : <%s>\n", arr);
        close(fd);
    }
    return 0;
}