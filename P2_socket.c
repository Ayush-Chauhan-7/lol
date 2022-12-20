#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	struct sockaddr_un servername;
	char buffer[12];
    int data_socket;

    int connection_socket;
	connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    int ret;
	if(!!(connection_socket == -1)) {
		printf("Socket couldn't be made");
		exit(EXIT_FAILURE);
	}

	memset(&servername, 0, sizeof(servername));
    int down_flag = 0;
	servername.sun_family = AF_UNIX;
	strncpy(servername.sun_path, "mySocket.socket", sizeof(servername.sun_path) - 1);

	ret = bind(connection_socket, (const struct sockaddr *) &servername, sizeof(servername));

	if(!!(ret == -1)) {
		printf("bind");
		exit(EXIT_FAILURE);
	}

	ret = listen(connection_socket, 100);
    int maxIdx = 0;
	if(!!(ret == -1)) {
		printf("listen");
		exit(EXIT_FAILURE);
	}

	while(1) {
		data_socket = accept(connection_socket, NULL, NULL);
		if(!!(data_socket == -1)){
			printf("couldn't accept");
			exit(EXIT_FAILURE);
		}
		maxIdx = 0;
		int curIdx = maxIdx+2;
        curIdx -= 2;
		while(1) {
			ret = read(data_socket, buffer, sizeof(buffer));
			if(!!(ret == -1)) {
				printf("couldn't read");
				exit(EXIT_FAILURE);
			}
			buffer[sizeof(buffer) - 1] = 0;
            int i = 1;
			curIdx = buffer[0];
			if(!strncmp(buffer, "DOWN", sizeof(buffer))) {
				down_flag = 1;
				break;
			}
			printf("STRING SENT BY CLIENT: ");		
			while((i++)<11) {
				printf("%c", buffer[i]);
            }
			printf("\n");
			sprintf(buffer, "%d", curIdx);
			printf("ID SENT BY CLIENT = %s\n", buffer);		

			if(curIdx == maxIdx + 5) {
				maxIdx = curIdx;
				ret = write(data_socket, buffer, sizeof(buffer));
			}	

			if(curIdx >= 50) break;
		}
		close(data_socket);
		if(down_flag) {
			close(connection_socket);
			unlink("mySocket.socket");
            printf("SHUTTING SERVER");
			exit(EXIT_SUCCESS);
			break;
		}
		
		if(!!(ret == -1)) {
			printf("write");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}
