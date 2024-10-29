    Overview:
The server program takes as input a range of values from a client program through a named pipe and returns a list of all prime numbers between that range, inclusively.

The server expects an input in the form "RANGE X Y", where X and Y are positive integers denoting the lower and upper bounds of the range, respectfully.
"RANGE" is not case sensitive.
Any input after Y is simply ignored.
The server rejects an input and returns "Malformed request" to the client under any of the following conditions:
 - Y < X
 - X or Y is not positive
 - The input is not in the expected format

The path of the named pipe used for communication between the server and client is /tmp/myfifo

    Execution:
To run the program in Ubuntu, navigate to the directory containing it and execute:
make
./server

Then use a client program to communicate with it.
The server runs forever unless manually terminated.