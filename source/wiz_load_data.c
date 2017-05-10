
#define _CRT_SECURE_NO_WARNINGS

#include "wiz_load_data.h"
#include "wiz_array_queue_token.h"
#include "wiz_load_data_condition.h"
#include "utility.h"

// rename to find_idx?
size_t find( char* cstr,  char x,  size_t before,  size_t n)
{
	size_t i = 0;

	for (i = before + 1; i < n; ++i) {
		if (cstr[i] == x) {
			return i;
		}
	}
	return -1;
}

// move to global.h?
int is_whitespace( char ch)
{
	return ' ' == ch || '\t' == ch || '\r' == ch || '\n' == ch;
}

// to do
int is_state0( long long state_reserve)
{
	return 1 == state_reserve;
}

int equal(wiz_string* str1, wiz_string* str2)
{
	if (!str1 || !str2) { return 0; }
	return 0 == strcmp(get_cstr_wiz_string(str1), get_cstr_wiz_string(str2));
}

int chk_comment(user_type* ut, wiz_array_queue_token* token_vec, int offset)
{
	int count = 0;
	size_t idx = 0;
	token* x;

	if (empty_wiz_array_queue_token(token_vec)) {
		return 0;
	}

	do {
		x = get_wiz_array_queue_token(token_vec, idx);

		if (x->isComment) {
			push_comment_in_user_type(ut, &(x->str));
			erase_in_wiz_array_queue_token(token_vec, idx);
		}
		else if (count == offset - 1) {
			return 1;
		}
		else {
			count++;
			idx++;
		}

		if (idx == size_wiz_array_queue_token(token_vec)) {
			break;
		}
	} while (1);
	return 0;
}

wiz_string* Top(user_type* ut, wiz_array_queue_token* token_vec)
{
	if (get_wiz_array_queue_token(token_vec, 0)->isComment && 0 == chk_comment(ut, token_vec, 1)) {
		return NULL;
	}
	if (empty_wiz_array_queue_token(token_vec)) {
		return NULL;
	}

	return &(get_wiz_array_queue_token(token_vec, 0)->str);
}

int Pop(user_type* ut, wiz_array_queue_token* token_vec, wiz_string* result)
{
	token temp;

	if (get_wiz_array_queue_token(token_vec, 0)->isComment && 0 == chk_comment(ut, token_vec, 1)) {
		return 0;
	}

	if (empty_wiz_array_queue_token(token_vec)) {
		return 0;
	}

	temp = *front_wiz_array_queue_token(token_vec);
	pop_wiz_array_queue_token(token_vec);

	if (result != NULL) {
		free_wiz_string(result);
		*result = temp.str;
	}
	else {
		free_wiz_string(&(temp.str));
	}

	return 1;
}
pair_int_and_token LookUp(user_type* ut, wiz_array_queue_token* token_vec) {
	pair_int_and_token temp;

	if (!(size_wiz_array_queue_token(token_vec) >= 2 && !get_wiz_array_queue_token(token_vec, 0)->isComment && !get_wiz_array_queue_token(token_vec, 1))) {
		if (0 == chk_comment(ut, token_vec, 2)) {
			temp.first = 0;
			return temp;
		}
	}

	if (size_wiz_array_queue_token(token_vec) >= 2) {
		temp.first = 1;
		temp.second = *get_wiz_array_queue_token(token_vec, 1);
	}
	else {
		temp.first = 0;
	}

	return temp;
}

// only one exist or do not exist
item_type GetItem(user_type* ut, wiz_string* name) {
	 wiz_vector_item_type temp = get_item_in_user_type(ut, name);
	 item_type* result = get_wiz_vector_item_type(&temp, 0);
	 return *result;
}

wiz_vector_any GetUserType(user_type* ut, wiz_string* name)
{
	return get_user_type_item_in_user_type(ut, name);
}
wiz_string Find(user_type* ut, wiz_string* str, wiz_string_builder* builder)
{ // string ´ë½Å vector<string> ??
	int count = 0;
	int idx = -1;
	wiz_string result;
	size_t i;
	
	
	for (i = size_wiz_string(str) - 1; i >= 0; --i) {
		if ('/' == get_cstr_wiz_string(str)[i]) {
			if (count == 0) {
				idx = i;
			}
			count++;
		}
	}

	
	if (count == 1)
	{
		return make_empty_wiz_string();
	}
	else {
		pair_int_and_wiz_vector_any x;
		wiz_string substr;
		size_t i, j;
		substr = substr_wiz_string(str, 0, idx + 1);

		x = find_user_type_in_user_type(ut, &substr, builder);
		free_wiz_string(&substr);
		
		clear_wiz_string_builder(builder);
		if (x.first == 0) { return make_empty_wiz_string(); }
		for (i = 0; i < size_wiz_vector_any(&x.second); ++i) {
			wiz_string itemName = substr_wiz_string(str, idx + 1, size_wiz_vector_any(&x.second));
			wiz_string it;
			
			init_wiz_string(&it, "$it", 3);

			if (starts_with_wiz_string(&itemName, &it) && size_wiz_string(&itemName) >= 4) {
				wiz_string number = substr_wiz_string(&itemName, 3, size_wiz_string(&itemName));
				int itemIdx = atoi(get_cstr_wiz_string(&number));
				wiz_string* value = &get_item_list_in_user_type((user_type*)get_wiz_vector_any(&x.second, i), itemIdx)->value;
				append_wiz_string_builder(builder, get_cstr_wiz_string(value), size_wiz_string(value));

				if (i < size_wiz_vector_any(&x.second) - 1) {
					append_char_wiz_string_builder(builder, '\n');
				}
				free_wiz_string(&number);
			}
			else {
				wiz_vector_item_type temp;
				if (0 == strcmp(get_cstr_wiz_string(&itemName), "_")) {
					free_wiz_string(&itemName);
					init_wiz_string(&itemName, "", 0);
				}
				temp = get_item_in_user_type((user_type*)get_wiz_vector_any(&x.second, i), &itemName);
				if (!empty_wiz_vector_item_type(&temp)) {
					for (j = 0; j < size_wiz_vector_item_type(&temp); ++j) {
						item_type* item = get_wiz_vector_item_type(&temp, j);
						append_wiz_string_builder(builder, get_cstr_wiz_string(&item->value), size_wiz_string(&item->value));

						if (j < size_wiz_vector_item_type(&temp) - 1) {
							append_char_wiz_string_builder(builder, ' ');
						}
					}
					if (i < size_wiz_vector_any(&x.second) - 1) {
						append_char_wiz_string_builder(builder, '\n');
					}
				}
				free_wiz_vector_item_type(&temp);
			}
			free_wiz_string(&itemName);
			free_wiz_string(&it);
		}
	}
	init_wiz_string(&result, str_wiz_string_builder(builder, NULL), size_wiz_string_builder(builder));
	return result;
}
 int Exist(user_type* ut, wiz_string* dir, wiz_string_builder* builder)
{
	pair_int_and_wiz_vector_any x = find_user_type_in_user_type(ut, dir, builder);
	int val = x.first;
	
	if (x.first) {
		free_wiz_vector_any(&x.second);
	}
	
	return val;
}
// to do - rename!
pair_wiz_string_and_wiz_string Find2(user_type* ut,  wiz_string* str)
{
	int idx = -1;
	size_t i;
	pair_wiz_string_and_wiz_string result;

	for (i = size_wiz_string(str) - 1; i >= 0; --i) {
		if ('/' == get_cstr_wiz_string(str)[i]) {
			idx = i;
			break;
		}
	}

	result.first = substr_wiz_string(str, 0, idx + 1);
	result.second = substr_wiz_string(str, idx + 1, size_wiz_string(str));
	
	return result;
}

 wiz_string FindParameters(wiz_map_wiz_string_and_wiz_string* parameters,  wiz_string* operand)
{
	 wiz_string parameter_text;
	 pair_wiz_string_and_wiz_string data;
	 wiz_string value;

	 init_wiz_string(&parameter_text, "$parameter.", 11);
	 if (starts_with_wiz_string(operand, &parameter_text)) {
		 data.first = substr_wiz_string(operand, 11, size_wiz_string(operand));
		 if (get_wiz_map_wiz_string_and_wiz_string(parameters, &data))
		 {
			 value = data.second;
		 }
		 else {
			 value = make_empty_wiz_string();
		 }
	}

	free_wiz_string(&parameter_text);
	free_wiz_string(&data.first);

	return value;
}
 wiz_string FindLocals(wiz_map_wiz_string_and_wiz_string* locals,  wiz_string* operand)
{
	wiz_string local_text;
	
	init_wiz_string(&local_text, "$local.", 7);

	if (starts_with_wiz_string(operand, &local_text)) 
	{
		pair_wiz_string_and_wiz_string data;
		data.first = substr_wiz_string(operand, 0, 7);

		get_wiz_map_wiz_string_and_wiz_string(locals, &data);

		free_wiz_string(&local_text);
		free_wiz_string(&data.first);
		return data.second;
	}
	free_wiz_string(&local_text);
	return make_empty_wiz_string();
}
//need to renewal. add $AND $OR $NOT

