// - Create a client and server program communicating over TCP socket
// - Establish TLS connection between the client and server
// - Implement a hash service on the server
// 	- Take a number from the client
// 	- Send this number over TLS to the server
// 	- The server uses the Sha256 hash function within the OpenSSL library to generate a hash code for the number
// 	- The server sends the hash code back to the client for display.

// Please use OpenSSL, version 1.1.1
// https://www.openssl.org/source/

// reference
// socket programming in cpp: https://www.geeksforgeeks.org/socket-programming-cc/
// how to compile and debug cpp file in visual studio code : https://code.visualstudio.com/docs/cpp/config-linux

#include <iostream>
#include <unistd.h> 
#include <string.h>
#include <sys/socket.h> 
#include <netinet/in.h> 

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[Error]: " << message << '\n';
	system("pause");
	exit(1);
}

int main(void) {
    
	int serverSocket, clientSocket;
	struct sockaddr_in serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	// create a TCP socket
	if ((serverSocket = socket(PF_INET, SOCK_STREAM, 0)) == 0) {
		ShowErrorMessage("socket() failed");
	}

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	// convert 4 byte-integer into network bytes
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	// convert 2 byte-integer into network bytes
	serverAddress.sin_port = htons(serverPort);

	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
		ShowErrorMessage("bind() failed");
	}
	cout << "[current state] bind()\n";

	if (listen(serverSocket, 5) < 0) {
		ShowErrorMessage("listen() failed");
	}
	cout << "[current state] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&sizeClientAddress)) < 0) {
        ShowErrorMessage("accept() failed");
    }
	cout << "[current state] accept()\n";

	// receive and send the client's message (echo)
	while (1) {
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';
		cout << "[client's message]: " << received << '\n';
		cout << "[send message]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[terminate server]\n";
			break;
		}

        /* client presses enter sending a message. 
        Erase enter by doing sizeof(received) - 1 */
		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	close(clientSocket);
	close(serverSocket);
	system("pause");
	return 0;
}