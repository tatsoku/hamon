#ifndef ENV_H
#define ENV_H

#include <stdbool.h>

extern char *env[4096];
extern int envc;

bool is_env_format(const char *str);
void init_env();
void deinit_env();

#endif // !ENV_H
