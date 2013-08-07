
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

#include "server.h"
#include "state.h"


#line 70 "consensus.rl"



#line 28 "consensus_state.c"
static const int Consensus_start = 1;
static const int Consensus_first_final = 8;
static const int Consensus_error = 0;

static const int Consensus_en_Consensus = 1;


#line 73 "consensus.rl"

int HAVEN_init_consensus_state(HAVEN_state_t *state, HAVEN_state_actions_t *actions)
{
    int cs;
    
#line 42 "consensus_state.c"
	{
	 state->cs = Consensus_start;
	}

#line 78 "consensus.rl"
}

int HAVEN_exec_consensus_state(HAVEN_state_t *state, HAVEN_server_t* server)
{
    int *p = NULL;
    int *pe = NULL;
    int *eof = NULL;
    int cs = state->cs;

    
#line 58 "consensus_state.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch (  state->cs )
	{
case 1:
	switch( (*p) ) {
		case 7: goto st2;
		case 8: goto tr2;
		case 9: goto st7;
	}
	goto st0;
tr4:
#line 31 "consensus.rl"
	{
        HAVEN_state_error();
    }
	goto st0;
#line 77 "consensus_state.c"
st0:
 state->cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 10 )
		goto st3;
	goto tr4;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 4 )
		goto st4;
	goto tr4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 0: goto st4;
		case 1: goto st5;
	}
	goto tr4;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 2: goto st5;
		case 3: goto st3;
		case 4: goto st4;
	}
	goto tr4;
tr2:
#line 27 "consensus.rl"
	{
        HAVEN_create_location_quorum(server);
    }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 124 "consensus_state.c"
	if ( (*p) == 5 )
		goto st3;
	goto tr4;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 6 )
		goto st4;
	goto tr4;
	}
	_test_eof2:  state->cs = 2; goto _test_eof; 
	_test_eof3:  state->cs = 3; goto _test_eof; 
	_test_eof4:  state->cs = 4; goto _test_eof; 
	_test_eof5:  state->cs = 5; goto _test_eof; 
	_test_eof6:  state->cs = 6; goto _test_eof; 
	_test_eof7:  state->cs = 7; goto _test_eof; 

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
#line 31 "consensus.rl"
	{
        HAVEN_state_error();
    }
	break;
#line 158 "consensus_state.c"
	}
	}

	_out: {}
	}

#line 88 "consensus.rl"
}

/* EOF */
