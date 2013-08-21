#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timex.h>
#include <time.h>

#include "common.h"
#include "log.h"
#include "xtime.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

int HVN_time_now(HVN_time_interval_t* interval)
{
    struct ntptimeval tv;
    struct timeval delta;

    struct timeval* earliest_p = &(interval->earliest);
    struct timeval* latest_p = &(interval->latest);
    struct timeval* now_p = &(tv.time);
    struct timeval* delta_p = &delta;

    timerclear(&delta);
    timerclear(&interval->earliest);
    timerclear(&interval->latest);

    if(ntp_gettime(&tv) == 0) {
        tv.maxerror = tv.maxerror > 0 ? tv.maxerror : -(tv.maxerror);

        delta.tv_sec = delta.tv_sec + (tv.maxerror / 1000);
        delta.tv_usec = delta.tv_usec + ((tv.maxerror % 1000) * 1000);

        if(delta.tv_usec > 1000000) {
            delta.tv_usec -= 1000000;
            delta.tv_sec++;
        }

        timeradd(now_p, delta_p, latest_p);
        timersub(now_p, delta_p, earliest_p);
    } else {
        LOG(HVN_LOG_ERR, "Error on ntp_gettime. %s", strerror(errno));
        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

int HVN_time_before(HVN_time_interval_t* interval, bool* before)
{
    return HVN_time_check(interval, before, HVN_TIME_CHECK_BEFORE);
}

int HVN_time_after(HVN_time_interval_t* interval, bool* after)
{
    return HVN_time_check(interval, after, HVN_TIME_CHECK_AFTER);
}

int HVN_time_check(HVN_time_interval_t* interval, bool* success, int mode)
{
    struct timeval* latest_p;
    struct timeval* earliest_p;
    HVN_time_interval_t now;

    if(HVN_time_now(&now) != HVN_SUCCESS) {
        return HVN_ERROR;
    }

    if(mode == HVN_TIME_CHECK_AFTER) {
        earliest_p = &(interval->latest);
        latest_p = &(now.earliest);
    } else if(mode == HVN_TIME_CHECK_BEFORE) {
        latest_p = &(interval->latest);
        earliest_p = &(now.earliest);
    } else {
        return HVN_ERROR;
    }

    if(timercmp(latest_p, earliest_p, <) != 0) {
        *success = true;
        return HVN_SUCCESS;
    } else {
        *success = false;
        return HVN_SUCCESS;
    }

    return HVN_ERROR;
}

void HVN_time_interval_print(HVN_time_interval_t* interval)
{
    char fmt_earliest[64], buf_earliest[64];
    char fmt_latest[64], buf_latest[64];

    time_t* earliest_sec_p = &(interval->earliest.tv_sec);
    time_t* latest_sec_p = &(interval->latest.tv_sec);

    struct tm* tm_earliest = localtime(earliest_sec_p);
    struct tm* tm_latest = localtime(latest_sec_p);

    strftime(fmt_earliest, sizeof(fmt_earliest), "%Y-%m-%d %H:%M:%S.%%06u %z", tm_earliest);
    snprintf(buf_earliest, sizeof(buf_earliest), fmt_earliest, interval->earliest.tv_usec);

    strftime(fmt_latest, sizeof(fmt_latest), "%Y-%m-%d %H:%M:%S.%%06u %z", tm_latest);
    snprintf(buf_latest, sizeof(buf_latest), fmt_latest, interval->latest.tv_usec);

    LOG(HVN_LOG_INFO, "HVN_time_interval:['%s', '%s']", buf_earliest, buf_latest); 
}

int HVN_time_interval_validate(HVN_time_interval_t* interval)
{
    struct timeval* earliest_p = &(interval->earliest);
    struct timeval* latest_p = &(interval->latest);

    if(timercmp(earliest_p, latest_p, <) != 0) {
        LOG(HVN_LOG_INFO, "PASSED: Basic time interval validation.");
        return HVN_SUCCESS;
    }

    LOG(HVN_LOG_ERR, "FAILED: Basic time interval validation.");
    return HVN_ERROR;
}

/* EOF */
