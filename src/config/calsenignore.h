#ifndef INCLUDE_CALSENIGNORE_H
#define INCLUDE_CALSENIGNORE_H

#include "linked_list.h"

LinkedList *parse_ignore_file(const char *filepath);
int check_file_name_is_ignored(LinkedList *patterns, const char *str);
void drop_pattern_list_cache();

#endif