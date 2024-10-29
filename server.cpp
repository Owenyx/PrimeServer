#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <regex>
#include <vector>
#include <cmath>

using namespace std;

vector<int> primesInRange(int, int);
void malformed(int fd, const char * fifoName);

int main() {
    int fd;

    // FIFO file path
	const char * myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
	// mkfifo(<pathname>, <permission>)
	mkfifo(myfifo, 0666);

    // Buffer size
    int size = 256;

    char buffer[size];
    while(1) {
        //Clear buffer with null bytes because if it has zeroes then things go wack
        for (int i = 0; i < size; ++i) 
            buffer[i] = '\0';

        // Open the FIFO in read-only and read it
        fd = open(myfifo, O_RDONLY);
        read(fd, buffer, size);

        // Process input from FIFO to retireve range of primes
        int upperBound;
        int lowerBound;
        {
            string input = buffer;

            // ignore case by setting to upper case
            for (char &c : input) 
                c = toupper(c); 

            // Check for malformed request, if so, tell the client and continue to next input
            // String must be "RANGE X Y..." where X and Y are positive integers and ... is anything after and is ignored
            regex expected("^RANGE\\s+\\d+\\s+\\d+");
            if (!regex_match(input, expected)) {
                malformed(fd, myfifo);
                continue;
            }

            // Extract range from string
            smatch match;
            regex extract("(\\d+)\\s+(\\d+)");
            regex_search(input, match, extract);
            lowerBound = stoi(match[1].str().substr(0, 9));
            upperBound = stoi(match[2].str().substr(0, 9));
            // The substring is needed to truncate numbers too big for int
        }

        // Check bound validity - Negatives are already covered by regex
        if (lowerBound > upperBound || lowerBound < 1) {
            malformed(fd, myfifo);
            continue;
        }

        // Calculate primes within range 
        vector<int> primes = primesInRange(lowerBound, upperBound);

        // Put results into a string then a C string
        string result = "Primes found : ";
        for (int p : primes) 
            result += to_string(p) + " ";
        result.pop_back(); // Erase last added space
        const char * msg = result.c_str();
        
        // Send results back to client through FIFO
        // Open FIFO in write only mode and write
        close(fd);
		fd = open(myfifo,O_WRONLY);
		write(fd, msg, strlen(msg)+1);
		close(fd);
    }
}

vector<int> primesInRange(int lowerBound, int upperBound) {
    // Modified version of Sieve
    /* 
    Create a vector of all primes from 2 up to and including the square root of up_bound using sieve.
    This way we will use these prime numbers to perform the algorithm very efficiently on the given range, 
    as any value that is a multiple of any of the primes <= root is not prime
    */
    int root = (int)sqrt(upperBound);
    vector<int> divisors(root-1, 0); // root-1 as it will include root but not 1 or 0
    for (int i = 0; i < (int)divisors.size(); i++) divisors[i] = i+2; // load vector. i+2 because the first number will be 2

    // now iterate through the vector, deleting every multiple of i
    for (int i = 0; i < (int)divisors.size(); i++) 
        for(int j = i + 1; j < (int)divisors.size(); j++)  // j starts at i + 1 as all values i and lower are prime
            if (divisors[j] % divisors[i] == 0) divisors.erase(divisors.begin() + j);

    // now divisors contains all prime numbers <= root

    // Create a vector with all numbers within the range
    vector<int> vec(upperBound-lowerBound+1, 0); // +1 since upper bound is inclusive
    for (int i = 0; i < (int)vec.size(); i++) vec[i] = i + lowerBound; // load vector
    
    // Erase 1 if present
    if (lowerBound == 1)
        vec.erase(vec.begin());

    // Now check which numbers in vec are divisible by the primes in divisors and erase them
    for (int i : divisors) 
        for(int j = 0; j < (int)vec.size(); j++) 
            if (vec[j] != i && vec[j] % i == 0) vec.erase(vec.begin() + j);

    return vec;
}

// Closes current file and opens it in write-only to write "Malformed request"
void malformed(int fd, const char * fifoName) {
    close(fd);
    fd = open(fifoName, O_WRONLY);
    const char * str = "Malformed request";
    write(fd, str, strlen(str)+1); // +1 for null byte
    close(fd);
    return;
}