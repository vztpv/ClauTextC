
#ifndef PAIR_H
#define PAIR_H

#include "wiz_vector.h"
#include "wiz_load_data_token.h"

struct pair_int_and_wiz_vector_user_type_ptr
{
	int first;
	wiz_vector_any second;
};

struct pair_bool_and_token
{
	int first;
	token second;
};

struct pair_int_and_token
{
	int first;
	token second;
};

typedef struct pair_int_and_wiz_vector_user_type_ptr 
		pair_int_and_wiz_vector_user_type_ptr;

typedef struct pair_bool_and_token pair_bool_and_token;

typedef struct pair_int_and_token pair_int_and_token;

#endif
