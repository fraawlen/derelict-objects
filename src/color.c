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

#include <cassette/cobj.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/
 
static void         _bind_color    (cobj_color_t *color);
static void         _bind_d        (double *d);
static uint8_t      _hex_to_int    (char c);
static cobj_color_t _convert_hex   (const char *str, bool *err);
static cobj_color_t _convert_ulong (const char *str, bool *err);

/************************************************************************************************************/
/* PUBLIC ***************************************************************************************************/
/************************************************************************************************************/

cobj_color_t
cobj_color_convert_argb_uint(uint32_t argb)
{
	return cobj_color_convert_rgba(
		(argb >> 16) & 0xFF,
		(argb >>  8) & 0xFF,
		(argb >>  0) & 0xFF,
		(argb >> 24) & 0xFF);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

cobj_color_t
cobj_color_convert_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	cobj_color_t color;

	color.a = a / 255.0;
	color.r = r / 255.0;
	color.g = g / 255.0;
	color.b = b / 255.0;

	return color;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

cobj_color_t
cobj_color_convert_str(const char *str, bool *err)
{
	cobj_color_t color;

	bool fail = false;

	if (!str)
	{
		fail = true;
		color = COBJ_COLOR_TRANSPARENT;
	}
	else if (str[0] == '#')
	{
		color = _convert_hex(str + 1, &fail);
	}
	else
	{
		color = _convert_ulong(str, &fail);
	}
	
	if (err)
	{
		*err = fail;
	}

	return color;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

uint32_t
cobj_color_get_argb_uint(cobj_color_t color)
{
	uint32_t a;
	uint32_t r;
	uint32_t g;
	uint32_t b;

	_bind_color(&color);

	a = color.a * 255;
	r = color.r * 255;
	g = color.g * 255;
	b = color.b * 255;

	return (a << 24) + (r << 16) + (g << 8) + b;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

cobj_color_t
cobj_color_interpolate(cobj_color_t color_1, cobj_color_t color_2, double ratio)
{
	cobj_color_t color;

	_bind_color(&color_1);
	_bind_color(&color_2);
	_bind_d(&ratio);

	color.r = color_2.r * ratio + color_1.r * (1.0 - ratio);
	color.g = color_2.g * ratio + color_1.g * (1.0 - ratio);
	color.b = color_2.b * ratio + color_1.b * (1.0 - ratio);
	color.a = color_2.a * ratio + color_1.a * (1.0 - ratio);

	return color;
}

/************************************************************************************************************/
/* _ ********************************************************************************************************/
/************************************************************************************************************/

static void
_bind_color(cobj_color_t *color)
{
	_bind_d(&color->r);
	_bind_d(&color->g);
	_bind_d(&color->b);
	_bind_d(&color->a);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void
_bind_d(double *d)
{
	if (*d > 1.0)
	{
		*d = 1.0;
	}
	else if (*d < 0.0)
	{
		*d = 0.0;
	}
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

cobj_color_t
_convert_hex(const char *str, bool *err)
{
	uint8_t v[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xF, 0xF};
	size_t  i;

	for (i = 0; i < 8 && str[i] != '\0'; i++)
	{
		v[i] = _hex_to_int(str[i]);
		if (v[i] == UINT8_MAX)
		{
			*err = true;
		}
	}

	if ((i != 6 && i != 8) || str[i] != '\0')
	{
		*err = true;
	}

	/* apply conversion */

	return cobj_color_convert_rgba(
		(v[0] << 4) + v[1],
		(v[2] << 4) + v[3],
		(v[4] << 4) + v[5],
		(v[6] << 4) + v[7]);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

cobj_color_t
_convert_ulong(const char *str, bool *err)
{
	char *endptr = NULL;
	uint32_t u = 0;

	u = strtoul(str, &endptr, 0);
	if (endptr == str)
	{
		*err = true;
	}

	return cobj_color_convert_argb_uint(u);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static uint8_t
_hex_to_int(char c)
{
	switch (c)
	{
		case '0':
			return 0x0;

		case '1':
			return 0x1;

		case '2':
			return 0x2;

		case '3':
			return 0x3;

		case '4':
			return 0x4;

		case '5':
			return 0x5;

		case '6':
			return 0x6;

		case '7':
			return 0x7;

		case '8':
			return 0x8;

		case '9':
			return 0x9;
		
		case 'a':
		case 'A':
			return 0xA;

		case 'b':
		case 'B':
			return 0xB;

		case 'c':
		case 'C':
			return 0xC;

		case 'd':
		case 'D':
			return 0xD;

		case 'e':
		case 'E':
			return 0xE;

		case 'f':
		case 'F':
			return 0xF;

		default:
			return UINT8_MAX;
	}
}

