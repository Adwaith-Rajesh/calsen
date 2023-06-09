#ifndef _INCLUDE_PATH_H
#define _INCLUDE_PATH_H

int is_dir(const char *pathname);
char *get_absolute_path(const char *path, char *resolved_path);
int get_file_size(const char *path);

#endif
