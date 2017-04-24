
#include "wiz_load_data_user_type.h"


void push_comment_in_user_type(user_type* ut, wiz_string* comment)
{
	push_back_wiz_vector_wiz_string(&(ut->comment_list), comment);
}
size_t get_comment_list_size_in_user_type(user_type* ut)
{ 
	return size_wiz_vector_wiz_string(&(ut->comment_list));
}
wiz_string* get_comment_list_in_user_type(user_type* ut, size_t idx)
{
	return get_wiz_vector_wiz_string(&(ut->comment_list), idx);
}
size_t get_ilist_size_in_user_type(user_type* ut)
{
	return size_wiz_vector_int(&(ut->ilist));
}
size_t get_item_list_size_in_user_type(user_type* ut)
{ 
	return size_wiz_vector_item_type(&(ut->item_list));
}
size_t get_user_type_list_size_in_user_type(user_type* ut)
{
	return size_wiz_vector_any2(&(ut->user_type_list));
}
item_type* get_item_list_in_user_type(user_type* ut, size_t idx)
{ 
	return get_wiz_vector_item_type(&(ut->item_list), idx);
}
user_type* get_user_type_list_in_user_type(user_type* ut, size_t idx)
{
	return (user_type*)get_wiz_vector_any2(&(ut->user_type_list), idx);
}

int is_item_list_in_user_type(user_type* ut, size_t idx)
{
	return 1 == *get_wiz_vector_int(&(ut->ilist), idx);
}
int is_user_type_List_in_user_type(user_type* ut, size_t idx)
{
	return 2 == *get_wiz_vector_int(&(ut->ilist), idx);
}

// remove? - void add_item_list_in_user_type(user_type* ut, item_type* item)

user_type* get_parent_in_user_type(user_type* ut)
{
	return ut->parent;
}


void link_user_type_in_user_type(user_type* ut, user_type* ut2) 
{
	int x = 2;
	push_back_wiz_vector_any2(&(ut->user_type_list), (void*)ut2);
	push_back_wiz_vector_int(&(ut->ilist), &x);
	ut2->parent = ut;
}

void _remove_in_user_type(user_type* ut)
{ 
	// todo - add clear!
	size_t i;

	free_wiz_string(&(ut->name));

	for (i = 0; i < size_wiz_vector_item_type(&(ut->item_list)); ++i) {
		free_wiz_string(&(get_wiz_vector_item_type(&(ut->item_list), i)->name));
		free_wiz_string(&(get_wiz_vector_item_type(&(ut->item_list), i)->value));
	}
	free_wiz_vector_item_type(&(ut->item_list));
	
	free_wiz_vector_int(&(ut->ilist));

	for (i = 0; i < size_wiz_vector_wiz_string(&(ut->comment_list)); ++i) {
		free_wiz_string(get_wiz_vector_wiz_string(&(ut->comment_list), i));
	}
	free_wiz_vector_wiz_string(&(ut->comment_list));

	for (i = 0; i < get_user_type_list_size_in_user_type(ut); ++i) {
		free_user_type_in_user_type(get_user_type_list_in_user_type(ut, i));
	}
	free_wiz_vector_any2(&(ut->user_type_list));
}

void init_in_user_type(user_type* ut, wiz_string* name)
{
	init_wiz_string(&(ut->name), "", 0);
	assign_wiz_string(&(ut->name), name);

	ut->parent = NULL;
	init_wiz_vector_int(&(ut->ilist), 1);
	init_wiz_vector_wiz_string(&(ut->comment_list), 1);
	init_wiz_vector_item_type(&(ut->item_list), 1);
	init_wiz_vector_any2((void*)(&(ut->user_type_list)), 1);
}

