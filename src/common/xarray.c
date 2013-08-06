/*
 * Copyright 2013 Los Alamos National Security, LLC.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "xarray.h"
#include "common.h"
#include "log.h"

#include <stdlib.h>

/** The debug stream to write log messages to. */
extern FILE* HAVEN_debug_stream;

/** The log level to write messages for. */
extern HAVEN_loglevel HAVEN_debug_level;

int HAVEN_xarray_init(HAVEN_xarray_t** array, size_t initial_size)
{
    *array = (HAVEN_xarray_t*) malloc(sizeof(HAVEN_xarray_t));
    (*array)->data = (intptr_t**) malloc(sizeof(intptr_t*) * initial_size);

    if(*array == NULL || (*array)->data == NULL) {
        LOG(HAVEN_LOG_ERR, "Failed to allocate memory for a dynamic array.");
        return HAVEN_ERROR;
    }

    (*array)->size = initial_size;
    (*array)->index = 0;

    return HAVEN_SUCCESS;
}

int HAVEN_xarray_free(HAVEN_xarray_t* array)
{
    //FIXME
    free(array);
    return HAVEN_SUCCESS;
}

int HAVEN_xarray_realloc(HAVEN_xarray_t* array, size_t new_size)
{
    array->data = (void*) realloc(array->data, sizeof(intptr_t*) * new_size);

    if(array->data == NULL) {
        LOG(HAVEN_LOG_ERR, "Failed to re-allocate memory for a dynamic array.");
        return HAVEN_ERROR;
    }

    array->size = new_size;
    return HAVEN_SUCCESS;
}

int HAVEN_xarray_push(HAVEN_xarray_t* array, void* data)
{
    if(array->size < sizeof(intptr_t*) * array->index + 1) {
        if(HAVEN_xarray_realloc(array, sizeof(intptr_t*) * array->size * 2) != HAVEN_SUCCESS) {
            LOG(HAVEN_LOG_ERR, "Failed to re-allocate memory for a dynamic array while pushing.");
            return HAVEN_ERROR;
        }
    }

    LOG(HAVEN_LOG_INFO, "Pushing to index `%d'.", array->index);

    array->data[array->index] = data;
    array->index++;

    return HAVEN_SUCCESS;
}

void* HAVEN_xarray_peek(HAVEN_xarray_t* array) {
    return array->data[array->index - 1];
}

int HAVEN_xarray_pop(HAVEN_xarray_t* array, void** data)
{
    if(array->index * 2 + 1 < array->size) {
        if(HAVEN_xarray_realloc(array, array->size / 2) != HAVEN_SUCCESS) {
            LOG(HAVEN_LOG_ERR, "Failed to re-allocate memory for a dynamic array while poping.");
            return HAVEN_ERROR;
        }
    }

    LOG(HAVEN_LOG_INFO, "Poping from index `%d'.", array->index - 1);

    *data = array->data[array->index - 1];
    array->index--;

    return HAVEN_SUCCESS;
}

/* EOF */
