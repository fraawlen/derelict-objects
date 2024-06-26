/**
 * Copyright © 2024 Fraawlen <fraawlen@posteo.net>
 *
 * This file is part of the Derelict Utilities (DU) library.
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

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "du.h"

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/

static bool _extend (du_tracker_t *tracker);

/************************************************************************************************************/
/* PUBLIC ***************************************************************************************************/
/************************************************************************************************************/

void
du_tracker_clear(du_tracker_t *tracker)
{
	assert(tracker);
	du_status_test(tracker->status, return);

	free(tracker->ptr);
	tracker->ptr = NULL;
	tracker->n = 0;
	tracker->n_alloc = 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

bool
du_tracker_find(const du_tracker_t *tracker, const void *ptr, size_t *index)
{
	assert(tracker);
	du_status_test(tracker->status, return false);

	const size_t i0 = index && *index < tracker->n ? *index : tracker->n - 1;
	size_t i;

	/* first scan from i0 to 0 */

	for (i = i0; i < SIZE_MAX; i--) {
		if (tracker->ptr[i] == ptr) {
			goto found;
		}
	}

	/* second scan, from i0 + 1 to max */

	for (i = i0 + 1; i < tracker->n; i++) {
		if (tracker->ptr[i] == ptr) {
			goto found;
		}
	}

	/*****/

	return false;

found:

	if (index) {
		*index = i;
	}
	
	return true;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void
du_tracker_init(du_tracker_t *tracker, size_t n_alloc)
{
	assert(tracker);

	tracker->n = 0;
	tracker->n_alloc = n_alloc;
	tracker->ptr = n_alloc > 0 ? malloc(n_alloc * sizeof(void*)) : NULL;
	tracker->status = n_alloc == 0 || tracker->ptr ? DU_STATUS_SUCCESS : DU_STATUS_FAILURE;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void
du_tracker_pull_index(du_tracker_t *tracker, size_t index)
{
	assert(tracker);
	du_status_test(tracker->status, return);

	if (index < tracker->n) {
		tracker->n--;
	}

	for (; index < tracker->n; index++) {
		tracker->ptr[index] = tracker->ptr[index + 1];
	}
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void
du_tracker_pull_pointer(du_tracker_t *tracker, const void *ptr)
{
	assert(tracker);
	du_status_test(tracker->status, return);

	size_t i = 0;

	if (du_tracker_find(tracker, ptr, &i)) {
		du_tracker_pull_index(tracker, i);
	}
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void
du_tracker_push(du_tracker_t *tracker, const void *ptr, size_t *index)
{
	assert(tracker);
	du_status_test(tracker->status, return);

	if (du_tracker_find(tracker, ptr, index)) {
		return;
	}

	if (tracker->n >= tracker->n_alloc && !_extend(tracker)) {
		return;
	}

	if (index) {
		*index = tracker->n;
	}

	tracker->ptr[tracker->n] = ptr;
	tracker->n++;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void
du_tracker_reset(du_tracker_t *tracker)
{
	assert(tracker);

	free(tracker->ptr);
	tracker->status = DU_STATUS_NOT_INIT;
}

/************************************************************************************************************/
/* _ ********************************************************************************************************/
/************************************************************************************************************/

static bool
_extend(du_tracker_t *tracker)
{
	tracker->n_alloc = tracker->n_alloc > 0 ? tracker->n_alloc * 2 : 1;

	void *tmp = realloc(tracker->ptr, tracker->n_alloc * sizeof(void*));
	du_status_assert(tracker->status, tmp, return false);
	tracker->ptr = tmp;

	return true;
}
