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

#include "settings.h"
#include "log.h"
#include "common.h"

#include <unistd.h>
#include <uuid/uuid.h>
#include <stdlib.h>
#include <string.h>

/** The debug stream to write log messages to. */
extern FILE* HAVEN_debug_stream;

/** The log level to write messages for. */
extern HAVEN_loglevel HAVEN_debug_level;

int HAVEN_get_local_machine_uuid(HAVEN_ctx_t* ctx)
{
    int result;
    char* uuid_file_path = (char*) malloc(sizeof(char) * _POSIX_PATH_MAX);

    sprintf(uuid_file_path, "%s%s%s", \
            HAVEN_BASE_STATE_DIR, HAVEN_SETTINGS_DB_PREFIX, \
            LOCAL_UUID_SETTINGS_PATH);

    if(access(uuid_file_path, F_OK) != -1) {
        result = HAVEN_get_uuid_from_file(ctx, uuid_file_path);
    }
    else {
        result = HAVEN_configure_new_uuid(ctx);
    }

    free(uuid_file_path);
    return result;
}

int HAVEN_get_uuid_from_file(HAVEN_ctx_t* ctx, char* uuid_file_path)
{
    size_t bytes_read = 0;
    char* buf = (char*) malloc(sizeof(char) * UUID_STR_LEN);
    FILE* fh = fopen(uuid_file_path, "rd");

    if(fh == NULL) {
        LOG(HAVEN_LOG_ERR, "Could not open uuid file at `%s'.", uuid_file_path);
        return HAVEN_ERROR;
    }

    do {
        bytes_read = fread(buf + bytes_read, 1, sizeof(char) * UUID_STR_LEN, fh);
    }
    while(bytes_read < UUID_STR_LEN || feof(fh));

    if(ferror(fh) != 0) {
        LOG(HAVEN_LOG_ERR, "An error occured while reading the uuid data file.");
        return HAVEN_ERROR;
    }

    if(uuid_parse(buf, ctx->local_uuid) != 0) {
        LOG(HAVEN_LOG_ERR, "An error occured while parsing the uuid data file.");
        return HAVEN_ERROR;
    }

    fclose(fh);
    free(buf);
    return HAVEN_SUCCESS;
}

int HAVEN_configure_new_uuid(HAVEN_ctx_t* ctx)
{
    int result = uuid_generate_time_safe(ctx->local_uuid);
    char* uuid_string = (char*) malloc(sizeof(char) * UUID_STR_LEN);

    // TODO: store the generated uuid in a file and read if it exists.

    if(result != 0)  {
        LOG(HAVEN_LOG_ERR, "Could not safely generate a new UUID. " \
            "Please ensure that uuidd(8) is installed and working.");
        return HAVEN_ERROR;
    }

    uuid_unparse(ctx->local_uuid, uuid_string);
    printf("generated uuid is `%s'.\n", uuid_string);

    free(uuid_string);
    return HAVEN_SUCCESS;
}

int HAVEN_prepare_settings_db(HAVEN_ctx_t* ctx)
{
    char* settings_db_path = (char*) malloc(sizeof(char) * _POSIX_PATH_MAX);
    int offset;
    int result;

    offset = sprintf(settings_db_path, "%s%s", \
                     HAVEN_BASE_STATE_DIR, HAVEN_SETTINGS_DB_PREFIX);

    if(HAVEN_ensure_directory_exists(settings_db_path) != HAVEN_SUCCESS) {
        LOG(HAVEN_LOG_ERR, "Failed to create directory structure when preparing the settings DB.");
        return HAVEN_ERROR;
    }

    sprintf(settings_db_path + offset, "/%s", ctx->local_uuid);
    result = HAVEN_init_db(&ctx->settings_db, settings_db_path);

    free(settings_db_path);
    return result;
}

/* EOF */
