/*Создайте очередь сообщения SysV с ключем:
key_t key = ftok("/tmp/msg.temp", 0);
Тестовая среда пошлет в нее сообщение.
struct message {
    long mtype;
    char mtext[80];
}
Получите его и сохраните в /home/box/message.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 1024

struct message {
    long mtype;
    char mtext[80];
};

int main(int argc, char** argv){
	key_t key = ftok("/tmp/msg.temp", 0);
	if(key < 0) {
		perror("Error getting token");
		return 1;
	}
	
	int msqid = msgget(key, IPC_CREAT | 0660);
	if(msqid < 0) {
		perror("Error creating message queue");
		return 2;
	}
	
	struct message strm;
	memset(&strm, 0, sizeof(struct message));
	strm.mtype = 1;
	
	
	if(argc > 1) strncpy(strm.mtext, argv[1], 80);
	else strncpy(strm.mtext, "Hello!", 80);
	
	int res = msgsnd(msqid, &strm, sizeof(struct message) - sizeof(long), 0);
	
	return 0;
}
