#include <iostream>
#include <regex>
#include <string>

using namespace std;

int main() {
    int fd;

    // FIFO file path
	char * myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
	// mkfifo(<pathname>, <permission>)
	mkfifo(myfifo, 0666);

    // Buffer size
    int size = 40;
    // More than enough to read the expected input 

    char buffer[size];
    while(1) {
        // Open the FIFO in read-only and read it
        fd = open(myfifo, O_RDONLY);
        read(fd, buffer, size);

        // Process input
        string input = buffer;
        input = toupper(input);

        // Check for malformed request, if so, tell the client and continue to next input
        regex pattern(R"RANGE\s\d+\s\d+");
        if (!regex_match(input, pattern)) {
            close(fd);
            fd = open(myfifo, O_WRONLY);
            char* str = "Malformed request";
            write(fd, str, strlen(str)+1) // +1 for null byte
            continue;
        }

        // Extract range from string
        string tmp;
        stringstream ss(input);
        ss >> tmp; // toss "RANGE"
        ss >> tmp;
        upperBound = stoi(tmp);
        ss >> tmp;
        lowerBound = stoi(tmp);
        
        
    }
}