#ifndef _FILE_H_
#define _FILE_H_

#include <stdbool.h>
#include "list.h"

bool save(LIST* lst, const char* filename);

bool load(LIST** lst, const char* filename);

bool serialize(LIST* lst, const char* filename);

bool deserialize(LIST** lst, const char* filename);

#endif