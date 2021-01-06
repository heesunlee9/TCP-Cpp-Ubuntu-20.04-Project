#include <iostream>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h> 

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[Error]: " << message << '\n';
	system("pause");
	exit(1);
}

int main(void) {

	int clientSocket;
	struct sockaddr_in serverAddress;

	int serverPort = 9876;
	char received[256];
	string sent;

	// create a TCP socket 
	if ((clientSocket = socket(PF_INET, SOCK_STREAM, 0)) == 0) {
		ShowErrorMessage("socket() failed");
	}

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	// convert string ip into network byte
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	// convert 2 byte integer into network byte
	serverAddress.sin_port = htons(serverPort);

	if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
		ShowErrorMessage("connect()");
	}
	cout << "[current state] connect()\n";

	// repeatedly send server message and receive echo message from server (echo)
	while (1) {
		cout << "[send message]: ";
		getline(cin, sent);
		if (sent == "") {
			continue;
		}
		send(clientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';
		if (strcmp(received, "[exit]") == 0) {
			cout << "[terminate server]\n";
			break;
		}
		cout << "[server's message(echo)]:" << received << '\n';
	}

	close(clientSocket);
	system("pause");
	return 0;
}