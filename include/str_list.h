#ifndef STR_LIST_H
  typedef struct str_list_node {
    char *element;
    struct str_list_node *next;
  } str_list_node_t;

  typedef str_list_node_t *str_list_t;

  void str_list_add(str_list_t *list, char *str);
  void str_list_free(str_list_t *list);
  int str_list_len(str_list_t list);

  #define STR_LIST_H
  #ifndef NULL
    #define NULL 0
  #endif
#endif