/// remove /, parameter chk!!
 wiz_string ToBool4(user_type* now, user_type* global,  wiz_string* temp,  ExcuteData* excuteData, wiz_string_builder* builder);

 /*
 
 int operation(user_type* now, user_type* global, wiz_string* str,
	 wiz_stack_wiz_string* operandStack, ExcuteData* excuteData, wiz_string_builder* builder)
 {
	 int operandNum = 0;
	 int count = 0; // for brace!
	 size_t i;
	 wiz_string empty;

	 if (!empty_wiz_stack_wiz_string(operandStack) &&
		 0 == strcmp(get_cstr_wiz_string(top_wiz_stack_wiz_string(operandStack)), "ERROR")) { // starts_with ~ , "ERROR"?
		 return 0;
	 }

	 {
		 for (i = size_wiz_stack_wiz_string(operandStack) - 1; i >= 0; --i) {
			 if (0 == strcmp("{", get_cstr_wiz_string(get_wiz_stack_wiz_string(operandStack, i)))) {
				 count++;
			 }
			 if (0 == strcmp("}", get_cstr_wiz_string(get_wiz_stack_wiz_string(operandStack, i)))) {
				 count--;
				 if (count < 0) {
					 break;
				 }
				 else {
					 operandNum++;
				 }
			 }
			 else {
				 operandNum++;
			 }
		 }
	 }

	 empty = make_empty_wiz_string();
	 for (i = 0; i < operandNum; ++i) {
		 if (0 == strcmp("^", get_cstr_wiz_string(get_wiz_stack_wiz_string(operandStack, size_wiz_stack_wiz_string(operandStack) - 1 - i)))) {
			 assign_wiz_string(get_wiz_stack_wiz_string(operandStack, size_wiz_stack_wiz_string(operandStack) - 1 - i), &empty);
		 }
	 }
	 free_wiz_string(&empty);

	 if ("$EQ" == str) {
		 wiz_string* x;
		 wiz_string* y;
		 int idx = -1;

		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 if (compare_wiz_string_in_utility(x, y, builder, 0) == 0) {
			 wiz_string TRUE_TEXT;
			 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
			 push_wiz_stack_wiz_string(operandStack, &TRUE_TEXT);
		 }
		 else {
			 wiz_string FALSE_TEXT;
			 init_wiz_string(&FALSE_TEXT, "FALSE", 5);
			 push_wiz_stack_wiz_string(operandStack, &FALSE_TEXT);
		 }
	 }
	 else if ("$NOTEQ" == str)
	 {
		 wiz_string* x; wiz_string* y;
		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 if (compare_wiz_string_in_utility(x, y, builder, 0) != "== 0") {
			 wiz_string TRUE_TEXT;
			 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
			 push_wiz_stack_wiz_string(operandStack, &TRUE_TEXT);

		 }
		 else {
			 wiz_string FALSE_TEXT;
			 init_wiz_string(&FALSE_TEXT, "FALSE", 5);
			 push_wiz_stack_wiz_string(operandStack, &FALSE_TEXT);
		 }
	 }
	 else if ("$AND" == str)
	 {
		 wiz_string* x; wiz_string* y;
		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 if (x == "TRUE" && y == "TRUE") {
			 wiz_string TRUE_TEXT;
			 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
			 push_wiz_stack_wiz_string(operandStack, &TRUE_TEXT);

		 }
		 else {
			 wiz_string FALSE_TEXT;
			 init_wiz_string(&FALSE_TEXT, "FALSE", 5);
			 push_wiz_stack_wiz_string(operandStack, &FALSE_TEXT);
		 }
	 }
	 else if ("$AND_ALL" == str) {
		 wiz_vector_wiz_string store;

		 init_wiz_vector_wiz_string(&store, 1);

		 for (i = 0; i < operandNum; ++i) {
			 push_back_wiz_vector_wiz_string(&store, (top_wiz_stack_wiz_string(operandStack)));  pop_wiz_stack_wiz_string(operandStack);
		 }
		 for (i = 0; i < size_wiz_vector_wiz_string(&store); ++i) {
			 wiz_string TRUE_TEXT;
			 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
			 if (0 != strcmp("TRUE", get_cstr_wiz_string(get_wiz_vector_wiz_string(&store, i)))) {
				 wiz_string FALSE_TEXT;
				 init_wiz_string(&FALSE_TEXT, "FALSE", 5);
				 push_wiz_stack_wiz_string(operandStack, &FALSE_TEXT);

				 return 1;
			 }
		 }
		 {
			 wiz_string TRUE_TEXT;
			 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
			 push_wiz_stack_wiz_string(operandStack, &TRUE_TEXT);
		 }
	 }
	 else if ("$OR" == str)
	 {
		 wiz_string* x; wiz_string* y;
		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 if (x == "TRUE" || y == "TRUE") {
			 wiz_string TRUE_TEXT;
			 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
			 push_wiz_stack_wiz_string(operandStack, &TRUE_TEXT);
		 }
		 else {
			 wiz_string FALSE_TEXT;
			 init_wiz_string(&FALSE_TEXT, "FALSE", 5);
			 push_wiz_stack_wiz_string(operandStack, &FALSE_TEXT);
		 }
	 }
	 else if ("$OR_ALL" == str)
	 {
		 wiz_vector_wiz_string store;

		 for (i = 0; i < operandNum; ++i) {
			 push_back_wiz_vector_wiz_string(&store, top_wiz_stack_wiz_string(operandStack));  pop_wiz_stack_wiz_string(operandStack);
		 }
		 for (i = 0; i < size_wiz_vector_wiz_string(&store); ++i) {
			 if ("TRUE" == get_wiz_vector_wiz_string(&store, i)) {
				 wiz_string TRUE_TEXT;
				 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
				 push_wiz_stack_wiz_string(operandStack, &TRUE_TEXT);

				 return 1;
			 }
		 }
		 {
			 wiz_string FALSE_TEXT;
			 init_wiz_string(&FALSE_TEXT, "FALSE", 5);
			 push_wiz_stack_wiz_string(operandStack, &FALSE_TEXT);
		 }
	 }
	 else if ("$NOT" == str)
	 {
		 wiz_string* x;
		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 if (x == "TRUE") {
			 wiz_string FALSE_TEXT;
			 init_wiz_string(&FALSE_TEXT, "FALSE", 5);
			 push_wiz_stack_wiz_string(operandStack, &FALSE_TEXT);

		 }
		 else {
			 wiz_string TRUE_TEXT;
			 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
			 push_wiz_stack_wiz_string(operandStack, &TRUE_TEXT);

		 }
	 }
	 else if ("$COMP<" == str)
	 {
		 wiz_string* x; wiz_string* y;
		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 if (compare_wiz_string_in_utility(x, y, builder, 0) == "< 0") {
			 wiz_string TRUE_TEXT;
			 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
			 push_wiz_stack_wiz_string(operandStack, &TRUE_TEXT);

		 }
		 else
		 {
			 wiz_string FALSE_TEXT;
			 init_wiz_string(&FALSE_TEXT, "FALSE", 5);
			 push_wiz_stack_wiz_string(operandStack, &FALSE_TEXT);

		 }
	 }
	 else if ("$COMP>" == str)
	 {
		 wiz_string* x; wiz_string* y;
		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 if (compare_wiz_string_in_utility(x, y, builder, 0) == "> 0") {
			 wiz_string TRUE_TEXT;
			 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
			 push_wiz_stack_wiz_string(operandStack, &TRUE_TEXT);

		 }
		 else
		 {
			 wiz_string FALSE_TEXT;
			 init_wiz_string(&FALSE_TEXT, "FALSE", 5);
			 push_wiz_stack_wiz_string(operandStack, &FALSE_TEXT);

		 }
	 }
	 else if ("$COMP<EQ" == str)
	 {
		 wiz_string* x; wiz_string* y;
		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 if (compare_wiz_string_in_utility(x, y, builder, 0) == -1 ||
			 compare_wiz_string_in_utility(x, y, builder, 0) == 0) {
			 wiz_string TRUE_TEXT;
			 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
			 push_wiz_stack_wiz_string(operandStack, &TRUE_TEXT);

		 }
		 else
		 {
			 wiz_string FALSE_TEXT;
			 init_wiz_string(&FALSE_TEXT, "FALSE", 5);
			 push_wiz_stack_wiz_string(operandStack, &FALSE_TEXT);

		 }
	 }
	 else if ("$COMP>EQ" == str)
	 {
		 wiz_string* x; wiz_string* y;
		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 if (compare_wiz_string_in_utility(x, y, builder, 0) == 1 ||
			 compare_wiz_string_in_utility(x, y, builder, 0) == 0) {
			 wiz_string TRUE_TEXT;
			 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
			 push_wiz_stack_wiz_string(operandStack, &TRUE_TEXT);
		 }
		 else
		 {
			 wiz_string FALSE_TEXT;
			 init_wiz_string(&FALSE_TEXT, "FALSE", 5);
			 push_wiz_stack_wiz_string(operandStack, &FALSE_TEXT);

		 }
	 }
	 else if ("$add" == str) // todo! = int operator double => double operator double!
	 {
		 wiz_string* x; wiz_string* y;
		 int typex, typey;

		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 // other *, /, mod, ..
		 typex = get_type(x);
		 typey = get_type(y);

		 if (typex == typey && typey == TYPE_WIZ_INTEGER) { /// only integer -> BigInteger
			 wiz_string temp = wiz_ll_to_string(atoll(get_cstr_wiz_string(x)) + atoll(get_cstr_wiz_string(y)));

			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
		 else if (typex == typey && typey == TYPE_WIZ_DOUBLE) {
			 char* end;

			 wiz_string temp = wiz_ld_to_string(strtold(get_cstr_wiz_string(x), &end) + strtold(get_cstr_wiz_string(y), &end));

			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
		 else
		 {
			 wiz_string temp = make_wiz_string("ERROR", 5);
			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
	 }
	 else if ("$multiple" == str) // todo! = int operator double => double operator double!
	 {
		 wiz_string* x; wiz_string* y;
		 int typex, typey;

		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 // other *, /, mod, ..
		 typex = get_type(x);
		 typey = get_type(y);


		 if (typex == typey && typey == TYPE_WIZ_INTEGER) { /// only integer -> BigInteger
			 wiz_string temp = wiz_ll_to_string(atoll(get_cstr_wiz_string(x)) * atoll(get_cstr_wiz_string(y)));

			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
		 else if (typex == typey && typey == TYPE_WIZ_DOUBLE) {
			 char* end;
			 wiz_string temp = wiz_ld_to_string(strtold(get_cstr_wiz_string(x), &end) * strtold(get_cstr_wiz_string(y), &end));

			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
		 else
		 {
			 wiz_string temp = make_wiz_string("ERROR", 5);
			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
	 }
	 else if ("$divide" == str) // todo! = int operator double => double operator double!
	 {
		 wiz_string* x; wiz_string* y;
		 int typex, typey;

		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 // other *, /, mod, ..
		 typex = get_type(x);
		 typey = get_type(y);

		 if (typex == typey && typey == TYPE_WIZ_INTEGER) { /// only integer -> BigInteger
			 wiz_string temp = wiz_ll_to_string(atoll(get_cstr_wiz_string(x)) / atoll(get_cstr_wiz_string(y)));

			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
		 else if (typex == typey && typey == TYPE_WIZ_DOUBLE) {
			 char* end;
			 wiz_string temp = wiz_ld_to_string(strtold(get_cstr_wiz_string(x), &end) / strtold(get_cstr_wiz_string(y), &end));

			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
		 else
		 {
			 wiz_string temp = make_wiz_string("ERROR", 5);
			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
	 }
	 else if ("$modular" == str)
	 {
		 wiz_string* x; wiz_string* y;
		 int typex, typey;

		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 // other *, /, mod, ..
		 typex = get_type(x);
		 typey = get_type(y);

		 if (typex == typey && typey == TYPE_WIZ_INTEGER) { /// only integer -> BigInteger
			 wiz_string temp = wiz_ll_to_string(atoll(get_cstr_wiz_string(x)) % atoll(get_cstr_wiz_string(y)));

			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
		 else
		 {
			 wiz_string temp = make_wiz_string("ERROR", 5);
			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
	 }
	 else if ("$rand" == str)
	 {
		 wiz_string* x; wiz_string* y;
		 int typex, typey;

		 x = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 // other *, /, mod, ..
		 typex = get_type(x);
		 typey = get_type(y);

		 if (typex == typey && (typey == TYPE_WIZ_INTEGER)) { /// only integer -> BigInteger
			 int _x = atoi(get_cstr_wiz_string(x));
			 int _y = atoi(get_cstr_wiz_string(y));
			 if (_y - _x + 1 <= 0) {
				 wiz_string temp = make_wiz_string("ERROR x / 0", 11);
				 push_wiz_stack_wiz_string(operandStack, &temp);
			 }
			 else {
				 long long _z = rand_int() % (_y - _x + 1) + _x; // _x <= _z <= _y
				 wiz_string temp = wiz_ll_to_string(_z);
				 push_wiz_stack_wiz_string(operandStack, &temp);
			 }
		 }
		 else
		 {
			 wiz_string temp = make_wiz_string("ERROR", 5);
			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
	 }
	 else if ("$concat" == str)
	 {
		 wiz_string temp;
		 int total_size = 0;

		 clear_wiz_string_builder(builder);

		 for (i = 0; i < 2; ++i)
		 {
			 wiz_string* temp = top_wiz_stack_wiz_string(operandStack);
			 total_size += size_wiz_string(top_wiz_stack_wiz_string(operandStack));
			 pop_wiz_stack_wiz_string(operandStack);
			 append_wiz_string_builder(builder, get_cstr_wiz_string(temp), size_wiz_string(temp)); // chk
			 free_wiz_string(temp);
		 }
		 temp = make_wiz_string(str_wiz_string_builder(builder, NULL), total_size);
		 push_wiz_stack_wiz_string(operandStack, &temp);
	 }
	 else if ("$concat2" == str) /// with space
	 {
		 wiz_string* x; wiz_string* y;
		 wiz_string temp;

		 x = top_wiz_stack_wiz_string(operandStack);
		 pop_wiz_stack_wiz_string(operandStack);
		 y = top_wiz_stack_wiz_string(operandStack);
		 pop_wiz_stack_wiz_string(operandStack);

		 clear_wiz_string_builder(builder); // recapacity?

		 append_wiz_string_builder(builder, get_cstr_wiz_string(x), size_wiz_string(x));
		 append_char_wiz_string_builder(builder, ' ');
		 append_wiz_string_builder(builder, get_cstr_wiz_string(y), size_wiz_string(y));

		 temp = make_wiz_string(str_wiz_string_builder(builder, NULL), size_wiz_string_builder(builder));
		 push_wiz_stack_wiz_string(operandStack, &temp);

		 free_wiz_string(x);
		 free_wiz_string(y);
	 }
	 else if ("$concat_all" == str)
	 {
		 int total_size = 0;
		 wiz_string temp;
		 clear_wiz_string_builder(builder);

		 for (i = 0; i < operandNum; ++i)
		 {
			 wiz_string* temp = top_wiz_stack_wiz_string(operandStack);
			 total_size += size_wiz_string(top_wiz_stack_wiz_string(operandStack));

			 pop_wiz_stack_wiz_string(operandStack);
			 append_wiz_string_builder(builder, get_cstr_wiz_string(temp), size_wiz_string(temp)); // chk
			 free_wiz_string(temp);
		 }
		 temp = make_wiz_string(str_wiz_string_builder(builder, NULL), total_size);
		 push_wiz_stack_wiz_string(operandStack, &temp);
	 }
	 else if ("$concat_all2" == str)
	 {
		 wiz_string result;

		 clear_wiz_string_builder(builder);

		 for (i = 0; i < operandNum; ++i) {
			 wiz_string* temp = top_wiz_stack_wiz_string(operandStack);
			 append_wiz_string_builder(builder, get_cstr_wiz_string(temp), size_wiz_string(temp));
			 pop_wiz_stack_wiz_string(operandStack);
			 if (i < operandNum - 1) {
				 append_char_wiz_string_builder(builder, ' ');
			 }
			 free_wiz_string(temp);
		 }
		 result = make_wiz_string(str_wiz_string_builder(builder, NULL), size_wiz_string_builder(builder));
		 push_wiz_stack_wiz_string(operandStack, &result);
	 }
	 else if ("$concat3" == str) { // for special case? "abc" "def" "ghi" -> "abcdefghi"
		 wiz_string result;

		 init_wiz_string(&result, "", 0);
		 clear_wiz_string_builder(builder);

		 for (i = 0; i < operandNum; ++i) {
			 wiz_string* temp = top_wiz_stack_wiz_string(operandStack);
			 pop_wiz_stack_wiz_string(operandStack);

			 if (size_wiz_string(temp) >= 3 && back_wiz_string(temp) == front_wiz_string(temp) && back_wiz_string(temp) == '\"') {
				 //
			 }
			 else {
				 wiz_string temp = make_wiz_string("ERROR in $concat3, 1. must be \" \" ", strlen("ERROR in $concat3, 1. must be \" \" "));
				 push_wiz_stack_wiz_string(operandStack, &temp);
				 free_wiz_string(&result);
				 return 0;
			 }
			 if (i < operandNum - 1) {
				 erase_wiz_string(temp, size_wiz_string(temp) - 1);

				 if (0 != i) {
					 erase_wiz_string(temp, 0);
				 }

				 append_wiz_string_builder(builder, get_cstr_wiz_string(temp), size_wiz_string(temp));
				 append_char_wiz_string_builder(builder, ' ');
			 }
			 else {
				 erase_wiz_string(temp, 0);

				 append_wiz_string_builder(builder, get_cstr_wiz_string(temp), size_wiz_string(temp));
			 }
		 }
		 free_wiz_string(&result);
		 init_wiz_string(&result, str_wiz_string_builder(builder, NULL), size_wiz_string_builder(builder));
		 push_wiz_stack_wiz_string(operandStack, &result);
	 }
	 else if ("$return_value" == str)
	 {
		 wiz_string result = make_wiz_string(get_cstr_wiz_string(&excuteData->info.return_value),
			 size_wiz_string(&excuteData->info.return_value));

		 push_wiz_stack_wiz_string(operandStack, &result);
	 }
	 // cf) empty test!!
	 ///ToDo - GetList -> // GetItemListIdxByIListIdx, GetUserTypeLisIdxtByIListIdx ?
	 else if ("$back" == str) // ex) for x  = { 0 get_item_list_in_user_type1 2 3 .. }, for usertaypelist? and mixed? and need more test!
	 {
		 wiz_string* x = top_wiz_stack_wiz_string(operandStack);
		 wiz_string TRUE_TEXT;
		 wiz_string value;
		 user_type ut;

		 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
		 value = get_item_list_data_in_load_data(global, x, &TRUE_TEXT, excuteData, builder);
		 //
		 free_wiz_string(&TRUE_TEXT);

		 pop_wiz_stack_wiz_string(operandStack);

		 ut = load_data_from_string_in_load_data(&value);
		 //
		 free_wiz_string(x);
		 free_wiz_string(&value);

		 if (is_item_list_in_user_type(&ut, get_ilist_size_in_user_type(&ut) - 1))
		 {
			 item_type* x = get_item_list_in_user_type(&ut, get_item_list_size_in_user_type(&ut) - 1);
			 wiz_string temp = make_wiz_string_from_other_wiz_string(&x->value);

			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
		 else {
			 user_type* x = get_user_type_list_in_user_type(&ut, get_user_type_list_size_in_user_type(&ut) - 1);
			 wiz_string temp = to_string_in_user_type(x, builder);

			 clear_wiz_string_builder(builder);

			 append_char_wiz_string_builder(builder, '\"');
			 append_wiz_string_builder(builder, get_cstr_wiz_string(&temp), size_wiz_string(&temp));
			 append_char_wiz_string_builder(builder, '\"');

			 free_wiz_string(&temp);

			 init_wiz_string(&temp, str_wiz_string_builder(builder, NULL), size_wiz_string_builder(builder));

			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }

		 // free ut
		 free_user_type_in_user_type(&ut);
	 }
	 // pop_back or front - remove this function?
	 else if ("$pop_back" == str) // and for usertypelist? and mixed?, usertype-> "~"
	 {
		 wiz_string* x = top_wiz_stack_wiz_string(operandStack);
		 user_type* ut;
		 pair_int_and_wiz_vector_any finded;

		 pop_wiz_stack_wiz_string(operandStack);
		 for (i = size_wiz_string(x) - 1; i >= 0; --i)
		 {
			 if (get_cstr_wiz_string(x)[i] == '/' && i != 0) {
				 substr_and_assign_wiz_string(x, 0, i);
				 break;
			 }
		 }

		 if (empty_wiz_string(x)) { free_wiz_string(x); init_wiz_string(x, ".", 1); }

		 finded = find_user_type_in_user_type(global, x, builder);
		 free_wiz_string(x);
		 if (finded.first) {
			 ut = (user_type*)get_wiz_vector_any(&finded.second, 0);

			 free_wiz_vector_any(&finded.second);

			 if (is_item_list_in_user_type(ut, get_ilist_size_in_user_type(ut) - 1))
			 {
				 item_type* x = get_item_list_in_user_type(ut, get_item_list_size_in_user_type(ut) - 1);
				 push_wiz_stack_wiz_string(operandStack, &x->value);
				 remove_item_list_by_idx_in_user_type(ut, get_item_list_size_in_user_type(ut) - 1);
			 }
			 else {
				 user_type* x = get_user_type_list_in_user_type(ut, get_user_type_list_size_in_user_type(ut) - 1);
				 wiz_string temp = to_string_in_user_type(x, builder);

				 clear_wiz_string_builder(builder);

				 append_char_wiz_string_builder(builder, '\"');
				 append_wiz_string_builder(builder, get_cstr_wiz_string(&temp), size_wiz_string(&temp));
				 append_char_wiz_string_builder(builder, '\"');

				 free_wiz_string(&temp);

				 init_wiz_string(&temp, str_wiz_string_builder(builder, NULL), size_wiz_string_builder(builder));
				 push_wiz_stack_wiz_string(operandStack, &temp);

				 remove_user_type_list_by_idx_in_user_type(ut, get_user_type_list_size_in_user_type(ut) - 1);
			 }

			 free_wiz_vector_any(&finded.second);
		 }
	 }
	 else if ("$front" == str)
	 {
		 wiz_string* x = top_wiz_stack_wiz_string(operandStack);
		 wiz_string TRUE_TEXT;
		 wiz_string value;
		 user_type ut;

		 init_wiz_string(&TRUE_TEXT, "TRUE", 4);
		 value = get_item_list_data_in_load_data(global, x, &TRUE_TEXT, excuteData, builder);
		 free_wiz_string(&TRUE_TEXT);
		 free_wiz_string(x);

		 pop_wiz_stack_wiz_string(operandStack);
		 ut = load_data_from_string_in_load_data(&value);

		 if (is_item_list_in_user_type(&ut, 0))
		 {
			 item_type* x = get_item_list_in_user_type(&ut, 0);
			 push_wiz_stack_wiz_string(operandStack, &x->value);
		 }
		 else {
			 user_type* x = get_user_type_list_in_user_type(&ut, 0);
			 wiz_string temp = to_string_in_user_type(x, builder);

			 clear_wiz_string_builder(builder);
			 append_char_wiz_string_builder(builder, '\"');
			 append_wiz_string_builder(builder, get_cstr_wiz_string(&temp), size_wiz_string(&temp));
			 append_char_wiz_string_builder(builder, '\"');

			 free_wiz_string(&temp);
			 init_wiz_string(&temp, str_wiz_string_builder(builder, NULL), size_wiz_string_builder(builder));
			 push_wiz_stack_wiz_string(operandStack, &temp);
		 }
	 }
	 else if ("$pop_front" == str)
	 {
		 size_t i;
		 wiz_string* x = top_wiz_stack_wiz_string(operandStack);
		 user_type* ut = NULL;
		 pair_int_and_wiz_vector_any finded;

		 pop_wiz_stack_wiz_string(operandStack);
		 for (i = size_wiz_string(x) - 1; i >= 0; --i)
		 {
			 if (get_cstr_wiz_string(x)[i] == '/' && i != 0) {
				 substr_and_assign_wiz_string(x, 0, i); // i = 1 + 1
				 break;
			 }
		 }

		 if (empty_wiz_string(x)) { free_wiz_string(x); init_wiz_string(x, ".", 1); }

		 finded = find_user_type_in_user_type(global, x, builder);
		 ut = (user_type*)get_wiz_vector_any(&finded.second, 0);

		 free_wiz_string(x);

		 if (is_item_list_in_user_type(ut, 0))
		 {
			 item_type* x = get_item_list_in_user_type(ut, 0);
			 push_wiz_stack_wiz_string(operandStack, &x->value);
			 remove_item_list_by_idx_in_user_type(ut, 0);
		 }
		 else {
			 user_type* x = get_user_type_list_in_user_type(ut, 0);
			 wiz_string temp = to_string_in_user_type(x, builder);

			 clear_wiz_string_builder(builder);
			 append_char_wiz_string_builder(builder, '\"');
			 append_wiz_string_builder(builder, get_cstr_wiz_string(&temp), size_wiz_string(&temp));
			 append_char_wiz_string_builder(builder, '\"');

			 free_wiz_string(&temp);
			 init_wiz_string(&temp, str_wiz_string_builder(builder, NULL), size_wiz_string_builder(builder));
			 push_wiz_stack_wiz_string(operandStack, &temp);
			 remove_user_type_list_by_idx_in_user_type(ut, 0);
		 }
	 }
	 else if ("$get" == str)
	 {
		 wiz_string* x = top_wiz_stack_wiz_string(operandStack);
		 wiz_string NOW_TEXT;
		 pop_wiz_stack_wiz_string(operandStack);

		 init_wiz_string(&NOW_TEXT, "now.", 4);

		 if ('@' == get_cstr_wiz_string(&NOW_TEXT)[0]) { // chk..
			 erase_wiz_string(x, 0);
		 }

		 if (starts_with_wiz_string(x, &NOW_TEXT) && NULL != now)
		 {
			 substr_and_assign_wiz_string(x, 4, size_wiz_string(x));

			 clear_wiz_string_builder(builder);

			 append_char_wiz_string_builder(builder, '/');
			 append_char_wiz_string_builder(builder, '.');
			 append_char_wiz_string_builder(builder, '/');

			 append_wiz_string_builder(builder, get_cstr_wiz_string(x), size_wiz_string(x));

			 free_wiz_string(x);

			 init_wiz_string(x, str_wiz_string_builder(builder, NULL), size_wiz_string_builder(builder));

			 if ('/' == get_cstr_wiz_string(x)[0])
			 {
				 wiz_string temp = Find(now, x, builder);
				 if (!empty_wiz_string(&temp)) { *x = temp; }
			 }
		 }
		 else {
			 if ('/' == get_cstr_wiz_string(x)[0])
			 {
				 wiz_string temp = Find(global, x, builder);
				 if (!empty_wiz_string(&temp)) { *x = temp; }
			 }
		 }

		 {
			 wiz_string temp = FindParameters(&excuteData->info.parameters, x);
			 if (!empty_wiz_string(&temp)) { *x = temp; }
		 }
		 {
			 wiz_string temp = FindLocals(&excuteData->info.locals, x);
			 if (!empty_wiz_string(&temp)) { *x = temp; }
		 }

		 free_wiz_string(&NOW_TEXT);

		 push_wiz_stack_wiz_string(operandStack, x);
	 }
	 else if ("$size" == str)
	 {
		 wiz_string x = *top_wiz_stack_wiz_string(operandStack);
		 pop_wiz_stack_wiz_string(operandStack);

		 if ('/' == get_cstr_wiz_string(&x)[0])
		 {
			 pair_int_and_wiz_vector_any pairTemp = find_user_type_in_user_type(global, &x, builder);
			 if (pairTemp.first) {
				 free_wiz_string(&x);
				 x = wiz_ll_to_string(get_item_list_size_in_user_type(get_wiz_vector_any(&pairTemp.second, 0)));
			 }
			 else {
				 //
			 }
		 }
		 else
		 {
			 free_wiz_string(&x);
			 x = make_wiz_string("FALSE", 5);
		 }

		 push_wiz_stack_wiz_string(operandStack, &x);
	 }
	 else if ("$size2" == str)
	 {
		 wiz_string x = *top_wiz_stack_wiz_string(operandStack);
		 pop_wiz_stack_wiz_string(operandStack);

		 if ('/' == get_cstr_wiz_string(&x)[0])
		 {
			 pair_int_and_wiz_vector_any tempPair = find_user_type_in_user_type(global, &x, builder);
			 if (tempPair.first) {
				 user_type* ut = get_wiz_vector_any(&tempPair.second, 0);
				 free_wiz_string(&x);
				 x = wiz_ll_to_string(get_user_type_list_size_in_user_type(ut));
			 }
			 else {
				 //
			 }
		 }
		 else
		 {
			 free_wiz_string(&x);
			 x = make_wiz_string("FALSE", 5);
		 }

		 push_wiz_stack_wiz_string(operandStack, &x);
	 }
	 else if ("$element" == str) // for list
	 {
		 wiz_string x = *top_wiz_stack_wiz_string(operandStack);
		 wiz_string* y;
		 long long idx;
		 pop_wiz_stack_wiz_string(operandStack); // list_name
		 y = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack); // idx
		 idx = atoll(get_cstr_wiz_string(y));

		 free_wiz_string(y);

		 if ('/' == get_cstr_wiz_string(&x)[0])
		 {
			 pair_int_and_wiz_vector_any pairTemp = find_user_type_in_user_type(global, &x, builder);
			 if (pairTemp.first) {
				 user_type* ut = get_wiz_vector_any(&pairTemp.second, 0);
				 free_wiz_string(&x);
				 x = get_item_list_in_user_type(ut, idx)->value;
			 }
			 else {
				 //
			 }
		 }
		 else
		 {
			 free_wiz_string(&x);
			 make_wiz_string("NONE", 4);
		 }

		 push_wiz_stack_wiz_string(operandStack, &x);
	 }
	 else if ("$eval" == str) { // $eval = ""   -  more think!
		 wiz_string str = *top_wiz_stack_wiz_string(operandStack);
		 int chk = ChkExist(&str); // chk "" >

		 pop_wiz_stack_wiz_string(operandStack);

		 if (chk) {
		 }
		 else {
			 char* errMsg = "ERROR in $eval, must be \" \" ";
			 wiz_string temp = make_wiz_string(errMsg, strlen(errMsg));

			 free_wiz_string(&str);

			 push_wiz_stack_wiz_string(operandStack, &temp);
			 return 0;
		 }

		 substr_and_assign_wiz_string(&str, 1, size_wiz_string(&str) - 1);
		 {
			 wiz_string result = ToBool4(now, global, &str, excuteData, builder);

			 free_wiz_string(&str);

			 push_wiz_stack_wiz_string(operandStack, &result);
		 }
	 }
	 else if ("$toQuotedStr" == str) {
		 wiz_string str = *top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 clear_wiz_string_builder(builder);

		 append_char_wiz_string_builder(builder, '\"');
		 append_wiz_string_builder(builder, get_cstr_wiz_string(&str), size_wiz_string(&str));
		 append_char_wiz_string_builder(builder, '\"');

		 free_wiz_string(&str);
		 str = make_wiz_string(str_wiz_string_builder(builder, NULL), size_wiz_string_builder(builder));
		 push_wiz_stack_wiz_string(operandStack, &str);
	 }
	 else if ("$addSmallQuoted" == str) {
		 wiz_string str = *top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 clear_wiz_string_builder(builder);

		 append_char_wiz_string_builder(builder, '\'');
		 append_wiz_string_builder(builder, get_cstr_wiz_string(&str), size_wiz_string(&str));
		 append_char_wiz_string_builder(builder, '\'');

		 free_wiz_string(&str);
		 str = make_wiz_string(str_wiz_string_builder(builder, NULL), size_wiz_string_builder(builder));
		 push_wiz_stack_wiz_string(operandStack, &str);
	 }
	 else if ("$removeQuoted" == str) { // chk "" string problem?
		 wiz_string str = *top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);

		 if (size_wiz_string(&str) > 0 && front_wiz_string(&str) == back_wiz_string(&str)
			 && '\"' == back_wiz_string(&str)
			 )
		 {
			 substr_and_assign_wiz_string(&str, 1, size_wiz_string(&str) - 1);
		 }

		 push_wiz_stack_wiz_string(operandStack, &str);
	 }
	 else if ("$getObjectStr" == str) {
		 wiz_string object_name = *top_wiz_stack_wiz_string(operandStack);
		 wiz_string event_id;
		 pair_wiz_string_and_user_type val;
		 user_type* ut;

		 substr_and_assign_wiz_string(&object_name, 1, size_wiz_string(&object_name) - 1);
		 pop_wiz_stack_wiz_string(operandStack);
		 event_id = *top_wiz_stack_wiz_string(operandStack);
		 pop_wiz_stack_wiz_string(operandStack);


		 val.first = object_name;
		 val.second = NULL;
		 if (get_wiz_map_wiz_string_and_user_type(excuteData->pObjectMap, &val)) {
			 ut = val.second;

			 int pass = 0;
			 for (i = 0; i < get_user_type_list_size_in_user_type(ut); ++i) {
				 for (int j = 0; j < get_item_list_size_in_user_type(get_user_type_list_in_user_type(ut, i)); ++j) {
					 if (0 == strcmp("id", get_cstr_wiz_string(&get_item_list_in_user_type(get_user_type_list_in_user_type(ut, i), j)->name))
						 && equal(&event_id, &get_item_list_in_user_type(get_user_type_list_in_user_type(ut, i), j)->value)
						 ) {
						 wiz_string first = make_wiz_string("Event = { ", 10);
						 wiz_string last = make_wiz_string(" } ", 3);
						 wiz_string data;

						 data = to_string_in_user_type(get_user_type_list_in_user_type(ut, i), builder);

						 clear_wiz_string_builder(builder);
						 append_wiz_string_builder(builder, get_cstr_wiz_string(&first), size_wiz_string(&first));
						 append_wiz_string_builder(builder, get_cstr_wiz_string(&data), size_wiz_string(&data));
						 append_wiz_string_builder(builder, get_cstr_wiz_string(&last), size_wiz_string(&last));

						 free_wiz_string(&first);
						 free_wiz_string(&last);
						 free_wiz_string(&data);
						 free_wiz_string(&object_name);
						 free_wiz_string(&event_id);
						 return 1;
					 }
				 }
			 }
		 }
		 {
			 char* errMsg = "ERROR in $getOjbectStr";
			 wiz_string temp = make_wiz_string(errMsg, strlen(errMsg));

			 push_wiz_stack_wiz_string(operandStack, &temp);

			 free_wiz_string(&object_name);
			 free_wiz_string(&event_id);
		 }
	 }

	else if ("$add_paren" == str) { // removal?
		wiz_string* temp = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		
		push_wiz_stack_wiz_string(operandStack, " { " + temp + " } ");
	}
	else if ("$get_item_name" == str) {
		user_type* ut;
		wiz_string statement;

		for (i = 0; i < operandNum; ++i) {
			statement = statement + top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		}
		load_data_from_string_in_load_data(statement, ut);

		push_wiz_stack_wiz_string(operandStack, ut.GetItemList(0).GetName());
	}
	else if ("$get_item_value" == str) {
		user_type* ut;
		wiz_string statement;
		int idx = 0;

		for (i = 0; i < operandNum - 1; ++i) {
			statement = statement + top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		}
		idx = stoi(top_wiz_stack_wiz_string(operandStack));  pop_wiz_stack_wiz_string(operandStack);

		load_data_from_string_in_load_data(statement, ut);

		push_wiz_stack_wiz_string(operandStack, ut.GetItemList(idx).Get(0));
	}
	else if ("$get_item_size" == str) {
		user_type* ut;
		wiz_string statement;

		for (i = 0; i < operandNum; ++i) {
			statement = statement + top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
		}
		load_data_from_string_in_load_data(statement, ut);

		push_wiz_stack_wiz_string(operandStack, wiz_toString(ut.GetItem(ut.GetItemList(0).GetName()).size()));
	}
	else if ("$is_empty_string" == str) {
		push_wiz_stack_wiz_string(operandStack, operandStack.pop().empty() ? "TRUE" : "FALSE");
	}
	else if ("$event_result" == str) {
		vector<wiz_string> eventVec;
		for (i = 0; i < operandNum; ++i) {
			eventVec.push_back(top_wiz_stack_wiz_string(operandStack));  pop_wiz_stack_wiz_string(operandStack);
		}

		wiz_string statements2 = "Event = { id = NONE" + wiztoStr(excuteData->depth + 1) + " $call = { ";
		for (i = 0; i < eventVec.size(); ++i) {
			statements2 = statements2 + eventVec[i] + " ";
		}
		statements2 = statements2 + " } }";
		user_type* eventsTemp = excuteData->pEvents;
		wizload_dataLoadDataAddData(*eventsTemp, "/root", statements2, "TRUE", ExcuteData(), builder);
		//cout << " chk " << statements2 << endl;
		ExcuteData _excuteData;
		_excuteData->pModule = excuteData->pModule;
		_excuteData->pObjectMap = excuteData->pObjectMap;
		_excuteData->pEvents = eventsTemp;
		_excuteData->depth = excuteData->depth + 1;

		push_wiz_stack_wiz_string(operandStack, excute_module("Main = { $call = { id = NONE" + wiztoStr(_excuteData->depth) + " } }", &global, _excuteData));

		{
			for (int idx = 0; idx < eventsTemp->GetUserTypeListSize(); ++idx) {
				if (eventsTemp->GetUserTypeList(idx)->GetItem("id")[0].Get(0) == "NONE" + wiztoStr(_excuteData->depth)) {
					eventsTemp->RemoveUserTypeList(idx);
					break;
				}
			}
		}
	}
	else if ("$get_item_value2" == str) {
		 i = stoi(top_wiz_stack_wiz_string(operandStack));  pop_wiz_stack_wiz_string(operandStack);

		if (now) {
			push_wiz_stack_wiz_string(operandStack, now->GetItemList(i).Get(0)); //chk
			return 1;
		}
		else {
			wiz_string temp = make_wiz_string("ERROR", 5); push_wiz_stack_wiz_string(operandStack, &temp);
			return 0;
		}
	}
	else if ("$space" == str) {
		push_wiz_stack_wiz_string(operandStack, " ");
		return 1;
	}
	else if ("$empty" == str) {
		push_wiz_stack_wiz_string(operandStack, "");
		return 1;
	}
	else if ("$move_up" == str) {
		wiz_string dir;

		for (i = 0; i < operandNum; ++i) {
			string temp = top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack);
			dir = dir + temp;
			//	cout << "temp is " << temp << endl;
		}

		//cout << " dir is  " << dir << endl;
		if (StringstartsWith(dir, "/."))
		{
			dir = Stringsubstring(dir, 3);
		}

		StringTokenizer tokenizer(dir, "/", builder, 1);
		vector<string> tokenVec;
		while (tokenizer.hasMoreTokens()) {
			tokenVec.push_back(tokenizer.nextToken());
		}
		dir = "/./";
		if (tokenVec.empty()) { push_wiz_stack_wiz_string(operandStack, dir); return 1; }
		for (i = 0; i < tokenVec.size() - 1; ++i) {
			dir = dir + tokenVec[i] + "/";
		}
		push_wiz_stack_wiz_string(operandStack, dir);
		return 1;
	}
	else {
		return 0;
	}
	return 1;
	// cf)
	// remove "
}


wiz_string ToBool3(user_type* global, wiz_map_wiz_string_and_wiz_string* parameters,  wiz_string* temp,
	EventInfo* info, wiz_string_builder* builder) /// has bug!
{
	wiz_string_tokenizer tokenizer(temp, wiz_vector_wiz_string{ "/" }, builder, 1);
	wiz_vector_wiz_string tokenVec;
	wiz_string result;

	while (tokenizer.hasMoreTokens()) {
		tokenVec.push_back(tokenizer.nextToken());
	}

	for (i = 0; i < tokenVec.size(); ++i)
	{
		result = result + "/";
		if (wizStringstartsWith(tokenVec[i], "$parameter.")) {
			int last = -1;
			for (int j = 0; j < tokenVec[i].size(); ++j)
			{
				if (tokenVec[i][j] == ' ' || tokenVec[i][j] == '{' || tokenVec[i][j] == '}' || tokenVec[i][j] == '=') {
					last = j - 1;
					break;
				}
			}
			if (last != -1)
			{
				wiz_string temp = FindParameters(parameters, wizStringsubstring(tokenVec[i], 0, last));

				if (!temp.empty()) {
					tokenVec[i] = wizStringreplace(wizStringsubstring(tokenVec[i], 0, last), wizStringsubstring(tokenVec[i], 0, last), temp)
						+ wizStringsubstring(tokenVec[i], last + 1);
				}
			}
			else
			{
				wiz_string temp = FindParameters(parameters, tokenVec[i]);
				if (!temp.empty()) {
					tokenVec[i] = temp;
				}
			}
		}
		else if (wizStringstartsWith(tokenVec[i], "$local.")) {
			int last = -1;
			for (int j = 0; j < tokenVec[i].size(); ++j)
			{
				if (tokenVec[i][j] == ' ' || tokenVec[i][j] == '{' || tokenVec[i][j] == '}' || tokenVec[i][j] == '=') {
					last = j - 1;
					break;
				}
			}
			if (last != -1)
			{
				wiz_string temp = FindLocals(info.locals, wizStringsubstring(tokenVec[i], 0, last));

				if (!temp.empty()) {
					tokenVec[i] = wizStringreplace(wizStringsubstring(tokenVec[i], 0, last), wizStringsubstring(tokenVec[i], 0, last), temp)
						+ wizStringsubstring(tokenVec[i], last + 1);
				}
			}
			else
			{
				string temp = FindLocals(info.locals, tokenVec[i]);
				if (!temp.empty()) {
					tokenVec[i] = temp;
				}
			}
		}

		result = result + tokenVec[i];
	}
	return result;
}
wiz_string ToBool4(user_type* now, user_type* global,  wiz_string* temp,  ExcuteData* excuteData, wiz_string_builder* builder)
{
	wiz_string result = temp;

	wizArrayStack<string> resultStack;
	user_type* ut;
	int chk = 0;
	int count_change = 0;

	int flag_A = 0;
	if (result.size() > 1 && result[0] == '/')
	{
		flag_A = 1;
	}
	result = ToBool3(global, excuteData->info.parameters, result, excuteData->info, builder);
	if (result.empty()) { return ""; }
	if (!flag_A) {
		result = string(result.c_str() + 1);
	}
	load_data_from_string_in_load_data(result, ut);
	result = ut.ToString();
	if (result.empty()) { return result; }

	if (ut.empty()) {
		return "";
	}
	if (get_user_type_list_size(&ut) == 0 && get_item_list_size_in_user_type(&ut) == 1) /// chk
	{
		if ('/' == result[0] && result.size() > 1)
		{
			string temp = Find(&global, result, builder);

			if (!temp.empty()) {
				result = move(temp);
			}return result;
		}
		else if (wizStringstartsWith(result, "$local.")) {
			string _temp = FindLocals(excuteData->info.locals, result);
			if (!_temp.empty()) {
				result = move(_temp);
			}return result;
		}
		else if (wizStringstartsWith(result, "$parameter.")) {
			string _temp = FindParameters(excuteData->info.parameters, result);
			if (!_temp.empty()) {
				result = move(_temp);
			}return result;
		}

	}

	vector<string> tokenVec;
	{
		wizStringTokenizer tokenizer(result, { " ", "\n", "\t", "\r" }, builder, 1); // , "{", "=", "}" }); //
																					   //wizStringTokenizer tokenizer2(result, { " ", "\n", "\t", "\r" } ); //

																					   //vector<string> tokenVec2;

		while (tokenizer.hasMoreTokens()) {
			tokenVec.push_back(tokenizer.nextToken());
		}

		for (i = tokenVec.size() - 1; i >= 0; --i)
		{
			string before = tokenVec[i];
			if ('/' == tokenVec[i][0] && tokenVec[i].size() > 1)
			{
				string _temp = Find(&global, tokenVec[i], builder);

				if ("" != _temp) {
					tokenVec[i] = move(_temp);
				}
			}
			else if (wizStringstartsWith(tokenVec[i], "$local.")) { // && length?
				string _temp = FindLocals(excuteData->info.locals, tokenVec[i]);
				if (!_temp.empty()) {
					tokenVec[i] = move(_temp);
				}
			}
			else if (wizStringstartsWith(tokenVec[i], "$parameter.")) { // && length?
				string _temp = FindParameters(excuteData->info.parameters, tokenVec[i]);
				if (!_temp.empty()) {
					tokenVec[i] = move(_temp);
				}
			}
		}
	}
	//cout << "result is " << result << endl;
	//
	wizArrayStack<string> operandStack;
	wizArrayStack<string> operatorStack;


	for (i = tokenVec.size() - 1; i >= 0; --i) {

		if (StringstartsWith(tokenVec[i], "$parameter.") ||
			StringstartsWith(tokenVec[i], "$local.") ||
			'$' != tokenVec[i][0] || ('$' == tokenVec[i][0] && tokenVec[i].size() == 1)
			) {
			push_wiz_stack_wiz_string(operandStack, tokenVec[i]);
		}
		else
		{
			// cout << tokenVec[i] << endl;
			top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack); // =
			top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack); // {
			operatorStack.push(tokenVec[i]);

			if (0 == operation(now, global, tokenVec[i], operandStack, excuteData, builder)) // chk!!
			{
				// chk removal here?
				cout << " 0 " << endl;
				_getch();
				//
				operatorStack.pop();
				push_wiz_stack_wiz_string(operandStack, "{");
				push_wiz_stack_wiz_string(operandStack, "=");
				push_wiz_stack_wiz_string(operandStack, tokenVec[i]);
				continue;
			}

			operandStack[size_wiz_stack_wiz_string(operandStack) - 2] = operandStack[size_wiz_stack_wiz_string(operandStack) - 1];
			top_wiz_stack_wiz_string(operandStack); pop_wiz_stack_wiz_string(operandStack); // } ?
		}
	}


	// ex) A = { B = 1 $C = { 3 } } D = { E }
	// =>  A = { B = 1 $C = 3  }  D = E
	// =>  A = { B = 1 $C = { 3 } } D = E  : ToDo! 

	vector<string> strVec;
	stack<int> chkBrace;

	chkBrace.push(0);

	for (i = size_wiz_stack_wiz_string(operandStack) - 1; i >= 0; --i)
	{
		if (get_wiz_stack_wiz_string(operandStack, i) == "}") {
			chkBrace.top()++;
			if (chkBrace.top() == 2)
			{
				string temp = strVec.back();
				strVec.pop_back();
				strVec.pop_back();
				strVec.push_back(temp);

				chkBrace.pop();
				continue;
			}
			chkBrace.pop();
		}
		else if (get_wiz_stack_wiz_string(operandStack, i) == "{") {
			chkBrace.top()++;
			chkBrace.push(0);
		}
		else {
			chkBrace.top()++;
		}
		strVec.push_back(get_wiz_stack_wiz_string(operandStack, i));
	}

	// todo!  $C = 3 => $C = { 3 } 
	{
		//StringTokenizer tokenizer(result, builder, 1);
		//result = "";
		clear_wiz_string_builder(builder);

		//while (tokenizer.hasMoreTokens()) {
		for (i = 0; i < strVec.size(); ++i) {
			 string temp = strVec[i]; // tokenizer.nextToken();

										   // chk!! @$paramter - removal? @$. (for regex)??
			if (temp.size() >= 3 && StringstartsWith(temp, "$.")) { // cf) @$. ?
																	  //result = result + temp + " ";
				append_wiz_string_builder(builder, temp.c_str(), temp.size());
				append_wiz_string_builder(builder, " ", 1);
			}
			else if (temp.size() >= 12 && StringstartsWith(temp, "$parameter.") || (temp.size()) >= 13 && StringstartsWith(temp, "@$parameter.")) {
				//result = result + temp + " ";
				append_wiz_string_builder(builder, temp.c_str(), temp.size());
				append_wiz_string_builder(builder, " ", 1);
			}
			else if (temp.size() >= 8 && StringstartsWith(temp, "$local.") || (temp.size() >= 9 && StringstartsWith(temp, "@$local."))) {
				//result = result + temp + " ";
				append_wiz_string_builder(builder, temp.c_str(), temp.size());
				append_wiz_string_builder(builder, " ", 1);
			}
			else if (
				(temp.size() >= 3 && temp[0] == '@' && temp[1] == '$')) {
				++i; // tokenizer.nextToken(); // = 
				string temp2 = strVec[i]; //tokenizer.nextToken();
										  //result = result + temp + " = { " + temp2 + " } ";

				append_wiz_string_builder(builder, temp.c_str(), temp.size());
				append_wiz_string_builder(builder, " = { ", 5);

				append_wiz_string_builder(builder, temp2.c_str(), temp2.size());
				append_wiz_string_builder(builder, " } ", 3);
			}
			else {
				//result = result + temp + " ";
				append_wiz_string_builder(builder, temp.c_str(), temp.size());
				append_wiz_string_builder(builder, " ", 1);
			}
		}
	}
	result = string(builder->Str(), builder->size());
	if (!result.empty()) {
		result.erase(result.begin() + result.size() - 1);
	}

	return result;
}

////////////////////////////public//////////////////////////////////
*/
// todo - error chk!
user_type load_data_from_file_in_load_data(wiz_string* fileName)
{
	user_type global;
	wiz_array_queue_token aq;
	// getline remake?
	 size_t line_max = 10000; // line_max = 1 is very slow? - solved!
	 size_t num = 102400;
	wiz_vector_wiz_string vec_of_str;
	wiz_string_builder builder;
	wiz_string_builder temp_builder;
	FILE* file = fopen(get_cstr_wiz_string(fileName), "rt");
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
	int error = 0;

	init_wiz_string(&temp4, "\n", 1);

	init_wiz_vector_wiz_string(&vec_of_str, 1024);
	init_wiz_string_builder(&builder, num * 10, "", 0);

	i = 0;
	temp2 = -1;

	while (1) {
		if (chk) {
			init_wiz_string_builder(&temp_builder, num, "", 0);
			real_count = fread((void*)str_wiz_string_builder(&temp_builder, NULL), sizeof(char), num, file);
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

				//free_wiz_string(&temp); //

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



	//free_wiz_string(&temp);
	free_wiz_string_builder(&builder);

	fclose(file);


	init_wiz_array_queue_token(&aq);

	{
		wiz_vector_wiz_string aqTemp = vec_of_str;
		size_t left = 0, right = size_wiz_vector_wiz_string(&vec_of_str) - 1;
		size_t x;

		for (x = left; x <= right; ++x)
		{
			//StringTokenizer tokenizer(std( (*aqTemp)[x] ) );
			//while (tokenizer.hasMoreTokens()) {
			//	aq.push(tokenizer.nextToken());
			//}
			wiz_string* statement = get_wiz_vector_wiz_string(&aqTemp, x);
			long long token_first = 0, token_last = 0; // idx of token in statement.
			int state = 0;
			long long i, j;

			for (i = 0; i < size_wiz_string(statement); ++i) {
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

						push_wiz_array_queue_token(&aq, &temp);

						//free_wiz_string(&temp.str);
					}
					{
						token temp;
						wiz_string str;

						temp.isComment = 0;
						init_wiz_string(&str, "=", 1);
						temp.str = str;

						push_wiz_array_queue_token(&aq, &temp);
						//free_wiz_string(&temp.str);
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

						//free_wiz_string(&temp.str);
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

						//	free_wiz_string(&temp.str);
					}
					{
						token temp;
						wiz_string str;

						temp.isComment = 0;
						init_wiz_string(&str, "{", 1);
						temp.str = str;

						push_wiz_array_queue_token(&aq, &temp);

						//		free_wiz_string(&temp.str);
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

						//		free_wiz_string(&temp.str);
					}
					{
						token temp;
						wiz_string str;

						temp.isComment = 0;
						init_wiz_string(&str, "}", 1);
						temp.str = str;

						push_wiz_array_queue_token(&aq, &temp);

						//		free_wiz_string(&temp.str);
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

						//	free_wiz_string(&temp.str);
					}
					j = 0;
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
						temp.str = substr_wiz_string(statement, i, j + 1);

						push_wiz_array_queue_token(&aq, &temp);

						//	free_wiz_string(&temp.str);
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
			}

			free_wiz_string(statement);
		}
	}


	{
		size_t i;

		for (i = 0; i < size_wiz_vector_wiz_string(&vec_of_str); ++i) {
			free_wiz_string(&(vec_of_str.vec[i]));
		}
		free_wiz_vector_wiz_string(&vec_of_str);
	}

	{
		wiz_string LEFT, RIGHT, EQ_STR, EMPTY;
		int state = 0;
		int braceNum = 0;
		long long state_reserve = 0;
		wiz_vector_any nestedUT;
		wiz_string var1, var2, val;

		int varOn = 0;

		init_wiz_string(&EMPTY, "", 0);

		init_in_user_type(&global, &EMPTY);
		init_wiz_vector_any(&nestedUT, 1);
		init_wiz_string(&var1, "", 0);
		init_wiz_string(&var2, "", 0);
		init_wiz_string(&val, "", 0);

		init_wiz_string(&LEFT, "{", 1);
		init_wiz_string(&RIGHT, "}", 1);
		init_wiz_string(&EQ_STR, "=", 1);


		push_back_wiz_vector_any(&nestedUT, (void*)&global);

		while (0 == empty_wiz_array_queue_token(&aq)) {
			//printf("test %d %s\n", state, get_cstr_wiz_string(Top(get_wiz_vector_any(&nestedUT, braceNum), &aq)));

			switch (state)
			{
			case 0:
				if (equal(&LEFT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					state = 2;
				}
				else {
					pair_int_and_token bsPair = LookUp(get_wiz_vector_any(&nestedUT, braceNum), &aq);

					if (bsPair.first) {
						if (equal(&EQ_STR, &bsPair.second.str)) {
							Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &var2);
							Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);
							state = 2;
						}
						else {
							if (Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &var1)) {
								add_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &EMPTY, &var1);
								state = 0;
							}
						}
					}
					else {
						if (Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &var1)) {
							add_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &EMPTY, &var1);
							state = 0;
						}
					}
				}
				break;
			case 1:
				if (equal(&RIGHT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);
					state = 0;
				}
				else {
					// syntax error.
					//rror = 1;
				}
				break;
			case 2:
				if (equal(&LEFT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					user_type ut_temp;
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);

					init_in_user_type(&ut_temp, &var2);
					add_user_type_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &ut_temp);

					user_type* pTemp;
					get_last_user_type_item_ref_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &var2, &pTemp);

					braceNum++;

					/// new nestedUT
					if (size_wiz_vector_any(&nestedUT) == braceNum) { /// changed 2014.01.23..
						push_back_wiz_vector_any(&nestedUT, NULL);
					}

					/// initial new nestedUT.
					set_wiz_vector_any(&nestedUT, braceNum, (void*)pTemp);
					///
					state = 3;
				}
				else {
					if (Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &val)) {
						add_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &var2, &val);

						assign_wiz_string(&var2, &EMPTY);
						assign_wiz_string(&val, &EMPTY);

						state = 0;
					}
				}
				break;
			case 3:
				if (equal(&RIGHT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);

					set_wiz_vector_any(&nestedUT, braceNum, NULL);
					braceNum--;

					state = 0;
				}
				else {
					{
						/// uisng struct
						state_reserve++;
						state = 4;
					}
					//else
					{
						//	throw  "syntax error 2 ";
					}
				}
				break;
			case 4:
				if (equal(&LEFT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);

					user_type temp;
					init_in_user_type(&temp, &EMPTY);

					add_user_type_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &temp);
					user_type* pTemp;
					get_last_user_type_item_ref_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &EMPTY, &pTemp);

					braceNum++;

					/// new nestedUT
					if (size_wiz_vector_any(&nestedUT) == braceNum) { /// changed 2014.01.23..
						push_back_wiz_vector_any(&nestedUT, NULL);
					}

					/// initial new nestedUT.
					set_wiz_vector_any(&nestedUT, braceNum, pTemp);
					///
					//}

					state = 5;
				}
				else if (equal(&RIGHT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);
					state = is_state0(state_reserve) ? 0 : 4;
					state_reserve--;

					{
						set_wiz_vector_any(&nestedUT, braceNum, NULL);
						braceNum--;
					}
				}
				else {
					pair_int_and_token bsPair = LookUp(get_wiz_vector_any(&nestedUT, braceNum), &aq);
					if (bsPair.first) {
						if (equal(&EQ_STR, &bsPair.second.str)) {
							// var2
							Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &var2);
							Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL); // pass EQ_STR
							state = 6;
						}
						else {
							// var1
							if (Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &var1)) {
								add_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &EMPTY, &var1);
								assign_wiz_string(&var1, &EMPTY);

								state = 4;
							}
						}
					}
					else
					{
						// var1
						if (Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &var1))
						{
							add_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &EMPTY, &var1);
							assign_wiz_string(&var1, &EMPTY);

							state = 4;
						}
					}
				}
				break;
			case 5:
				if (equal(&RIGHT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);

					//if (flag1 == 0) {
					set_wiz_vector_any(&nestedUT, braceNum, NULL);
					braceNum--;
					// }
					//
					state = 4;
				}

				else {
					int idx = -1;
					int num = -1;


					{
						/// uisng struct
						state_reserve++;
						state = 4;
					}
					//else
					{
						//	throw "syntax error 4  ";
					}
				}
				break;
			case 6:
				if (equal(&LEFT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);

					///
					{
						user_type ut_temp;
						init_in_user_type(&ut_temp, &var2);
						add_user_type_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &ut_temp);
						user_type* pTemp;
						get_last_user_type_item_ref_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &var2, &pTemp);
						assign_wiz_string(&var2, &EMPTY);
						braceNum++;

						/// new nestedUT
						if (size_wiz_vector_any(&nestedUT) == braceNum) {/// changed 2014.01.23..
							push_back_wiz_vector_any(&nestedUT, NULL);
						}

						/// initial new nestedUT.
						set_wiz_vector_any(&nestedUT, braceNum, pTemp);
					}
					///
					state = 7;
				}
				else {
					if (Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &val)) {
						add_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &var2, &val);
						assign_wiz_string(&var2, &EMPTY);
						assign_wiz_string(&val, &EMPTY);

						if (empty_wiz_array_queue_token(&aq))
						{
							//
						}
						else if (equal(&RIGHT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
							Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);

							{
								state = is_state0(state_reserve) ? 0 : 4;
								state_reserve--;

								{
									set_wiz_vector_any(&nestedUT, braceNum, NULL);
									braceNum--;
								}
							}
							{
								//state = 4;
							}
						}
						else {
							state = 4;
						}
					}
				}
				break;
			case 7:
				if (equal(&RIGHT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);
					//

					set_wiz_vector_any(&nestedUT, braceNum, NULL);
					braceNum--;
					//
					state = 4;
				}
				else {
					int idx = -1;
					int num = -1;

					{
						/// uisng struct
						state_reserve++;

						state = 4;
					}
					//else
					{
						//throw "syntax error 5 ";
					}
				}
				break;
			}
		}
		free_wiz_vector_any(&nestedUT);
		free_wiz_string(&var1);
		free_wiz_string(&var2);
		free_wiz_string(&val);

		free_wiz_string(&LEFT);
		free_wiz_string(&RIGHT);
		free_wiz_string(&EQ_STR);
		free_wiz_string(&EMPTY);

	//	free_user_type_in_user_type(&global);
	}

	{
		free_wiz_array_queue_token(&aq);
	}

	return global;
}

