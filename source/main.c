
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// wiz_string
#include "wiz_string.h"
// wiz_any
// wiz_vector for int, long long, long double, wiz_string, ItemType, UserType, Any?
#include "wiz_vector.h"

// wiz_array
// wiz_secondary_array : based wiz_array
// wiz_array_stack : based wiz_array
// wiz_array_deque( can random access ) : based wiz_array
// wiz_list : double linked list? or wiz::Deck?
// wiz_set // using red_black tree? or sorting + binarysearch tree
// wiz_map // using red_black tree? or sorting + binarysearch tree
// iterator -> using idx?

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
