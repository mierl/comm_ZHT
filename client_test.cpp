#include <iostream>
#include <string>
using namespace std;
#include "comm_ZHT.h"

int main(int argc, char* argv[]) {
	cout << "Hello, I'm a client." << endl;
	int sock = makeClientSocket("127.0.0.1", 50000);
	string str = "Msg.";
	int max = 10*1024*1024;
	char* big = (char*)malloc(max*sizeof(char));
	memset(big, 'z', max);
	str.assign(big);
	sendTo(sock, str, str.length());

	return 0;
}
