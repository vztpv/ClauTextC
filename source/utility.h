
#ifndef UTILITY_H
#define UTILITY_H

#include <stdlib.h>
#include "wiz_string.h"
#include "wiz_string_builder.h"
#include "wiz_load_data_condition.h"


enum { TYPE_WIZ_INTEGER = 1, TYPE_WIZ_DOUBLE, TYPE_WIZ_STRING, TYPE_WIZ_DATE,
				TYPE_WIZ_DATETIMEA, TYPE_WIZ_DATETIMEB};


int get_type(wiz_string* str);


wiz_string make_wiz_string_from_cstr(const char* cstr);

wiz_string make_wiz_string(const char* cstr, const size_t size);

wiz_string make_empty_wiz_string();

int compare_wiz_string_in_utility(wiz_string* str1, wiz_string* str2, wiz_string_builder* builder, int type); // type = 0

wiz_string bool_operation(wiz_string* op, wiz_string* x, wiz_string* y);

void add_space(wiz_string* str, wiz_string* temp, wiz_string_builder* builder);

wiz_string replace_wiz_string(wiz_string* origin, wiz_string* mark, wiz_string* changed, wiz_string_builder* builder);


wiz_string wiz_ll_to_string(long long x);

wiz_string wiz_ld_to_string(long double x);

int rand_int();

wiz_string make_wiz_string_from_other_wiz_string(wiz_string* other);


void clear_now_condition(condition* cond);


int ChkExist(wiz_string* str);

int comp_wiz_string_and_cstr(wiz_string* x, char* y);

wiz_string union_wiz_string(wiz_string* x, wiz_string* y);

#endif

