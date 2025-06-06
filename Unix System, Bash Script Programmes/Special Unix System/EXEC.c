#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

void printLastEditTime(const char *filename, int detailed) {
    struct stat fileStat;

    if (stat(filename, &fileStat) == -1) {
        printf("File doesn't exists\n");
        return;
    }
   
    time_t lastModificationTime = fileStat.st_mtime;
    if(detailed == 2){
	   lastModificationTime -= 24*60*68;
    } 
    struct tm *timeInfo = localtime(&lastModificationTime);
    char buffer[80];

    if (detailed == 1) {
        strftime(buffer, sizeof(buffer), "%A %B %d %I:%M:%S %p %Z %Y", timeInfo);
    } else if (detailed == 0 ) {
        strftime(buffer, sizeof(buffer), "%A, %d %B %Y %H:%M:%S %z", timeInfo);
    }
    else if (detailed == 2){
      
        strftime(buffer, sizeof(buffer), "%A %B %d %I:%M:%S %p %Z %Y", timeInfo);
    }

    printf("Last Edit Time of %s: %s\n", filename, buffer);
}

int main(int argc, char** argv) 
{   
       	if ( strcmp(argv[1], "date") == 0) {
        if (argc == 4 && strcmp(argv[2], "-R") == 0) {
            printLastEditTime(argv[3], 0); // Detailed timestamp
        } else if (argc == 3) {
            printLastEditTime(argv[2], 1); // Regular timestamp
        } else if (argc == 5 && strcmp(argv[2], "-d") == 0) {
            printLastEditTime(argv[4], 2); // Yesterday timestamp
        } else {
            printf("The command is incorrect\n");
        }
    } else {
        printf("Command does not start with 'date'\n");
    }
    
    return 0;
}
