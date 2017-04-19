
#ifndef WIZ_STRING_BUILDER_H
#define WIZ_STRING_BUILDER_H

#include "wiz_string.h"

struct wiz_string_builder
{
	char* buffer_first;
	char* buffer;
	size_t len;
	size_t capacity;
};

typedef struct wiz_string_builder wiz_string_builder;

void init_wiz_string_builder(wiz_string_builder* builder, const size_t buffer_size, const char* cstr, const int len);
void free_wiz_string_builder(wiz_string_builder* builder);

wiz_string_builder* append_wiz_string_builder(wiz_string_builder* builder, const char* cstr, const int len);

char* divide_wiz_string_builder(wiz_string_builder* builder, const int idx); // need to rename!l, chk idx range!

char* str_wiz_string_builder(wiz_string_builder* builder, int* size);

void clear_wiz_string_builder(wiz_string_builder* builder);

size_t size_wiz_string_builder(wiz_string_builder* builder);

wiz_string_builder* left_shift_wiz_string_builder(wiz_string_builder* builder, const int offset);


#endif
