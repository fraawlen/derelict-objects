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

#ifndef DU_INPUTS_H
#define DU_INPUTS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "du-status.h"
#include "du-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/

/**
 * Input list slot.
 *
 * @param id     : input's identifier (touch id, mouse button, keycode, ...)
 * @param ref    : arbitrary reference to link the input to
 * @param coords : input coordinates
 */
typedef struct {
	uint32_t id;
	void *ref;
	du_coordinates_t coords;
} du_inputs_slot_t;

/**
 * Specialized structure to keep track of active end-user inputs, such as, but not limited to, screen touches,
 * button and key presses. Hence the coordinate associated to each tracked input. n <= n_alloc. Unlike
 * du_tracker_t, its array is not auto-extensible, instead, if the maximum amount of inputs is reached, the
 * following inputs get ignored.
 * If status is not set to DU_STATUS_SUCCESS all handler functions will have no effect with the exception of
 * du_inputs_reset() and du_inputs_init().
 *
 * @param slots   : slot array
 * @param n       : number of occupied slots
 * @param n_alloc : total number of allocated slots
 * @param status  : error state
 */
typedef struct {
	du_inputs_slot_t *slots;
	size_t n;
	size_t n_alloc;
	du_status_t status;
} du_inputs_t;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/**
 * Pre-allocate memory to the input tracker and set its variables appropriately. Allocated memory in the
 * array is initialised to 0. If n = 0, no memory is pre-allocated and but the structure will still be
 * considered to have been initialised. In case of error, tracker->status will be set to DU_STATUS_FAILURE.
 * It's set to DU_STATUS_SUCCESS otherwhise.
 *
 * @param inputs  : input list to init
 * @param n_alloc : initial size of the pointer array to pre-allocate.
 */
void du_inputs_init(du_inputs_t *inputs, size_t n_alloc);

/**
 * Allocated memory is freed and the structure will be put in an unitialised state with inputs->status set to
 * DU_STATUS_NOT_INIT. The given structure itself is not freed, and may require an explicit free operation.
 *
 * @param inputs : input list to reset
 */
void du_inputs_reset(du_inputs_t *inputs);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/**
 * Removes all tracked values. Internal memory however is not freed, use du_inputs_reset() for that.
 * The given structure needs to be initialised beforehand.
 *
 * @param inputs : input list to clear
 */
void du_inputs_clear(du_inputs_t *inputs);

/**
 * Removes a tracked input of a given id.If the given id was not already tracked then nothing happens.
 * The given structure needs to be initialised beforehand.
 *
 * @param inputs : input list to pull from
 * @param id     : input id to match
 */
void du_inputs_pull_id(du_inputs_t *inputs, uint32_t id);

/**
 * Removes a tracked input at the given index.If the given index was out of bounds (index >= n) then nothing
 * happens.
 * The given structure needs to be initialised beforehand.
 *
 * @param inputs : input list to pull from
 * @param index  : position of the input to pull
 */
void du_inputs_pull_index(du_inputs_t *inputs, size_t index);

/**
 * Adds a new input to the end of the array. If the input's id is already present in the input list it's
 * data will just be updated. If the maximum amount of inputs is reached, this a function won't have an
 * effect until one or more inputs get untracked.
 * The given structure needs to be initialised beforehand.
 *
 * @param inputs : input list to push to
 * @param id     : input id to match
 * @param ref    : arbitrary pointer reference of the input
 * @param x      : coordinate x
 * @param y      : coordinate y
 */
void du_inputs_push(du_inputs_t *inputs, uint32_t id, void *ref, du_position_t x, du_position_t y);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/**
 * Locates a given input id if it is present in the array.
 * The given structure needs to be initialised beforehand.
 *
 * @param inputs : input list to search
 * @param id     : input id to match
 * @param index  : optional, if non NULL, the given pointer's value will be set to the position index of the
 *                 found input. If the input is not found this paramater is not modified.
 */
bool du_inputs_find(const du_inputs_t *inputs, uint32_t id, size_t *index);

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* DU_INPUTS_H */
