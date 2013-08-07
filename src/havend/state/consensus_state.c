
#line 1 "consensus.rl"

#include "actions.h"


#line 54 "consensus.rl"



#line 12 "consensus_state.c"
static const int State_start = 1;
static const int State_first_final = 8;
static const int State_error = 0;

static const int State_en_Consensus = 1;


#line 57 "consensus.rl"

int HAVEN_init_consensus_state(HAVEN_state_t *state, HAVEN_state_actions_t *actions)
{
    int cs;
    
#line 26 "consensus_state.c"
	{
	 machine->cs = State_start;
	}

#line 62 "consensus.rl"
}

int HAVEN_exec_consensus_state(HAVEN_state_t *state, HAVEN_server_t* server)
{
    int *p = NULL;
    int *pe = NULL;
    int *eof = NULL;
    int cs = machine->cs;

    
#line 42 "consensus_state.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch (  machine->cs )
	{
case 1:
	switch( (*p) ) {
		case 7: goto st2;
		case 8: goto tr2;
		case 9: goto st7;
	}
	goto st0;
tr4:
#line 15 "consensus.rl"
	{
        HAVEN_consensus_error_cb();
    }
	goto st0;
#line 61 "consensus_state.c"
st0:
 machine->cs = 0;
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
#line 11 "consensus.rl"
	{
        HAVEN_create_location_quorum(server);
    }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 108 "consensus_state.c"
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
	_test_eof2:  machine->cs = 2; goto _test_eof; 
	_test_eof3:  machine->cs = 3; goto _test_eof; 
	_test_eof4:  machine->cs = 4; goto _test_eof; 
	_test_eof5:  machine->cs = 5; goto _test_eof; 
	_test_eof6:  machine->cs = 6; goto _test_eof; 
	_test_eof7:  machine->cs = 7; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch (  machine->cs ) {
	case 2: 
	case 3: 
	case 4: 
	case 5: 
	case 6: 
	case 7: 
#line 15 "consensus.rl"
	{
        HAVEN_consensus_error_cb();
    }
	break;
#line 142 "consensus_state.c"
	}
	}

	_out: {}
	}

#line 72 "consensus.rl"
}

/* EOF */
