
#define _CRT_SECURE_NO_WARNINGS
#include "wiz_string.h"


// cstr cf) "" and NULL
void init_wiz_string(wiz_string* str, const char* cstr, int n)
{
	if (0 == n) {
		str->len = 0;
		str->str = "";
		str->buffer[0] = '\0';
	}
	else {
		if (n > wiz_string_buffer_size) {
			str->str = malloc(sizeof(char)*(n + 1));
			strncpy(str->str, cstr, n + 1);
			str->len = n;
		}
		else {
			str->str = "";
			strncpy(str->buffer, cstr, n + 1);
			str->len = n;
		}
	}
}
void free_wiz_string(wiz_string* str)
{
	if (0 != str->len) {
		if (str->len > wiz_string_buffer_size) {
			free(str->str);
		}
		str->buffer[0] = '\0';
		str->str = "";
		str->len = 0;
	}
}
char* get_cstr_wiz_string(wiz_string* str)
{
	if (str->len > wiz_string_buffer_size) { return str->str; }
	return str->buffer;
}
size_t size_wiz_string(wiz_string* str) {
	return str->len;
}
int empty_wiz_string(wiz_string* str)
{
	return 0 == str->len;
}
wiz_string concat_wiz_string(wiz_string* str1, wiz_string* str2)
{
	wiz_string temp;

	temp.len = str1->len + str2->len;

	if (temp.len == 0) {
		temp.buffer[0] = '\0';
		temp.str = "";
	}
	else {
		if (temp.len > wiz_string_buffer_size) {
			temp.str = malloc(sizeof(char)*(temp.len + 1));
			strncpy(temp.str, get_cstr_wiz_string(str1), str1->len + 1);
			strncpy(temp.str + str1->len, get_cstr_wiz_string(str2), str2->len + 1);
		}
		else {
			strncpy(temp.buffer, get_cstr_wiz_string(str1), str1->len + 1);
			strncpy(temp.buffer + str1->len, get_cstr_wiz_string(str2), str2->len + 1);
		}
	}
	return temp;
}
wiz_string substr_wiz_string(wiz_string* str, size_t begin, size_t end)
{
	wiz_string temp;

	temp.len = (end - 1) - begin + 1;

	if (temp.len > wiz_string_buffer_size) {
		temp.str = malloc(sizeof(char) * (temp.len + 1));

		strncpy(temp.str, get_cstr_wiz_string(str) + begin, end - begin);
		temp.str[end] = '\0';
	}
	else {
		strncpy(temp.buffer, get_cstr_wiz_string(str), end - begin);
		temp.buffer[end] = '\0';
	}
	return temp;
}
void assign_wiz_string(wiz_string* str1, wiz_string* str2)
{
	// str1 != str2 
	if (str1->len != str2->len) {
		free_wiz_string(str1);
		str1->len = str2->len;
		if (str2->len > wiz_string_buffer_size) {
			str1->str = malloc(sizeof(char)*(str2->len + 1));
		}
	}
	strncpy(get_cstr_wiz_string(str1), get_cstr_wiz_string(str2), str2->len + 1);
}
// concat_and_assign_wiz_string  A = A + B
void concat_and_assign_wiz_string(wiz_string* str1, wiz_string* str2)
{
	const int len = str1->len + str2->len;
	wiz_string temp;

	if (empty_wiz_string(str1)) {
		assign_wiz_string(str1, str2);
	}
	else {
		temp.len = str1->len + str2->len;
		if (temp.len > wiz_string_buffer_size) {
			temp.str = malloc(sizeof(char)*(temp.len + 1));
		}
		strncpy(get_cstr_wiz_string(&temp), get_cstr_wiz_string(str1), str1->len + 1);
		strncpy(get_cstr_wiz_string(&temp) + str1->len, get_cstr_wiz_string(str2), str2->len + 1);

		free_wiz_string(str1);

		*str1 = temp;
	}
}

// substr_and_assign_wiz_string  A = A.substring();
void substr_and_assign_wiz_string(wiz_string* str, size_t begin, size_t end)
{
	wiz_string temp;
	char x = str->str[end - 1];
	temp.len = (end - 1) - begin + 1;

	if (temp.len > wiz_string_buffer_size) {
		temp.str = malloc(sizeof(char) * (temp.len + 1));
	}
	strncpy(get_cstr_wiz_string(&temp), get_cstr_wiz_string(str) + begin, end - begin + 1);

	//
	free_wiz_string(str);

	*str = temp;
}
