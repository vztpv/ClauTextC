
#define _CRT_SECURE_NO_WARNINGS


#include <vld.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


// wiz_string
#include "wiz_string.h"

// wiz_string_builder
#include "wiz_string_builder.h"

// Token
#include "wiz_load_data_token.h"
#include "wiz_array_queue_token.h"

// ItemType
#include "wiz_load_data_item_type.h"

// user_type
#include "wiz_load_data_user_type.h"

// wiz_vector for int, double, wiz_string, long long, long double, ItemType, user_type, Any?
#include "wiz_vector.h"

//
#include "wiz_load_data_token.h"
#include "pair.h"

#include "wiz_load_data.h"

#include "utility.h"

// wiz_array_stack : based wiz_vector
// wiz_deck for string( can random access )

// wiz_set // using red_black tree? or sorting + binarysearch tree
// wiz_map // using red_black tree? or sorting + binarysearch tree

// iterator -> using idx?



// user_type



wiz_string excute_module(wiz_string* mainStr, user_type* _global, ExcuteData* excuteData)
{
	// todo!
}


void test_for_wiz_vector()
{
	wiz_vector_int test;
	int i = 0;
	init_wiz_vector_int(&test, 1);

	for (i = 0; i < 100; ++i) {
		push_back_wiz_vector_int(&test, &i);
		printf("%d\n", *back_wiz_vector_int(&test));
	}

	free_wiz_vector_int(&test);
}


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

		free_wiz_string(&D);
		init_wiz_string(&D, get_cstr_wiz_string(&C), C.len);
		free_wiz_string(&C);
	}
	b = clock();

	printf("%d ms\n %s", b - a, get_cstr_wiz_string(&D));

	free_wiz_string(&D);
	free_wiz_string(&_A);
	free_wiz_string(&_B);
}

void test()
{
	srand(time(NULL));
	//test_for_wiz_string();
	//test_for_wiz_vector();
	{
		wiz_string fileName;
		wiz_string fileName2;
		user_type global;

		init_wiz_string(&fileName, "input.eu4", 9);
		global = load_data_from_file_in_load_data(&fileName);


		init_wiz_string(&fileName2, "output.txt", 10);

		save_data_in_load_data(&fileName2, &global, 2);

		free_wiz_string(&fileName);
		free_wiz_string(&fileName2);

		free_user_type_in_user_type(&global);
	}

	{
		user_type global;
		wiz_string str;

		init_wiz_string(&str, "test=3", 6);
		global = load_data_from_string_in_load_data(&str);

		free_wiz_string(&str);
		free_user_type_in_user_type(&global);
	}
	{
		// excute_module
		// to_bool4
		// to_bool3
		// map
	}
}
void test_for_load_data()
{
	const char* text = "$OR = { "
		" $EQ = { 3 0 } "
		" $EQ = { 4 $add = { 1 5 } } } ";
	wiz_string str;
	user_type ut;
	size_t i;

	init_wiz_string(&str, text, strlen(text));

	for (i = 0; i < 50000; ++i) {
		ut = load_data_from_string_in_load_data(&str);
		free_user_type_in_user_type(&ut);
	}
	free_wiz_string(&str);
}

int main(void)
{

	{
		for (int i = 0; i < 500000; ++i) {
			const char* text = "id = 1 i = 3 j = 4";
			wiz_string str;
			wiz_string result;
			user_type global;
			ExcuteData excuteData;
			wiz_string_builder builder;
			wiz_string EMPTY = make_wiz_string("", 0);
			init_wiz_string(&str, text, strlen(text));
			init_in_user_type(&global, &EMPTY);
			init_wiz_string_builder(&builder, 1024, "", 0);

			excuteData.valid = 0;

		
			result = ToBool4(NULL, &global, &str, &excuteData, &builder);


			free_wiz_string(&str);
			free_wiz_string(&result);
			free_user_type_in_user_type(&global);
			free_wiz_string_builder(&builder);
			free_wiz_string(&EMPTY);
		}
	}
	return 0;
}
