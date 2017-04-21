
#define _CRT_SECURE_NO_WARNINGS


#include <vld.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



// wiz_string
#include "wiz_string.h"

// wiz_string_builder
#include "wiz_string_builder.h"

// Token
#include "wiz_load_data_token.h"
#include "wiz_array_queue_token.h"

// ItemType
#include "wiz_load_data_item_type.h"

// wiz_vector for int, double, wiz_string, long long, long double, ItemType, UserType, Any?
#include "wiz_vector.h"

// wiz_array : wiz_vector?

// wiz_array_stack : based wiz_array
// wiz_array_deque( can random access ) : based wiz_array -> ArrayQueue? (use pop_front and push_back! )
// wiz_list : double linked list? or wiz::Deck? - removal?
// wiz_set // using red_black tree? or sorting + binarysearch tree
// wiz_map // using red_black tree? or sorting + binarysearch tree
// iterator -> using idx?



// UserType

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

size_t find(const char* cstr, const char x, const size_t before, const size_t n)
{
	size_t i = 0;

	for (i = before+1; i < n; ++i) {
		if (cstr[i] == x) {
			return i;
		}
	}
	return -1;
}

int is_whitespace(const char ch)
{
	return ' ' == ch || '\t' == ch || '\r' == ch || '\n' == ch;
}
void test_for_wiz_string2(const char* fileName)
{
	wiz_array_queue_token aq;
	// getline remake?
	const size_t line_max = 10000; // line_max = 1 is very slow? - solved!
	const size_t num = 102400;
	wiz_vector_wiz_string vec_of_str;
	wiz_string_builder builder;
	wiz_string_builder temp_builder;
	FILE* file = fopen(fileName, "rt");
	size_t real_count;
	size_t find_idx; // find \n
	size_t i;
	size_t size;
	char* builder_cstr;
	wiz_string temp;
	size_t temp2;
	char* temp3;
	wiz_string temp4;
	int chk = 1;

	init_wiz_string(&temp4, "\n", 1);

	init_wiz_vector_wiz_string(&vec_of_str, 1024);
	init_wiz_string_builder(&builder, num * 10, "", 0);
	
	i = 0;
	temp2 = -1;

	while (1) {
		if (chk) {
			init_wiz_string_builder(&temp_builder, num, "", 0);
			real_count = fread((void*)(str_wiz_string_builder(&temp_builder, NULL)), sizeof(char), num, file);
			temp_builder.len = real_count;
			temp_builder.buffer[real_count] = '\0';


			append_wiz_string_builder(&builder, str_wiz_string_builder(&temp_builder, NULL), size_wiz_string_builder(&temp_builder));

			free_wiz_string_builder(&temp_builder);
		}
		builder_cstr = str_wiz_string_builder(&builder, &size);
		//printf("chk %d %d\n", strlen(builder_cstr), size);
		
		find_idx = -1;
		for (; i < line_max; ++i) {
			temp2 = find(builder_cstr, '\n', temp2, size);
			if (temp2 != (size_t)-1) {
				find_idx = temp2;
			}
			else {
				chk = 1;
				break;
			}
		}
		if (i == line_max || real_count != num) {
			if (find_idx != (size_t)-1) {
				divide_wiz_string_builder(&builder, find_idx);
				
				temp3 = str_wiz_string_builder(&builder, NULL);
				
			//	printf("\n%d %d\n", strlen(temp3), find_idx);

				init_wiz_string(&temp, temp3, find_idx);
				
				concat_and_assign_wiz_string(&temp, &temp4);

				push_back_wiz_vector_wiz_string(&vec_of_str, &temp);
 
				left_shift_wiz_string_builder(&builder, find_idx + 1);

				free_wiz_string(&temp);
				
				i = 0;
				temp2 = -1;
				chk = 0;
			}
		}
		else {
			continue;
		}

		if (real_count != num) {
			temp3 = str_wiz_string_builder(&builder, &size);
			init_wiz_string(&temp, temp3, size);
			push_back_wiz_vector_wiz_string(&vec_of_str, &temp);
			break;
		}
	}

	

	free_wiz_string(&temp);
	free_wiz_string_builder(&builder);

	fclose(file);
	
	/*file = fopen("test.txt", "wt");

	for (i = 0; i < size_wiz_vector_wiz_string(&vec_of_str); ++i) {
		fprintf(file, "%s", get_cstr_wiz_string(get_wiz_vector_wiz_string(&vec_of_str, i)));
	}
	fclose(file);
	*/

	init_wiz_array_queue_token(&aq);

	{
		wiz_vector_wiz_string strVecTemp = vec_of_str; 
		size_t left = 0, right = size_wiz_vector_wiz_string(&vec_of_str) - 1;
		size_t x;

		for (x = left; x <= right; ++x)
		{
			//StringTokenizer tokenizer(std::move( (*strVecTemp)[x] ) );
			//while (tokenizer.hasMoreTokens()) {
			//	aq.push(tokenizer.nextToken());
			//}
			wiz_string* statement = get_wiz_vector_wiz_string(&strVecTemp, x);
			size_t token_first = 0, token_last = 0; // idx of token in statement.
			int state = 0;


			for (size_t i = 0; i < size_wiz_string(statement); ++i) {
				if (0 == state && '\"' == get_cstr_wiz_string(statement)[i]) {
					//token_last = i - 1;
					//if (token_last >= 0 && token_last - token_first + 1 > 0) {
					//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
					//}
					state = 1;
					//token_first = i; 
					token_last = i;
				}
				else if (1 == state && '\\' == get_cstr_wiz_string(statement)[i - 1] && '\"' == get_cstr_wiz_string(statement)[i]) {
					token_last = i;
				}
				else if (1 == state && '\"' == get_cstr_wiz_string(statement)[i]) {
					state = 0; token_last = i;

					//aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
					//token_first = i + 1;
				}

				if (0 == state && '=' == get_cstr_wiz_string(statement)[i]) {
					token_last = i - 1;
					if (token_last >= 0 && token_last - token_first + 1 > 0) {
						token temp;
						temp.isComment = 0;
						temp.str = substr_wiz_string(statement, token_first, token_last + 1);

						push_wiz_array_queue_token(&aq , &temp);

						free_wiz_string(&temp.str);
					}
					{
						token temp;
						wiz_string str;

						temp.isComment = 0;
						init_wiz_string(&str, "=", 1);
						temp.str = str;

						push_wiz_array_queue_token(&aq, &temp);
						free_wiz_string(&temp.str);
					}
					token_first = i + 1;
				}
				else if (0 == state && is_whitespace(get_cstr_wiz_string(statement)[i])) { // isspace ' ' \t \r \n , etc... ?
					token_last = i - 1;
					if (token_last >= 0 && token_last - token_first + 1 > 0) {
						token temp;
						temp.isComment = 0;
						temp.str = substr_wiz_string(statement, token_first, token_last + 1);

						push_wiz_array_queue_token(&aq, &temp);

						free_wiz_string(&temp.str);
					}
					token_first = i + 1;
				}
				else if (0 == state && '{' == get_cstr_wiz_string(statement)[i]) {
					token_last = i - 1;
					if (token_last >= 0 && token_last - token_first + 1 > 0) {
						token temp;
						temp.isComment = 0;
						temp.str = substr_wiz_string(statement, token_first, token_last + 1);

						push_wiz_array_queue_token(&aq, &temp);

						free_wiz_string(&temp.str);
					}
					{
						token temp;
						wiz_string str;

						temp.isComment = 0;
						init_wiz_string(&str, "{", 1);
						temp.str = str;

						push_wiz_array_queue_token(&aq, &temp);

						free_wiz_string(&temp.str);
					}
					token_first = i + 1;
				}
				else if (0 == state && '}' == get_cstr_wiz_string(statement)[i]) {
					token_last = i - 1;
					if (token_last >= 0 && token_last - token_first + 1 > 0) {
						token temp;
						temp.isComment = 0;
						temp.str = substr_wiz_string(statement, token_first, token_last + 1);

						push_wiz_array_queue_token(&aq, &temp);

						free_wiz_string(&temp.str);
					}
					{
						token temp;
						wiz_string str;

						temp.isComment = 0;
						init_wiz_string(&str, "}", 1);
						temp.str = str;

						push_wiz_array_queue_token(&aq, &temp);

						free_wiz_string(&temp.str);
					}
					token_first = i + 1;
				}

				if (0 == state && '#' == get_cstr_wiz_string(statement)[i]) { // different from load_data_from_file
					token_last = i - 1;
					if (token_last >= 0 && token_last - token_first + 1 > 0) {
						token temp;
						temp.isComment = 0;
						temp.str = substr_wiz_string(statement, token_first, token_last + 1);

						push_wiz_array_queue_token(&aq, &temp);

						free_wiz_string(&temp.str);
					}
					size_t j = 0;
					for (j = i; j < size_wiz_string(statement); ++j) {
						if (get_cstr_wiz_string(statement)[j] == '\n') // cf) '\r' ?
						{
							break;
						}
					}
					--j; // "before enter key" or "before end"

					if (j - i + 1 > 0) {
						token temp;
						temp.isComment = 1;
						temp.str = substr_wiz_string(statement, token_first, token_last + 1);

						push_wiz_array_queue_token(&aq, &temp);

						free_wiz_string(&temp.str);
					}
					token_first = j + 2;
					i = token_first - 1;
				}
			}

			if (token_first < size_wiz_string(statement))
			{
				token temp;
				temp.isComment = 0;
				temp.str = substr_wiz_string(statement, token_first, size_wiz_string(statement));

				push_wiz_array_queue_token(&aq, &temp);

				free_wiz_string(&temp.str);
			}
		}
	}

	{
		size_t i;
		for (i = 0; i < size_wiz_array_queue_token(&aq); ++i) {
			free_wiz_vector_token(&(aq.que));
		}
		for (i = 0; i < size_wiz_vector_wiz_string(&vec_of_str); ++i) {
			free_wiz_string(&(vec_of_str.vec[i]));
		}
	}

	free_wiz_array_queue_token(&aq);
	free_wiz_vector_wiz_string(&vec_of_str);
}

int main(void)
{
	//test_for_wiz_string();
	//test_for_wiz_vector();
	test_for_wiz_string2("input.eu4");

	//getchar();
	return 0;
}
