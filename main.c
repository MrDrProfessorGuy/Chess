//
// Created by guy cohen on 20/05/2021.
//
#include <stdlib.h>
#include <stdio.h>
#include "chessSystem.h"

static int count_digits(int num){
    int count = 0;
    do{
        num = num / 10;
        count++;
    }while (num != 0);
    return count;
}


static char* id_to_str(int num){
    int digits = count_digits(num);
    char* str = malloc(sizeof(char) * (digits + 1));// for null terminator
    if (str == NULL){
        return NULL;
    }
    
    str[digits] = '\n';
    int index = digits - 1;
    do{
        str[index] = (num % 10) + '0';
        num = num / 10;
        index--;
    }while (index >= 0);
    
    return str;
}

int main(){
    int num = -1;
    int count = count_digits(num);
    printf("count_digits = %d\n", count);
    char* str = id_to_str(num);
    printf("id_to_str(): %s\n", str);
    free(str);
    
    return 0;
}





