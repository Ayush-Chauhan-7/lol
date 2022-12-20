#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
void sync(void);
//client
//P2 FIFO 
struct pair {
    int ID;
    char str_value[7];
};
int main()
{
    mkfifo("/tmp/randomStrings_2020026_FIFO", 0666);
    int i=0;
    while ((i++)<10)
    {
        struct pair receiving[5];
        printf("Reading strings from FIFO file\n");
        int fd = open("/tmp/randomStrings_2020026_FIFO", O_RDONLY);
        read(fd, receiving, sizeof(receiving));
        struct pair maxIDstring;
        int maxID = INT_MIN;
        for(int j=0; j<(sizeof(receiving)/sizeof(struct pair)); j++){
            printf("String ID: %d\n", receiving[j].ID);
            printf("String value: %s\n", receiving[j].str_value);
            if(receiving[j].ID>maxID){
                maxID = receiving[j].ID;
                maxIDstring.ID = maxID;
                strncpy(maxIDstring.str_value, receiving[j].str_value, sizeof(receiving[j].str_value));                
            }
        }
        close(fd);
        printf("Writing the string with highest ID into the FIFO file\n\n");
        fd = open("/tmp/randomStrings_2020026_FIFO", O_WRONLY);
        write(fd, &maxIDstring , sizeof(maxIDstring));
        close(fd);
    }
    unlink("/tmp/randomStrings_2020026_FIFO");
    exit(EXIT_SUCCESS);
    return 0;
}
