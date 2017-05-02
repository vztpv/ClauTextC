
#include "wiz_map_wiz_string_and_wiz_string.h"

#define FORM(A, B) A##B

struct FORM(binary_node_, wiz_string_and_wiz_string)
{
	pair_wiz_string_and_wiz_string val;
	struct FORM(binary_node_, wiz_string_and_wiz_string)* left;
	struct FORM(binary_node_, wiz_string_and_wiz_string)* right;
};

typedef struct FORM(binary_node_, wiz_string_and_wiz_string) FORM(binary_node_, wiz_string_and_wiz_string);


void init_binary_node_wiz_string_and_wiz_string(FORM(binary_node_, wiz_string_and_wiz_string)* node) 
{
	node->left = NULL;
	node->right = NULL;
}

void remove_all_wiz_string_and_wiz_string(FORM(binary_node_, wiz_string_and_wiz_string)* node)
{
	FORM(binary_node_, wiz_string_and_wiz_string)* left;
	FORM(binary_node_, wiz_string_and_wiz_string)* right;
	if (NULL == node) {
		return;
	}

	left = node->left;
	right = node->right;

	free(node);

	remove_all_wiz_string_and_wiz_string(left);
	remove_all_wiz_string_and_wiz_string(right);
}

void inorder_wiz_string_and_wiz_string(FORM(binary_node_, wiz_string_and_wiz_string)* node, pair_wiz_string_and_wiz_string arr[], size_t* count)
{
	if (NULL == node) { return; }

	inorder_wiz_string_and_wiz_string(node->left, arr, count);

	arr[*count] = node->val;
	++(*count);

	inorder_wiz_string_and_wiz_string(node->right, arr, count);
}

FORM(binary_node_, wiz_string_and_wiz_string)* balancing_string_and_wiz_string(pair_wiz_string_and_wiz_string arr[], size_t start, size_t last)
{
	FORM(binary_node_, wiz_string_and_wiz_string)* temp = NULL;
	size_t middle = (start + last) / 2;
	if (start > last) { return NULL; }
	
	temp = malloc(sizeof(FORM(binary_node_, wiz_string_and_wiz_string)*) * 1);
	temp->val = arr[middle];
	
	temp->left = balancing_string_and_wiz_string(arr, start, middle - 1);
	temp->right = balancing_string_and_wiz_string(arr, middle + 1, last);
	
	return temp;
}

void FORM(init_wiz_map_, wiz_string_and_wiz_string)(FORM(wiz_map_, wiz_string_and_wiz_string)* _map)
{
	_map->root = NULL;
	_map->count = 0;
}
void FORM(free_wiz_map_, wiz_string_and_wiz_string)(FORM(wiz_map_, wiz_string_and_wiz_string)* _map)
{
	remove_all_wiz_string_and_wiz_string(_map->root);
}

void FORM(balancing_wiz_map_, wiz_string_and_wiz_string)(FORM(wiz_map_, wiz_string_and_wiz_string)* _map)
{
	size_t count = 0;
	pair_wiz_string_and_wiz_string* arr;

	arr = malloc(sizeof(pair_wiz_string_and_wiz_string) * _map->count);

	inorder_wiz_string_and_wiz_string(_map->root, arr, &count);

	FORM(free_wiz_map_, wiz_string_and_wiz_string)(_map);
	
	_map->root = balancing_string_and_wiz_string(arr, 0, count - 1);

	free(arr);
}

int FORM(insert_wiz_map_, wiz_string_and_wiz_string)(FORM(wiz_map_, wiz_string_and_wiz_string)* _map,
	pair_wiz_string_and_wiz_string* val, int option) // optino, 0 : nothing, 1 : insert and balancing!
{
	FORM(binary_node_, wiz_string_and_wiz_string)* temp = _map->root;
	FORM(binary_node_, wiz_string_and_wiz_string)* temp2 = temp;
	
	if (is_exist_wiz_map_wiz_string_and_wiz_string(_map, val)) {
		return 0;
	}

	if (NULL == _map->root)
	{
		_map->root = malloc(sizeof(FORM(binary_node_, wiz_string_and_wiz_string)) * 1);
		_map->count = 0;
	}
	else 
	{
		while (NULL != temp) {
			if (strcmp(get_cstr_wiz_string(&(temp->val.first)), get_cstr_wiz_string(&(val->first))) < 0) {
				temp2 = temp;
				temp = temp->right;
			}
			else if (strcmp(get_cstr_wiz_string(&(temp->val.first)), get_cstr_wiz_string(&(val->first))) > 0) {
				temp2 = temp;
				temp = temp->left;
			}
		}
		if (strcmp(get_cstr_wiz_string(&(temp2->val.first)), get_cstr_wiz_string(&(val->first))) < 0) {
			temp2->right = malloc(sizeof(FORM(binary_node_, wiz_string_and_wiz_string)) * 1);
			temp2->right->val = *val;
		}
		else if (strcmp(get_cstr_wiz_string(&(temp2->val.first)), get_cstr_wiz_string(&(val->first))) > 0) {
			temp2->left = malloc(sizeof(FORM(binary_node_, wiz_string_and_wiz_string)) * 1);
			temp2->left->val = *val;
		}
		_map->count++;
	}

	if (1 == option)
	{
		FORM(balancing_wiz_map_, wiz_string_and_wiz_string)(_map);
	}

	return 1;
}


int FORM(get_wiz_map_, wiz_string_and_wiz_string)(FORM(wiz_map_, wiz_string_and_wiz_string)* _map, pair_wiz_string_and_wiz_string* val)
{
	FORM(binary_node_, wiz_string_and_wiz_string)* temp = _map->root;

	if (NULL == _map->root) {
		return 0;
	}

	while (NULL != temp) {
		if (strcmp(get_cstr_wiz_string(&(temp->val.first)), get_cstr_wiz_string(&(val->first))) < 0) {
			temp = temp->right;
		}
		else if (strcmp(get_cstr_wiz_string(&(temp->val.first)), get_cstr_wiz_string(&(val->first))) > 0) {
			temp = temp->left;
		}
		else {
			*val = temp->val;
			return 1;
		}
	}

	return 0;
}


int FORM(is_exist_wiz_map_, wiz_string_and_wiz_string)(FORM(wiz_map_, wiz_string_and_wiz_string)* _map, pair_wiz_string_and_wiz_string* val)
{
	FORM(binary_node_, wiz_string_and_wiz_string)* temp = _map->root;

	if (NULL == _map->root) {
		return 0;
	}

	while (NULL != temp) {
		if (strcmp(get_cstr_wiz_string(&(temp->val.first)), get_cstr_wiz_string(&(val->first))) < 0) {
			temp = temp->right;
		}
		else if (strcmp(get_cstr_wiz_string(&(temp->val.first)), get_cstr_wiz_string(&(val->first))) > 0) {
			temp = temp->left;
		}
		else {
			return 1;
		}
	}

	return 0;
}

int FORM(empty_wiz_map_, wiz_string_and_wiz_string)(FORM(wiz_map_, wiz_string_and_wiz_string)* _map)
{
	return 0 == _map->count;
}