user_type load_data_from_string_in_load_data(wiz_string* str) // , int error? = 0
{
	user_type global;
	wiz_array_queue_token aq;
	int chk = 1;
	int error = 0;

	wiz_string* statement = str;
	long long token_first = 0, token_last = 0; // idx of token in statement.
	int state = 0;
	long long i, j;

	init_wiz_array_queue_token(&aq);


	for (i = 0; i < size_wiz_string(statement); ++i) {
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

				push_wiz_array_queue_token(&aq, &temp);

				//free_wiz_string(&temp.str);
			}
			{
				token temp;
				wiz_string str;

				temp.isComment = 0;
				init_wiz_string(&str, "=", 1);
				temp.str = str;

				push_wiz_array_queue_token(&aq, &temp);
				//free_wiz_string(&temp.str);
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

				//free_wiz_string(&temp.str);
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

				//	free_wiz_string(&temp.str);
			}
			{
				token temp;
				wiz_string str;

				temp.isComment = 0;
				init_wiz_string(&str, "{", 1);
				temp.str = str;

				push_wiz_array_queue_token(&aq, &temp);

				//		free_wiz_string(&temp.str);
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

				//		free_wiz_string(&temp.str);
			}
			{
				token temp;
				wiz_string str;

				temp.isComment = 0;
				init_wiz_string(&str, "}", 1);
				temp.str = str;

				push_wiz_array_queue_token(&aq, &temp);

				//		free_wiz_string(&temp.str);
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

				//	free_wiz_string(&temp.str);
			}
			j = 0;
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
				temp.str = substr_wiz_string(statement, i, j + 1);

				push_wiz_array_queue_token(&aq, &temp);

				//	free_wiz_string(&temp.str);
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
	}

	free_wiz_string(statement);

	{
		wiz_string LEFT, RIGHT, EQ_STR, EMPTY;
		int state = 0;
		int braceNum = 0;
		long long state_reserve = 0;
		wiz_vector_any nestedUT;
		wiz_string var1, var2, val;

		int varOn = 0;

		init_wiz_string(&EMPTY, "", 0);

		init_in_user_type(&global, &EMPTY);
		init_wiz_vector_any(&nestedUT, 1);
		init_wiz_string(&var1, "", 0);
		init_wiz_string(&var2, "", 0);
		init_wiz_string(&val, "", 0);

		init_wiz_string(&LEFT, "{", 1);
		init_wiz_string(&RIGHT, "}", 1);
		init_wiz_string(&EQ_STR, "=", 1);


		push_back_wiz_vector_any(&nestedUT, (void*)&global);

		while (0 == empty_wiz_array_queue_token(&aq)) {
			//printf("test %d %s\n", state, get_cstr_wiz_string(Top(get_wiz_vector_any(&nestedUT, braceNum), &aq)));

			switch (state)
			{
			case 0:
				if (equal(&LEFT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					state = 2;
				}
				else {
					pair_int_and_token bsPair = LookUp(get_wiz_vector_any(&nestedUT, braceNum), &aq);

					if (bsPair.first) {
						if (equal(&EQ_STR, &bsPair.second.str)) {
							Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &var2);
							Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);
							state = 2;
						}
						else {
							if (Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &var1)) {
								add_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &EMPTY, &var1);
								state = 0;
							}
						}
					}
					else {
						if (Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &var1)) {
							add_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &EMPTY, &var1);
							state = 0;
						}
					}
				}
				break;
			case 1:
				if (equal(&RIGHT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);
					state = 0;
				}
				else {
					// syntax error.
					//rror = 1;
				}
				break;
			case 2:
				if (equal(&LEFT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					user_type ut_temp;
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);

					init_in_user_type(&ut_temp, &var2);
					add_user_type_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &ut_temp);

					user_type* pTemp;
					get_last_user_type_item_ref_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &var2, &pTemp);

					braceNum++;

					/// new nestedUT
					if (size_wiz_vector_any(&nestedUT) == braceNum) { /// changed 2014.01.23..
						push_back_wiz_vector_any(&nestedUT, NULL);
					}

					/// initial new nestedUT.
					set_wiz_vector_any(&nestedUT, braceNum, (void*)pTemp);
					///
					state = 3;
				}
				else {
					if (Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &val)) {
						add_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &var2, &val);

						assign_wiz_string(&var2, &EMPTY);
						assign_wiz_string(&val, &EMPTY);

						state = 0;
					}
				}
				break;
			case 3:
				if (equal(&RIGHT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);

					set_wiz_vector_any(&nestedUT, braceNum, NULL);
					braceNum--;

					state = 0;
				}
				else {
					{
						/// uisng struct
						state_reserve++;
						state = 4;
					}
					//else
					{
						//	throw  "syntax error 2 ";
					}
				}
				break;
			case 4:
				if (equal(&LEFT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);

					user_type temp;
					init_in_user_type(&temp, &EMPTY);

					add_user_type_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &temp);
					user_type* pTemp;
					get_last_user_type_item_ref_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &EMPTY, &pTemp);

					braceNum++;

					/// new nestedUT
					if (size_wiz_vector_any(&nestedUT) == braceNum) { /// changed 2014.01.23..
						push_back_wiz_vector_any(&nestedUT, NULL);
					}

					/// initial new nestedUT.
					set_wiz_vector_any(&nestedUT, braceNum, pTemp);
					///
					//}

					state = 5;
				}
				else if (equal(&RIGHT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);
					state = is_state0(state_reserve) ? 0 : 4;
					state_reserve--;

					{
						set_wiz_vector_any(&nestedUT, braceNum, NULL);
						braceNum--;
					}
				}
				else {
					pair_int_and_token bsPair = LookUp(get_wiz_vector_any(&nestedUT, braceNum), &aq);
					if (bsPair.first) {
						if (equal(&EQ_STR, &bsPair.second.str)) {
							// var2
							Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &var2);
							Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL); // pass EQ_STR
							state = 6;
						}
						else {
							// var1
							if (Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &var1)) {
								add_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &EMPTY, &var1);
								assign_wiz_string(&var1, &EMPTY);

								state = 4;
							}
						}
					}
					else
					{
						// var1
						if (Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &var1))
						{
							add_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &EMPTY, &var1);
							assign_wiz_string(&var1, &EMPTY);

							state = 4;
						}
					}
				}
				break;
			case 5:
				if (equal(&RIGHT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);

					//if (flag1 == 0) {
					set_wiz_vector_any(&nestedUT, braceNum, NULL);
					braceNum--;
					// }
					//
					state = 4;
				}

				else {
					int idx = -1;
					int num = -1;


					{
						/// uisng struct
						state_reserve++;
						state = 4;
					}
					//else
					{
						//	throw "syntax error 4  ";
					}
				}
				break;
			case 6:
				if (equal(&LEFT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);

					///
					{
						user_type ut_temp;
						init_in_user_type(&ut_temp, &var2);
						add_user_type_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &ut_temp);
						user_type* pTemp;
						get_last_user_type_item_ref_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &var2, &pTemp);
						assign_wiz_string(&var2, &EMPTY);
						braceNum++;

						/// new nestedUT
						if (size_wiz_vector_any(&nestedUT) == braceNum) {/// changed 2014.01.23..
							push_back_wiz_vector_any(&nestedUT, NULL);
						}

						/// initial new nestedUT.
						set_wiz_vector_any(&nestedUT, braceNum, pTemp);
					}
					///
					state = 7;
				}
				else {
					if (Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, &val)) {
						add_item_in_user_type((user_type*)get_wiz_vector_any(&nestedUT, braceNum), &var2, &val);
						assign_wiz_string(&var2, &EMPTY);
						assign_wiz_string(&val, &EMPTY);

						if (empty_wiz_array_queue_token(&aq))
						{
							//
						}
						else if (equal(&RIGHT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
							Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);

							{
								state = is_state0(state_reserve) ? 0 : 4;
								state_reserve--;

								{
									set_wiz_vector_any(&nestedUT, braceNum, NULL);
									braceNum--;
								}
							}
							{
								//state = 4;
							}
						}
						else {
							state = 4;
						}
					}
				}
				break;
			case 7:
				if (equal(&RIGHT, Top(get_wiz_vector_any(&nestedUT, braceNum), &aq))) {
					Pop(get_wiz_vector_any(&nestedUT, braceNum), &aq, NULL);
					//

					set_wiz_vector_any(&nestedUT, braceNum, NULL);
					braceNum--;
					//
					state = 4;
				}
				else {
					int idx = -1;
					int num = -1;

					{
						/// uisng struct
						state_reserve++;

						state = 4;
					}
					//else
					{
						//throw "syntax error 5 ";
					}
				}
				break;
			}
		}
		free_wiz_vector_any(&nestedUT);
		free_wiz_string(&var1);
		free_wiz_string(&var2);
		free_wiz_string(&val);

		free_wiz_string(&LEFT);
		free_wiz_string(&RIGHT);
		free_wiz_string(&EQ_STR);
		free_wiz_string(&EMPTY);

		//	free_user_type_in_user_type(&global);
	}

	{
		free_wiz_array_queue_token(&aq);
	}

	return global;
}


