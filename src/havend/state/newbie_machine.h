#ifndef __HAVEN_HAVEND_NEWBIE_MACHINE_H
#define __HAVEN_HAVEND_NEWBIE_MACHINE_H

#include "actions.h"

typedef struct HAVEN_newbie_machine_t {
    int cs;
    void* data;
    HAVEN_state_actions_t* actions;
} HAVEN_newbie_machine_t;

#endif /* __HAVEN_HAVEND_NEWBIE_MACHINE_H */
