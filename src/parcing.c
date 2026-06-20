#include "header.h"


char **parse_argv(char *argv){
    static int argc = 0;
    char **res;
    int i = 0;
    while  (argv[i] != '\0'){
        if ( argv[i] != ' ' ){
            while (argv[i] != ' '){
                i++;
            }
            argc ++;
            
        }
        if ( argv [i] == ' '){
            while (argv[i] == ' '){
                i ++;
            }
            
        }        
    }
    //argc++;
    printf("argc %d \n " ,argc);
    res = malloc(sizeof(char  ) * argc);
    

    return res;
}