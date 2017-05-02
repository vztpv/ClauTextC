
#ifndef WIZ_LOAD_DATA_H
#define WIZ_LOAD_DATA_H

#include <stdlib.h>
#include "wiz_load_data_user_type.h"
#include "wiz_map_wiz_string_and_wiz_string.h"
#include "wiz_map_wiz_string_and_user_type.h"
#include "wiz_stack_int.h"
#include "wiz_stack_wiz_string.h"
#include "wiz_stack_any.h"
#include "wiz_stack_size_t.h"

struct EventInfo
{
	user_type* eventUT;
	wiz_stack_any nowUT; //
	wiz_stack_size_t userType_idx;
	wiz_map_wiz_string_and_wiz_string parameters;
	wiz_map_wiz_string_and_wiz_string locals;
	wiz_string id; //
	wiz_stack_wiz_string conditionStack;
	wiz_stack_int state;
	wiz_string return_value;
	wiz_string option;
};

typedef struct EventInfo EventInfo;

struct ExcuteData
{
	user_type* pEvents;
	EventInfo info; // chk!
	int chkInfo;

	wiz_map_wiz_string_and_user_type* pObjectMap;
	wiz_map_wiz_string_and_user_type* pModule;

	long long depth;
};

typedef struct ExcuteData ExcuteData;

//
user_type load_data_from_file_in_load_data(wiz_string* fileName);
user_type load_data_from_string_in_load_data(wiz_string* str);
int save_data_in_load_data(wiz_string* fileName, user_type* global, int option);

// search_item
// search_user_type
// add_data_at_afront
// add_data
// insert
// add_no_name_user_type
// set_data (by_name)
// set_data_by_index
// get_data
// get_item_list_data
// get_item_list_names_data
// get_user_type_list_names_data
// get_data_by_var_name
// remove_by_var_name
// remove
// remove_user_type
// remove_item_type
// remove_by_index
// exist_data
// exist_user_type
// exist_one_user_type
// exist_item
// 
// search_item
// search_user_type
// replace_item
// remove_user_type_total
// remove_data_type
// replace_date_type
// replace_date_type2
// replace_data_type1
// replace_data_type1_2
// replace_data_type2
//

#endif

