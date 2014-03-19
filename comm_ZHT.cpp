/*
 * comm_ZHT.h
 *
 *  Created on: Feb 22, 2014
 *      Author: tony
 */

#ifndef COMM_ZHT_H_
#define COMM_ZHT_H_

#endif /* COMM_ZHT_H_ */

#include <errno.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>

#include "comm_ZHT.h"
//#include "ip_proxy_stub.h"
#include "bigdata_transfer.h"
#include "Env.h"
#include "ZHTUtil.h"
#include "ConfHandler.h"
#include "ConfEntry.h"
#include "Const.h"
#include "zpack.pb.h"

int sendTo(int sock, const void* sendbuf, int sendcount);
int sendTo(int sock, const string sendbuf, int sendcount);
int recvFrom(int sock, void* recvbuf);
int recvFrom(int sock, string& result);
int makeClientSocket(const string& host, const uint& port);
//int sendRecv(string host, int pot, const void* sendBuff, void* recvBuff); // wrap BdSendBase::bsend(int sock, void *senderAddr) const
//int recvSend(int sock, void *senderAddr, string &srec); // wrap int IPProtoProxy::loopedrecv(int sock, void *senderAddr, string &srecv)

//Copied from ip_proxy_stub.cpp
int loopedrecv(int sock, void *senderAddr, string &srecv) {
	//cout << "-1" << endl;

	ssize_t recvcount = -2;
	socklen_t addr_len = sizeof(struct sockaddr);
//	socklen_t addr_len = sizeof( sockaddr);

	BdRecvBase *pbrb = new BdRecvFromServer();

	char buf[Env::BUF_SIZE];// Tony: cause error when using big message. should be using char*.
	//char* buf = (char*) calloc(1, Env::BUF_SIZE); // Tony: array doesn't allow more than 1MB allocation.

	while (1) {

		memset(buf, '\0', sizeof(buf));

		ssize_t count;
		if (senderAddr == NULL) {
			//cout << "-5" << endl;

			count = ::recv(sock, buf, sizeof(buf), 0);
			//cout<<"-7"<<endl;

		} else {
			count = ::recvfrom(sock, buf, sizeof(buf), 0,
					(struct sockaddr *) senderAddr, &addr_len);
		}

		if (count == -1 || count == 0) {

			recvcount = count;

			break;
		}

		bool ready = false;
		//cout<<"-7.5"<<endl;

		string bd = pbrb->getBdStr(sock, buf, count, ready);
		usleep(1); //This is needed to avoid a weird error, especially when running locally. Remote works should work.
		if (ready) {
			//cout<<"-10"<<endl;

			srecv = bd;
			recvcount = srecv.size();

			break;
		}
		//cout<<"-11"<<endl;

		memset(buf, '\0', sizeof(buf));
	}

	delete pbrb;
	pbrb = NULL;

	return recvcount;
}

//Copied from TCPProxy
int makeClientSocket(const string& host, const uint& port) {

	struct sockaddr_in dest;
	memset(&dest, 0, sizeof(struct sockaddr_in)); /*zero the struct*/
	dest.sin_family = PF_INET; /*storing the server info in sockaddr_in structure*/
	dest.sin_port = htons(port);

	struct hostent * hinfo = gethostbyname(host.c_str());
	if (hinfo == NULL) {
		cerr << "comm_ZHT:makeClientSocket(): ";
		herror(host.c_str());
		return -1;
	}

	memcpy(&dest.sin_addr, hinfo->h_addr, sizeof(dest.sin_addr));

	int to_sock = socket(PF_INET, SOCK_STREAM, 0); //try change here.................................................

	if (to_sock < 0) {

		cerr << "comm_ZHT:makeClientSocket(): error on ::socket(...): "
				<< strerror(errno) << endl;
		return -1;
	}

	int ret_con = connect(to_sock, (struct sockaddr *) &dest, sizeof(sockaddr));

	if (ret_con < 0) {

		cerr << "comm_ZHT:makeClientSocket(): error on ::connect(...): "
				<< strerror(errno) << endl;
		return -1;
	}

	return to_sock;
}

int sendTo(int sock, const string sendbuf, int sendcount) {
	void* buff = (void*) calloc(1, sendbuf.size());
	memcpy(buff, sendbuf.c_str(), sendbuf.size());
	int ret = sendTo(sock, (const void*) buff, sendbuf.size());
	free(buff);
	return ret;
}

int sendTo(int sock, const void* sendbuf, int sendcount) {

	BdSendBase *pbsb = new BdSendToServer((char*) sendbuf);
	int sentSize = pbsb->bsend(sock);
	delete pbsb;
	pbsb = NULL;

	//prompt errors
	if (sentSize < sendcount) {

		//todo: bug prone
		/*cerr << "TCPProxy::sendTo(): error on BdSendToServer::bsend(...): "
		 << strerror(errno) << endl;*/
	}

	return sentSize;
}

//TCP only
int recvFrom(int sock, string& result) {
	int recvcount = loopedrecv(sock, NULL, result);
	return recvcount;
}

int recvFrom(int sock, void* recvbuf) {

	string result;
	int recvcount = loopedrecv(sock, NULL, result);

	memcpy(recvbuf, result.c_str(), result.size());

	//prompt errors
	if (recvcount < 0) {

		cerr << "TCPProxy::recvFrom(): error on loopedrecv(...): "
				<< strerror(errno) << endl;
	}

	return recvcount;
}

using namespace std;
/*
 int main(int argc, char* argv[]) {
 cout << "comm_ZHT test.\n";

 }
 */