// deep copy?
void init_from_other_user_type_in_user_type(user_type* ut_this, user_type* ut_other)
{
	size_t i;
	int moved_temp;

	ut_this->parent = ut_other->parent;
	init_wiz_string(&(ut_this->name), "", 0);
	init_wiz_vector_int(&(ut_this->ilist), 1);
	init_wiz_vector_wiz_string(&(ut_this->comment_list), 1);
	init_wiz_vector_item_type(&(ut_this->item_list), 1);
	init_wiz_vector_any2((void*)(&(ut_this->user_type_list)), 1);
	
	moved_temp = ut_other->name.moved;
	assign_wiz_string(&(ut_this->name), &(ut_other->name));
	ut_other->name.moved = moved_temp;

	for (i = 0; i < get_ilist_size_in_user_type(ut_other); ++i) {
		push_back_wiz_vector_int(&(ut_this->ilist), get_wiz_vector_int(&(ut_other->ilist), i));
	}
	for (i = 0; i < get_item_list_size_in_user_type(ut_other); ++i) {
		item_type* p_temp_item = (get_wiz_vector_item_type(&(ut_other->item_list), i));
		item_type temp_item;
		init_item_value_in_item_type(&temp_item, "", 0, "", 0);
		
		moved_temp = p_temp_item->name.moved;
		assign_wiz_string(&temp_item.name, &(p_temp_item->name));
		p_temp_item->name.moved = moved_temp;
		
		moved_temp = p_temp_item->value.moved;
		assign_wiz_string(&temp_item.value, &(p_temp_item->value));
		p_temp_item->value.moved = moved_temp;

		push_back_wiz_vector_item_type(&(ut_this->item_list), &temp_item);
	}
	for (i = 0; i < get_comment_list_size_in_user_type(ut_other); ++i) {
		wiz_string* p_temp = (get_wiz_vector_wiz_string(&(ut_other->comment_list), i));
		wiz_string temp;
		init_wiz_string(&temp, "", 0);

		moved_temp = p_temp->moved;
		assign_wiz_string(&temp, (p_temp));
		p_temp->moved = moved_temp;

		push_back_wiz_vector_wiz_string(&(ut_this->comment_list), &temp);
	}

	for (i = 0; i < get_user_type_list_size_in_user_type(ut_other); ++i) {
		user_type* temp = get_user_type_list_in_user_type(ut_other, i);
		user_type copy_temp;
		init_from_other_user_type_in_user_type(&copy_temp, temp);
		push_back_wiz_vector_any2(&(ut_this->user_type_list), (void*)&copy_temp);
		free_user_type_in_user_type(&copy_temp);
	}
}

void free_user_type_in_user_type(user_type* ut)
{
	_remove_in_user_type(ut);
}

// cf) c++ user_type&&, not used?
//void Reset_in_user_type(user_type* ut_this, user_type* ut) { }

// val : 1 or 2
size_t _get_index_in_user_type(user_type* ut, wiz_vector_int ilist, int val, size_t start){ }
// test? - need more thinking!
size_t _get_item_index_from_ilist_index_in_user_type(user_type* ut, wiz_vector_int ilist, size_t ilist_idx){ }
size_t _get_user_type_index_from_ilist_index_in_user_type(user_type* ut, wiz_vector_int ilist, size_t ilist_idx){ }
// type : 1 or 2
size_t _get_ilist_index_in_user_type(user_type* ut, wiz_vector_int ilist, size_t index, int type){ }
void remove_item_list_by_idx_in_user_type(user_type* ut, size_t idx){ }//
void remove_user_type_list_by_idx_in_user_type(user_type* ut, size_t idx, int chk){ }// chk
void remove_item_list_by_var_name_in_user_type(user_type* ut, wiz_string* varName){ }
void remove_item_list_in_user_type(user_type* ut){ }//
void remove_empty_item_in_user_type(user_type* ut){ }
void remove_user_type_in_user_type(user_type* ut){ }
// remove? - void remove_user_type_list_in_user_type(user_type* ut){ } // chk
void remove_user_type_list_by_var_name_in_user_type(user_type* ut, wiz_string* varName, int chk){ }
//			
void remove_list_in_user_type(user_type* ut, size_t idx){ } // ilis_t_idx!

int empty_in_user_type(user_type* ut){ }

void insert_item_by_ilist_in_user_type(user_type* ut, size_t ilis_t_idx, wiz_string* name, wiz_string* item){ }

// chk
void insert_user_type_by_ilist_in_user_type(user_type* ut, int ilis_t_idx, user_type* item){ }
void add_item_in_user_type(user_type* ut, wiz_string* name, wiz_string* item)
{ 
	item_type temp;
	int x = 1;

	init_item_value_in_item_type(&temp, "", 0, "", 0);

	temp.name = *name;
	temp.value = *item;

	temp.name.moved = 0;
	temp.name.moved = 0;

	name->moved = 1;
	item->moved = 1;

	push_back_wiz_vector_item_type(&(ut->item_list), &temp);
	push_back_wiz_vector_int(&(ut->ilist), &x);
}
void add_user_type_item_in_user_type(user_type* ut_this, user_type* item)
{
	user_type temp;
	int x = 2;
	init_from_other_user_type_in_user_type(&temp, item); // deep copy!
	free_user_type_in_user_type(item);

	temp.parent = ut_this;

	push_back_wiz_vector_int(&(ut_this->ilist), &x);

	push_back_wiz_vector_any2(&(ut_this->user_type_list), (void*)&temp); // deep copy
	free_user_type_in_user_type(&temp);
}
void add_item_at_front_in_user_type(user_type* ut, wiz_string* name, wiz_string* item){ }
void add_user_type_item_at_front_in_user_type(user_type* ut, user_type* item){ }


