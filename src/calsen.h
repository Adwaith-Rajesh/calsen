#ifndef _INCLUDE_CALSEN_H
#define _INCLUDE_CALSEN_H

#include "linked_list.h"

void calsen_index_files(LinkedList *dir_list, const char *output_file);
LinkedList *search(const char *query, const char *index_file);

#endif
