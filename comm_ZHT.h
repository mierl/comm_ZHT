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
