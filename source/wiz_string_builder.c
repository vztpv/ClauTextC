
#define _CRT_SECURE_NO_WARNINGS

#include "wiz_string_builder.h"

char* end(wiz_string_builder* builder)
{
	return builder->buffer_first + builder->capacity;
}

void init_wiz_string_builder(wiz_string_builder* builder, const size_t buffer_size, const char* cstr, const int len)
{
	builder->buffer = (char*)malloc(sizeof(char) * (buffer_size + 1)); // 1 for '\0'
	builder->len = len;
	builder->capacity = buffer_size;
	memcpy(builder->buffer, cstr, builder->len);
	builder->buffer[builder->len] = '\0';
	builder->buffer_first = builder->buffer;
}
void erase_wiz_string_builder(wiz_string_builder* builder)
{
	free(builder->buffer);
	builder->len = 0;
	builder->buffer = NULL;
	builder->buffer_first = NULL;
}

wiz_string_builder* append_wiz_string_builder(wiz_string_builder* builder, const char* cstr, const int len)
{
	if (builder->buffer + builder->len + len < end(builder))
	{
		memcpy(builder->buffer + builder->len, cstr, len);
		builder->buffer[builder->len + len] = '\0';
		builder->len = builder->len + len;
	}
	else {
		if (builder->buffer_first + builder->len + len < end(builder)) {
			memmove(builder->buffer_first, builder->buffer, builder->len);
			memcpy(builder->buffer_first + builder->len, cstr, len);
			builder->buffer_first[builder->len + len] = '\0';
			builder->buffer = builder->buffer_first;
			builder->len = builder->len + len;
		}
		else {
			char* new_buffer = (char*)malloc(sizeof(char) * (builder->len + len + 1));
			memcpy(new_buffer, builder->buffer, builder->len);
			memcpy(new_buffer + builder->len, cstr, len);
			new_buffer[builder->len + len] = '\0';
			free(builder->buffer);
			builder->buffer = new_buffer;
			builder->buffer_first = builder->buffer;
			builder->len = builder->len + len;
		}
	}
	return builder;
}

char* divide_wiz_string_builder(wiz_string_builder* builder, const int idx) // need to rename!l, chk idx range!
{
	builder->buffer[idx] = '\0';
	return builder->buffer;
}
char* str_wiz_string_builder(wiz_string_builder* builder, int* size)
{
	if (size) { *size = builder->len; }
	return builder->buffer;
}

void clear_wiz_string_builder(wiz_string_builder* builder)
{
	builder->len = 0;
	builder->buffer = builder->buffer_first;
	builder->buffer[0] = '\0';
}

size_t size_wiz_string_builder(wiz_string_builder* builder)
{
	return builder->len;
}

wiz_string_builder* left_shift_wiz_string_builder(wiz_string_builder* builder, const int offset)
{
	if (offset < 1) { return builder; }

	if (builder->buffer + offset < end(builder)) {
		builder->buffer = builder->buffer + offset;
	}
	else {
		memmove(builder->buffer_first, builder->buffer + offset, builder->len - offset);
		builder->buffer = builder->buffer_first;
		builder->buffer[builder->len - offset] = '\0';
	}
	builder->len = builder->len - offset;
	return builder;
}

