
#ifndef WIZ_STRING_BUILDER_H
#define WIZ_STRING_BUILDER_H

#include "wiz_string.h"

struct wiz_string_builder
{
	char* buffer;
	char* first;
	size_t capacity;
	size_t num;
};

void init_wiz_string_builder(wiz_string_builder* builder, size_t capacity);
wiz_string_builder* append_wiz_string_builder(wiz_string_builder* builder, wiz_string* str);

typedef struct wiz_string_builder wiz_string_builder;

#endif