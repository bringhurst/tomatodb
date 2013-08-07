
#line 1 "consensus.rl"
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


#line 73 "consensus.rl"


#include "consensus_state.h"


#line 27 "consensus_state.c"
static const int Consensus_start = 1;
static const int Consensus_first_final = 10;
static const int Consensus_error = 0;

static const int Consensus_en_Consensus = 1;


#line 78 "consensus.rl"

int HAVEN_init_consensus_state(HAVEN_state_t *state, HAVEN_state_actions_t *actions)
{
    int cs;

    
#line 42 "consensus_state.c"
	{
	 state->cs = Consensus_start;
	}

#line 84 "consensus.rl"
}

int HAVEN_exec_consensus_state(HAVEN_state_t *state, int event, HAVEN_server_t* server)
{
    int event_queue[2] = {0};
    event_queue[0] = event;
    int next = 0;

    const int *p = event_queue;
    const int *pe = p+1;
    const int *eof = event == GONE ? pe : NULL;

    
#line 61 "consensus_state.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch (  state->cs )
	{
case 1:
	switch( (*p) ) {
		case 0: goto tr0;
		case 1: goto tr2;
		case 2: goto st9;
	}
	goto st0;
tr4:
#line 29 "consensus.rl"
	{ HAVEN_state_error(); }
	goto st0;
#line 78 "consensus_state.c"
st0:
 state->cs = 0;
	goto _out;
tr0:
#line 24 "consensus.rl"
	{ TRANSITION(HAVEN_consensus_create_quorum_as_leader); }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 90 "consensus_state.c"
	if ( (*p) == 4 )
		goto st3;
	goto tr4;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 11 )
		goto st4;
	goto tr4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 6: goto st5;
		case 7: goto st4;
		case 8: goto tr8;
	}
	goto tr4;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 9: goto st5;
		case 10: goto st3;
		case 11: goto st4;
	}
	goto tr4;
tr8:
#line 26 "consensus.rl"
	{ TRANSITION(HAVEN_consensus_leave_quorum); }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 129 "consensus_state.c"
	if ( (*p) == 12 )
		goto tr9;
	goto tr4;
tr9:
#line 27 "consensus.rl"
	{ TRANSITION(HAVEN_consensus_shutdown_server); }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 141 "consensus_state.c"
	goto tr4;
tr2:
#line 25 "consensus.rl"
	{ TRANSITION(HAVEN_consensus_create_location_quorum); }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 151 "consensus_state.c"
	if ( (*p) == 3 )
		goto st3;
	goto tr4;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 5 )
		goto st4;
	goto tr4;
	}
	_test_eof2:  state->cs = 2; goto _test_eof; 
	_test_eof3:  state->cs = 3; goto _test_eof; 
	_test_eof4:  state->cs = 4; goto _test_eof; 
	_test_eof5:  state->cs = 5; goto _test_eof; 
	_test_eof6:  state->cs = 6; goto _test_eof; 
	_test_eof7:  state->cs = 7; goto _test_eof; 
	_test_eof8:  state->cs = 8; goto _test_eof; 
	_test_eof9:  state->cs = 9; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch (  state->cs ) {
	case 2: 
	case 3: 
	case 4: 
	case 5: 
	case 6: 
	case 7: 
	case 8: 
	case 9: 
#line 29 "consensus.rl"
	{ HAVEN_state_error(); }
	break;
#line 187 "consensus_state.c"
	}
	}

	_out: {}
	}

#line 97 "consensus.rl"

    return next;
}

/* EOF */
