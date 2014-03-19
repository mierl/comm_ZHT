src=$1
fileName=`echo $src| awk -F "." ' {print $1}'`
rm $fileName

g++ -g $src comm_ZHT.cpp bigdata_transfer.cpp Env.cpp ZHTUtil.cpp ConfHandler.cpp ConfEntry.cpp Const.cpp Util.cpp StrTokenizer.cpp -lprotobuf zpack.pb.cc -lpthread -I$USER_INCLUDE -L$USER_LIB -o $fileName
