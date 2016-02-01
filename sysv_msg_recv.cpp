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
	
	struct message msg;
	memset(&msg, 0, sizeof(struct message));
	
	ssize_t recv_len = msgrcv(msqid, &msg, sizeof(struct message) - sizeof(long), 0, 0);
	if(recv_len < 0){
		perror("Error recieving message");
		return 3;
	}
	
	msgctl(msqid, IPC_RMID, 0);
	
	int fd = open("/home/box/message.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR| S_IRGRP| S_IWGRP| S_IXGRP| S_IROTH | S_IWOTH | S_IXOTH); // /home/box/message.txt
	if(fd < 0){
		perror("Error creating file");
		return 4;
	}
	
	write(fd, msg.mtext, recv_len);
	close(fd);
	
	return 0;
}
