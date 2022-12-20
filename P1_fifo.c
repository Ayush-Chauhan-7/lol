#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void sync(void);
struct pair {
    char str_value[7];
    int ID;
    int flag;
};

int main()
{
    char strings[50][7];
    char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int j=0;
    int i=0;
    while((i++)<50){
        while((j++)<6) {
            strings[i][j] = alphabet[rand() % 52];
        }
        strings[i][6]='\0';
    }
    int num=0;
    mkfifo("/tmp/randomStrings_2020026_FIFO", 0666);
    while(num<50) {
        int i = 0;
        struct pair sending[5];
        while((i++)<5){
            sending[i].ID = num+i;
            strncpy(sending[i].str_value, strings[sending[i].ID], sizeof(strings[sending[i].ID]));
        }
        int fd = open("/tmp/randomStrings_2020026_FIFO", O_WRONLY);
        printf("Writing the 5 arrays into the FIFO file ...\n");
        int write_ret = write(fd, sending, sizeof(sending));
        close(fd);
        struct pair p;
        fd = open("/tmp/randomStrings_2020026_FIFO", O_RDONLY);
        int read_ret = read(fd, &p, sizeof(p));
        if (read_ret == -1) {
            printf("Error\n");
            exit(EXIT_FAILURE);
        }
        printf("Resultant string received from P2 = %s\n\n", p.str_value);
        sleep(1);
        num = p.ID+1;
        close(fd);
    }
    unlink("/tmp/randomStrings_2020026_FIFO");
    return 0;
}
