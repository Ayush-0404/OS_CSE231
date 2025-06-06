#include <stdio.h>
#include <string.h>

int wordcount(const char *filename) {
    int wordCount = 0;
    int insideWord = 0; // Flag to indicate if we are currently inside a word
    char ch;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File could not be opened\n");
        return -1; // Returning -1 to indicate an error
    } else {
        while ((ch = fgetc(file)) != EOF) {
            if (ch == ' ' || ch == '\t' || ch == '\n') {
                // If we were inside a word, increment word count and reset insideWord flag
                if (insideWord) {
                    insideWord = 0;
                    wordCount++;
                }
            } else {
                insideWord = 1; // We are inside a word
            }
        }
        // Check if a word continues until the end of the file
        if (insideWord) {
            wordCount++;
        }
    }
    fclose(file);
    return wordCount;
}

void count(char *filename,int detailed){
    if(detailed==1){
        int wn = wordcount(filename);
        printf("no. of words : %d\n",wn);
    }
    else{
        int wn = wordcount(filename);
        printf("no. of words : %d\n",wn);
    }
}

void count1(char *filename,char *filename1){
    int wn1 = wordcount(filename);
    int wn2 = wordcount(filename1);
    if(wn1>wn2){
        printf("Difference of words is : %d\n",(wn1-wn2));
    }
    else{
        printf("Difference of words is : %d \n",(wn2-wn1));
    }
}

int main(int argc, char** argv) 
{
    if (argc > 1 && strcmp(argv[1], "word") == 0) {
        if (argc == 4 && strcmp(argv[2], "-n") == 0) {
            count(argv[3], 0); // Detailed timestamp
        }else if (argc == 5 && strcmp(argv[2], "-d")==0) {
            count1(argv[3], argv[4]); // Regular timestamp
        } else if (argc == 3 ) {
            count(argv[2], 1); // Yesterday timestamp
        } else {
            printf("The command is incorrect\n");
        }
    } else {
        printf("Command does not start with 'word'\n");
    }
    
    return 0;
}