wiz_vector_item_type get_item_in_user_type(user_type* ut, wiz_string* name){ }

// regex to SetItem?
int set_item_in_user_type(user_type* ut, wiz_string* name, wiz_string* value){ }

// add set Data
int set_item_by_name_in_user_type(user_type* ut, size_t var_idx, wiz_string* value){ }

wiz_vector_any get_user_type_item_in_user_type(user_type* ut, wiz_string* name){ }
// deep copy.
wiz_vector_any get_copy_user_type_item_in_user_type(user_type* ut, wiz_string* name){ }
int get_user_type_item_ref_in_user_type(user_type* ut, size_t idx, user_type* ref){ }
int get_last_user_type_item_ref_in_user_type(user_type* ut, wiz_string* name, user_type** ref)
{ 
	int idx = -1;

	for (size_t i = get_user_type_list_size_in_user_type(ut) - 1; i >= 0; --i)
	{
		if (0 == strcmp(get_cstr_wiz_string(name), 
			get_cstr_wiz_string(&(((user_type*)get_wiz_vector_any2(&(ut->user_type_list), i))->name)))
		)
		{
			idx = i;
			break;
		}
	}
	if (idx > -1) {
		(*ref) = (user_type*)get_wiz_vector_any2(&(ut->user_type_list), idx);
	}
	return idx > -1;
}

void save1_in_user_type(FILE* stream, user_type* ut, int depth)
{
	int itemListCount = 0;
	int userTypeListCount = 0;

	size_t i;
	const size_t commentListSize = get_comment_list_size_in_user_type(ut);
	
	for (i = 0; i < commentListSize; ++i) {
		for (int k = 0; k < depth; ++k) {
			fputc('\t', stream);
		}
		fputs(get_cstr_wiz_string(get_wiz_vector_wiz_string(&(ut->comment_list), i)), stream);

		if (i < commentListSize - 1 || 0 == empty_wiz_vector_int(&(ut->ilist))) {
			fputc('\n', stream);
		}
	}

	for (i = 0; i < get_ilist_size_in_user_type(ut); ++i) {
		if (1 == *get_wiz_vector_int(&(ut->ilist), i)) { // item_type
			for (int j = 0; j < 1; j++) { 
				for (int k = 0; k < depth; ++k) {
					fputc('\t', stream);
				}
				if (0 != strcmp(get_cstr_wiz_string(&(get_wiz_vector_item_type(&(ut->item_list), itemListCount))->name), "")) {
					fputs(get_cstr_wiz_string(&get_wiz_vector_item_type(&(ut->item_list), itemListCount)->name), stream);
					fputc('=', stream);
				}
				fputs(get_cstr_wiz_string(&get_wiz_vector_item_type(&(ut->item_list), itemListCount)->value), stream);
			}
			if (i != get_ilist_size_in_user_type(ut) - 1) {
				fputc('\n', stream);
			}
			itemListCount++;
		}
		else if (2 == *get_wiz_vector_int(&(ut->ilist), i)) { // user_type
			// std::cout << "UserTypeList" << endl;
			for (int k = 0; k < depth; ++k) {
				fputc('\t', stream);
			}

			if (0 != strcmp(get_cstr_wiz_string(&((user_type*)get_wiz_vector_any2(&(ut->user_type_list), userTypeListCount))->name), "")) {
				fputs(get_cstr_wiz_string(&((user_type*)get_wiz_vector_any2(&(ut->user_type_list), userTypeListCount))->name), stream);
				fputc('=', stream);
			}

			fputc('{', stream);
			fputc('\n', stream);

			save1_in_user_type(stream, (user_type*)get_wiz_vector_any2(&(ut->user_type_list), userTypeListCount), depth + 1);

			fputc('\n', stream);

			for (int k = 0; k < depth; ++k) {
				fputc('\t', stream);
			}
			fputc('}', stream);
			if (i != get_ilist_size_in_user_type(ut) - 1) {
				fputc('\n', stream);
			}

			userTypeListCount++;
		}
	}
}

// todo?
void save2_in_user_type(FILE* stream, user_type* ut, int depth) { }

// todo
//wiz_string item_list_to_string_in_user_type(user_type* ut){ }
//wiz_string item_list_names_to_string_in_user_type(user_type* ut){ }
//wiz_vector_wiz_string userType_list_names_to_string_array_in_user_type(user_type* ut){ }
//wiz_string user_type_list_names_to_string_in_user_type(user_type* ut){ }
//wiz_string to_string_in_user_type(user_type* ut){ }
//pair_int_and_wiz_vector_user_type_ptr find_user_type_in_user_type(user_type* ut, user_type* global, wiz_string* _position, wiz_string_builder* builder){ }