int save_data_in_load_data(wiz_string* fileName, user_type* global, int option)
{
	FILE* file = fopen(get_cstr_wiz_string(fileName), "wt");
	if (NULL == file) { return 0; }

	if (1 == option) {
		save1_in_user_type(file, global, 0);
	}
	else if (2 == option) {
		save2_in_user_type(file, global, 0);
	}

	fclose(file);

	return 1;
}


/*
int add_data_in_load_data(user_type* global, wiz_string* position, wiz_string* data, wiz_string* cond_str, ExcuteData* excuteData, wiz_string_builder* builder) {
	user_type utTemp;
	int isTrue = 0;
	pair_int_and_wiz_vector_any finded;
	size_t i, k;

	utTemp = load_data_from_string_in_load_data(data);

	finded = find_user_type_in_user_type(global, position, builder);
	if (finded.first) {
		for (i = 0; i < size_wiz_vector_any(&finded.second); ++i) {
			int item_n = 0;
			int user_n = 0;

			if (size_wiz_string(cond_str) == 0) {
				wiz_string* _cond = cond_str;
				condition cond;
				
				// _cond = ToBool4(get_wiz_vector_any(&finded.second, i), global, _cond, excuteData, builder);

				init_in_cond(&cond, _cond, (user_type*)get_wiz_vector_any(&finded.second, i), global, builder);

				while (next_in_cond(&cond));
				
				if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0]) // || cond.Now().size()  != 1
				{
					// free all cond data?

					continue;
				}
				// free all cond data?
			}

			for (k = 0; k < utTemp.GetIListSize(); ++k) {
				if (utTemp.IsItemList(k)) {
					get_wiz_vector_any(&finded.second, i)->AddItem(utTemp.GetItemList(item_n).GetName(), utTemp.GetItemList(item_n).Get(0));
					item_n++;
				}
				else {
					get_wiz_vector_any(&finded.second, i)->AddUserTypeItem(*utTemp.GetUserTypeList(user_n));
					user_n++;
				}
			}
			isTrue = 1;
		}
		return isTrue;
	}
	else {
		return 0;
	}
}

wiz_string get_item_list_data_in_load_data(user_type* global, wiz_string* position, wiz_string* condstr, ExcuteData* excuteData, wiz_string_builder* builder)
{
	wiz_string str;
	pair_int_and_wiz_vector_any finded = find_user_type_in_user_type(global, position, builder);
	size_t i;

	if (finded.first) {
		wiz_string_builder temp_builder;

		init_wiz_string_builder(&temp_builder, 1024, "", 0);

		for (i = 0; i <  size_wiz_vector_any(&finded.second); ++i) {
			wiz_string temp;
			if (0 == empty_wiz_string(condstr)) {
				wiz_string _cond;
				condition cond;

				//_cond = ToBool4((user_type*)get_wiz_vector_any(&finded.second, i), global, condstr, excuteData, builder);

				init_in_condition(&cond, &_cond, (user_type*)get_wiz_vector_any(&finded.second, i), global, builder);

				while (next_in_condition(&cond));

				if (size_wiz_stack_wiz_string(now_in_condition(&cond)) != 1 || 
					0 != strcmp("TRUE", get_cstr_wiz_string(get_wiz_stack_wiz_string(now_in_condition(&cond), 0))))
				{
					clear_now_condition(&cond);

					free_in_condition(&cond);
					free_wiz_string(&_cond);
					continue;
				}
				clear_now_condition(&cond);

				free_in_condition(&cond);
				free_wiz_string(&_cond);
			}
			temp = item_list_to_string_in_user_type((user_type*)get_wiz_vector_any(&finded.second, i), builder);
			append_wiz_string_builder(&temp_builder, get_cstr_wiz_string(&temp), size_wiz_string(&temp));
			append_char_wiz_string_builder(&temp_builder, '\n');
			free_wiz_string(&temp);
		}
		init_wiz_string(&str, str_wiz_string_builder(&temp_builder, NULL), size_wiz_string_builder(&temp_builder));

		free_wiz_string_builder(&temp_builder);
		free_wiz_vector_any(&finded.second);
		return str;
	}
	else {
		return make_empty_wiz_string();
	}
}
*/


