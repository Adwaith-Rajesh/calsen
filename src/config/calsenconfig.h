// License: GPL v3 Copyright: 2023, Adwaith Rajesh <adwaithrajesh3180@gmail.com>

#ifndef INCLUDE_CALSENCONFIG_H
#define INCLUDE_CALSENCONFIG_H

#include <linux/limits.h>

#ifndef CALSENCONFIG
#define CALSENCONFIG "./calsenconfig"
#endif

#ifndef LOAD_CONFIG
#define LOAD_CONFIG 1
#endif

typedef struct {
    char parsers_dir[PATH_MAX];
    char ignore_file[PATH_MAX];
    char config_file[PATH_MAX];
    char index_file[PATH_MAX];
} config_t;

config_t *get_calsen_config();

#endif
