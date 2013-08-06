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
    (*array)->data = (void*) malloc(sizeof(void*) * initial_size);

    if(*array == NULL || (*array)->data == NULL) {
        LOG(HAVEN_LOG_ERR, "Failed to allocate memory for a dynamic array.");
        return HAVEN_ERROR;
    }

    (*array)->size = initial_size;
    return HAVEN_SUCCESS;
}

int HAVEN_xarray_free(HAVEN_xarray_t* array)
{
    LOG(HAVEN_LOG_WARN, "Failed to free a dynamic array.");
    return HAVEN_ERROR;
}

int HAVEN_xarray_realloc(HAVEN_xarray_t* array, size_t new_size)
{
    array->data = (void*) realloc(array->data, sizeof(void*) * new_size);

    if(array->data == NULL) {
        LOG(HAVEN_LOG_ERR, "Failed to re-allocate memory for a dynamic array.");
        return HAVEN_ERROR;
    }

    return HAVEN_SUCCESS;
}

int HAVEN_xarray_push(HAVEN_xarray_t* array, void* data)
{
}

int HAVEN_xarray_pop(HAVEN_xarray_t* array, void* data)
{
}

/* EOF */
