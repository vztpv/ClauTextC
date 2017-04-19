
#include "wiz_vector_wiz_string.h"

#define FORM(A, B) A##B

void FORM(expand_wiz_vector_, wiz_string) (FORM(wiz_vector_, wiz_string)* vec)
{
	wiz_string* tempArr;
	size_t i;

	tempArr = malloc(sizeof(wiz_string) * vec->capacity * 2);
	for (i = 0; i < vec->capacity; ++i) {
		tempArr[i] = vec->vec[i];
	}
	vec->capacity *= 2;
	free(vec->vec);
	vec->vec = tempArr;
}

void FORM(init_wiz_vector_, wiz_string) (FORM(wiz_vector_, wiz_string)* vec, const size_t capacity)
{
	vec->num = 0;
	vec->capacity = capacity;
	vec->vec = malloc(sizeof(wiz_string) * capacity);
}
void FORM(free_wiz_vector_, wiz_string) (FORM(wiz_vector_, wiz_string)* vec)
{
	if (vec->vec != NULL) {
		free(vec->vec);
		vec->vec = NULL;
		vec->num = 0;
	}
}

wiz_string* FORM(get_wiz_vector_, wiz_string) (FORM(wiz_vector_, wiz_string)* vec, size_t idx)
{
	return &vec->vec[idx];
}
void FORM(set_wiz_vector_, wiz_string) (FORM(wiz_vector_, wiz_string)* vec, size_t idx, wiz_string* pval)
{
	vec->vec[idx] = *pval;
}

void FORM(push_back_wiz_vector_, wiz_string) (FORM(wiz_vector_, wiz_string)* vec, wiz_string* pval)
{
	if (vec->capacity == vec->num) {
		FORM(expand_wiz_vector_, wiz_string) (vec);
	}
	vec->vec[vec->num] = *pval;
	vec->num++;
}
void FORM(pop_back_wiz_vector_, wiz_string) (FORM(wiz_vector_, wiz_string)* vec)
{
	vec->num--;
}
wiz_string* FORM(back_wiz_vector_, wiz_string) (FORM(wiz_vector_, wiz_string)* vec)
{
	return &vec->vec[vec->num - 1];
}

// boolean
int FORM(empty_wiz_vector_, wiz_string) (FORM(wiz_vector_, wiz_string)* vec)
{
	return 0 == vec->num;
}

int FORM(size_wiz_vector_, wiz_string) (FORM(wiz_vector_, wiz_string)* vec)
{
	return vec->num;
}
