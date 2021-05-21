//
// Created by guy cohen on 20/05/2021.
//
#include <stdlib.h>
#include <stdio.h>
#include "chessSystem.h"


#define PLAYER_SEPARATOR '#'

static int countDigits(int num){
    int count = 0;
    do{
        num = num / 10;
        count++;
    }while (num != 0);
    return count;
}


static char* intToSting(int player1_id, int player2_id){
    int player1_digits = countDigits(player1_id);
    int player2_digits = countDigits(player2_id);
    // 1 for null terminator and 1 for PLAYER_SEPARATOR (a.k.a '#')
    char* str = malloc(sizeof(char) * (player1_digits + player2_digits + 2));
    if (str == NULL){
        return NULL;
    }
    
    
    int index = player1_digits - 1;
    do{
        str[index] = (char)((player1_id % 10) + '0');
        player1_id = player1_id / 10;
        index--;
    }while (index >= 0);
    str[player1_digits] = PLAYER_SEPARATOR;
    index = player1_digits + player2_digits + 1;
    str[index--] = '\n';
    do{
        str[index] = (char)((player2_id % 10) + '0');
        player2_id = player2_id / 10;
        index--;
    }while (index > player1_digits);
    
    return str;
}

int main(){
    int num = 123;
    int count = countDigits(num);
    printf("count_digits = %d\n", count);
    char* str = intToSting(num, num+1);
    printf("id_to_str(): %s\n", str);
    free(str);
    
    return 0;
}





