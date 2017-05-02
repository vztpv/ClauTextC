
#define _CRT_SECURE_NO_WARNINGS

#include "wiz_load_data.h"
#include "wiz_array_queue_token.h"


size_t find(const char* cstr, const char x, const size_t before, const size_t n)
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
int is_whitespace(const char ch)
{
	return ' ' == ch || '\t' == ch || '\r' == ch || '\n' == ch;
}

// to do
int is_state0(const long long state_reserve)
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

////////////////////////////public//////////////////////////////////

// todo - error chk!
user_type load_data_from_file_in_load_data(wiz_string* fileName)
{
	user_type global;
	wiz_array_queue_token aq;
	// getline remake?
	const size_t line_max = 10000; // line_max = 1 is very slow? - solved!
	const size_t num = 102400;
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
			real_count = fread((void*)(str_wiz_string_builder(&temp_builder, NULL)), sizeof(char), num, file);
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
			//StringTokenizer tokenizer(std::( (*aqTemp)[x] ) );
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

user_type load_data_from_string_in_load_data(wiz_string* str)
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
