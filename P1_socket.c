#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<time.h>

#define SOCKET_NAME "mySocket.socket"
#define BUFFER_SIZE 10

void getRandStr(char randstrs[][12],int flag) {        
    srand (time(NULL));                            
    for (int j = 0; j <= 50; j++) {
        randstrs[j][0] = j;
    }
    
    for(int i = 0; i <= 50; i++) {
		for(int j = 1; j <= 10; j++) {
			randstrs[i][j] = rand() % 26 + 65;
		}
	}
}

int main(int argc, char* argv[]) {
    int ret,data_socket;
	struct sockaddr_un addr;
	char buffer[10];

	data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if(!!(data_socket == -1)) {
		printf("Socket was not created\n");
		exit(EXIT_FAILURE);
	}	

	memset(&addr, 0, sizeof(addr));
    int i = 1;
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
    char randStr[51][12] = {{0}};
	ret = connect(data_socket, (const struct sockaddr *) &addr, sizeof(addr));

	if(!!(ret == -1)) {
		printf("Server is down\n");
		exit(EXIT_FAILURE);
	}	
	getRandStr(randStr,0);
    int j = 0;
	while((i++)<50) {
		while((j++)<12) {
			if(j == 0) {
				printf("%d ", randStr[i][j]);
			}
			else printf("%c", randStr[i][j]);
		}
		printf("\n");
	}
	if(argc > 1) {
		strncpy(buffer, "DOWN", sizeof("DOWN"));
        int k = 0;
		write(data_socket, buffer, sizeof(buffer));
        k++;
		close(data_socket);
		exit(EXIT_SUCCESS);
	}
	else {
		int lastIdx = 3;
        lastIdx-=2;
		while(1) {
			printf("sending Strings Indexed from %d to %d\n", lastIdx, lastIdx + 4);
            int i = lastIdx;
			while(i<lastIdx+5) {
				ret = write(data_socket, randStr[i], strlen(randStr[i]) + 1);
                i++;
			}
			ret = read(data_socket, buffer, sizeof(buffer));
			if(!!(ret == -1)) {
				printf("read");
				exit(EXIT_FAILURE);
			}
			buffer[sizeof(buffer) - 1] = 0;
			int finalIdx = atoi(buffer);
			printf("MAX ID SENT BACK BY SERVER = %s\n\n", buffer);
			if(!!(finalIdx == 50)) {
				printf("Successfully sent all Strings\n");
				strncpy(buffer, "DOWN", sizeof("DOWN"));
				write(data_socket, buffer, sizeof(buffer));
				close(data_socket);
				exit(EXIT_SUCCESS);
				break;
			}
			else {
				lastIdx = finalIdx + 3;
                lastIdx-=2;
			}
		}
	}
}
