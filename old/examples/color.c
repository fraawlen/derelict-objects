#include <stdio.h>
#include <stdlib.h>

#include "du.h"

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/

static void _print_cl(const char *str);

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/

int
main(void)
{
	_print_cl("#ffffff");
	_print_cl("00000080");
	_print_cl("#aaa");
	_print_cl("#A0A0WA");
	_print_cl("000000");
	_print_cl("A0043012");

	return 0;
}

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/

static void
_print_cl(const char *str)
{
	bool err;
	du_color_t cl = du_color_from_str(str, &err);

	if (!err) {
		printf("color (#%08x) : r = %f,\tg = %f,\tb = %f,\ta = %f\n", du_color_to_argb_uint(cl), cl.r, cl.g, cl.b, cl.a);
	} else {
		printf("failed to convert color \'%s\'\n", str);
	}
}
