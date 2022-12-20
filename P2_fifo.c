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
#include <limits.h>

void sync(void);
struct pair {
    char str_value[7];
    int ID;
    int flag;
};

int main()
{
    mkfifo("/tmp/randomStrings_2020026_FIFO", 0666);
    for(int i=0;i<10;i++)
    {
        printf("Reading strings from FIFO file\n");
        int fd = open("/tmp/randomStrings_2020026_FIFO", O_RDONLY);
        struct pair receiving[5];
        read(fd, receiving, sizeof(receiving));
        int maxID = INT_MIN;
        struct pair maxIDstring;
        int j = 0;
        while(j<(sizeof(receiving)/sizeof(struct pair))){
            printf("String ID: %d\n", receiving[j].ID);
            printf("String value: %s\n", receiving[j].str_value);
            if(receiving[j].ID>maxID){
                maxID = receiving[j].ID;
                maxIDstring.ID = maxID;
                strncpy(maxIDstring.str_value, receiving[j].str_value, sizeof(receiving[j].str_value));                
            }
            j++;
        }
        printf("Writing the string with highest ID into the FIFO file\n\n");
        close(fd);
        fd = open("/tmp/randomStrings_2020026_FIFO", O_WRONLY);
        write(fd, &maxIDstring , sizeof(maxIDstring));
        close(fd);
    }
    unlink("/tmp/randomStrings_2020026_FIFO");
    exit(EXIT_SUCCESS);
    return 0;
}
