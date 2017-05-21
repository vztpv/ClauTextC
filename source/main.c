

#define _CRT_SECURE_NO_WARNINGS


#include <vld.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


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
	int i;
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
	wiz_string MAIN_STR;
	wiz_string CALL_STR;
	wiz_string IF_STR;
	wiz_string ELSE_STR;
	wiz_string EVENT_STR;
	wiz_string ROOT_STR;

	// init
	init_wiz_string(&EMPTY_STR, "", 0);
	init_wiz_string(&ID_STR, "id", 2);
	init_wiz_string(&LOCAL_STR, "$local", 6);
	init_wiz_string(&MAIN_STR, "Main", 4);
	init_wiz_string(&CALL_STR, "$call", 5);
	init_wiz_string(&ELSE_STR, "$else", 5);
	init_wiz_string(&EVENT_STR, "Event", 5);
	init_wiz_string(&IF_STR, "$if", 3);
	init_wiz_string(&ROOT_STR, "root", 4);
	
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
			temp.first = get_wiz_vector_item_type(&x, 0)->value;
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
				free_wiz_string(&LOCAL_STR);
				free_wiz_string(&MAIN_STR);
				free_wiz_string(&CALL_STR);
				free_wiz_string(&IF_STR);
				free_wiz_string(&ELSE_STR);
				free_wiz_string(&EVENT_STR);
				free_wiz_string(&ROOT_STR);

				free_wiz_map_wiz_string_and_user_type(&objectMap);
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
		remove_item_list_by_var_name_in_user_type(&global, &MAIN_STR, 0);
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
					for (int j = 0; j < get_item_list_size_in_user_type(get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), i)); ++j) {
						wiz_string name = get_item_list_in_user_type(get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), i), j)->value;
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
		push_wiz_stack_event_info(&eventStack, &excuteData->info);
	}

	// main loop
	while (!empty_wiz_stack_event_info(&eventStack))
	{
		event_info info = *top_wiz_stack_event_info(&eventStack);
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
				module_value = top_wiz_stack_event_info(&eventStack)->return_value;
			}

			free_all_event_info(top_wiz_stack_event_info(&eventStack));
			pop_wiz_stack_event_info(&eventStack);
			continue;
		}

		{ /// has bug!! WARNNING!!
			user_type* val = NULL;
			if (size_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) == 1) {
				if (get_user_type_list_size_in_user_type(get_user_type_list_in_user_type(eventPtr, no)) > 
						*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)) {

					val = get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), 
						*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx));

					if (*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) >= 1 && equal_wiz_string(&val->name, &ELSE_STR)
						&& equal_wiz_string(&get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no),
							*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) - 1)->name, &IF_STR) ) {
						
						// free
						{
							free_wiz_string(&EMPTY_STR);
							free_wiz_string(&ID_STR);
							free_wiz_string(&LOCAL_STR);
							free_wiz_string(&MAIN_STR);
							free_wiz_string(&CALL_STR);
							free_wiz_string(&IF_STR);
							free_wiz_string(&ELSE_STR);
							free_wiz_string(&EVENT_STR);
							free_wiz_string(&ROOT_STR);

							free_wiz_map_wiz_string_and_user_type(&objectMap);
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
						return make_wiz_string_from_cstr("ERROR not exist $if, front $else.");
					}
					if (*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) == 0 && equal_wiz_string(&val->name, &ELSE_STR) ) {
						
						// free
						{
							free_wiz_string(&EMPTY_STR);
							free_wiz_string(&ID_STR);
							free_wiz_string(&LOCAL_STR);
							free_wiz_string(&MAIN_STR);
							free_wiz_string(&CALL_STR);
							free_wiz_string(&IF_STR);
							free_wiz_string(&ELSE_STR);
							free_wiz_string(&EVENT_STR);
							free_wiz_string(&ROOT_STR);

							free_wiz_map_wiz_string_and_user_type(&objectMap);
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
						return make_wiz_string_from_cstr("ERROR not exist $if, front $else.");
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
						&& equal_wiz_string(&get_user_type_list_in_user_type(top_wiz_stack_any(&top_wiz_stack_event_info(&eventStack)->nowUT), 
							(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) - 1))->name, &IF_STR)
						) {
						// free
							{
								free_wiz_string(&EMPTY_STR);
								free_wiz_string(&ID_STR);
								free_wiz_string(&LOCAL_STR);
								free_wiz_string(&MAIN_STR);
								free_wiz_string(&CALL_STR);
								free_wiz_string(&IF_STR);
								free_wiz_string(&ELSE_STR);
								free_wiz_string(&EVENT_STR);
								free_wiz_string(&ROOT_STR);

								free_wiz_map_wiz_string_and_user_type(&objectMap);
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
						return make_wiz_string_from_cstr("ERROR not exist $if, front $else.");
					}
					if (top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) == 0 && equal_wiz_string(&val->name, &ELSE_STR)) {
						// free
						{
							free_wiz_string(&EMPTY_STR);
							free_wiz_string(&ID_STR);
							free_wiz_string(&LOCAL_STR);
							free_wiz_string(&MAIN_STR);
							free_wiz_string(&CALL_STR);
							free_wiz_string(&IF_STR);
							free_wiz_string(&ELSE_STR);
							free_wiz_string(&EVENT_STR);
							free_wiz_string(&ROOT_STR);

							free_wiz_map_wiz_string_and_user_type(&objectMap);
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
						return make_wiz_string_from_cstr("ERROR not exist $if, front $else.");
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
							&& equal_wiz_string(&get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no),
								*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) - 1)->name, &IF_STR)) {
							// free
								{
									free_wiz_string(&EMPTY_STR);
									free_wiz_string(&ID_STR);
									free_wiz_string(&LOCAL_STR);
									free_wiz_string(&MAIN_STR);
									free_wiz_string(&CALL_STR);
									free_wiz_string(&IF_STR);
									free_wiz_string(&ELSE_STR);
									free_wiz_string(&EVENT_STR);
									free_wiz_string(&ROOT_STR);

									free_wiz_map_wiz_string_and_user_type(&objectMap);
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
							return make_wiz_string_from_cstr("ERROR not exist $if, front $else.");
						}
						if (top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) == 0 && equal_wiz_string(&val->name, &ELSE_STR)) {
							// free
							{
								free_wiz_string(&EMPTY_STR);
								free_wiz_string(&ID_STR);
								free_wiz_string(&LOCAL_STR);
								free_wiz_string(&MAIN_STR);
								free_wiz_string(&CALL_STR);
								free_wiz_string(&IF_STR);
								free_wiz_string(&ELSE_STR);
								free_wiz_string(&EVENT_STR);
								free_wiz_string(&ROOT_STR);

								free_wiz_map_wiz_string_and_user_type(&objectMap);
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
							return make_wiz_string_from_cstr("ERROR not exist $if, front $else.");
						}
					}
					else {
						val = NULL;
					}
				}
			}
			
			(*top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx))++;

			//while (val != NULL)
			{
				// ToDo -$filter, $map, $reduce?
				// list = { base_tax } // /root/ X , root/ O
				// $filter = { utDir = { /./test } 
				// condition = { AND = { EXSITITEMBYVAL = { base_tax root/list } COMP> = { ~~ / 5.000 ~ 0 } } } 
				// recursive = { 0 or 1 } } // return UserType.?
				/*if ("$for_each" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					const string eventID = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					const string dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);
					const string condition = val->get_user_type_list_in_user_type(2)->ToString();

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
							if ("$parameter" == event->get_user_type_list_in_user_type(i)->GetName()) {
								if (1 == event->get_user_type_list_in_user_type(i)->GetItemListSize()) {
									parameter = parameter + event->get_user_type_list_in_user_type(i)->GetItemList(0).Get(0);
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
						top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
						break;
					}
					// chk loop and condition! chk do not use ""
					{
						int count = 0;
						top_wiz_stack_event_info(&eventStack).return_value = "";
						user_type* ut = user_type::Find(&global, dir, &builder).second[0];
						for (i = 0; i < ut->GetItemListSize(); ++i) {
							string _condition = condition;

							_condition = wiz::String::replace(_condition, "~~~", ut->GetItemList(i).Get(0)); //
							_condition = wiz::String::replace(_condition, "////", dir);
							_condition = wiz::String::replace(_condition, "///", wiz::_toString(i));
							// ToDo - chk : empty name!!
							if (ut->GetItemList(i).GetName().empty()) {
								_condition = wiz::String::replace(_condition, "~~", "^"); // chk..
							}
							else
							{
								_condition = wiz::String::replace(_condition, "~~", ut->GetItemList(i).GetName());
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
							_excuteData.depth = excuteData->depth;
							_excuteData.pEvents = &eventsTemp;
							_excuteData.pModule = moduleMapPtr;
							_excuteData.pObjectMap = objectMapPtr;

							wiz::load_data::LoadData::AddData(eventsTemp, "/root", "Event = { id = NONE $call = { " + parameter + " = { " + ut->GetItemList(i).ToString() + " } } }", "TRUE", _excuteData, &builder);

							const string return_value = excute_module("Main = { $call = { id = NONE } }", &global, _excuteData);

							user_type return_value_ut;

							wiz::load_data::LoadData::LoadDataFromString(return_value, return_value_ut);

							ut->GetItemList(i).SetName(return_value_ut.GetItemList(0).GetName());
							ut->GetItemList(i).Set(0, return_value_ut.GetItemList(0).Get(0));

							if (count != 0) {
								top_wiz_stack_event_info(&eventStack).return_value = top_wiz_stack_event_info(&eventStack).return_value + " ";
							}

							top_wiz_stack_event_info(&eventStack).return_value = top_wiz_stack_event_info(&eventStack).return_value + return_value;

							count++;
						}
					}

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$do" == val->GetName()) { // chk?
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					user_type subGlobal;
					wiz::load_data::LoadData::LoadDataFromString(val->get_user_type_list_in_user_type(1)->ToString(), subGlobal);
					user_type inputUT;
					wiz::load_data::LoadData::LoadDataFromString(ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder), inputUT);


					wiz::load_data::LoadData::AddData(subGlobal, "/./", inputUT.ToString(), "TRUE", _excuteData, &builder);


					top_wiz_stack_event_info(&eventStack).return_value = excute_module("", &subGlobal, ExcuteData()); // return ?


					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				// add option! for "".support eu3, eu4.
				else if ("$replace_datatype1" == val->GetName()) { // name
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string rex = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					rex = rex.substr(1, rex.size() - 2);
					vector<string> sval;
					vector<string> scondition;
					string start_dir = "root";

					if (val->get_user_type_list_size_in_user_type() >= 3)
					{
						int n = (val->get_user_type_list_size_in_user_type() - 1) / 2;
						for (i = 0; i < n; ++i) {
							sval.push_back(val->get_user_type_list_in_user_type(1)->ToString());

							scondition.push_back(val->get_user_type_list_in_user_type(2)->ToString());
						}
					}
					if (val->get_user_type_list_size_in_user_type() >= 4) {
						start_dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(3)->ToString(), _excuteData, &builder);
					}

					wiz::load_data::LoadData::ReplaceDataType1(global, rex, sval, scondition, start_dir, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$replace_datatype1_2" == val->GetName()) { //val
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string rex = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					rex = rex.substr(1, rex.size() - 2);
					vector<string> sval;
					vector<string> scondition;
					string start_dir = "root";

					if (val->get_user_type_list_size_in_user_type() >= 3)
					{
						int n = (val->get_user_type_list_size_in_user_type() - 1) / 2;
						int count = 1;
						for (i = 0; i < n; ++i) {
							sval.push_back(val->get_user_type_list_in_user_type(count)->ToString());
							count++;
							scondition.push_back(val->get_user_type_list_in_user_type(count)->ToString());
							count++;
						}
					}

					if (val->get_user_type_list_size_in_user_type() >= 4 && (val->get_user_type_list_size_in_user_type() % 2 == 0)) {
						start_dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(val->get_user_type_list_size_in_user_type() - 1)->ToString(), _excuteData, &builder);
					}

					wiz::load_data::LoadData::ReplaceDataType1_2(global, rex, sval, scondition, start_dir, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$replace_datatype2" == val->GetName()) { // usertype name
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string rex = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					rex = rex.substr(1, rex.size() - 2);
					//cout << val->get_user_type_list_in_user_type(1)->ToString() << endl;
					vector<string> sval;
					vector<string> scondition;
					string start_dir = "root";

					if (val->get_user_type_list_size_in_user_type() >= 3)
					{
						int n = (val->get_user_type_list_size_in_user_type() - 1) / 2;
						int count = 1;
						for (i = 0; i < n; ++i) {
							sval.push_back(val->get_user_type_list_in_user_type(count)->ToString());
							count++;
							scondition.push_back(val->get_user_type_list_in_user_type(count)->ToString());
							count++;
						}
					}

					if (val->get_user_type_list_size_in_user_type() >= 4 && (val->get_user_type_list_size_in_user_type() % 2 == 0)) {
						start_dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(val->get_user_type_list_size_in_user_type() - 1)->ToString(), _excuteData, &builder);
					}

					//cout << scondition << endl;
					//cout << "sval " << sval << endl;
					wiz::load_data::LoadData::ReplaceDataType2(global, rex, sval, scondition, start_dir, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$replace_datetype" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string sval = val->get_user_type_list_in_user_type(0)->ToString();

					string scondition = "TRUE";
					string start_dir = "root";

					if (val->get_user_type_list_size_in_user_type() >= 2)
					{
						scondition = val->get_user_type_list_in_user_type(1)->ToString();
					}
					if (val->get_user_type_list_size_in_user_type() >= 3) {
						start_dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(2)->ToString(), _excuteData, &builder);
					}

					wiz::load_data::LoadData::ReplaceDateType(global, sval, scondition, start_dir, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$replace_datetype2" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string sval = val->get_user_type_list_in_user_type(0)->ToString();
					string scondition = "TRUE";
					string start_dir = "root";


					if (val->get_user_type_list_size_in_user_type() >= 2)
					{
						scondition = val->get_user_type_list_in_user_type(1)->ToString();
					}
					if (val->get_user_type_list_size_in_user_type() >= 3) {
						start_dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(2)->ToString(), _excuteData, &builder);
					}

					wiz::load_data::LoadData::ReplaceDateType2(global, sval, scondition, start_dir, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$remove_usertype_total" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string ut_name = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					string condition = "TRUE";
					string start_dir = "root";

					if (val->get_user_type_list_size_in_user_type() >= 2)
					{
						condition = val->get_user_type_list_in_user_type(1)->ToString();
					}
					if (val->get_user_type_list_size_in_user_type() >= 3) {
						start_dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(2)->ToString(), _excuteData, &builder);
					}

					wiz::load_data::LoadData::RemoveUserTypeTotal(global, ut_name, condition, start_dir, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$replace_item" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					// chk NULL vs val?
					string svar = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					string sval = val->get_user_type_list_in_user_type(1)->ToString();
					string scondition = "TRUE";
					string start_dir = "root";

					if (val->get_user_type_list_size_in_user_type() >= 3)
					{
						scondition = val->get_user_type_list_in_user_type(2)->ToString();
					}
					if (val->get_user_type_list_size_in_user_type() >= 4) {
						start_dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(3)->ToString(), _excuteData, &builder);
					}

					wiz::load_data::LoadData::ReplaceItem(global, svar, sval, scondition, start_dir, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$edit_mode" == val->GetName()) // chk!!
				{
					MStyleTest(&global);

					system("cls");

					gotoxy(0, 0);
					setcolor(7, 0);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$save" == val->GetName()) // save data, event, main!
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					//todo
					// "filename" save_option(0~2)
					string fileName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string option = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);

					wiz::load_data::LoadData::SaveWizDB(global, fileName, option, "");
					wiz::load_data::LoadData::SaveWizDB(Main, fileName, option, "APPEND");
					wiz::load_data::LoadData::SaveWizDB(events, fileName, option, "APPEND");

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$save_data_only" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					//todo
					// "filename" save_option(0~2)
					string fileName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string option = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);

					wiz::load_data::LoadData::SaveWizDB(global, fileName, option, "");

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}

				else if ("$save_data_only2" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					//todo
					// "filename" save_option(0~2)
					string dirName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					string fileName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string option = ToBool4(NULL, global, val->get_user_type_list_in_user_type(2)->ToString(), _excuteData, &builder);

					// todo - for? auto x = global.get_user_type_item_in_user_type(dirName);
					user_type* utTemp = global.get_user_type_item_in_user_type(dirName)[0];
					wiz::load_data::LoadData::SaveWizDB(*utTemp, fileName, option, "");


					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				/// module name or object name -> must "~" .
				else if ("$register_module" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string moduleFileName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					user_type moduleUT;
					wiz::load_data::LoadData::LoadDataFromFile(moduleFileName, moduleUT);

					//moduleMapPtr.insert(make_pair(moduleFileName, moduleUT));
					(*moduleMapPtr)[moduleFileName] = move(moduleUT);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$call_registered_module" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string moduleFileName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					string input;

					if (val->get_user_type_list_size_in_user_type() >= 2) {
						input = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);
					}

					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					user_type moduleUT = (*moduleMapPtr).at(moduleFileName);
					wiz::load_data::LoadData::AddData(moduleUT, "", input, "TRUE", _excuteData, &builder);

					top_wiz_stack_event_info(&eventStack).return_value = excute_module("", &moduleUT, ExcuteData());

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$module" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string moduleFileName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					string input;

					if (val->get_user_type_list_size_in_user_type() >= 2) {
						input = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);
					}

					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					user_type moduleUT;
					wiz::load_data::LoadData::LoadDataFromFile(moduleFileName, moduleUT);
					wiz::load_data::LoadData::AddData(moduleUT, "", input, "TRUE", _excuteData, &builder);

					top_wiz_stack_event_info(&eventStack).return_value = excute_module("", &moduleUT, ExcuteData());

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				// todo - register module from file
				// todo	- register module from string
				// todo - call registered module.  $registered_module = { name = { ~ } input = { input = { n = 1 } } }

				else if ("$register_object" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string objectFileName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);

					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);

					user_type objectUT;
					wiz::load_data::LoadData::LoadDataFromFile(objectFileName, objectUT);

					//objectMapPtr.insert(make_pair(objectFileName, data));
					(*objectMapPtr)[objectFileName] = move(objectUT);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$copy_object" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string objectNameA = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					string objectNameB = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);

					objectNameA = wiz::String::substring(objectNameA, 1, objectNameA.size() - 2);
					objectNameB = wiz::String::substring(objectNameB, 1, objectNameB.size() - 2);

					(*objectMapPtr)[objectNameB] = (*objectMapPtr).at(objectNameA);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$register_object_from_string" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string objectName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					string objectData = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);
					objectName = wiz::String::substring(objectName, 1, objectName.size() - 2);
					objectData = wiz::String::substring(objectData, 1, objectData.size() - 2);
					user_type objectUT;
					wiz::load_data::LoadData::LoadDataFromString(objectData, objectUT); // error chk?

																						//objectMapPtr.insert(make_pair(objectFileName, data));
					(*objectMapPtr)[objectName] = move(objectUT);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$call_registered_object" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string objectFileName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);
					string parameter;
					user_type objectUT = objectMapPtr->at(objectFileName);

					parameter = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);

					string data = " Event = { id = NONE $call = { " + parameter + "  } } ";

					wiz::load_data::LoadData::AddData(objectUT, "/./", data, "TRUE", _excuteData, &builder);

					top_wiz_stack_event_info(&eventStack).return_value = excute_module(" Main = { $call = { id = NONE } } ", &objectUT, ExcuteData());

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$call_registered_object2" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string objectFileName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);
					string parameter;

					parameter = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);

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

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				/// object of class?
				else if ("$object" == val->GetName()) { // "fileName"
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string objectFileName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);
					string parameter;

					parameter = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);


					user_type objectUT;
					wiz::load_data::LoadData::LoadDataFromFile(objectFileName, objectUT);

					string data = objectUT.ToString();

					data = data + "Event = { id = NONE $call = { " + parameter + "  } } ";

					objectUT.Remove();
					wiz::load_data::LoadData::LoadDataFromString(data, objectUT);

					top_wiz_stack_event_info(&eventStack).return_value = excute_module(" Main = { $call = { id = NONE } } ", &objectUT, ExcuteData());

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$object_from_string" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string data = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					string parameter;

					parameter = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);


					user_type objectUT;
					wiz::load_data::LoadData::LoadDataFromString(data, objectUT);

					string str = objectUT.ToString();

					str = str + " Event = { id = NONE $call = { " + parameter + "  } } ";

					objectUT.Remove();
					wiz::load_data::LoadData::LoadDataFromString(str, objectUT);

					top_wiz_stack_event_info(&eventStack).return_value = excute_module(" Main = { $call = { id = NONE } } ", &objectUT, ExcuteData());

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				// todo - register object from file.
				//		~.ToString() + "Main = { $call = { id = 0 } } Event = { id = 0 $call = { id = " + id_val + " " + param_name1 + " = " + param_val1 + "  } } "
				// todo - register object from string.
				// todo - call registered object.  $registered_object = { name = { "ex2.txt" } parameter = { id = 1 i = 1 j = 1 } }  
				else if ("$option" == val->GetName()) // first
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					top_wiz_stack_event_info(&eventStack).option = ToBool4(NULL, global, val->ToString(), _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				// done - ($push_back-insert!) $pop_back, $push_front, $pop_front ($front?, $back?)
				else if ("$pop_back" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					user_type* ut = NULL;
					auto finded = user_type::Find(&global, dir, &builder);
					ut = finded.second[0];

					string condition = "TRUE";
					if (val->get_user_type_list_size_in_user_type() >= 2) {
						condition = val->get_user_type_list_in_user_type(1)->ToString();
					}
					wiz::load_data::LoadData::Remove(global, dir, ut->GetIListSize() - 1, condition, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$push_front" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string value = val->get_user_type_list_in_user_type(1)->ToString();
					string dir;
					if (val->get_user_type_list_in_user_type(0)->GetItemListSize() > 0) {
						dir = val->get_user_type_list_in_user_type(0)->GetItemList(0).Get(0);
						dir = ToBool4(NULL, global, dir, _excuteData, &builder);
					}
					else ///val->Ge
					{
						dir = string(val->get_user_type_list_in_user_type(0)->ToString());
						dir = ToBool4(NULL, global, dir, _excuteData, &builder);
					}


					value = ToBool4(NULL, global, value, _excuteData, &builder);

					string condition = "TRUE";
					if (val->get_user_type_list_size_in_user_type() >= 3) {
						condition = val->get_user_type_list_in_user_type(2)->ToString();
					}
					wiz::load_data::LoadData::AddDataAtFront(global, dir, value, condition, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$pop_front" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);

					string condition = "TRUE";
					if (val->get_user_type_list_size_in_user_type() >= 2) {
						condition = val->get_user_type_list_in_user_type(1)->ToString();
					}
					wiz::load_data::LoadData::Remove(global, dir, 0, condition, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$wait" == val->GetName()) {
					for (i = 0; i < waits.size(); ++i) {
						waits[i]->join();
						delete waits[i];
					}
					waits.resize(0);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$call" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					if (!val->GetItem("id").empty()) {
						info.id = val->GetItem("id")[0].Get(0);
					}
					else {
						info.id = ToBool4(NULL, global, val->get_user_type_item_in_user_type("id")[0]->ToString(), _excuteData, &builder);
					}

					info.eventUT = get_user_type_list_in_user_type(eventPtr, no);
					info.userType_idx.clear();
					info.userType_idx.push(0);
					info.return_value.clear();
					info.nowUT.clear();

					event_info info2; //
					info2 = info;

					if (info.id != top_wiz_stack_event_info(&eventStack).id) {
						info.parameters.clear();
					}
					info.conditionStack.clear();

					//
					if (info.id != top_wiz_stack_event_info(&eventStack).id) {
						for (int j = 0; j < val->GetItemListSize(); ++j) {
							if (val->GetItemListSize() > 0) {
								_excuteData.info = info2;
								string temp = ToBool4(NULL, global, val->GetItemList(j).Get(0), _excuteData, &builder);
								info.parameters.insert(make_pair(val->GetItemList(j).GetName(), temp));
							}
						}
						for (int j = 0; j < val->get_user_type_list_size_in_user_type(); ++j) {
							if (val->get_user_type_list_size_in_user_type() > 0) {
								_excuteData.info = info2;
								string temp = ToBool4(NULL, global, val->get_user_type_list_in_user_type(j)->ToString(), _excuteData, &builder);
								info.parameters.insert(make_pair(val->get_user_type_list_in_user_type(j)->GetName(), temp));
							}
						}
						top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					}
					else { // recursive call!
						if (val->GetItemListSize() > 0) {
							for (int j = 0; j < val->GetItemListSize(); ++j) {
								_excuteData.info = info;
								_excuteData.info.parameters = info2.parameters;

								string temp = ToBool4(NULL, global, val->GetItemList(j).Get(0), _excuteData, &builder);

								map_wiz_string_string>::iterator x;
								if ((x = info.parameters.find(val->GetItemList(j).GetName())) != info.parameters.end())
								{
									x->second = temp;
								}
							}
						}
						if (val->get_user_type_list_size_in_user_type() > 0) {
							for (int j = 0; j < val->get_user_type_list_size_in_user_type(); ++j) {
								_excuteData.info = info;
								_excuteData.info.parameters = info2.parameters;

								string temp = ToBool4(NULL, global, val->get_user_type_list_in_user_type(j)->ToString(), _excuteData, &builder);

								map_wiz_string_string>::iterator x;
								if ((x = info.parameters.find(val->get_user_type_list_in_user_type(j)->GetName())) != info.parameters.end())
								{
									x->second = temp;
								}
							}
						}

						top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;

						// remove remove_now_event_stack_a?
						if (top_wiz_stack_event_info(&eventStack).option == "REMOVE_NOW_EVENT_STACK_A" || "REMOVE_IF_CALL_ONESELF_EVENT" == top_wiz_stack_event_info(&eventStack).option) //
						{
							pop_wiz_stack_event_info(&eventStack);
						}
					}

					if (0 == eventStack.empty() && top_wiz_stack_event_info(&eventStack).option == "REMOVE_IF_CALL_ANY_EVENT")
					{
						pop_wiz_stack_event_info(&eventStack);
					}


					info.locals.clear();
					const int no = convert.at(info.id);
					for (i = 0; i < get_user_type_list_in_user_type(eventPtr, no)->get_user_type_list_size_in_user_type(); ++i) {
						if (get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), i)->GetName() == "$local") {
							for (int j = 0; j < get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), i)->GetItemListSize(); ++j) {
								string name = get_user_type_list_in_user_type(get_user_type_list_in_user_type(eventPtr, no), i)->GetItemList(j).Get(0);
								string value = "";
								info.locals.insert(make_pair(name, value));
							}
							break;
						}
					}

					if (waits.size() >= 4) {
						for (i = 0; i < waits.size(); ++i) {
							waits[i]->join();
							delete waits[i]; // chk ?
						}
						waits.resize(0);
					}

					if (0 == val->GetItem("option").empty() && val->GetItem("option")[0].Get(0) == "USE_THREAD") {
						_excuteData.info = info;
						thread* A = new thread(excute_module, "", &global, _excuteData);

						waits.push_back(A);
					}
					else {
						eventStack.push(info);
					}

					break;
				}

				else if ("$assign" == val->GetName()) /// -> assign2?
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					pair_wiz_string_string> dir = Find2(&global, val->GetItemList(0).Get(0));
					string data = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);

					if (dir.first == "" && wiz::String::startsWith(dir.second, "$local."))
					{
						top_wiz_stack_event_info(&eventStack).locals[wiz::String::substring(dir.second, 7)] = data;
					}
					else {
						wiz::load_data::LoadData::SetData(global, dir.first, dir.second, data, "TRUE", _excuteData, &builder);
					}
					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}

				else if ("$assign2" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					pair_wiz_string_string> dir = Find2(&global, ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder));
					string data = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);

					{
						if (dir.first == "" && wiz::String::startsWith(dir.second, "$local."))
						{
							top_wiz_stack_event_info(&eventStack).locals[wiz::String::substring(dir.second, 7)] = data;
						}
						else {
							wiz::load_data::LoadData::SetData(global, dir.first, dir.second, data, "TRUE", _excuteData, &builder);
						}
					}

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$assign_local" == val->GetName()) /// no condition, 
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					pair_wiz_string_string> dir = Find2(&global, ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder));
					string data = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);

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

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$assign_global" == val->GetName()) // 주의!! dir=> dir/name ( dir= { name = val } } , @�??�에 붙여???�다. 
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					pair_wiz_string_string> dir = Find2(&global, ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder));
					string data = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);

					//string condition;
					//if (val->get_user_type_list_size_in_user_type() >= 3) {
					//	condition = ToBool4(NULL, global, val->get_user_type_list_in_user_type(2)->ToString(), _excuteData, &builder);
					//}
					wiz::load_data::LoadData::SetData(global, dir.first, dir.second, data, "TRUE", _excuteData, &builder);

					// chk local?

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				/// cf) insert3? - any position?
				else if ("$push_back" == val->GetName() || "$insert" == val->GetName() || "$insert2" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string value = val->get_user_type_list_in_user_type(1)->ToString();
					string dir;
					if (val->get_user_type_list_in_user_type(0)->GetItemListSize() > 0) {
						dir = val->get_user_type_list_in_user_type(0)->GetItemList(0).Get(0);
						dir = ToBool4(NULL, global, dir, _excuteData, &builder);
					}
					else ///val->Ge
					{
						dir = string(val->get_user_type_list_in_user_type(0)->ToString());
						dir = ToBool4(NULL, global, dir, _excuteData, &builder);
					}

					value = ToBool4(NULL, global, value, _excuteData, &builder);

					string condition = "TRUE";
					if (val->get_user_type_list_size_in_user_type() >= 3) {
						condition = val->get_user_type_list_in_user_type(2)->ToString();
					}
					wiz::load_data::LoadData::AddData(global, dir, value, condition, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$insert_noname_usertype" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string position = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);;
					string data = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);;
					string condition = val->get_user_type_list_in_user_type(2)->ToString();

					wiz::load_data::LoadData::AddNoNameUserType(global, position, data, condition, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$insert_by_idx" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string value = val->get_user_type_list_in_user_type(2)->ToString();
					int idx = atoi(ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder).c_str());
					string dir;
					if (val->get_user_type_list_in_user_type(0)->GetItemListSize() > 0) {
						dir = val->get_user_type_list_in_user_type(0)->GetItemList(0).Get(0);
						dir = ToBool4(NULL, global, dir, _excuteData, &builder);
					}
					else ///val->Ge
					{
						dir = string(val->get_user_type_list_in_user_type(0)->ToString());
						dir = ToBool4(NULL, global, dir, _excuteData, &builder);
					}

					value = ToBool4(NULL, global, value, _excuteData, &builder);

					string condition = "TRUE";
					if (val->get_user_type_list_size_in_user_type() >= 4) {
						condition = val->get_user_type_list_in_user_type(3)->ToString();
					}
					wiz::load_data::LoadData::Insert(global, dir, idx, value, condition, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$make" == val->GetName()) // To Do? - make2? or remake? 
													// cf) make empty ut??
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir;
					bool is2 = 0;
					if (val->GetItemListSize() > 0) {
						dir = val->GetItemList(0).Get(0);
						dir = ToBool4(NULL, global, dir, _excuteData, &builder);
					}
					else // 
					{
						dir = string(val->get_user_type_list_in_user_type(0)->ToString());
						dir = ToBool4(NULL, global, dir, _excuteData, &builder);
						is2 = 1;
					}

					string name;
					for (i = dir.size() - 1; i >= 0; --i)
					{
						if (dir[i] == '/') {
							name = wiz::String::substring(dir, i + 1);
							dir = wiz::String::substring(dir, 0, i - 1);
							break;
						}
					}
					if (dir.empty()) { dir = "."; }

					string condition = "TRUE";
					if (is2 && val->get_user_type_list_size_in_user_type() >= 2) {
						condition = val->get_user_type_list_in_user_type(1)->ToString();
					}
					else if (0 == is2 && val->get_user_type_list_size_in_user_type() >= 1) {
						condition = val->get_user_type_list_in_user_type(0)->ToString();
					}

					wiz::load_data::LoadData::AddUserType(global, dir, name, "", condition, _excuteData, &builder);


					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$findIndex" == val->GetName()) // For list : { 1 2  3 4 5 }
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					string value = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);

					user_type ut;
					wiz::load_data::LoadData::LoadDataFromString(user_type::Find(&global, dir, &builder).second[0]->ToString(), ut);

					for (i = 0; i < ut.GetItemListSize(); ++i) {
						if (ut.GetItemList(i).Get(0) == value) {
							top_wiz_stack_event_info(&eventStack).return_value = wiz::toStr(i);
							break;
						}
					}

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$remove" == val->GetName()) // remove by dir., remove all?
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = string(val->GetItemList(0).Get(0).c_str()); // item -> userType

					dir = ToBool4(NULL, global, dir, _excuteData, &builder);

					string condition = "TRUE";
					if (val->get_user_type_list_size_in_user_type() >= 2) {
						condition = val->get_user_type_list_in_user_type(1)->ToString();
					}
					wiz::load_data::LoadData::Remove(global, dir, condition, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$remove2" == val->GetName()) // remove /dir/name 
														// if name is empty, then chk!!
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = string(val->GetItemList(0).Get(0).c_str()); // item -> userType
					dir = ToBool4(NULL, global, dir, _excuteData, &builder);
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
					if (val->get_user_type_list_size_in_user_type() >= 2) {
						condition = val->get_user_type_list_in_user_type(1)->ToString();
					}
					wiz::load_data::LoadData::Remove(global, dir, name, condition, _excuteData, &builder);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$remove3" == val->GetName()) /// remove itemlist by idx.
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					string value = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);

					int idx = atoi(value.c_str());  // long long -> int?

					string condition = "TRUE";

					if (val->get_user_type_list_size_in_user_type() >= 3) {
						condition = val->get_user_type_list_in_user_type(2)->ToString();
					}

					wiz::load_data::LoadData::Remove(global, dir, idx, condition, _excuteData, &builder);
					// remove -> UserType::Find(&global, dir).second[0]->RemoveItemList(idx); /// change ?

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}

				else if ("$setElement" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					string idx = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);
					string value = ToBool4(NULL, global, val->get_user_type_list_in_user_type(2)->ToString(), _excuteData, &builder);

					int _idx = stoi(idx);
					user_type::Find(&global, dir, &builder).second[0]->SetItem(_idx, value);

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$swap" == val->GetName()) // $swap2
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = string(val->GetItemList(0).Get(0).c_str()); // + 0
					string value1 = val->get_user_type_list_in_user_type(0)->ToString();
					string value2 = val->get_user_type_list_in_user_type(1)->ToString();

					value1 = ToBool4(NULL, global, value1, _excuteData, &builder);
					value2 = ToBool4(NULL, global, value2, _excuteData, &builder);
					if (value1 != value2) {
						int x = atoi(value1.c_str());
						int y = atoi(value2.c_str());

						string temp = user_type::Find(&global, dir, &builder).second[0]->GetItemList(x).Get(0);
						string temp2 = user_type::Find(&global, dir, &builder).second[0]->GetItemList(y).Get(0);

						wiz::load_data::LoadData::SetData(global, dir, x, temp2, "TRUE", _excuteData, &builder);
						wiz::load_data::LoadData::SetData(global, dir, y, temp, "TRUE", _excuteData, &builder);
					}

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$print" == val->GetName()) /// has many bugs..!?, for print list or print item?.
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					if (val->get_user_type_list_size_in_user_type() == 1
						&& val->get_user_type_list_in_user_type(0)->GetItemListSize() == 1)
					{
						string listName = val->get_user_type_list_in_user_type(0)->GetItemList(0).Get(0);

						if (listName.size() >= 2 && listName[0] == '\"' && listName.back() == '\"')
						{
							listName = wiz::String::substring(listName, 1, listName.size() - 2);
							cout << listName;
						}
						else if (listName.size() == 2 && listName[0] == '\\' && listName[1] == 'n')
						{
							cout << endl;
						}
						else if (wiz::String::startsWith(listName, "$local.")
							|| wiz::String::startsWith(listName, "$parameter.")
							)
						{
							string temp = ToBool4(NULL, global, listName, _excuteData, &builder);
							if (temp.empty()) {
								cout << "EMPTY";
							}
							else {
								cout << temp;
							}
						}
						else if (wiz::String::startsWith(listName, "/") && listName.size() > 1)
						{
							string temp = ToBool4(NULL, global, listName, _excuteData, &builder);
							if (temp != listName) // chk 
							{
								cout << temp;
							}
							else {
								user_type* ut = user_type::Find(&global, listName, &builder).second[0];
								if (ut->GetItemListSize() == 0 && ut->GetItemList(0).GetName().empty()) {
									cout << ut->GetItemList(0).Get(0);
								}
							}
						}
						else
						{
							auto x = user_type::Find(&global, listName, &builder);
							if (x.first) {
								user_type* ut = x.second[0];
								cout << ut->ToString();
							}
							else {
								cout << listName;
							}
						}
					}
					// ?
					else if (val->get_user_type_list_size_in_user_type() == 1
						&& val->get_user_type_list_in_user_type(0)->GetItemListSize() == 0
						&& val->get_user_type_list_in_user_type(0)->get_user_type_list_size_in_user_type() == 1)
					{
						string temp = val->get_user_type_list_in_user_type(0)->ToString();

						string name = ToBool4(NULL, global,
							temp, _excuteData, &builder);
						cout << name;
					}
					else
					{
						string start = val->get_user_type_list_in_user_type(1)->ToString();
						string last = val->get_user_type_list_in_user_type(2)->ToString();

						start = ToBool4(NULL, global, start, _excuteData, &builder);
						last = ToBool4(NULL, global, last, _excuteData, &builder);

						string listName = val->get_user_type_list_in_user_type(0)->GetItemList(0).Get(0);
						int _start = atoi(start.c_str());
						int _last = atoi(last.c_str());
						user_type* ut = user_type::Find(&global, listName, &builder).second[0];
						for (i = _start; i <= _last; ++i)
						{
							if (i != _start) { cout << " "; }
							cout << ut->GetItemList(i).Get(0);
						}
					}

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$print2" == val->GetName()) /// for print usertype.ToString();
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string dir = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					auto x = user_type::Find(&global, dir, &builder);

					for (auto& ut : x.second) {
						cout << ut->ToString();
						cout << endl;
					}

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$load" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// to do, load data and events from other file!
					for (i = 0; i < val->GetItemListSize(); ++i) {
						user_type ut;
						string fileName = ToBool4(NULL, global, val->GetItemList(i).Get(0), _excuteData, &builder);
						fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);

						if (wiz::load_data::LoadData::LoadDataFromFile(fileName, ut)) {
							{
								int item_count = 0;
								int userType_count = 0;

								for (i = 0; i < ut.GetIListSize(); ++i) {
									if (ut.IsItemList(i)) {
										global.AddItem(std::move(ut.GetItemList(item_count).GetName()),
											std::move(ut.GetItemList(item_count).Get(0)));
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

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;

				}
				else if ("$load_only_data" == val->GetName()) // $load2?
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// to do, load data and events from other file!
					user_type ut;
					string fileName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string dirName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);
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
									utTemp->AddItem(std::move(ut.GetItemList(item_count).GetName()),
										std::move(ut.GetItemList(item_count).Get(0)));
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

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;

				}
				else if ("$clear_screen" == val->GetName())
				{
					system("cls");
					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$_getch" == val->GetName())
				{
					_getch();
					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$input" == val->GetName())
				{
					string str;
					cin >> str;
					top_wiz_stack_event_info(&eventStack).return_value = str;
					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$return" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					if (size_wiz_stack_event_info(&eventStack) > 1)
					{
						string temp = ToBool4(NULL, global, val->ToString(), _excuteData, &builder);
						/// if temp just one
						eventStack[size_wiz_stack_event_info(&eventStack) - 2].return_value = temp;
					}

					if (size_wiz_stack_event_info(&eventStack) == 1)
					{
						string temp = ToBool4(NULL, global, val->ToString(), _excuteData, &builder);

						module_value = temp;
					}

					pop_wiz_stack_event_info(&eventStack);
					break;
				}
				else if ("$parameter" == val->GetName())
				{
					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$local" == val->GetName())
				{
					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				// make sort stable.
				else if ("$sort" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					vector<SortInfo> siVec;
					user_type* utTemp =
						user_type::Find(&global,
							ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder), &builder).second[0];

					vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
							temp.push_back(&(utTemp->GetItemList(item_count)));
							siVec.emplace_back(utTemp->GetItemList(item_count).GetName(), 1, i, &builder);
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


					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$sort2" == val->GetName()) { // colName -> just one! ? 
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					/// condition = has just one? in one usertype!
					vector<SortInfo> siVec;
					user_type* utTemp =
						user_type::Find(&global,
							ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder), &builder).second[0];
					const string colName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);

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


					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$sort2_dsc" == val->GetName()) { // colName -> just one! ? 
															/// condition = has just one? in one usertype!
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					vector<SortInfo2> siVec;
					user_type* utTemp =
						user_type::Find(&global,
							ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder), &builder).second[0];
					const string colName = ToBool4(NULL, global, val->get_user_type_list_in_user_type(1)->ToString(), _excuteData, &builder);

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


					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				// removal?
				else if ("$stable_sort" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// todo
					// todo
					vector<SortInfo> siVec;
					user_type* utTemp =
						user_type::Find(&global,
							ToBool4(NULL, global, val->get_user_type_list_in_user_type(0)->ToString(), _excuteData, &builder), &builder).second[0];

					vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
							temp.push_back(&(utTemp->GetItemList(item_count)));
							siVec.emplace_back(utTemp->GetItemList(item_count).GetName(), 1, i, &builder);
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


					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				}
				else if ("$if" == val->GetName()) // ToDo!!
				{
					ExcuteData _excuteData; _excuteData.valid = 1; _excuteData.depth = excuteData->depth;
					_excuteData.chkInfo = 1;
					_excuteData.info = top_wiz_stack_event_info(&eventStack);
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					string temp = val->get_user_type_list_in_user_type(0)->ToString();
					temp = ToBool4(NULL, global, temp, _excuteData, &builder);

					if (!top_wiz_stack_event_info(&eventStack).conditionStack.empty())
					{
						if ("TRUE" == temp && top_wiz_stack_event_info(&eventStack).conditionStack.top() == "FALSE")
						{
							temp = "FALSE";
						}
						else if ("FALSE" == temp && top_wiz_stack_event_info(&eventStack).conditionStack.top() == "FALSE")
						{
							temp = "FALSE";
						}
						else if (!top_wiz_stack_event_info(&eventStack).nowUT.empty() && top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) + 1 < top_wiz_stack_event_info(&eventStack).nowUT.top()->get_user_type_list_size_in_user_type()
							&& (top_wiz_stack_event_info(&eventStack).nowUT.top()->get_user_type_list_in_user_type(top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) + 1)->GetName() == "$else"))
						{
							top_wiz_stack_event_info(&eventStack).conditionStack.push(temp);
						}
						else if ("TRUE" == temp)
						{
							top_wiz_stack_event_info(&eventStack).conditionStack.push(temp);
						}
					}
					else
					{
						if (top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) + 1 < get_user_type_list_in_user_type(eventPtr, no)->get_user_type_list_size_in_user_type() &&
							get_user_type_list_in_user_type(eventPtr, no)->get_user_type_list_in_user_type(top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx) + 1)->GetName() == "$else")
						{
							top_wiz_stack_event_info(&eventStack).conditionStack.push(temp);
						}
						else if ("TRUE" == temp)
						{
							top_wiz_stack_event_info(&eventStack).conditionStack.push(temp);
						}
					}

					if ("TRUE" == temp)
					{
						top_wiz_stack_event_info(&eventStack).nowUT.push(val->get_user_type_list_in_user_type(1));
						//val = top_wiz_stack_event_info(&eventStack).nowUT.top()->get_user_type_list_in_user_type(0); 
						top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
						top_wiz_stack_event_info(&eventStack).userType_idx.push(0);
						//top_wiz_stack_event_info(&eventStack).state.push(1);
						//state = 1;
						break;
					}//
					else if ("FALSE" == temp)
					{
						top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
						break;
					}
					else
					{
						// debug..
						std::cout << "Error Debug : " << temp << endl;
						// free


						return "ERROR -3";
					}
				}
				else if ("$else" == val->GetName())
				{
					if (!top_wiz_stack_event_info(&eventStack).conditionStack.empty() && "FALSE" == top_wiz_stack_event_info(&eventStack).conditionStack.top())
					{
						top_wiz_stack_event_info(&eventStack).conditionStack.top() = "TRUE";
						top_wiz_stack_event_info(&eventStack).nowUT.push(val->get_user_type_list_in_user_type(0));
						//val = top_wiz_stack_event_info(&eventStack).nowUT.top()->get_user_type_list_in_user_type(0); // empty chk
						top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
						top_wiz_stack_event_info(&eventStack).userType_idx.push(0);
						//top_wiz_stack_event_info(&eventStack).state.push(2);
						//state = 2; //
						break;
					}
					else
					{
						top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
						break;
					}
				}
				else { //
					cout << "it does not work. : " << val->GetName() << endl;

					top_wiz_stack_size_t(&top_wiz_stack_event_info(&eventStack)->userType_idx)++;
					break;
				} */
			}
		}
	}

	// free
	{
		free_wiz_string(&EMPTY_STR);
		free_wiz_string(&ID_STR);
		free_wiz_string(&LOCAL_STR);
		free_wiz_string(&MAIN_STR);
		free_wiz_string(&CALL_STR);
		free_wiz_string(&IF_STR);
		free_wiz_string(&ELSE_STR);
		free_wiz_string(&EVENT_STR);
		free_wiz_string(&ROOT_STR);

		//
		free_wiz_map_wiz_string_and_user_type(&objectMap);
		//
		free_wiz_map_wiz_string_and_user_type(&moduleMap);

		free_wiz_string(&module_value);

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

		free_user_type_in_user_type(&global);
	}

	{
		user_type global;
		wiz_string str;

		init_wiz_string(&str, "test=3", 6);
		global = load_data_from_string_in_load_data(&str);

		free_wiz_string(&str);
		free_user_type_in_user_type(&global);
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
		wiz_string value = make_wiz_string("1", 1);
		init_wiz_string(&str, text, strlen(text));
		init_in_user_type(&global, &EMPTY);
			
		add_item_in_user_type(&global, &name, &value);

		init_wiz_string_builder(&builder, 1024, "", 0);

		excuteData.valid = 0;
		init_wiz_map_wiz_string_and_wiz_string(&excuteData.info.parameters);
		init_wiz_map_wiz_string_and_wiz_string(&excuteData.info.locals);

		result = ToBool4(NULL, &global, &str, &excuteData, &builder);

		free_wiz_string(&name);
		free_wiz_string(&value);
		free_wiz_string(&str);
		free_wiz_string(&result);
		free_user_type_in_user_type(&global);
		free_wiz_string_builder(&builder);
		free_wiz_string(&EMPTY);
		free_wiz_map_wiz_string_and_wiz_string(&excuteData.info.parameters);
		free_wiz_map_wiz_string_and_wiz_string(&excuteData.info.locals);
	}
}

int main(void)
{
	user_type global;
	wiz_string EMPTY_STR = make_empty_wiz_string();
	wiz_string fileName = make_wiz_string_from_cstr("test.txt");
	ExcuteData excuteData;
	wiz_string return_value;
	

	global = load_data_from_file_in_load_data(&fileName);

	return_value = excute_module(&EMPTY_STR, &global, NULL);

	free_wiz_string(&fileName);
	free_user_type_in_user_type(&global);
	free_wiz_string(&return_value);

	return 0;
}
