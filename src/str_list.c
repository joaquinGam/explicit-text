#include <stdlib.h>
#include <stdio.h>
#include "../include/str_list.h"

void str_list_add(str_list_t *list, char *str){
  str_list_t tmp = *list;
  *list = malloc(sizeof(str_list_t));
  (*list)->element = str;
  (*list)->next = tmp;
}

void str_list_free(str_list_t *list){
  if ((*list) != NULL){
    free((*list)->element);
    str_list_free(&((*list)->next));
    free((*list));
  }
}

int str_list_len(str_list_t list){
  if (list != NULL){
    return (1+str_list_len(list->next));
  } else {
    return 0;
  }
}
