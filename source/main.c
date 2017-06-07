

#define _CRT_SECURE_NO_WARNINGS


#include <vld.h> // only visual studio.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

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

#include "wiz_stack_event_info.h"

#include "wiz_map_wiz_string_and_int.h"


wiz_string excute_module(wiz_string* mainStr, user_type* _global, ExcuteData* excuteData)
{
	int exit = 0;
	int i, j;
	user_type global = *_global;
	wiz_map_wiz_string_and_user_type objectMap; // string -> user_type
	wiz_map_wiz_string_and_user_type moduleMap;
	wiz_map_wiz_string_and_user_type* objectMapPtr;
	wiz_map_wiz_string_and_user_type* moduleMapPtr;
	wiz_string module_value; // = "";
	// data, event load..
	wiz_stack_event_info eventStack;
	wiz_map_wiz_string_and_int convert;
	wiz_vector_any _events;
	user_type events;
	user_type* eventPtr;
	user_type Main;
	wiz_string_builder builder;
	//
	wiz_string EMPTY_STR;
	wiz_string ID_STR;
	wiz_string LOCAL_STR;
	wiz_string LOCAL_STR2;
	wiz_string MAIN_STR;
	wiz_string CALL_STR;
	wiz_string IF_STR;
	wiz_string ELSE_STR;
	wiz_string EVENT_STR;
	wiz_string ROOT_STR;
	wiz_string TRUE_STR;

	// init
	init_wiz_string(&EMPTY_STR, "", 0);
	init_wiz_string(&ID_STR, "id", 2);
	init_wiz_string(&LOCAL_STR, "$local", 6);
	init_wiz_string(&LOCAL_STR2, "$local.", 7);
	init_wiz_string(&MAIN_STR, "Main", 4);
	init_wiz_string(&CALL_STR, "$call", 5);
	init_wiz_string(&ELSE_STR, "$else", 5);
	init_wiz_string(&EVENT_STR, "Event", 5);
	init_wiz_string(&IF_STR, "$if", 3);
	init_wiz_string(&ROOT_STR, "root", 4);
	init_wiz_string(&TRUE_STR, "TRUE", 4);
	
	init_wiz_map_wiz_string_and_user_type(&objectMap);
	init_wiz_map_wiz_string_and_user_type(&moduleMap);
	init_wiz_string(&module_value, "", 0);
	init_wiz_stack_event_info(&eventStack, 1);
	init_wiz_map_wiz_string_and_int(&convert);
		//init_wiz_vector_any(&_events, 1);
	init_in_user_type(&events, &EMPTY_STR);
	init_in_user_type(&Main, &EMPTY_STR);
	init_wiz_string_builder(&builder, 102400, "", 0);


	if (NULL == excuteData || NULL == excuteData->pModule) {
		moduleMapPtr = &moduleMap;
	}
	else {
		moduleMapPtr = excuteData->pModule;
	}

	if (NULL == excuteData || NULL == excuteData->pObjectMap) {
		objectMapPtr = &objectMap;
	}
	else {
		objectMapPtr = excuteData->pObjectMap;
	}
	
	if (NULL == excuteData || NULL == excuteData->pEvents) {
		_events = get_user_type_item_in_user_type(&global, &EVENT_STR);
		for (i = 0; i < size_wiz_vector_any(&_events); ++i) {
			link_user_type_in_user_type(&events, (user_type*)get_wiz_vector_any(&_events, i));
		}
		//remove_user_type_list_by_var_name_in_user_type(&global, &EVENT_STR, 0); // no free for Event data
		eventPtr = &events;
	}
	else {
		init_wiz_vector_any(&_events, 1);
		eventPtr = excuteData->pEvents;
	}

	// event table setting
	for (i = 0; i < get_user_type_list_size_in_user_type(eventPtr); ++i)
	{
		wiz_vector_item_type x = get_item_in_user_type(get_user_type_list_in_user_type(eventPtr, i), &ID_STR);
		if (!empty_wiz_vector_item_type(&x)) {
			pair_wiz_string_and_int temp;
			temp.first = make_wiz_string_from_other_wiz_string(&get_wiz_vector_item_type(&x, 0)->value);
			temp.second = i;

			insert_wiz_map_wiz_string_and_int(&convert, &temp, 0); // no balancing!
		}
		else {
			// error
		}
		free_wiz_vector_item_type(&x);
	}
	// covert balancing!?

	// start from Main.
	if (NULL == excuteData || excuteData->chkInfo == 0) { // chk!!
		user_type* _Main = NULL;
		wiz_vector_any temp = get_user_type_item_in_user_type(&global, &MAIN_STR);
		if (temp.num == 0 && empty_wiz_string(mainStr))
		{
			printf("do not exist Main\n");
			
			free_wiz_vector_any(&temp);
			// free
			{
				free_wiz_string(&EMPTY_STR);
				free_wiz_string(&ID_STR);
				free_wiz_string(&LOCAL_STR); free_wiz_string(&LOCAL_STR2);
				free_wiz_string(&MAIN_STR);
				free_wiz_string(&CALL_STR);
				free_wiz_string(&IF_STR);
				free_wiz_string(&ELSE_STR);
				free_wiz_string(&EVENT_STR);
				free_wiz_string(&ROOT_STR);
				free_wiz_string(&TRUE_STR);
				{

				}
				free_wiz_map_wiz_string_and_user_type(&objectMap);
				{

				}
				free_wiz_map_wiz_string_and_user_type(&moduleMap);
				free_wiz_string(&module_value);
				for (i = 0; i < size_wiz_stack_event_info(&eventStack); ++i) {
					free_all_event_info(get_wiz_stack_event_info(&eventStack, i));
				}
				free_wiz_stack_event_info(&eventStack);
				free_wiz_map_wiz_string_and_int(&convert);
				free_wiz_vector_any(&_events);
				shallow_free_user_type_in_user_type(&events);
				shallow_free_user_type_in_user_type(&Main);
				free_wiz_string_builder(&builder);
			}

			return make_empty_wiz_string();
		}

		if (empty_wiz_string(mainStr)) {
			wiz_vector_any temp = get_user_type_item_in_user_type(&global, &MAIN_STR);
			_Main = (user_type*)get_wiz_vector_any(&temp, 0);
			link_user_type_in_user_type(&Main, _Main); //Main.LinkUserType(_Main);

			free_wiz_vector_any(&temp);
		}
		else {
			Main = load_data_from_string_in_load_data(mainStr);
		}

		free_wiz_vector_any(&temp);
		remove_item_list_by_var_name_in_user_type(&global, &MAIN_STR, 0); //?
		{
			int no;
			event_info info;
			pair_wiz_string_and_wiz_string temp;
			wiz_vector_any call_str_ut;
			wiz_vector_item_type id_str_vec;
			int idx_val = 0;

			init_event_info(&info);

			info.eventUT = get_user_type_list_in_user_type(&Main, 0);

			push_wiz_stack_size_t(&info.userType_idx, &idx_val);
			
			temp.first = ID_STR;
			call_str_ut = get_user_type_item_in_user_type(info.eventUT, &CALL_STR);
			id_str_vec = get_item_in_user_type((user_type*)get_wiz_vector_any(&call_str_ut, 0), &ID_STR);
			temp.second = get_wiz_vector_item_type(&id_str_vec, 0)->value;
			
			// make_pair("id", info.eventUT->get_user_type_item_in_user_type("$call")[0]->GetItem("id")[0].Get(0))

			insert_wiz_map_wiz_string_and_wiz_string(&info.parameters, &temp, 0);
			
			// free
			free_wiz_vector_item_type(&id_str_vec);
			free_wiz_vector_any(&call_str_ut);

			temp.first = ID_STR;
			get_wiz_map_wiz_string_and_wiz_string(&info.parameters, &temp);
			info.id = temp.second;
			{
				pair_wiz_string_and_int pairTemp;
				pairTemp.first = info.id;
				get_wiz_map_wiz_string_and_int(&convert, &pairTemp);
				no = pairTemp.second;
			}
			for (i = 0; i < get_user_type_list_size_in_user_type(get_user_type_list_in_user_type(eventPtr, no)); ++i) {
				if (equal_wiz_string(&get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), i)->name, &LOCAL_STR)) {
					for (j = 0; j < get_item_list_size_in_user_type(get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), i)); ++j) {
						wiz_string name = make_wiz_string_from_other_wiz_string(&get_item_list_in_user_type(get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), i), j)->value);
						wiz_string value = EMPTY_STR;
						pair_wiz_string_and_wiz_string temp;

						temp.first = name;
						temp.second = value;

						insert_wiz_map_wiz_string_and_wiz_string(&info.locals, &temp, 0);
					}
					break;
				}
			}

			push_wiz_stack_event_info(&eventStack, &info);
		}
	}
	else {
		push_wiz_stack_event_info(&eventStack, &excuteData->info); // chk..
	}

	// main loop
	while (!empty_wiz_stack_event_info(&eventStack))
	{
		int info_chk = 1;
		event_info info = deep_copy_event_info(top_wiz_stack_event_info(&eventStack)); // copy??
		pair_wiz_string_and_wiz_string temp;
		pair_wiz_string_and_int temp2;
		wiz_string str;
		int state = 0; 
		int no;

		temp.first = ID_STR;

		get_wiz_map_wiz_string_and_wiz_string(&info.parameters, &temp);
		str = temp.second;

		temp2.first = str;
		get_wiz_map_wiz_string_and_int(&convert, &temp2);
		no = temp2.second;

		if (size_wiz_stack_size_t(&info.userType_idx) == 1 && 
			*get_wiz_stack_size_t(&info.userType_idx, 0) >= get_user_type_list_size_in_user_type(get_user_type_list_in_user_type(eventPtr, no))
		)
		{
			if (size_wiz_stack_event_info(&eventStack) == 1)
			{
				module_value = make_wiz_string_from_other_wiz_string(&top_wiz_stack_event_info(&eventStack)->return_value);
			}
			
			free_all_event_info(top_wiz_stack_event_info(&eventStack));
			pop_wiz_stack_event_info(&eventStack);
			
			free_all_event_info(&info);
			
			continue;
		}

		{ // has bug!! WARNNING!!
			user_type* val = NULL;
			if (size_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) == 1) {
				if (get_user_type_list_size_in_user_type(get_user_type_list_in_user_type(eventPtr, no)) > 
						*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)) {

					val = get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), 
						*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx));

					if (*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) >= 1 && equal_wiz_string(&val->name, &ELSE_STR)
						&& !equal_wiz_string(&get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no),
							*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) - 1)->name, &IF_STR) ) {
						
						// free
						{
							free_wiz_string(&EMPTY_STR);
							free_wiz_string(&ID_STR);
							free_wiz_string(&LOCAL_STR); free_wiz_string(&LOCAL_STR2);
							free_wiz_string(&MAIN_STR);
							free_wiz_string(&CALL_STR);
							free_wiz_string(&IF_STR);
							free_wiz_string(&ELSE_STR);
							free_wiz_string(&EVENT_STR);
							free_wiz_string(&ROOT_STR);
							free_wiz_string(&TRUE_STR);

							{

							}
							free_all_wiz_map_wiz_string_and_user_type(&objectMap);
							{

							}
							free_all_wiz_map_wiz_string_and_user_type(&moduleMap);
							free_wiz_string(&module_value);
							for (i = 0; i < size_wiz_stack_event_info(&eventStack); ++i) {
								free_all_event_info(get_wiz_stack_event_info(&eventStack, i));
							}
							free_wiz_stack_event_info(&eventStack);
							if (convert.count > 0) {
								pair_wiz_string_and_int* temp = malloc(sizeof(pair_wiz_string_and_int) * convert.count);
								int i;

								inorder_wiz_string_and_int(&convert, temp); // rename to inorder_wiz_map_wiz_string_and_int!
								for (i = 0; i < convert.count; ++i) {
									free_wiz_string(&temp[i].first);
								}

								free(temp);
							}
							free_wiz_map_wiz_string_and_int(&convert);
							free_wiz_vector_any(&_events);
							shallow_free_user_type_in_user_type(&events);
							shallow_free_user_type_in_user_type(&Main);
							free_wiz_string_builder(&builder);
							free_all_event_info(&info);
						}
						return make_wiz_string_from_cstr("ERROR1 not exist $if, front $else.");
					}
					if (*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) == 0 
						&& equal_wiz_string(&val->name, &ELSE_STR) ) {
						
						// free
						{
							free_wiz_string(&EMPTY_STR);
							free_wiz_string(&ID_STR);
							free_wiz_string(&LOCAL_STR); free_wiz_string(&LOCAL_STR2);
							free_wiz_string(&MAIN_STR);
							free_wiz_string(&CALL_STR);
							free_wiz_string(&IF_STR);
							free_wiz_string(&ELSE_STR);
							free_wiz_string(&EVENT_STR);
							free_wiz_string(&ROOT_STR);
							free_wiz_string(&TRUE_STR);

							{

							}
							free_all_wiz_map_wiz_string_and_user_type(&objectMap);
							{

							}
							free_all_wiz_map_wiz_string_and_user_type(&moduleMap);
							free_wiz_string(&module_value);
							for (i = 0; i < size_wiz_stack_event_info(&eventStack); ++i) {
								free_all_event_info(get_wiz_stack_event_info(&eventStack, i));
							}
							free_wiz_stack_event_info(&eventStack);
							if (convert.count > 0) {
								pair_wiz_string_and_int* temp = malloc(sizeof(pair_wiz_string_and_int) * convert.count);
								int i;

								inorder_wiz_string_and_int(&convert, temp); // rename to inorder_wiz_map_wiz_string_and_int!
								for (i = 0; i < convert.count; ++i) {
									free_wiz_string(&temp[i].first);
								}

								free(temp);
							}
							free_wiz_map_wiz_string_and_int(&convert);
							free_wiz_vector_any(&_events);
							shallow_free_user_type_in_user_type(&events);
							shallow_free_user_type_in_user_type(&Main);
							free_wiz_string_builder(&builder);
							free_all_event_info(&info);
						}
						return make_wiz_string_from_cstr("ERROR2 not exist $if, front $else.");
					}
				}
				else {
					val = NULL;
				}
			}
			else
			{
				// # of userType_idx == nowUT.size() + 1, and nowUT.size() == conditionStack.size()..
				while (!empty_wiz_stack_any(&top_wiz_stack_event_info(&eventStack)->nowUT) && 
					get_user_type_list_size_in_user_type(top_wiz_stack_any(&top_wiz_stack_event_info(&eventStack)->nowUT))
					<= *top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))
				{
					pop_wiz_stack_any(&top_wiz_stack_event_info(&eventStack)->nowUT);
					pop_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx);
					free_wiz_string(top_wiz_stack_wiz_string(&top_wiz_stack_event_info(&eventStack)->conditionStack));
					pop_wiz_stack_wiz_string(&top_wiz_stack_event_info(&eventStack)->conditionStack);
				}

				if (!empty_wiz_stack_any(&top_wiz_stack_event_info(&eventStack)->nowUT) && 
					get_user_type_list_size_in_user_type(top_wiz_stack_any(&top_wiz_stack_event_info(&eventStack)->nowUT)) > 
							*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)) {

					val = get_user_type_list_in_user_type(top_wiz_stack_any(&top_wiz_stack_event_info(&eventStack)->nowUT), *top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx));

					if (*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) >= 1 && equal_wiz_string(&val->name, &ELSE_STR)
						&& !equal_wiz_string(&get_user_type_list_in_user_type(top_wiz_stack_any(&top_wiz_stack_event_info(&eventStack)->nowUT), 
							(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) - 1))->name, &IF_STR)
						) {
						// free
							{
								free_wiz_string(&EMPTY_STR);
								free_wiz_string(&ID_STR);
								free_wiz_string(&LOCAL_STR); free_wiz_string(&LOCAL_STR2);
								free_wiz_string(&MAIN_STR);
								free_wiz_string(&CALL_STR);
								free_wiz_string(&IF_STR);
								free_wiz_string(&ELSE_STR);
								free_wiz_string(&EVENT_STR);
								free_wiz_string(&ROOT_STR);
								free_wiz_string(&TRUE_STR);

								{

								}
								free_all_wiz_map_wiz_string_and_user_type(&objectMap);
								{

								}
								free_all_wiz_map_wiz_string_and_user_type(&moduleMap);
								free_wiz_string(&module_value);
								for (i = 0; i < size_wiz_stack_event_info(&eventStack); ++i) {
									free_all_event_info(get_wiz_stack_event_info(&eventStack, i));
								}
								free_wiz_stack_event_info(&eventStack);
								
								free_all_event_info(&info);
								if (convert.count > 0) {
									pair_wiz_string_and_int* temp = malloc(sizeof(pair_wiz_string_and_int) * convert.count);
									int i;

									inorder_wiz_string_and_int(&convert, temp); // rename to inorder_wiz_map_wiz_string_and_int!
									for (i = 0; i < convert.count; ++i) {
										free_wiz_string(&temp[i].first);
									}

									free(temp);
								}
								free_wiz_map_wiz_string_and_int(&convert);
								free_wiz_vector_any(&_events);
								shallow_free_user_type_in_user_type(&events);
								shallow_free_user_type_in_user_type(&Main);
								free_wiz_string_builder(&builder);
							}
						return make_wiz_string_from_cstr("ERROR3 not exist $if, front $else.");
					}
					if (top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) == 0 && 
						equal_wiz_string(&val->name, &ELSE_STR)) {
						// free
						{
							free_wiz_string(&EMPTY_STR);
							free_wiz_string(&ID_STR);
							free_wiz_string(&LOCAL_STR); free_wiz_string(&LOCAL_STR2);
							free_wiz_string(&MAIN_STR);
							free_wiz_string(&CALL_STR);
							free_wiz_string(&IF_STR);
							free_wiz_string(&ELSE_STR);
							free_wiz_string(&EVENT_STR);
							free_wiz_string(&ROOT_STR);
							free_wiz_string(&TRUE_STR);

							{

							}
							free_all_wiz_map_wiz_string_and_user_type(&objectMap);
							{

							}
							free_all_wiz_map_wiz_string_and_user_type(&moduleMap);
							free_wiz_string(&module_value);
							for (i = 0; i < size_wiz_stack_event_info(&eventStack); ++i) {
								free_all_event_info(get_wiz_stack_event_info(&eventStack, i));
							}
							free_wiz_stack_event_info(&eventStack);
							free_all_event_info(&info);
							if (convert.count > 0) {
								pair_wiz_string_and_int* temp = malloc(sizeof(pair_wiz_string_and_int) * convert.count);
								int i;

								inorder_wiz_string_and_int(&convert, temp); // rename to inorder_wiz_map_wiz_string_and_int!
								for (i = 0; i < convert.count; ++i) {
									free_wiz_string(&temp[i].first);
								}

								free(temp);
							}
							free_wiz_map_wiz_string_and_int(&convert);
							free_wiz_vector_any(&_events);
							shallow_free_user_type_in_user_type(&events);
							shallow_free_user_type_in_user_type(&Main);
							free_wiz_string_builder(&builder);
						}
						return make_wiz_string_from_cstr("ERROR4 not exist $if, front $else.");
					}
				}
				else // same to else if( top_wiz_stack_event_info(&eventStack).nowUT.empty() ), also same to else if ( 1 == top_wiz_stack_event_info(&eventStack).userType_idx.size() )
				{
					if (get_user_type_list_size_in_user_type(get_user_type_list_in_user_type(eventPtr, no)) > 
						*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)) {
						val = get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no),
							*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx));

						if (*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) >= 1 && 
							equal_wiz_string(&val->name, &ELSE_STR)
							&& !equal_wiz_string(&get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no),
								*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) - 1)->name, &IF_STR)) {
							// free
								{
									free_wiz_string(&EMPTY_STR);
									free_wiz_string(&ID_STR);
									free_wiz_string(&LOCAL_STR); free_wiz_string(&LOCAL_STR2);
									free_wiz_string(&MAIN_STR);
									free_wiz_string(&CALL_STR);
									free_wiz_string(&IF_STR);
									free_wiz_string(&ELSE_STR);
									free_wiz_string(&EVENT_STR);
									free_wiz_string(&ROOT_STR);
									free_wiz_string(&TRUE_STR);

									{

									}
									free_all_wiz_map_wiz_string_and_user_type(&objectMap);
									{

									}
									free_all_wiz_map_wiz_string_and_user_type(&moduleMap);
									free_wiz_string(&module_value);
									for (i = 0; i < size_wiz_stack_event_info(&eventStack); ++i) {
										free_all_event_info(get_wiz_stack_event_info(&eventStack, i));
									}
									free_wiz_stack_event_info(&eventStack);

									free_all_event_info(&info);
									if (convert.count > 0) {
										pair_wiz_string_and_int* temp = malloc(sizeof(pair_wiz_string_and_int) * convert.count);
										int i;

										inorder_wiz_string_and_int(&convert, temp); // rename to inorder_wiz_map_wiz_string_and_int!
										for (i = 0; i < convert.count; ++i) {
											free_wiz_string(&temp[i].first);
										}

										free(temp);
									}
									free_wiz_map_wiz_string_and_int(&convert);
									free_wiz_vector_any(&_events);
									shallow_free_user_type_in_user_type(&events);
									shallow_free_user_type_in_user_type(&Main);
									free_wiz_string_builder(&builder);
								}
							return make_wiz_string_from_cstr("ERROR5 not exist $if, front $else.");
						}
						if (top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) == 0 
							&& equal_wiz_string(&val->name, &ELSE_STR)) {
							// free
							{
								free_wiz_string(&EMPTY_STR);
								free_wiz_string(&ID_STR);
								free_wiz_string(&LOCAL_STR); free_wiz_string(&LOCAL_STR2);
								free_wiz_string(&MAIN_STR);
								free_wiz_string(&CALL_STR);
								free_wiz_string(&IF_STR);
								free_wiz_string(&ELSE_STR);
								free_wiz_string(&EVENT_STR);
								free_wiz_string(&ROOT_STR);
								free_wiz_string(&TRUE_STR);

								{

								}
								free_all_wiz_map_wiz_string_and_user_type(&objectMap);
								{

								}
								free_all_wiz_map_wiz_string_and_user_type(&moduleMap);
								free_wiz_string(&module_value);
								for (i = 0; i < size_wiz_stack_event_info(&eventStack); ++i) {
									free_all_event_info(get_wiz_stack_event_info(&eventStack, i));
								}
								free_wiz_stack_event_info(&eventStack);
								free_all_event_info(&info);

								if (convert.count > 0) {
									pair_wiz_string_and_int* temp = malloc(sizeof(pair_wiz_string_and_int) * convert.count);
									int i;

									inorder_wiz_string_and_int(&convert, temp); // rename to inorder_wiz_map_wiz_string_and_int!
									for (i = 0; i < convert.count; ++i) {
										free_wiz_string(&temp[i].first);
									}

									free(temp);
								}
								free_wiz_map_wiz_string_and_int(&convert);
								free_wiz_vector_any(&_events);
								shallow_free_user_type_in_user_type(&events);
								shallow_free_user_type_in_user_type(&Main);
								free_wiz_string_builder(&builder);
							}
							return make_wiz_string_from_cstr("ERROR6 not exist $if, front $else.");
						}
					}
					else {
						val = NULL;
					}
				}
			}

			while (val != NULL)
			{

				// ToDo -$filter, $map, $reduce?
				// list = { base_tax } // /root/ X , root/ O
				// $filter = { utDir = { /./test } 
				// condition = { AND = { EXSITITEMBYVAL = { base_tax root/list } COMP> = { ~~ / 5.000 ~ 0 } } } 
				// recursive = { 0 or 1 } } // return UserType.?
				/*if(0 == strcmp("$for_each" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					const string eventID = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					const string dir = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);
					const string condition = to_string_in_user_type(get_user_type_list_in_user_type(val, 2));

					user_type* event = NULL;
					string parameter = "id = " + eventID + " ";
					// find event
					{
						for (i = 0; i < eventPtr->get_user_type_list_size_in_user_type(); ++i) {
							if (eventID == eventPtr->get_user_type_list_in_user_type(i)->GetItem("id")[0].Get(9)) {
								event = eventPtr->get_user_type_list_in_user_type(i);
								break;
							}
						}
					}
					// find parameter and validate?
					bool pass = 0;
					{
						for (i = 0; i < event->get_user_type_list_size_in_user_type(); ++i) {
							if(0 == strcmp("$parameter" == event->get_user_type_list_in_user_type(i)->GetName()) {
								if (1 == event->get_user_type_list_in_user_type(i)->get_item_list_in_user_typeSize()) {
									parameter = parameter + event->get_user_type_list_in_user_type(i)->get_item_list_in_user_type(0).Get(0);
								}
								else {
									pass = 1;
									break;
								}
								break;
							}
						}
					}
					if (pass) {
						(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
						break;
					}
					// chk loop and condition! chk do not use ""
					{
						int count = 0;
						top_wiz_stack_event_info(&eventStack).return_value = "";
						user_type* ut = user_type::Find(&global, dir, &builder).second[0];
						for (i = 0; i < ut->get_item_list_in_user_typeSize(); ++i) {
							string _condition = condition;

							_condition = wiz::String::replace(_condition, "~~~", ut->get_item_list_in_user_type(i).Get(0)); //
							_condition = wiz::String::replace(_condition, "////", dir);
							_condition = wiz::String::replace(_condition, "///", wiz::_toString(i));
							// ToDo - chk : empty name!!
							if (ut->get_item_list_in_user_type(i).GetName().empty()) {
								_condition = wiz::String::replace(_condition, "~~", "^"); // chk..
							}
							else
							{
								_condition = wiz::String::replace(_condition, "~~", ut->get_item_list_in_user_type(i).GetName());
							}

							_condition = ToBool4(ut, global, _condition, _excuteData, &builder);

							wiz::load_data::Condition _cond(_condition, ut, &global, &builder);

							while (_cond.Next());

							if (_cond.Now().size() != 1 || "TRUE" != _cond.Now()[0]) // || cond.Now().size()  != 1
							{
								//std::cout << cond.Now()[0] << endl;
								continue;
							}


							user_type eventsTemp = events;

							ExcuteData _excuteData; _excuteData.valid = 1;
							if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
							_excuteData.pEvents = &eventsTemp;
							_excuteData.pModule = moduleMapPtr;
							_excuteData.pObjectMap = objectMapPtr;

							wiz::load_data::LoadData::AddData(eventsTemp, "/root", "Event = { id = NONE $call = { " + parameter + " = { " + ut->get_item_list_in_user_type(i).ToString() + " } } }", "TRUE", _excuteData, &builder);

							const string return_value = excute_module("Main = { $call = { id = NONE } }", &global, _excuteData);

							user_type return_value_ut;

							wiz::load_data::LoadData::LoadDataFromString(return_value, return_value_ut);

							ut->get_item_list_in_user_type(i).SetName(return_value_ut.get_item_list_in_user_type(0).GetName());
							ut->get_item_list_in_user_type(i).Set(0, return_value_ut.get_item_list_in_user_type(0).Get(0));

							if (count != 0) {
								top_wiz_stack_event_info(&eventStack).return_value = top_wiz_stack_event_info(&eventStack).return_value + " ";
							}

							top_wiz_stack_event_info(&eventStack).return_value = top_wiz_stack_event_info(&eventStack).return_value + return_value;

							count++;
						}
					}

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$do" , get_cstr_wiz_string(&val->name))) { // chk?
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					user_type subGlobal;
					wiz::load_data::LoadData::LoadDataFromString(get_user_type_list_in_user_type(val, 1)->ToString(), subGlobal);
					user_type inputUT;
					wiz::load_data::LoadData::LoadDataFromString(ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder), inputUT);


					wiz::load_data::LoadData::AddData(subGlobal, "/./", inputUT.ToString(), "TRUE", _excuteData, &builder);


					top_wiz_stack_event_info(&eventStack).return_value = excute_module("", &subGlobal, ExcuteData()); // return ?


					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				// add option! for "".support eu3, eu4.
				else if(0 == strcmp("$replace_datatype1" , get_cstr_wiz_string(&val->name))) { // name
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string rex = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					rex = rex.substr(1, rex.size() - 2);
					vector<string> sval;
					vector<string> scondition;
					string start_dir = "root";

					if (get_user_type_list_size_in_user_type(val)() >= 3)
					{
						int n = (get_user_type_list_size_in_user_type(val)() - 1) / 2;
						for (i = 0; i < n; ++i) {
							sval.push_back(get_user_type_list_in_user_type(val, 1)->ToString());

							scondition.push_back(get_user_type_list_in_user_type(val, 2)->ToString());
						}
					}
					if (get_user_type_list_size_in_user_type(val)() >= 4) {
						start_dir = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 3)->ToString(), _excuteData, &builder);
					}

					wiz::load_data::LoadData::ReplaceDataType1(global, rex, sval, scondition, start_dir, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$replace_datatype1_2" , get_cstr_wiz_string(&val->name))) { //val
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string rex = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					rex = rex.substr(1, rex.size() - 2);
					vector<string> sval;
					vector<string> scondition;
					string start_dir = "root";

					if (get_user_type_list_size_in_user_type(val)() >= 3)
					{
						int n = (get_user_type_list_size_in_user_type(val)() - 1) / 2;
						int count = 1;
						for (i = 0; i < n; ++i) {
							sval.push_back(get_user_type_list_in_user_type(val, count)->ToString());
							count++;
							scondition.push_back(get_user_type_list_in_user_type(val, count)->ToString());
							count++;
						}
					}

					if (get_user_type_list_size_in_user_type(val)() >= 4 && (get_user_type_list_size_in_user_type(val)() % 2 == 0)) {
						start_dir = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, get_user_type_list_size_in_user_type(val)() - 1)->ToString(), _excuteData, &builder);
					}

					wiz::load_data::LoadData::ReplaceDataType1_2(global, rex, sval, scondition, start_dir, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$replace_datatype2" , get_cstr_wiz_string(&val->name))) { // usertype name
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string rex = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					rex = rex.substr(1, rex.size() - 2);
					//cout << get_user_type_list_in_user_type(val, 1)->ToString() << endl;
					vector<string> sval;
					vector<string> scondition;
					string start_dir = "root";

					if (get_user_type_list_size_in_user_type(val)() >= 3)
					{
						int n = (get_user_type_list_size_in_user_type(val)() - 1) / 2;
						int count = 1;
						for (i = 0; i < n; ++i) {
							sval.push_back(get_user_type_list_in_user_type(val, count)->ToString());
							count++;
							scondition.push_back(get_user_type_list_in_user_type(val, count)->ToString());
							count++;
						}
					}

					if (get_user_type_list_size_in_user_type(val)() >= 4 && (get_user_type_list_size_in_user_type(val)() % 2 == 0)) {
						start_dir = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, get_user_type_list_size_in_user_type(val)() - 1)->ToString(), _excuteData, &builder);
					}

					//cout << scondition << endl;
					//cout << "sval " << sval << endl;
					wiz::load_data::LoadData::ReplaceDataType2(global, rex, sval, scondition, start_dir, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$replace_datetype" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string sval = to_string_in_user_type(get_user_type_list_in_user_type(val, 0));

					string scondition = "TRUE";
					string start_dir = "root";

					if (get_user_type_list_size_in_user_type(val)() >= 2)
					{
						scondition = to_string_in_user_type(get_user_type_list_in_user_type(val, 1));
					}
					if (get_user_type_list_size_in_user_type(val)() >= 3) {
						start_dir = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 2)->ToString(), _excuteData, &builder);
					}

					wiz::load_data::LoadData::ReplaceDateType(global, sval, scondition, start_dir, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$replace_datetype2" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string sval = to_string_in_user_type(get_user_type_list_in_user_type(val, 0));
					string scondition = "TRUE";
					string start_dir = "root";


					if (get_user_type_list_size_in_user_type(val)() >= 2)
					{
						scondition = to_string_in_user_type(get_user_type_list_in_user_type(val, 1));
					}
					if (get_user_type_list_size_in_user_type(val)() >= 3) {
						start_dir = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 2)->ToString(), _excuteData, &builder);
					}

					wiz::load_data::LoadData::ReplaceDateType2(global, sval, scondition, start_dir, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$remove_usertype_total" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string ut_name = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					string condition = "TRUE";
					string start_dir = "root";

					if (get_user_type_list_size_in_user_type(val)() >= 2)
					{
						condition = to_string_in_user_type(get_user_type_list_in_user_type(val, 1));
					}
					if (get_user_type_list_size_in_user_type(val)() >= 3) {
						start_dir = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 2)->ToString(), _excuteData, &builder);
					}

					wiz::load_data::LoadData::RemoveUserTypeTotal(global, ut_name, condition, start_dir, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$replace_item" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					// chk NULL vs val?
					string svar = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					string sval = to_string_in_user_type(get_user_type_list_in_user_type(val, 1));
					string scondition = "TRUE";
					string start_dir = "root";

					if (get_user_type_list_size_in_user_type(val)() >= 3)
					{
						scondition = to_string_in_user_type(get_user_type_list_in_user_type(val, 2));
					}
					if (get_user_type_list_size_in_user_type(val)() >= 4) {
						start_dir = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 3)->ToString(), _excuteData, &builder);
					}

					wiz::load_data::LoadData::ReplaceItem(global, svar, sval, scondition, start_dir, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$edit_mode" , get_cstr_wiz_string(&val->name))) // chk!!
				{
					MStyleTest(&global);

					system("cls");

					gotoxy(0, 0);
					setcolor(7, 0);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$save" , get_cstr_wiz_string(&val->name))) // save data, event, main!
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					//todo
					// "filename" save_option(0~2)
					string fileName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string option = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);

					wiz::load_data::LoadData::SaveWizDB(global, fileName, option, "");
					wiz::load_data::LoadData::SaveWizDB(Main, fileName, option, "APPEND");
					wiz::load_data::LoadData::SaveWizDB(events, fileName, option, "APPEND");

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$save_data_only" , get_cstr_wiz_string(&val->name)))
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					//todo
					// "filename" save_option(0~2)
					string fileName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string option = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);

					wiz::load_data::LoadData::SaveWizDB(global, fileName, option, "");

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}

				else if(0 == strcmp("$save_data_only2" , get_cstr_wiz_string(&val->name)))
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					//todo
					// "filename" save_option(0~2)
					string dirName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					string fileName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string option = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 2)->ToString(), _excuteData, &builder);

					// todo - for? auto x = global.get_user_type_item_in_user_type(dirName);
					user_type* utTemp = global.get_user_type_item_in_user_type(dirName)[0];
					wiz::load_data::LoadData::SaveWizDB(*utTemp, fileName, option, "");


					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				/// module name or object name -> must "~" .
				else if(0 == strcmp("$register_module" , get_cstr_wiz_string(&val->name)))
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string moduleFileName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					user_type moduleUT;
					wiz::load_data::LoadData::LoadDataFromFile(moduleFileName, moduleUT);

					//moduleMapPtr.insert(make_pair(moduleFileName, moduleUT));
					(*moduleMapPtr)[moduleFileName] = move(moduleUT);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$call_registered_module" , get_cstr_wiz_string(&val->name)))
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string moduleFileName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					string input;

					if (get_user_type_list_size_in_user_type(val)() >= 2) {
						input = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);
					}

					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					user_type moduleUT = (*moduleMapPtr).at(moduleFileName);
					wiz::load_data::LoadData::AddData(moduleUT, "", input, "TRUE", _excuteData, &builder);

					top_wiz_stack_event_info(&eventStack).return_value = excute_module("", &moduleUT, ExcuteData());

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$module" , get_cstr_wiz_string(&val->name)))
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string moduleFileName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					string input;

					if (get_user_type_list_size_in_user_type(val)() >= 2) {
						input = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);
					}

					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					user_type moduleUT;
					wiz::load_data::LoadData::LoadDataFromFile(moduleFileName, moduleUT);
					wiz::load_data::LoadData::AddData(moduleUT, "", input, "TRUE", _excuteData, &builder);

					top_wiz_stack_event_info(&eventStack).return_value = excute_module("", &moduleUT, ExcuteData());

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				// todo - register module from file
				// todo	- register module from string
				// todo - call registered module.  $registered_module = { name = { ~ } input = { input = { n = 1 } } }

				else if(0 == strcmp("$register_object" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string objectFileName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);

					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);

					user_type objectUT;
					wiz::load_data::LoadData::LoadDataFromFile(objectFileName, objectUT);

					//objectMapPtr.insert(make_pair(objectFileName, data));
					(*objectMapPtr)[objectFileName] = move(objectUT);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$copy_object" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string objectNameA = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					string objectNameB = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);

					objectNameA = wiz::String::substring(objectNameA, 1, objectNameA.size() - 2);
					objectNameB = wiz::String::substring(objectNameB, 1, objectNameB.size() - 2);

					(*objectMapPtr)[objectNameB] = (*objectMapPtr).at(objectNameA);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$register_object_from_string" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string objectName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					string objectData = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);
					objectName = wiz::String::substring(objectName, 1, objectName.size() - 2);
					objectData = wiz::String::substring(objectData, 1, objectData.size() - 2);
					user_type objectUT;
					wiz::load_data::LoadData::LoadDataFromString(objectData, objectUT); // error chk?

																						//objectMapPtr.insert(make_pair(objectFileName, data));
					(*objectMapPtr)[objectName] = move(objectUT);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$call_registered_object" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string objectFileName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);
					string parameter;
					user_type objectUT = objectMapPtr->at(objectFileName);

					parameter = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);

					string data = " Event = { id = NONE $call = { " + parameter + "  } } ";

					wiz::load_data::LoadData::AddData(objectUT, "/./", data, "TRUE", _excuteData, &builder);

					top_wiz_stack_event_info(&eventStack).return_value = excute_module(" Main = { $call = { id = NONE } } ", &objectUT, ExcuteData());

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$call_registered_object2" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string objectFileName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);
					string parameter;

					parameter = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);

					string data = " Event = { id = NONE $call = { " + parameter + "  } } ";

					user_type objectUT = objectMapPtr->at(objectFileName);
					user_type objectUT2 = objectUT;
					wiz::load_data::LoadData::AddData(objectUT, "/./", data, "TRUE", _excuteData, &builder);
					top_wiz_stack_event_info(&eventStack).return_value = excute_module(" Main = { $call = { id = NONE } } ", &objectUT, ExcuteData());
					{
						vector<user_type*> _events;
						user_type events;

						_events = objectUT2.GetCopyUserTypeItem("Event");
						for (i = 0; i < _events.size(); ++i) {
							events.LinkUserType(_events[i]);
						}

						wiz::load_data::LoadData::AddData(objectUT, "/./", events.ToString(), "TRUE", _excuteData, &builder);
					}
					(*objectMapPtr)[objectFileName] = move(objectUT);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				/// object of class?
				else if(0 == strcmp("$object" , get_cstr_wiz_string(&val->name))) { // "fileName"
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string objectFileName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);
					string parameter;

					parameter = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);


					user_type objectUT;
					wiz::load_data::LoadData::LoadDataFromFile(objectFileName, objectUT);

					string data = objectUT.ToString();

					data = data + "Event = { id = NONE $call = { " + parameter + "  } } ";

					objectUT.Remove();
					wiz::load_data::LoadData::LoadDataFromString(data, objectUT);

					top_wiz_stack_event_info(&eventStack).return_value = excute_module(" Main = { $call = { id = NONE } } ", &objectUT, ExcuteData());

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$object_from_string" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string data = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					string parameter;

					parameter = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);


					user_type objectUT;
					wiz::load_data::LoadData::LoadDataFromString(data, objectUT);

					string str = objectUT.ToString();

					str = str + " Event = { id = NONE $call = { " + parameter + "  } } ";

					objectUT.Remove();
					wiz::load_data::LoadData::LoadDataFromString(str, objectUT);

					top_wiz_stack_event_info(&eventStack).return_value = excute_module(" Main = { $call = { id = NONE } } ", &objectUT, ExcuteData());

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				// todo - register object from file.
				//		~.ToString() + "Main = { $call = { id = 0 } } Event = { id = 0 $call = { id = " + id_val + " " + param_name1 + " = " + param_val1 + "  } } "
				// todo - register object from string.
				// todo - call registered object.  $registered_object = { name = { "ex2.txt" } parameter = { id = 1 i = 1 j = 1 } }  
				*/
				//else 
				if(0 == strcmp("$option" , get_cstr_wiz_string(&val->name))) // first
				{
					wiz_string temp = to_string_in_user_type(val, &builder);
					ExcuteData _excuteData; _excuteData.valid = 1;
					if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					
					free_wiz_string(&top_wiz_stack_event_info(&eventStack)->option);
					top_wiz_stack_event_info(&eventStack)->option = ToBool4(NULL, &global, &temp, &_excuteData, &builder);

					free_wiz_string(&temp);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				/*
				// done - ($push_back-insert!) $pop_back, $push_front, $pop_front ($front?, $back?)
				else if(0 == strcmp("$pop_back" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					user_type* ut = NULL;
					auto finded = user_type::Find(&global, dir, &builder);
					ut = finded.second[0];

					string condition = "TRUE";
					if (get_user_type_list_size_in_user_type(val)() >= 2) {
						condition = to_string_in_user_type(get_user_type_list_in_user_type(val, 1));
					}
					wiz::load_data::LoadData::Remove(global, dir, ut->GetIListSize() - 1, condition, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$push_front" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string value = to_string_in_user_type(get_user_type_list_in_user_type(val, 1));
					string dir;
					if (get_user_type_list_in_user_type(val, 0)->get_item_list_in_user_typeSize() > 0) {
						dir = get_user_type_list_in_user_type(val, 0)->get_item_list_in_user_type(0).Get(0);
						dir = ToBool4(NULL, &global, dir, _excuteData, &builder);
					}
					else ///val->Ge
					{
						dir = string(get_user_type_list_in_user_type(val, 0)->ToString());
						dir = ToBool4(NULL, &global, dir, _excuteData, &builder);
					}


					value = ToBool4(NULL, &global, value, _excuteData, &builder);

					string condition = "TRUE";
					if (get_user_type_list_size_in_user_type(val)() >= 3) {
						condition = to_string_in_user_type(get_user_type_list_in_user_type(val, 2));
					}
					wiz::load_data::LoadData::AddDataAtFront(global, dir, value, condition, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$pop_front" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);

					string condition = "TRUE";
					if (get_user_type_list_size_in_user_type(val)() >= 2) {
						condition = to_string_in_user_type(get_user_type_list_in_user_type(val, 1));
					}
					wiz::load_data::LoadData::Remove(global, dir, 0, condition, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else */
				if (0 == strcmp("$call", get_cstr_wiz_string(&val->name))) {
					event_info info2; //
					ExcuteData _excuteData; _excuteData.valid = 1;
					int isRecursiveCall = 0;
					if (NULL == excuteData) { _excuteData.depth = 0; }
					else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					{
						wiz_vector_item_type temp = get_item_in_user_type(val, &ID_STR);

						free_wiz_string(&info.id);

						if (!empty_wiz_vector_item_type(&temp)) {
							wiz_vector_item_type temp = get_item_in_user_type(val, &ID_STR);
							info.id = make_wiz_string_from_other_wiz_string(&get_wiz_vector_item_type(&temp, 0)->value);
							free_wiz_vector_item_type(&temp);
						}
						else {
							wiz_vector_any temp = get_user_type_item_in_user_type(val, &ID_STR);
							wiz_string param1 = to_string_in_user_type((user_type*)get_wiz_vector_any(&temp, 0), &builder);
							info.id = ToBool4(NULL, &global, &param1, &_excuteData, &builder);

							free_wiz_vector_any(&temp);
							free_wiz_string(&param1);
						}

						free_wiz_vector_item_type(&temp);
					}

					info.eventUT = get_user_type_list_in_user_type(eventPtr, no);
					free_wiz_stack_size_t(&info.userType_idx);
					init_wiz_stack_size_t(&info.userType_idx, 1);
					///info.userType_idx.clear();

					{
						size_t zero = 0;
						push_wiz_stack_size_t(&info.userType_idx, &zero);
					}
					///info.userType_idx.push(0);

					free_wiz_string(&info.return_value);
					init_wiz_string(&info.return_value, "", 0);
					///info.return_value.clear();

					free_wiz_stack_any(&info.nowUT);
					init_wiz_stack_any(&info.nowUT, 1);
					///info.nowUT.clear();

					info2 = info; // chk!! bug!?

					if (!(equal_wiz_string(&info.id, &top_wiz_stack_event_info(&eventStack)->id))) {	
						info.parameters.root = NULL;
						info.parameters.count = 0;
					}
					else {
						info.parameters.root = NULL;
						info.parameters.count = 0;
						isRecursiveCall = 1;
					}
					info.conditionStack.count = 0;
					info.conditionStack.stack.num = 0;
					info.conditionStack.stack.vec = NULL;

					init_wiz_stack_wiz_string(&info.conditionStack, 1);
					//
					if (!(equal_wiz_string(&info.id, &top_wiz_stack_event_info(&eventStack)->id))) {
						for (j = 0; j < get_item_list_size_in_user_type(val); ++j) {
							if (get_item_list_size_in_user_type(val) > 0) {
								wiz_string temp;
								pair_wiz_string_and_wiz_string pairTemp;
								_excuteData.info = info2;
								temp = ToBool4(NULL, &global, &get_item_list_in_user_type(val, j)->value, &_excuteData, &builder);
								pairTemp.first = make_wiz_string_from_other_wiz_string(&get_item_list_in_user_type(val, j)->name);
								pairTemp.second = temp;

								insert_wiz_map_wiz_string_and_wiz_string(&info.parameters, &pairTemp, 0);
							}
						}
						for (j = 0; j < get_user_type_list_size_in_user_type(val); ++j) {
							if (get_user_type_list_size_in_user_type(val) > 0) {
								wiz_string temp;
								wiz_string param1 = to_string_in_user_type(get_user_type_list_in_user_type(val, j), &builder);
								pair_wiz_string_and_wiz_string pairTemp;

								_excuteData.info = info2;
								temp = ToBool4(NULL, &global, &param1, &_excuteData, &builder);
								free_wiz_string(&param1);

								pairTemp.first = make_wiz_string_from_other_wiz_string(&get_user_type_list_in_user_type(val, j)->name);
								pairTemp.second = temp;

								insert_wiz_map_wiz_string_and_wiz_string(&info.parameters, &pairTemp, 0);
							}
						}
						(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					}
					else { // recursive call!
						if (get_item_list_size_in_user_type(val) > 0) {
							for (j = 0; j < get_item_list_size_in_user_type(val); ++j) {
								pair_wiz_string_and_wiz_string pairTemp;
								wiz_string temp;


								_excuteData.info = info;
								_excuteData.info.parameters = info2.parameters;

								temp = ToBool4(NULL, &global, &get_item_list_in_user_type(val, j)->value, &_excuteData, &builder);


								pairTemp.first = make_wiz_string_from_other_wiz_string(&get_item_list_in_user_type(val, j)->name);
								pairTemp.second = temp;

								insert_wiz_map_wiz_string_and_wiz_string(&info.parameters, &pairTemp, 0);
							}
						}
						if (get_user_type_list_size_in_user_type(val) > 0) {
							for (j = 0; j < get_user_type_list_size_in_user_type(val); ++j) {
								pair_wiz_string_and_wiz_string pairTemp;
								wiz_string temp;
								wiz_string utStr = to_string_in_user_type(get_user_type_list_in_user_type(val, j), &builder);

								_excuteData.info = info;
								_excuteData.info.parameters = info2.parameters;

								temp = ToBool4(NULL, &global, &utStr, &_excuteData, &builder);
								free_wiz_string(&utStr);

								pairTemp.first = make_wiz_string_from_other_wiz_string(&get_user_type_list_in_user_type(val, j)->name);
								pairTemp.second = temp;

								insert_wiz_map_wiz_string_and_wiz_string(&info.parameters, &pairTemp, 0);
							}
						}

						(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;

						// remove remove_now_event_stack_a?
						if ( // REMOVED - top_wiz_stack_event_info(&eventStack).option == "REMOVE_NOW_EVENT_STACK_A" || 
							0 == strcmp("REMOVE_IF_CALL_ONESELF_EVENT", get_cstr_wiz_string(&top_wiz_stack_event_info(&eventStack)->option)) //
							)
						{
							free_all_event_info(top_wiz_stack_event_info(&eventStack));
							pop_wiz_stack_event_info(&eventStack);
						}
					}

					if (!empty_wiz_stack_event_info(&eventStack) && 0 == strcmp(get_cstr_wiz_string(&top_wiz_stack_event_info(&eventStack)->option),
						"REMOVE_IF_CALL_ANY_EVENT"))
					{
						free_all_event_info(top_wiz_stack_event_info(&eventStack));
						pop_wiz_stack_event_info(&eventStack);
					}


					free_all_wiz_map_wiz_string_and_wiz_string(&info.locals);
					init_wiz_map_wiz_string_and_wiz_string(&info.locals);
					//	info.locals.clear();

					{
						pair_wiz_string_and_int temp;
						int no = -1;

						temp.first = info.id;

						get_wiz_map_wiz_string_and_int(&convert, &temp);

						no = temp.second;

						// how more faster?? 
						for (i = 0; i < get_user_type_list_size_in_user_type(get_user_type_list_in_user_type(eventPtr, no)); ++i) {
							if (0 == strcmp(get_cstr_wiz_string(&get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), i)->name), "$local")) {
								for (j = 0; j < get_item_list_size_in_user_type(get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), i)); ++j) {
									wiz_string name = make_wiz_string_from_other_wiz_string(&get_item_list_in_user_type(get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), i), j)->value);
									wiz_string value = EMPTY_STR;
									pair_wiz_string_and_wiz_string pairTemp = { name, value };

									insert_wiz_map_wiz_string_and_wiz_string(&info.locals, &pairTemp, 0);
								}
								break;
							}
						}
					}

					{
						free_all_wiz_map_wiz_string_and_wiz_string(&info2.parameters);

						for (i = 0; i < info2.conditionStack.count; ++i) {
							free_wiz_string(get_wiz_stack_wiz_string(&info2.conditionStack, i));
						}
						free_wiz_stack_wiz_string(&info2.conditionStack);
					}
					info_chk = 0; // 
					push_wiz_stack_event_info(&eventStack, &info);

					break;
				}
				else if(0 == strcmp("$assign" , get_cstr_wiz_string(&val->name))) /// -> assign2?
				{
					ExcuteData _excuteData; _excuteData.valid = 1; 
					if(NULL == excuteData) { _excuteData.depth = 0; } 
					else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					{
						pair_wiz_string_and_wiz_string dir = Find2(&global, &get_item_list_in_user_type(val, 0)->value);
						wiz_string temp = to_string_in_user_type(get_user_type_list_in_user_type(val, 0), &builder);
						wiz_string data = ToBool4(NULL, &global, &temp, &_excuteData, &builder);

						if (equal_wiz_string(&dir.first, &EMPTY_STR) && starts_with_wiz_string(&dir.second, &LOCAL_STR2))
						{
							pair_wiz_string_and_wiz_string temp;

							temp.first = substr_wiz_string(&dir.second, 7, size_wiz_string(&dir.second));
							temp.second = make_wiz_string_from_other_wiz_string(&data);

							if (is_exist_wiz_map_wiz_string_and_wiz_string(&top_wiz_stack_event_info(&eventStack)->locals, &temp))
							{
								set_wiz_map_wiz_string_and_wiz_string(&top_wiz_stack_event_info(&eventStack)->locals, &temp, 1);
							}
							else {
								insert_wiz_map_wiz_string_and_wiz_string(&top_wiz_stack_event_info(&eventStack)->locals, &temp, 0);
							}
						}
						else {
						//	printf("%s\n", get_cstr_wiz_string(&data));
							set_data_in_load_data(&global, &dir.first, &dir.second, &data, &TRUE_STR, &_excuteData, &builder);
						}

						free_wiz_string(&dir.first);
						free_wiz_string(&dir.second);
						free_wiz_string(&temp);
						free_wiz_string(&data);
					}
					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$assign2" , get_cstr_wiz_string(&val->name)))
				{
					ExcuteData _excuteData; 
					_excuteData.valid = 1; 
					if(NULL == excuteData) { _excuteData.depth = 0; } 
					else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					{ 
						wiz_string tempA = to_string_in_user_type(get_user_type_list_in_user_type(val, 0), &builder);
						wiz_string tempB = ToBool4(NULL, &global, &tempA, &_excuteData, &builder);
						pair_wiz_string_and_wiz_string dir = Find2(&global, &tempB);
						wiz_string tempC = to_string_in_user_type(get_user_type_list_in_user_type(val, 1), &builder);
						wiz_string data = ToBool4(NULL, &global, &tempC, &_excuteData, &builder);

						{
							wiz_string LOCAL = make_wiz_string_from_cstr("$local.");
							if (0 == size_wiz_string(&dir.first) && starts_with_wiz_string(&dir.second, &LOCAL))
							{
								pair_wiz_string_and_wiz_string val;
								val.first = substr_wiz_string(&dir.second, 7, size_wiz_string(&dir.second));
								val.second = make_wiz_string_from_other_wiz_string(&data);
								wiz_map_wiz_string_and_wiz_string* temp = &top_wiz_stack_event_info(&eventStack)->locals;
								if (is_exist_wiz_map_wiz_string_and_wiz_string(temp, &val)) {
									set_wiz_map_wiz_string_and_wiz_string(temp, &val, 1);
								}
								else {
									insert_wiz_map_wiz_string_and_wiz_string(temp, &val, 0);
								}
							}
							else {
								set_data_in_load_data(&global, &dir.first, &dir.second, &data, &TRUE_STR, &_excuteData, &builder);
							}
							free_wiz_string(&LOCAL);
						}

						free_wiz_string(&tempA);
						free_wiz_string(&tempB);
						free_wiz_string(&tempC);
						free_wiz_string(&data);
						free_wiz_string(&dir.first);
						free_wiz_string(&dir.second);
					}
					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				/*
				else if(0 == strcmp("$assign_local" , get_cstr_wiz_string(&val->name))) /// no condition, 
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					pair_wiz_string_and_wiz_string dir = Find2(&global, ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder));
					string data = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);

					{
						if (dir.first == "" && dir.second.size() > 1 && dir.second[0] == '@')
						{
							dir.second.erase(dir.second.begin());
						}
						if (dir.first == "" && wiz::String::startsWith(dir.second, "$local."))
						{
							top_wiz_stack_event_info(&eventStack).locals[wiz::String::substring(dir.second, 7)] = data;
						}
						else {
							// throw error??
						}
					}

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$assign_global" , get_cstr_wiz_string(&val->name))) // 주의!! dir=> dir/name ( dir= { name = val } } , @�??�에 붙여???�다. 
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					pair_wiz_string_and_wiz_string dir = Find2(&global, ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder));
					string data = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);

					//string condition;
					//if (get_user_type_list_size_in_user_type(val)() >= 3) {
					//	condition = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 2)->ToString(), _excuteData, &builder);
					//}
					wiz::load_data::LoadData::SetData(global, dir.first, dir.second, data, "TRUE", _excuteData, &builder);

					// chk local?

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				/// cf) insert3? - any position?
				*/
				else if(0 == strcmp("$push_back", get_cstr_wiz_string(&val->name)) 
					|| 0 == strcmp("$insert", get_cstr_wiz_string(&val->name)) 
					|| 0 == strcmp("$insert2", get_cstr_wiz_string(&val->name)))
				{
					wiz_string value;
					wiz_string dir;

					ExcuteData _excuteData;
					_excuteData.valid = 1; 
					if(NULL == excuteData) { _excuteData.depth = 0; } 
					else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					value = to_string_in_user_type(get_user_type_list_in_user_type(val, 1), &builder);
					
					if (get_item_list_size_in_user_type(get_user_type_list_in_user_type(val, 0)) > 0) {
						wiz_string temp;
						dir = get_item_list_in_user_type(get_user_type_list_in_user_type(val, 0), 0)->value;
						temp = ToBool4(NULL, &global, &dir, &_excuteData, &builder);
						dir = temp;
					}
					else ///val->Ge
					{
						wiz_string temp;
						dir = to_string_in_user_type(get_user_type_list_in_user_type(val, 0), &builder);
						temp = ToBool4(NULL, &global, &dir, &_excuteData, &builder);
						free_wiz_string(&dir);
						dir = temp;
					}
					{
						wiz_string temp;
						temp = ToBool4(NULL, &global, &value, &_excuteData, &builder);
						free_wiz_string(&value);
						value = temp;
					}
					{
						wiz_string condition = make_wiz_string_from_cstr("TRUE");

						if (get_user_type_list_size_in_user_type(val) >= 3) {
							condition = to_string_in_user_type(get_user_type_list_in_user_type(val, 2), &builder);
						}

						add_data_in_load_data(&global, &dir, &value, &condition, &_excuteData, &builder);

						free_wiz_string(&dir);
						free_wiz_string(&value);
						free_wiz_string(&condition);
					}

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				/*
				else if(0 == strcmp("$insert_noname_usertype" , get_cstr_wiz_string(&val->name)))
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string position = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);;
					string data = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);;
					string condition = to_string_in_user_type(get_user_type_list_in_user_type(val, 2));

					wiz::load_data::LoadData::AddNoNameUserType(global, position, data, condition, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$insert_by_idx" , get_cstr_wiz_string(&val->name)))
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string value = to_string_in_user_type(get_user_type_list_in_user_type(val, 2));
					int idx = atoi(ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder).c_str());
					string dir;
					if (get_user_type_list_in_user_type(val, 0)->get_item_list_in_user_typeSize() > 0) {
						dir = get_user_type_list_in_user_type(val, 0)->get_item_list_in_user_type(0).Get(0);
						dir = ToBool4(NULL, &global, dir, _excuteData, &builder);
					}
					else ///val->Ge
					{
						dir = string(get_user_type_list_in_user_type(val, 0)->ToString());
						dir = ToBool4(NULL, &global, dir, _excuteData, &builder);
					}

					value = ToBool4(NULL, &global, value, _excuteData, &builder);

					string condition = "TRUE";
					if (get_user_type_list_size_in_user_type(val)() >= 4) {
						condition = get_user_type_list_in_user_type(val, 3)->ToString();
					}
					wiz::load_data::LoadData::Insert(global, dir, idx, value, condition, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				*/
				else if(0 == strcmp("$make", get_cstr_wiz_string(&val->name))) // To Do? - make2? or remake? 
													// cf) make empty ut??
				{
					ExcuteData _excuteData; 
					wiz_string dir;
					int is2 = 0;
					wiz_string name = EMPTY_STR;
					wiz_string condition = make_wiz_string_from_cstr("TRUE");

					_excuteData.valid = 1; 
					if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					
					if (get_item_list_size_in_user_type(val) > 0) {
						dir = get_item_list_in_user_type(val, 0)->value;
						dir = ToBool4(NULL, &global, &dir, &_excuteData, &builder);
					}
					else // 
					{
						wiz_string temp;
						dir = to_string_in_user_type(get_user_type_list_in_user_type(val, 0), &builder);
						temp = ToBool4(NULL, &global, &dir, &_excuteData, &builder);
						free_wiz_string(&dir);
						dir = temp;

						is2 = 1;
					}

					for (i = size_wiz_string(&dir) - 1; i >= 0; --i)
					{
						if (get_cstr_wiz_string(&dir)[i] == '/') {
							wiz_string temp;
							name = substr_wiz_string(&dir, i + 1, size_wiz_string(&dir));
							temp = substr_wiz_string(&dir, 0, i - 1 + 1); // last + 1 == end
							free_wiz_string(&dir);
							dir = temp;

							break;
						}
					}
					if (empty_wiz_string(&dir)) { dir = make_wiz_string_from_cstr("."); }

					if (is2 && get_user_type_list_size_in_user_type(val) >= 2) {
						free_wiz_string(&condition);
						condition = to_string_in_user_type(get_user_type_list_in_user_type(val, 1), &builder);
					}
					else if (0 == is2 && get_user_type_list_size_in_user_type(val) >= 1) {
						free_wiz_string(&condition);
						condition = to_string_in_user_type(get_user_type_list_in_user_type(val, 0), &builder);
					}


					add_user_type_in_load_data(&global, &dir, &name, &EMPTY_STR, &condition, &_excuteData, &builder);


					free_wiz_string(&name);
					free_wiz_string(&dir);
					free_wiz_string(&condition);
					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				/*
				else if(0 == strcmp("$findIndex" , get_cstr_wiz_string(&val->name))) // For list : { 1 2  3 4 5 }
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					string value = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);

					user_type ut;
					wiz::load_data::LoadData::LoadDataFromString(user_type::Find(&global, dir, &builder).second[0]->ToString(), ut);

					for (i = 0; i < ut.get_item_list_in_user_typeSize(); ++i) {
						if (ut.get_item_list_in_user_type(i).Get(0) == value) {
							top_wiz_stack_event_info(&eventStack).return_value = wiz::toStr(i);
							break;
						}
					}

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$remove" , get_cstr_wiz_string(&val->name))) // remove by dir., remove all?
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = string(val->get_item_list_in_user_type(0).Get(0).c_str()); // item -> userType

					dir = ToBool4(NULL, &global, dir, _excuteData, &builder);

					string condition = "TRUE";
					if (get_user_type_list_size_in_user_type(val)() >= 2) {
						condition = to_string_in_user_type(get_user_type_list_in_user_type(val, 1));
					}
					wiz::load_data::LoadData::Remove(global, dir, condition, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$remove2" , get_cstr_wiz_string(&val->name))) // remove /dir/name 
														// if name is empty, then chk!!
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = string(val->get_item_list_in_user_type(0).Get(0).c_str()); // item -> userType
					dir = ToBool4(NULL, &global, dir, _excuteData, &builder);
					string name;
					for (i = dir.size() - 1; i >= 0; --i)
					{
						if (dir[i] == '/') {
							name = wiz::String::substring(dir, i + 1);
							dir = wiz::String::substring(dir, 0, i - 1);
							break;
						}
					}

					string condition = "TRUE";
					if (get_user_type_list_size_in_user_type(val)() >= 2) {
						condition = to_string_in_user_type(get_user_type_list_in_user_type(val, 1));
					}
					wiz::load_data::LoadData::Remove(global, dir, name, condition, _excuteData, &builder);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$remove3" , get_cstr_wiz_string(&val->name))) /// remove itemlist by idx.
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					string value = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);

					int idx = atoi(value.c_str());  // long long -> int?

					string condition = "TRUE";

					if (get_user_type_list_size_in_user_type(val)() >= 3) {
						condition = to_string_in_user_type(get_user_type_list_in_user_type(val, 2));
					}

					wiz::load_data::LoadData::Remove(global, dir, idx, condition, _excuteData, &builder);
					// remove -> UserType::Find(&global, dir).second[0]->RemoveItemList(idx); /// change ?

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				*/
				else if(0 == strcmp("$setElement" , get_cstr_wiz_string(&val->name)))
				{
					ExcuteData _excuteData; 
					_excuteData.valid = 1; 
					if(NULL == excuteData) { _excuteData.depth = 0; } 
					else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					{
						wiz_string dir_str = to_string_in_user_type(get_user_type_list_in_user_type(val, 0), &builder);
						wiz_string dir = ToBool4(NULL, &global, &dir_str, &_excuteData, &builder);
						wiz_string idx_str = to_string_in_user_type(get_user_type_list_in_user_type(val, 1), &builder);
						wiz_string idx = ToBool4(NULL, &global, &idx_str, &_excuteData, &builder);
						wiz_string value_str = to_string_in_user_type(get_user_type_list_in_user_type(val, 2), &builder);
						wiz_string value = ToBool4(NULL, &global, &value_str, &_excuteData, &builder);

						int _idx = atoi(get_cstr_wiz_string(&idx));
						pair_int_and_wiz_vector_any temp = find_user_type_in_user_type(&global, &dir, &builder);
						set_item_by_idx_in_user_type((user_type*)get_wiz_vector_any(&temp.second, 0), _idx, &value);
						
						free_wiz_string(&dir_str);
						free_wiz_string(&dir);
						free_wiz_string(&idx_str);
						free_wiz_string(&idx);
						free_wiz_string(&value_str);
						free_wiz_string(&value);
						free_wiz_vector_any(&temp.second);
					}
					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				/*
				else if(0 == strcmp("$swap" , get_cstr_wiz_string(&val->name))) // $swap2
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = string(val->get_item_list_in_user_type(0).Get(0).c_str()); // + 0
					string value1 = to_string_in_user_type(get_user_type_list_in_user_type(val, 0));
					string value2 = to_string_in_user_type(get_user_type_list_in_user_type(val, 1));

					value1 = ToBool4(NULL, &global, value1, _excuteData, &builder);
					value2 = ToBool4(NULL, &global, value2, _excuteData, &builder);
					if (value1 != value2) {
						int x = atoi(value1.c_str());
						int y = atoi(value2.c_str());

						string temp = user_type::Find(&global, dir, &builder).second[0]->get_item_list_in_user_type(x).Get(0);
						string temp2 = user_type::Find(&global, dir, &builder).second[0]->get_item_list_in_user_type(y).Get(0);

						wiz::load_data::LoadData::SetData(global, dir, x, temp2, "TRUE", _excuteData, &builder);
						wiz::load_data::LoadData::SetData(global, dir, y, temp, "TRUE", _excuteData, &builder);
					}

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				*/
				else if(0 == strcmp("$print" , get_cstr_wiz_string(&val->name))) /// has many bugs..!?, for print list or print item?.
				{
					wiz_string LOCAL_STR = make_wiz_string_from_cstr("$local.");
					wiz_string PARAMETER_STR = make_wiz_string_from_cstr("$parameter.");
					wiz_string SLUSH_STR = make_wiz_string("/", 1);

					ExcuteData _excuteData; _excuteData.valid = 1; 
					if(NULL == excuteData) { _excuteData.depth = 0; }
					else { _excuteData.depth = excuteData->depth; }

					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					if (get_user_type_list_size_in_user_type(val) == 1
						&& get_item_list_size_in_user_type(get_user_type_list_in_user_type(val, 0)) == 1)
					{
						wiz_string listName = make_wiz_string_from_other_wiz_string(
							&get_item_list_in_user_type(get_user_type_list_in_user_type(val, 0), 0)->value
							);

						if (size_wiz_string(&listName) >= 2 
							&& get_cstr_wiz_string(&listName)[0] == '\"' && back_wiz_string(&listName) == '\"')
						{
							wiz_string temp = substr_wiz_string(&listName, 1, size_wiz_string(&listName) - 1);
							free_wiz_string(&listName);
							listName = temp;// wiz::String::substring(listName, 1, listName.size() - 2);
							printf("%s", get_cstr_wiz_string(&listName));
						}
						else if (size_wiz_string(&listName) == 2
							&& get_cstr_wiz_string(&listName)[0] == '\\' && get_cstr_wiz_string(&listName)[1] == 'n')
						{
							printf("\n");
						}
						else if (starts_with_wiz_string(&listName, &LOCAL_STR)
							|| starts_with_wiz_string(&listName, &PARAMETER_STR)
							)
						{
							wiz_string temp = ToBool4(NULL, &global, &listName, &_excuteData, &builder);
							if (empty_wiz_string(&temp)) {
								printf("EMPTY");
							}
							else {
								printf("%s", get_cstr_wiz_string(&temp));
							}
							free_wiz_string(&temp);
						}
						else if (starts_with_wiz_string(&listName, &SLUSH_STR) && size_wiz_string(&listName) > 1)
						{
							wiz_string temp = ToBool4(NULL, &global, &listName, &_excuteData, &builder);
							if (!equal_wiz_string(&temp, &listName)) // chk 
							{
								printf("%s", get_cstr_wiz_string(&temp));
							}
							else {
								pair_int_and_wiz_vector_any temp = find_user_type_in_user_type(&global, &listName, &builder);
								user_type* ut = get_wiz_vector_any(&temp.second, 0);
								if (get_item_list_size_in_user_type(ut) == 0 && empty_wiz_string(&get_item_list_in_user_type(ut, 0)->name)) {
									printf("%s", get_cstr_wiz_string(&get_item_list_in_user_type(ut, 0)->value));
								}

								free_wiz_vector_any(&temp.second);
							}
							free_wiz_string(&temp);
						}
						else
						{
							pair_int_and_wiz_vector_any x = find_user_type_in_user_type(&global, &listName, &builder);
							if (x.first) {
								user_type* ut = get_wiz_vector_any(&x.second, 0); 
								wiz_string temp = to_string_in_user_type(ut, &builder);
								printf("%s", get_cstr_wiz_string(&temp));
								free_wiz_string(&temp);
								free_wiz_vector_any(&x.second);
							}
							else {
								printf("%s", get_cstr_wiz_string(&listName));
							}
						}
					
						free_wiz_string(&listName);
					}
					// ?
					else if (get_user_type_list_size_in_user_type(val) == 1
						&& get_item_list_size_in_user_type(get_user_type_list_in_user_type(val, 0)) == 0
						&& get_user_type_list_size_in_user_type(get_user_type_list_in_user_type(val, 0)) == 1)
					{
						wiz_string temp = to_string_in_user_type(get_user_type_list_in_user_type(val, 0), &builder);

						wiz_string name = ToBool4(NULL, &global, &temp, &_excuteData, &builder);
						printf("%s", get_cstr_wiz_string(&name));

						free_wiz_string(&temp);
						free_wiz_string(&name);
					}
					else
					{
						wiz_string start = to_string_in_user_type(get_user_type_list_in_user_type(val, 1), &builder);
						wiz_string last = to_string_in_user_type(get_user_type_list_in_user_type(val, 2), &builder);

						wiz_string start2 = ToBool4(NULL, &global, &start, &_excuteData, &builder);
						wiz_string last2 = ToBool4(NULL, &global, &last, &_excuteData, &builder);
						
						free_wiz_string(&start);
						free_wiz_string(&last);

						{
							wiz_string listName = get_item_list_in_user_type(get_user_type_list_in_user_type(val, 0), 0)->value;
							int _start = atoi(get_cstr_wiz_string(&start2)); // cf) long long ?
							int _last = atoi(get_cstr_wiz_string(&last2)); //
							wiz_vector_any temp = find_user_type_in_user_type(&global, &listName, &builder).second;
							user_type* ut = get_wiz_vector_any(&temp, 0);

							for (i = _start; i <= _last; ++i)
							{
								if (i != _start) { printf(" "); }
								printf("%s", get_cstr_wiz_string(&get_item_list_in_user_type(ut, i)->value));
							}

							free_wiz_vector_any(&temp);
							free_wiz_string(&start2);
							free_wiz_string(&last2);
						}
					}

					free_wiz_string(&LOCAL_STR);
					free_wiz_string(&PARAMETER_STR);
					free_wiz_string(&SLUSH_STR);

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				
				else if(0 == strcmp("$print2" , get_cstr_wiz_string(&val->name))) /// for print usertype.ToString();
				{
					wiz_string dir;
					pair_int_and_wiz_vector_any x;
					
					ExcuteData _excuteData; 
					_excuteData.valid = 1; 
					if(NULL == excuteData) { _excuteData.depth = 0; } 
					else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					{
						wiz_string temp = to_string_in_user_type(get_user_type_list_in_user_type(val, 0), &builder);
						dir = ToBool4(NULL, &global, &temp, &_excuteData, &builder);
						free_wiz_string(&temp);
					}
					x = find_user_type_in_user_type(&global, &dir, &builder);
					free_wiz_string(&dir);
					if (x.first)
					{
						int i;
						for (i = 0; i < size_wiz_vector_any(&x.second); ++i) {
							wiz_string temp = to_string_in_user_type(get_wiz_vector_any(&x.second, i), &builder);
							printf("%s\n", get_cstr_wiz_string(&temp));
							free_wiz_string(&temp);
						}
						free_wiz_vector_any(&x.second);
					}

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				/*
				else if(0 == strcmp("$load" , get_cstr_wiz_string(&val->name)))
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// to do, load data and events from other file!
					for (i = 0; i < get_item_list_size_in_user_type(val); ++i) {
						user_type ut;
						string fileName = ToBool4(NULL, &global, val->get_item_list_in_user_type(i).Get(0), _excuteData, &builder);
						fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);

						if (wiz::load_data::LoadData::LoadDataFromFile(fileName, ut)) {
							{
								int item_count = 0;
								int userType_count = 0;

								for (i = 0; i < ut.GetIListSize(); ++i) {
									if (ut.IsItemList(i)) {
										global.AddItem(std::move(ut.get_item_list_in_user_type(item_count).GetName()),
											std::move(ut.get_item_list_in_user_type(item_count).Get(0)));
										item_count++;
									}
									else {
										global.AddUserTypeItem(std::move(*ut.get_user_type_list_in_user_type(userType_count)));
										userType_count++;
									}
								}
							}

							auto _Main = ut.get_user_type_item_in_user_type("Main");
							if (!_Main.empty())
							{
								// error!
								cout << "err" << endl;

								return "ERROR -2"; /// exit?
							}
						}
						else {
							// error!
						}
					}

					{
						convert.clear();
						auto _events = global.GetCopyUserTypeItem("Event");
						for (i = 0; i < _events.size(); ++i) {
							eventPtr->LinkUserType(_events[i]);
						}
						global.RemoveUserTypeList("Event");

						// event table setting
						for (i = 0; i < eventPtr->get_user_type_list_size_in_user_type(); ++i)
						{
							auto x = eventPtr->get_user_type_list_in_user_type(i)->GetItem("id");
							if (!x.empty()) {
								//cout <<	x[0] << endl;
								convert.insert(pair_wiz_string_int>(x[0].Get(0), i));
							}
							else {
								// error
							}
						}

						// update no
						no = convert[str];
					}

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;

				}
				else if(0 == strcmp("$load_only_data" , get_cstr_wiz_string(&val->name))) // $load2?
				{
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// to do, load data and events from other file!
					user_type ut;
					string fileName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string dirName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);
					user_type* utTemp = global.get_user_type_item_in_user_type(dirName)[0];



					if (wiz::load_data::LoadData::LoadDataFromFile(fileName, ut)) {
						{
							for (i = 0; i < ut.GetCommentListSize(); ++i) {
								utTemp->PushComment(move(ut.GetCommentList(i)));
							}
							int item_count = 0;
							int userType_count = 0;

							for (i = 0; i < ut.GetIListSize(); ++i) {
								if (ut.IsItemList(i)) {
									utTemp->AddItem(std::move(ut.get_item_list_in_user_type(item_count).GetName()),
										std::move(ut.get_item_list_in_user_type(item_count).Get(0)));
									item_count++;
								}
								else {
									utTemp->AddUserTypeItem(std::move(*ut.get_user_type_list_in_user_type(userType_count)));
									userType_count++;
								}
							}
						}

						//	auto _Main = ut.get_user_type_item_in_user_type("Main");
						//	if (!_Main.empty())
						//	{
						// error!
						//		cout << "err" << endl;

						//			return "ERROR -2"; /// exit?
						//		}
					}
					else {
						// error!
					}

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;

				}
				else if(0 == strcmp("$clear_screen" , get_cstr_wiz_string(&val->name)))
				{
					system("cls");
					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}*/
				else if(0 == strcmp("$_getch" , get_cstr_wiz_string(&val->name)))
				{
					_getch();
					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				/*
				else if(0 == strcmp("$input" , get_cstr_wiz_string(&val->name)))
				{
					string str;
					cin >> str;
					top_wiz_stack_event_info(&eventStack).return_value = str;
					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				*/
				else if(0 == strcmp("$return" , get_cstr_wiz_string(&val->name)))
				{
					ExcuteData _excuteData; _excuteData.valid = 1; 
					if(NULL == excuteData) { _excuteData.depth = 0; } 
					else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					if (size_wiz_stack_event_info(&eventStack) > 1)
					{
						wiz_string val_data = to_string_in_user_type(val, &builder);
						wiz_string temp = ToBool4(NULL, &global, &val_data, &_excuteData, &builder);
						/// if temp just one
						free_wiz_string(&get_wiz_stack_event_info(&eventStack, size_wiz_stack_event_info(&eventStack) - 2)->return_value);
						get_wiz_stack_event_info(&eventStack, size_wiz_stack_event_info(&eventStack) - 2)->return_value = temp;
						
						free_wiz_string(&val_data);
					}

					if (size_wiz_stack_event_info(&eventStack) == 1)
					{
						wiz_string val_data = to_string_in_user_type(val, &builder);
						wiz_string temp = ToBool4(NULL, &global, &val_data, &_excuteData, &builder);
						free_wiz_string(&module_value);
						module_value = temp;
						free_wiz_string(&val_data);
					}

					free_all_event_info(top_wiz_stack_event_info(&eventStack));

					pop_wiz_stack_event_info(&eventStack);
					break;
				}
				else if(0 == strcmp("$parameter" , get_cstr_wiz_string(&val->name)))
				{
					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$local" , get_cstr_wiz_string(&val->name)))
				{
					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				/*
				// make sort stable.
				else if(0 == strcmp("$sort" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					vector<SortInfo> siVec;
					user_type* utTemp =
						user_type::Find(&global,
							ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder), &builder).second[0];

					vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
							temp.push_back(&(utTemp->get_item_list_in_user_type(item_count)));
							siVec.emplace_back(utTemp->get_item_list_in_user_type(item_count).GetName(), 1, i, &builder);
							item_count++;
						}
						else {
							temp.push_back(utTemp->get_user_type_list_in_user_type(ut_count));
							siVec.emplace_back(utTemp->get_user_type_list_in_user_type(ut_count)->GetName(), 2, i, &builder);
							ut_count++;
						}
					}

					std::sort(siVec.begin(), siVec.end());


					user_type ut;
					for (i = 0; i < temp.size(); ++i)
					{
						if (siVec[i].iElement == 1) {
							ut.AddItem(siVec[i].data, static_cast<wiz::load_data::ItemType<string>*>(temp[siVec[i].idx])->Get(0));
						}
						else {
							ut.AddUserTypeItem(*(static_cast<user_type*>(temp[siVec[i].idx])));
						}
					}

					utTemp->Remove();

					//cf) chk? *utTemp = ut;
					wiz::load_data::LoadData::AddData(*(utTemp), "", ut.ToString(), "TRUE", _excuteData, &builder);


					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$sort2" , get_cstr_wiz_string(&val->name))) { // colName -> just one! ? 
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					/// condition = has just one? in one usertype!
					vector<SortInfo> siVec;
					user_type* utTemp =
						user_type::Find(&global,
							ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder), &builder).second[0];
					const string colName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);

					vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
							//
							item_count++;
						}
						else {
							temp.push_back(utTemp->get_user_type_list_in_user_type(ut_count));
							if (utTemp->get_user_type_list_in_user_type(ut_count)->GetItem(colName).empty())
							{
								siVec.emplace_back("", 2, ut_count, &builder);
							}
							else {
								siVec.emplace_back(utTemp->get_user_type_list_in_user_type(ut_count)->GetItem(colName)[0].Get(0), 2, ut_count, &builder);
							}
							ut_count++;
						}
					}

					std::sort(siVec.begin(), siVec.end());


					user_type ut;
					for (i = 0; i < temp.size(); ++i)
					{
						if (siVec[i].iElement == 1) {
							//
						}
						else {
							ut.AddUserTypeItem(*(static_cast<user_type*>(temp[siVec[i].idx])));
						}
					}

					utTemp->RemoveUserTypeList();

					//cf) chk? *utTemp = ut;
					wiz::load_data::LoadData::AddData(*(utTemp), "", ut.ToString(), "TRUE", _excuteData, &builder);


					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				else if(0 == strcmp("$sort2_dsc" , get_cstr_wiz_string(&val->name))) { // colName -> just one! ? 
															/// condition = has just one? in one usertype!
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					vector<SortInfo2> siVec;
					user_type* utTemp =
						user_type::Find(&global,
							ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder), &builder).second[0];
					const string colName = ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 1)->ToString(), _excuteData, &builder);

					vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
							//
							item_count++;
						}
						else {
							temp.push_back(utTemp->get_user_type_list_in_user_type(ut_count));
							if (utTemp->get_user_type_list_in_user_type(ut_count)->GetItem(colName).empty())
							{
								siVec.emplace_back("", 2, ut_count, &builder);
							}
							else {
								siVec.emplace_back(utTemp->get_user_type_list_in_user_type(ut_count)->GetItem(colName)[0].Get(0), 2, ut_count, &builder);
							}
							ut_count++;
						}
					}

					std::sort(siVec.begin(), siVec.end());


					user_type ut;
					for (i = 0; i < temp.size(); ++i)
					{
						if (siVec[i].iElement == 1) {
							//
						}
						else {
							ut.AddUserTypeItem(*(static_cast<user_type*>(temp[siVec[i].idx])));
						}
					}

					utTemp->RemoveUserTypeList();

					//cf) chk? *utTemp = ut;
					wiz::load_data::LoadData::AddData(*(utTemp), "", ut.ToString(), "TRUE", _excuteData, &builder);


					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				// removal?
				else if(0 == strcmp("$stable_sort" , get_cstr_wiz_string(&val->name))) {
					ExcuteData _excuteData; _excuteData.valid = 1; if(NULL == excuteData) { _excuteData.depth = 0; } else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// todo
					// todo
					vector<SortInfo> siVec;
					user_type* utTemp =
						user_type::Find(&global,
							ToBool4(NULL, &global, get_user_type_list_in_user_type(val, 0)->ToString(), _excuteData, &builder), &builder).second[0];

					vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
							temp.push_back(&(utTemp->get_item_list_in_user_type(item_count)));
							siVec.emplace_back(utTemp->get_item_list_in_user_type(item_count).GetName(), 1, i, &builder);
							item_count++;
						}
						else {
							temp.push_back(utTemp->get_user_type_list_in_user_type(ut_count));
							siVec.emplace_back(utTemp->get_user_type_list_in_user_type(ut_count)->GetName(), 2, i, &builder);
							ut_count++;
						}
					}

					std::stable_sort(siVec.begin(), siVec.end());


					user_type ut;
					for (i = 0; i < temp.size(); ++i)
					{
						if (siVec[i].iElement == 1) {
							ut.AddItem(siVec[i].data, static_cast<wiz::load_data::ItemType<string>*>(temp[siVec[i].idx])->Get(0));
						}
						else {
							ut.AddUserTypeItem(*(static_cast<user_type*>(temp[siVec[i].idx])));
						}
					}

					utTemp->Remove();

					wiz::load_data::LoadData::AddData(*(utTemp), "", ut.ToString(), "TRUE", _excuteData, &builder);


					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
				*/
				// ToDo - $if, $else!
				else if(0 == strcmp("$if", get_cstr_wiz_string(&val->name))) // ToDo!!
				{
					wiz_string TRUE_STR = make_wiz_string_from_cstr("TRUE");
					wiz_string FALSE_STR = make_wiz_string_from_cstr("FALSE");
					ExcuteData _excuteData; _excuteData.valid = 1;
					wiz_string temp;
					wiz_string temp2;
					int pass = 0;

					if(NULL == excuteData) { _excuteData.depth = 0; } 
					else { _excuteData.depth = excuteData->depth; }
					_excuteData.chkInfo = 1;
					_excuteData.info = *top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					temp = to_string_in_user_type(get_user_type_list_in_user_type(val, 0), &builder);
					
					temp2 = ToBool4(NULL, &global, &temp, &_excuteData, &builder);
					
					free_wiz_string(&temp);
					temp = temp2;

					if (!empty_wiz_stack_wiz_string(&top_wiz_stack_event_info(&eventStack)->conditionStack))
					{
						if (equal_wiz_string(&TRUE_STR, &temp) &&
							equal_wiz_string(top_wiz_stack_wiz_string(&top_wiz_stack_event_info(&eventStack)->conditionStack), &FALSE_STR)
							)
						{
							free_wiz_string(&temp);
							temp = make_wiz_string_from_other_wiz_string(&FALSE_STR);
						}
						else if (equal_wiz_string(&FALSE_STR, &temp) &&
							equal_wiz_string(top_wiz_stack_wiz_string(&top_wiz_stack_event_info(&eventStack)->conditionStack), &FALSE_STR)
							)
						{
							free_wiz_string(&temp);
							temp = make_wiz_string_from_other_wiz_string(&FALSE_STR);
						}
						else if (!empty_wiz_stack_any(&top_wiz_stack_event_info(&eventStack)->nowUT) &&
							*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) + 1 
							< get_user_type_list_size_in_user_type(top_wiz_stack_any(&top_wiz_stack_event_info(&eventStack)->nowUT))
							&& 0 == strcmp(get_cstr_wiz_string(&get_user_type_list_in_user_type(top_wiz_stack_any(&top_wiz_stack_event_info(&eventStack)->nowUT),
								*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) + 1)->name), "$else")
							)
						{
							push_wiz_stack_wiz_string(&top_wiz_stack_event_info(&eventStack)->conditionStack, &temp);
							pass = 1;
						}
						else if (equal_wiz_string(&TRUE_STR, &temp))
						{
							push_wiz_stack_wiz_string(&top_wiz_stack_event_info(&eventStack)->conditionStack, &temp);
							pass = 1;
						}
					}
					else
					{
						if (*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) + 1 
							< get_user_type_list_size_in_user_type(get_user_type_list_in_user_type(eventPtr, no))
							&& 0 == strcmp(get_cstr_wiz_string(&get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), 
								*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) + 1)->name), "$else")
							)
						{
							push_wiz_stack_wiz_string(&top_wiz_stack_event_info(&eventStack)->conditionStack, &temp);
						}
						else if (equal_wiz_string(&TRUE_STR, &temp))
						{
							push_wiz_stack_wiz_string(&top_wiz_stack_event_info(&eventStack)->conditionStack, &temp);
						}
					}

					if (equal_wiz_string(&TRUE_STR, &temp))
					{
						size_t initial_value = 0;
						push_wiz_stack_any(&top_wiz_stack_event_info(&eventStack)->nowUT, get_user_type_list_in_user_type(val, 1));

						(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
						push_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx, &initial_value);

						free_wiz_string(&temp);
						free_wiz_string(&TRUE_STR);
						free_wiz_string(&FALSE_STR);
						break;
					}//
					else if (equal_wiz_string(&FALSE_STR, &temp))
					{
						free_wiz_string(&temp);
						free_wiz_string(&TRUE_STR);
						free_wiz_string(&FALSE_STR);

						(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
						break;
					}
					else
					{
						// debug..
						printf("Error Debug : //%s//\n", get_cstr_wiz_string(&temp));
						
						// free
						free_wiz_string(&temp);
						
						free_wiz_string(&TRUE_STR);
						free_wiz_string(&FALSE_STR);

						exit = 1;
						break;
					}
				}
				else if(0 == strcmp("$else" , get_cstr_wiz_string(&val->name)))
				{
					if (!empty_wiz_stack_wiz_string(&top_wiz_stack_event_info(&eventStack)->conditionStack) 
						&& 0 == strcmp("FALSE", get_cstr_wiz_string(top_wiz_stack_wiz_string(&top_wiz_stack_event_info(&eventStack)->conditionStack)))
						)
					{
						wiz_string TRUE_STR = make_wiz_string_from_cstr("TRUE");
						size_t initial_value = 0;
						assign_wiz_string(top_wiz_stack_wiz_string(&top_wiz_stack_event_info(&eventStack)->conditionStack), &TRUE_STR);
						push_wiz_stack_any(&top_wiz_stack_event_info(&eventStack)->nowUT, get_user_type_list_in_user_type(val, 0));
						//val = top_wiz_stack_event_info(&eventStack).nowUT.top()->get_user_type_list_in_user_type(0); // empty chk
						(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
						push_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx, &initial_value);
						//top_wiz_stack_event_info(&eventStack).state.push(2);
						//state = 2; //
						free_wiz_string(&TRUE_STR);
						break;
					}
					else
					{
						(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
						break;
					}
				}
				else { //
					printf("it does not work : %s\n", get_cstr_wiz_string(&val->name)); //

					(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;
					break;
				}
			}
		
			if (exit) {
				break;
			}
		}

		if(info_chk) {
			free_all_event_info(&info);	
		}
	}
	
	// free
	{
		free_wiz_string(&EMPTY_STR);
		free_wiz_string(&ID_STR);
		free_wiz_string(&LOCAL_STR); free_wiz_string(&LOCAL_STR2);
		free_wiz_string(&MAIN_STR);
		free_wiz_string(&CALL_STR);
		free_wiz_string(&IF_STR);
		free_wiz_string(&ELSE_STR);
		free_wiz_string(&EVENT_STR);
		free_wiz_string(&ROOT_STR);
		free_wiz_string(&TRUE_STR);

		//
		{
			// todo - make free_all ?
		}
		free_all_wiz_map_wiz_string_and_user_type(&objectMap);
		//
		{
			// todo - make free_all ?
		}
		free_all_wiz_map_wiz_string_and_user_type(&moduleMap);

		for (i = 0; i < size_wiz_stack_event_info(&eventStack); ++i) {
			free_all_event_info(get_wiz_stack_event_info(&eventStack, i));
		}
		free_wiz_stack_event_info(&eventStack);

		{
			pair_wiz_string_and_int* temp = malloc(sizeof(pair_wiz_string_and_int) * convert.count);
			int i;

			inorder_wiz_string_and_int(&convert, temp); // rename to inorder_wiz_map_wiz_string_and_int!
			for (i = 0; i < convert.count; ++i) {
				free_wiz_string(&temp[i].first);
			}

			free(temp);
		}
		free_wiz_map_wiz_string_and_int(&convert);
		free_wiz_vector_any(&_events);
		shallow_free_user_type_in_user_type(&events);
		shallow_free_user_type_in_user_type(&Main);
		free_wiz_string_builder(&builder);
	}

	*_global = global;

	return module_value;
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

		
	}

	{
		user_type global;
		wiz_string str;

		init_wiz_string(&str, "test=3", 6);
		global = load_data_from_string_in_load_data(&str);

		free_wiz_string(&str);
		
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

void test_for_bool4(){
	int i;

	for (i = 0; i < 500; ++i) {
		const char* text = "id = 1 i = 3 j = 4 k = /./x";
		wiz_string str;
		wiz_string result;
		user_type global;
		ExcuteData excuteData;
		wiz_string_builder builder;
		wiz_string EMPTY = make_wiz_string("", 0);
		wiz_string name = make_wiz_string("x", 1);
		wiz_string value = make_wiz_string("TRUE", 4);
		init_wiz_string(&str, text, strlen(text));
		init_in_user_type(&global, &EMPTY);
			
		add_item_in_user_type(&global, &name, &value);

		init_wiz_string_builder(&builder, 1024, "", 0);

		excuteData.valid = 0;
		init_wiz_map_wiz_string_and_wiz_string(&excuteData.info.parameters);
		init_wiz_map_wiz_string_and_wiz_string(&excuteData.info.locals);

		result = ToBool4(NULL, &global, &str, &excuteData, &builder);

		//free_wiz_string(&name);
		//free_wiz_string(&value);
		free_wiz_string(&str);
		free_wiz_string(&result);
		
		free_wiz_string_builder(&builder);
		free_wiz_string(&EMPTY);
		free_wiz_map_wiz_string_and_wiz_string(&excuteData.info.parameters);
		free_wiz_map_wiz_string_and_wiz_string(&excuteData.info.locals);
	}
}

int main(int argc, char* argv[])
{
	user_type global;
	wiz_string EMPTY_STR = make_empty_wiz_string();
	wiz_string fileName; // = make_wiz_string_from_cstr("test.txt"); //
	ExcuteData excuteData;
	wiz_string return_value;

	if (1 == argc) {
		printf("fileName : ");
		fileName = wiz_fgets(stdin);
	}
	else if (2 <= argc) {
		fileName = make_wiz_string_from_cstr(argv[1]);
	}

	printf("fileName is : %s\n", get_cstr_wiz_string(&fileName));
	global = load_data_from_file_in_load_data(&fileName);

	return_value = excute_module(&EMPTY_STR, &global, NULL);
	printf("%s\n", get_cstr_wiz_string(&return_value));

	free_wiz_string(&fileName);
	free_user_type_in_user_type(&global);
	free_wiz_string(&return_value);

	return 0;
}

