#include <iostream>
#include <string>
using namespace std;
#include "comm_ZHT.h"

int main(int argc, char* argv[]) {
	cout << "Hello, I'm server." << endl;
	int listenfd, connfd, n;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t clilen;
	pid_t childpid;
	char mesg[1000];

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(50000);
	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(listenfd, 1024);

	for (;;) {
		clilen = sizeof(cliaddr);
		cout<<"1"<<endl;

		connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
		cout<<"2"<<endl;

		string result;
		int len = recvFrom(connfd, result);
		cout<<"3"<<endl;

		//cout << "Received data: "<<result<<endl;
		cout << "Received size: "<<len<<" bytes."<<endl;
		/*
		if ((childpid = fork()) == 0) {
			close(listenfd);

			for (;;) {
				n = recvfrom(connfd, mesg, 1000, 0,
						(struct sockaddr *) &cliaddr, &clilen);
				sendto(connfd, mesg, n, 0, (struct sockaddr *) &cliaddr,
						sizeof(cliaddr));
				printf(
						"-------------------------------------------------------\n");
				mesg[n] = 0;
				printf("Received the following:\n");
				printf("%s", mesg);
				printf(
						"-------------------------------------------------------\n");
			}

		}
		*/
		close(connfd);
	}

	return 0;
}

