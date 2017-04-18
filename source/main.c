
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// wiz_string
// wiz_any
// wiz_vector for int, long long, long double, wiz_string, ItemType, UserType, Any?
// wiz_array
// wiz_secondary_array : based wiz_array
// wiz_array_stack : based wiz_array
// wiz_array_deque( can random access ) : based wiz_array
// wiz_list : double linked list? or wiz::Deck?
// wiz_set // using red_black tree? or sorting + binarysearch tree
// wiz_map // using red_black tree? or sorting + binarysearch tree
// iterator -> using idx?


#define wiz_string_buffer_size 1024

struct wiz_string
{
	char buffer[wiz_string_buffer_size + 1];
	char* str;
	size_t len;
	// int chk; // for DEBUG?
	// size_t capacity; // ?
};
typedef struct wiz_string wiz_string;

// cstr cf) "" and NULL
void init_wiz_string(wiz_string* str, char* cstr, int n)
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
void erase_wiz_string(wiz_string* str)
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
		erase_wiz_string(str1);
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

		erase_wiz_string(str1);
		
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
	erase_wiz_string(str);
	
	*str = temp;
}

void test_for_wiz_vector()
{

}


#include <stdlib.h>
#include <time.h>

void test_for_wiz_string()
{
	const int SIZE = 1000000;
	wiz_string _A;
	wiz_string _B; 
	char* temp1 = "wow                         ";
	char* temp2 = " Hello                      ";
	init_wiz_string(&_A, temp1, strlen(temp1));
	init_wiz_string(&_B, temp2, strlen(temp2));
	
	int a, b;
	wiz_string C, D;
	

	a = clock();
	for (int t = 0; t < SIZE; ++t) {
		init_wiz_string(&C, _A.str, _A.len);
		init_wiz_string(&D, _A.str, _A.len);
		concat_and_assign_wiz_string(&C, &_B);
		concat_and_assign_wiz_string(&D, &_B);
		concat_and_assign_wiz_string(&C, &D);

		erase_wiz_string(&D);
		init_wiz_string(&D, get_cstr_wiz_string(&C), C.len);
		erase_wiz_string(&C);
	}
	b = clock();

	printf("%d ms\n %s", b - a, get_cstr_wiz_string(&D));

	erase_wiz_string(&D);
	erase_wiz_string(&_A);
	erase_wiz_string(&_B);
}

int main(void)
{
	test_for_wiz_string();
	test_for_wiz_vector();

	getchar();
	return 0;
}
