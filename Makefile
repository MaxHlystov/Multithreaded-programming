all: send recv

send:
	g++ -std=c++11 -o send sysv_msg_send.cpp

recv:
	g++ -std=c++11 -o recv sysv_msg_recv.cpp
 
