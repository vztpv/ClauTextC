
#include "wiz_load_data_user_type.h"
#include "wiz_string_tokenizer.h"
#include "wiz_deck_pair_user_type_ptr_and_int.h"


void push_comment_in_user_type(user_type* ut, wiz_string* comment)
{
	push_back_wiz_vector_wiz_string(&(ut->comment_list), comment);
	//free_wiz_string(comment);
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
int get_ilist_in_user_type(user_type* ut, size_t idx)
{
	return *get_wiz_vector_int(&ut->ilist, idx);
}
int is_item_list_in_user_type(user_type* ut, size_t idx)
{
	return 1 == *get_wiz_vector_int(&(ut->ilist), idx);
}
int is_user_type_List_in_user_type(user_type* ut, size_t idx)
{
	return 2 == *get_wiz_vector_int(&(ut->ilist), idx);
}


void set_ilist_in_user_type(user_type* ut, size_t idx, int val)
{
	*get_wiz_vector_int(&ut->ilist, idx) = val;
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
		wiz_string* temp = get_wiz_vector_wiz_string(&(ut->comment_list), i);
		free_wiz_string(temp);
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
void remove_item_list_by_idx_in_user_type(user_type* ut, size_t idx)
{
	int count = 0;
	size_t i, k;
	// left shift start idx, to end, at itemList. and resize!
	free_item_value_in_item_type(get_item_list_in_user_type(ut, idx));

	for (i = idx + 1; i < get_item_list_size_in_user_type(ut); ++i) {
		*get_item_list_in_user_type(ut, i - 1) = *get_item_list_in_user_type(ut, i);
	}

	// resize - todo make! or decrease size?
	decrease_size_wiz_vector_item_type(&ut->item_list);

	//  ilist left shift and resize - count itemType!
	for (i = 0; i < get_ilist_size_in_user_type(ut); ++i) {
		if (get_ilist_in_user_type(ut, i) == 1) { count++; }
		if (count == idx + 1) {
			// left shift!and resize!
			for (k = i + 1; k < get_ilist_size_in_user_type(ut); ++k) {
				set_ilist_in_user_type(ut, k - 1, get_ilist_in_user_type(ut, k));
			}
			decrease_size_wiz_vector_int(&ut->ilist);
			break;
		}
	}

}//
void remove_user_type_list_by_idx_in_user_type(user_type* ut, size_t idx)
{
	int count = 0;
	size_t i, k;

	free_user_type_in_user_type(get_user_type_list_in_user_type(ut, idx));
	
	// left shift start idx, to end, and resize!
	for (i = idx + 1; i < get_user_type_list_size_in_user_type(ut); ++i) {
		*get_user_type_list_in_user_type(ut, i - 1) = *get_user_type_list_in_user_type(ut, i);
	}
	decrease_size_wiz_vector_any2(&ut->user_type_list);

	//  ilist left shift and resize - count itemType!
	for (i = 0; i < size_wiz_vector_int(&ut->ilist); ++i) {
		if (get_ilist_in_user_type(ut, i) == 2) { count++; }
		if (count == idx + 1) {
			// left shift!and resize!
			for (k = i + 1; k < size_wiz_vector_int(&ut->ilist); ++k) {
				set_ilist_in_user_type(ut, k - 1, get_ilist_in_user_type(ut, k));
			}
			decrease_size_wiz_vector_int(&ut->ilist);
			break;
		}
	}
}
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


wiz_vector_item_type get_item_in_user_type(user_type* ut, wiz_string* name)
{
	wiz_vector_item_type vec;
	size_t i;

	init_wiz_vector_item_type(&vec, 1);

	for (i = 0; i < size_wiz_vector_item_type(&(ut->item_list)); ++i) {
		if (equal_wiz_string(&get_wiz_vector_item_type(&(ut->item_list), i)->name, name)) {
			push_back_wiz_vector_item_type(&vec, get_wiz_vector_item_type(&ut->item_list, i));
		}
	}

	return vec;
}

// regex to SetItem?
int set_item_in_user_type(user_type* ut, wiz_string* name, wiz_string* value){ }

// add set Data
int set_item_by_name_in_user_type(user_type* ut, size_t var_idx, wiz_string* value){ }

wiz_vector_any get_user_type_item_in_user_type(user_type* ut, wiz_string* name)
{
	wiz_vector_any result;
	size_t i;

	init_wiz_vector_any(&result, 1);

	for (i = 0; i < get_user_type_list_size_in_user_type(ut); ++i) {
		if (equal_wiz_string(&((user_type*)get_user_type_list_in_user_type(ut, i))->name, name)) {
			push_back_wiz_vector_any(&result, get_user_type_list_in_user_type(ut, i));
		}
	}

	return result;
}
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
			// cout << "UserTypeList" << endl;
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
void save2_in_user_type(FILE* stream, user_type* ut, int depth) 
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
					fputs(" = ", stream);
				}
				fputs(get_cstr_wiz_string(&get_wiz_vector_item_type(&(ut->item_list), itemListCount)->value), stream);
			}
			if (i != get_ilist_size_in_user_type(ut) - 1) {
				fputc('\n', stream);
			}
			itemListCount++;
		}
		else if (2 == *get_wiz_vector_int(&(ut->ilist), i)) { // user_type
															  // cout << "UserTypeList" << endl;
			for (int k = 0; k < depth; ++k) {
				fputc('\t', stream);
			}

			if (0 != strcmp(get_cstr_wiz_string(&((user_type*)get_wiz_vector_any2(&(ut->user_type_list), userTypeListCount))->name), "")) {
				fputs(get_cstr_wiz_string(&((user_type*)get_wiz_vector_any2(&(ut->user_type_list), userTypeListCount))->name), stream);
				fputs(" = ", stream);
			}

			fputc('{', stream);
			fputc('\n', stream);

			save2_in_user_type(stream, (user_type*)get_wiz_vector_any2(&(ut->user_type_list), userTypeListCount), depth + 1);

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

// todo
//wiz_string item_list_names_to_string_in_user_type(user_type* ut){ }
//wiz_vector_wiz_string userType_list_names_to_string_array_in_user_type(user_type* ut){ }
//wiz_string user_type_list_names_to_string_in_user_type(user_type* ut){ }
wiz_string to_string_in_user_type(user_type* ut, wiz_string_builder* builder) 
{
	wiz_string temp;
	int itemListCount = 0;
	int userTypeListCount = 0;
	size_t i;

	clear_wiz_string_builder(builder);

	for (i = 0; i < get_ilist_size_in_user_type(ut); ++i) {
		if (get_ilist_in_user_type(ut, i) == 1) {
			wiz_string* name = &get_item_list_in_user_type(ut, itemListCount)->name;
			wiz_string* value = &get_item_list_in_user_type(ut, itemListCount)->value;

			if (0 != strcmp(get_cstr_wiz_string(name), "")) {
				append_wiz_string_builder(builder, get_cstr_wiz_string(name), size_wiz_string(name));
				append_char_wiz_string_builder(builder, ' ');
				append_char_wiz_string_builder(builder, '=');
				append_char_wiz_string_builder(builder, ' ');
			}

			append_wiz_string_builder(builder, get_cstr_wiz_string(value), size_wiz_string(value));
			
			if (i != get_ilist_size_in_user_type(ut) - 1) {
				append_char_wiz_string_builder(builder, ' ');
			}
			itemListCount++;
		}
		else if (get_ilist_in_user_type(ut, i) == 2) {
			wiz_string* name = &get_user_type_list_in_user_type(ut, userTypeListCount)->name;
			wiz_string data;
			wiz_string_builder tempBuilder;
			
			if (strcmp(get_cstr_wiz_string(name), "") != 0) {
				append_wiz_string_builder(builder, get_cstr_wiz_string(name), size_wiz_string(name));

				append_char_wiz_string_builder(builder, ' ');
				append_char_wiz_string_builder(builder, '=');
				append_char_wiz_string_builder(builder, ' ');
			}
			append_char_wiz_string_builder(builder, ' ');
			append_char_wiz_string_builder(builder, '{');
			append_char_wiz_string_builder(builder, ' ');

			init_wiz_string_builder(&tempBuilder, 1024, "", 0);
			data = to_string_in_user_type(ut, &tempBuilder);
			
			append_wiz_string_builder(builder, get_cstr_wiz_string(&data), size_wiz_string(&data));

			free_wiz_string(&data);
			free_wiz_string_builder(&tempBuilder);

			append_char_wiz_string_builder(builder, ' ');
			append_char_wiz_string_builder(builder, '}');

			if (i != get_ilist_size_in_user_type(ut) - 1) {
				append_char_wiz_string_builder(builder, ' ');
			}

			userTypeListCount++;
		}
	} 

	init_wiz_string(&temp, str_wiz_string_builder(builder, NULL), size_wiz_string_builder(builder));
	return temp;
}

wiz_string item_list_to_string_in_user_type(user_type* ut, wiz_string_builder* builder)
{
	wiz_string result;
	size_t itemListCount = 0;
	size_t i, j;

	clear_wiz_string_builder(builder);

	for (i = 0; i < get_item_list_size_in_user_type(ut); ++i) {
		for (j = 0; j < 1; j++) {
			if (0 != strcmp(get_cstr_wiz_string(&get_item_list_in_user_type(ut, itemListCount)->name), "")) {
				wiz_string* name = &get_item_list_in_user_type(ut, itemListCount)->name;
				append_wiz_string_builder(builder, get_cstr_wiz_string(name), size_wiz_string(name));
				append_char_wiz_string_builder(builder, ' ');
				append_char_wiz_string_builder(builder, '=');
				append_char_wiz_string_builder(builder, ' ');
			}
			wiz_string* value = &get_item_list_in_user_type(ut, itemListCount)->value;
			append_wiz_string_builder(builder, get_cstr_wiz_string(value), size_wiz_string(value));
		}
		if (i != get_item_list_size_in_user_type(ut) - 1)
		{
			append_char_wiz_string_builder(builder, ' '); // '/' -> ' '
		}
		itemListCount++;
	}

	init_wiz_string(&result, str_wiz_string_builder(builder, NULL), size_wiz_string_builder(builder));
	return result;
}

pair_int_and_wiz_vector_any find_user_type_in_user_type(user_type* global, wiz_string* _position, wiz_string_builder* builder)
{
	int exist = 0;
	int chk = 0; // for break?

	pair_int_and_wiz_vector_any result; // pair<bool, vector<UserType*>>
	wiz_string delim, start, EMPTY, UT;
	wiz_vector_wiz_string delim_vec;
	wiz_string_tokenizer tokenizer;// (position, "/", builder, 1);
	wiz_vector_wiz_string strVec;
	wiz_deck_pair_user_type_ptr_and_int utDeck;
	pair_user_type_ptr_and_int utTemp;
	wiz_string* position = _position;
	wiz_vector_any temp;
	wiz_vector_any temp2;

	init_wiz_string(&delim, "/", 1);
	init_wiz_string(&start, "/./", 3);
	init_wiz_string(&EMPTY, "", 0);
	init_wiz_string(&UT, "$ut", 3);
	init_wiz_vector_wiz_string(&delim_vec, 1);
	push_back_wiz_vector_wiz_string(&delim_vec, &delim);
	init_wiz_string_tokenizer(&tokenizer, position, &delim_vec, builder, 1); // option : 1
	init_wiz_vector_wiz_string(&strVec, 1);
	init_wiz_deck_pair_user_type_ptr_and_int(&utDeck);
	init_wiz_vector_any(&temp, 1);
	init_wiz_vector_any(&temp2, 1);

	while (1) { // it is a just trick!
		if (!empty_wiz_string(position) && get_cstr_wiz_string(position)[0] == '@') { 
			//position.erase(position.begin()); 
			erase_wiz_string(position, 0);
		}
		if (empty_wiz_string(position)) { 
			//temp.push_back(global); 
			push_back_wiz_vector_any(&temp, (void*)global);
			break; 
		}
		if (0 == strcmp(get_cstr_wiz_string(position), ".")) { 
			//temp.push_back(global); 
			push_back_wiz_vector_any(&temp, (void*)global);
			break;
		}
		if (0 == strcmp(get_cstr_wiz_string(position), "/./")) { 
			//temp.push_back(global); 
			push_back_wiz_vector_any(&temp, (void*)global);
			break;
		} // chk..
		if (0 == strcmp(get_cstr_wiz_string(position), "/.")) {
			//temp.push_back(global); 
			push_back_wiz_vector_any(&temp, (void*)global);
			break; 
		}
		
		if (starts_with_wiz_string(position, &start))
		{
			substr_and_assign_wiz_string(position, 3, size_wiz_string(position));
		}

		utTemp.first = global;
		utTemp.second = 0;

		for (int i = 0; i < count_tokens_wiz_string_tokenizer(&tokenizer); ++i) {
			wiz_string* strTemp = next_token_wiz_string_tokenizer(&tokenizer);
			if (0 == strcmp(get_cstr_wiz_string(strTemp), "root") && i == 0) {
			}
			else {
				//strVec.push_back(strTemp);
				push_back_wiz_vector_wiz_string(&strVec, strTemp);
			}

			if ((size_wiz_vector_wiz_string(&strVec) >= 1) && 
				(0 == strcmp(" ", get_cstr_wiz_string(get_wiz_vector_wiz_string(&strVec, 
													size_wiz_vector_wiz_string(&strVec)  - 1))))
				)
			{
				assign_wiz_string(get_wiz_vector_wiz_string(&strVec, size_wiz_vector_wiz_string(&strVec)  - 1), &EMPTY);
			}
			else if ((size_wiz_vector_wiz_string(&strVec)  >= 1) &&
				(0 == strcmp("_", get_cstr_wiz_string(get_wiz_vector_wiz_string(&strVec, 
																	size_wiz_vector_wiz_string(&strVec)  - 1))))
				)
			{
				assign_wiz_string(get_wiz_vector_wiz_string(&strVec, size_wiz_vector_wiz_string(&strVec) - 1), &EMPTY);
			}
		}

		// maybe, has bug!
		{
			int count = 0;
			size_t i;
			chk = 0;

			for (i = 0; i < size_wiz_vector_wiz_string(&strVec) ; ++i) {
				if (0 == strcmp(get_cstr_wiz_string(get_wiz_vector_wiz_string(&strVec, i)), "..")) {
					count++;
				}
				else {
					break;
				}
			}

			//reverse(strVec.begin(), strVec.end());
			reverse_wiz_vector_wiz_string(&strVec);

			for (int i = 0; i < count; ++i) {
				if (utTemp.first == NULL) {
					chk = 1;
					// return{ 0, vector< UserType* >() };
				}
				//utTemp.first = utTemp.first->GetParent();
				utTemp.first = get_parent_in_user_type((user_type*)utTemp.first);
				//strVec.pop_back();
				pop_back_wiz_vector_wiz_string(&strVec);
			}
			if (chk) { break; }
			//reverse(strVec.begin(), strVec.end());
			reverse_wiz_vector_wiz_string(&strVec);
		}

		//utDeck.push_front(utTemp);
		push_front_wiz_deck_pair_user_type_ptr_and_int(&utDeck, &utTemp);

		while (0 == empty_wiz_deck_pair_user_type_ptr_and_int(&utDeck)) {
			utTemp = *front_wiz_deck_pair_user_type_ptr_and_int(&utDeck);
			pop_front_wiz_deck_pair_user_type_ptr_and_int(&utDeck);

			if (utTemp.second < size_wiz_vector_wiz_string(&strVec)  &&
				starts_with_wiz_string(get_wiz_vector_wiz_string(&strVec, utTemp.second), &UT) // "$ut")
				)
			{
				wiz_string str_temp;
				int idx;
				wiz_string* pstr = get_wiz_vector_wiz_string(&strVec, utTemp.second);
				pair_user_type_ptr_and_int pair_temp;
				str_temp = substr_wiz_string(pstr, 3, size_wiz_string(pstr));
				idx = atoi(get_cstr_wiz_string(&str_temp));
				free_wiz_string(&str_temp);
				if (idx < 0 || idx >= get_user_type_list_size_in_user_type(utTemp.first)) {
					break;
				}
				pair_temp.first = get_user_type_list_in_user_type(utTemp.first, idx);
				pair_temp.second = utTemp.second + 1;
				
				push_front_wiz_deck_pair_user_type_ptr_and_int(&utDeck, &pair_temp);
			}
			else if (utTemp.second < size_wiz_vector_wiz_string(&strVec)  && 
				0 == strcmp(get_cstr_wiz_string(get_wiz_vector_wiz_string(&strVec, utTemp.second)), "$"))
			{
				for (int j = get_user_type_list_size_in_user_type(utTemp.first) - 1; j >= 0; --j) {
					user_type* x = get_user_type_list_in_user_type(utTemp.first, j);
					pair_user_type_ptr_and_int pair_temp;
					
					pair_temp.first = x;
					pair_temp.second = utTemp.second + 1;
					push_back_wiz_deck_pair_user_type_ptr_and_int(&utDeck, &pair_temp);
				}
			}
			else if (utTemp.second < size_wiz_vector_wiz_string(&strVec))
			{
				free_wiz_vector_any(&temp2);

				temp2 = get_user_type_item_in_user_type((user_type*)utTemp.first,
					get_wiz_vector_wiz_string(&strVec, utTemp.second));
				if (temp2.num == 0) {
					wiz_vector_any x = get_user_type_item_in_user_type(utTemp.first, get_wiz_vector_wiz_string(&strVec, utTemp.second));
					for (int j = size_wiz_vector_any(&x) - 1; j >= 0; --j) {
						pair_user_type_ptr_and_int pair_temp;
						pair_temp.first = get_wiz_vector_any(&x, j);
						pair_temp.second = utTemp.second + 1;

						push_front_wiz_deck_pair_user_type_ptr_and_int(&utDeck, &pair_temp);
					}
					free_wiz_vector_any(&x);
				}
			}

			if (utTemp.second == size_wiz_vector_wiz_string(&strVec) ) {
				exist = 1;

				push_back_wiz_vector_any(&temp, utTemp.first);
			}
		}
		
		break;
	}

	if (0 == exist) { 
		result.first = 0;
		// free
		free_wiz_vector_any(&temp);
	}
	else {
		result.first = 1;
		result.second = temp;
	}

	// free
	free_wiz_vector_any(&temp2);
	free_wiz_vector_wiz_string(&delim_vec);
	free_wiz_string(&delim);
	free_wiz_string(&start);
	free_wiz_string(&EMPTY);
	free_wiz_string(&UT);
	free_wiz_string_tokenizer(&tokenizer);
	free_wiz_vector_wiz_string(&strVec);
	free_wiz_deck_pair_user_type_ptr_and_int(&utDeck);
	
	return result;
}