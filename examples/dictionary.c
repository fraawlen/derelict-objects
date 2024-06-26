/**
 * Copyright © 2024 Fraawlen <fraawlen@posteo.net>
 *
 * This file is part of the Cassette Objects (COBJ) library.
 *
 * This library is free software; you can redistribute it and/or modify it either under the terms of the GNU
 * Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the
 * License or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.
 * See the LGPL for the specific language governing rights and limitations.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program. If not,
 * see <http://www.gnu.org/licenses/>.
 */

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <cassette/cobj.h>

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/

static void _print_info (cobj_dictionary_t *dict, const char *comment);
static void _test_key   (cobj_dictionary_t *dict, const char *key, uint8_t group);

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/

int
main(void)
{
	cobj_dictionary_t *dict;

	/* init */

	dict = cobj_dictionary_create(0, 0.6);
	
	/* operations */

	cobj_dictionary_write(dict, "key_A", 0, 10);
	cobj_dictionary_write(dict, "key_B", 0, 34);
	cobj_dictionary_write(dict, "key_C", 0, 86);
	cobj_dictionary_write(dict, "key_D", 0, 54);
	cobj_dictionary_write(dict, "key_D", 1, 72);
	cobj_dictionary_write(dict, "key_D", 2, 99);
	_print_info(dict, "dictionary initialised with 6 initial values and a max load factor of 0.6");

	_test_key(dict, "key_A", 0);
	_test_key(dict, "key_B", 0);
	_test_key(dict, "key_C", 0);
	_test_key(dict, "key_D", 0);
	_test_key(dict, "key_D", 1);
	_test_key(dict, "key_D", 2);

	_test_key(dict, "key_A", 7);
	_test_key(dict, "key_C", 1);
	_test_key(dict, "sdfbb", 0);
	_test_key(dict,  NULL,   0);

	cobj_dictionary_write(dict, "key_D", 2, 9999);
	_print_info(dict, "overwrote value of \"key_D\" in group 2");

	_test_key(dict, "key_D", 2);

	cobj_dictionary_erase(dict, "key_C", 0);
	cobj_dictionary_erase(dict, "key_C", 1);
	_print_info(dict, "erased \"key_C\" in groups 0 & 1");

	_test_key(dict, "key_C", 0);
	_test_key(dict, "key_C", 1);
	
	cobj_dictionary_clear_group(dict, 0);
	_print_info(dict, "cleared all keys from group 0");

	_test_key(dict, "key_B", 0);

	cobj_dictionary_clear(dict);
	_print_info(dict, "dictionary fully cleared");

	/* end */

	if (cobj_dictionary_has_failed(dict))
	{
		printf("Dictionary has failed during operation.\n");
	}

	cobj_dictionary_destroy(&dict);
	cobj_dictionary_destroy(&dict); /* api is safe against double destructions */

	return 0;
}

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/

static void
_print_info(cobj_dictionary_t *dict, const char *comment)
{
	printf("\n\t-> %zu / %zu = %f (%s)\n\n",
		cobj_dictionary_get_load(dict),
		cobj_dictionary_get_alloc_size(dict),
		cobj_dictionary_get_load_factor(dict),
		comment);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void
_test_key(cobj_dictionary_t *dict, const char *key, uint8_t group)
{
	size_t tmp;

//	printf("test\n");

	if (cobj_dictionary_find(dict, key, group, &tmp))
	{
		printf("key \"%s\" in group %u was found and has a value of %zu\n", key, group, tmp);
	}
	else
	{
		printf("key \"%s\" in group %u was not found\n", key ? key : "NULL", group);
	}
}
