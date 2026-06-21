#include "header.h"


char **parse_argv(char *argv){
    int argc = 0;
    char **res;
    int i = 0;
    while  (argv[i] != '\0'){
        if ( argv[i] != ' ' ){
            while ((argv[i] != ' ') && (argv[i] != '\0'))
                i++;
            argc ++;
            
        }
        else {
            while ((argv[i] == ' ') && (argv[i] != '\0'))
                i ++;
        }        
    }
    
    printf("argc %d \n " ,argc);
    res = malloc(sizeof(char  ) * (argc +1) );
    if (!res)return NULL;
    

    return res;
}