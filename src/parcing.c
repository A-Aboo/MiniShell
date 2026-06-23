#include "header.h"


char **parse_argv(char *line){
    int argc = 0;
    char **res;
    int i = 0;
    int len = 0;
    int j = 0;
    int start = 0;
    int count = 0;
    while  (line[i] != '\0'){
        if ( line[i] != ' ' ){
            while ((line[i] != ' ') && (line[i] != '\0'))
                i++;
            argc ++;
            
        }
        else {
            while ((line[i] == ' ') && (line[i] != '\0'))
                i ++;
        }        
    }
    
    printf("argc %d \n " ,argc);
    res = malloc(sizeof(char  *) * (argc +1) );
    if (!res)return NULL;
    i  = 0 ;
    while (line[i]){
        while (line[i] == ' ' ) i++;

        if (line[i] != ' ' &&  line[i]){
            start = i;
            while (line[i] && line[i] != ' '){
                len++;
                i++;
            }
            res[j] = malloc(sizeof(char ) * (len + 1 ));
            len =0 ;
            while(start < i){
                res[j][count++] = line[start++];
            }
            res[j++][count]  = '\0';
            count = 0 ;
        }
        
    }
    res[j] = NULL;
    

    return res;
}