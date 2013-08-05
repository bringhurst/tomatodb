
#line 1 "newbie.rl"
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

#include "newbie_machine.h"


#line 74 "newbie.rl"



#line 27 "newbie_machine.c"
static const int Newbie_start = 1;
static const int Newbie_first_final = 13;
static const int Newbie_error = 0;

static const int Newbie_en_Newbie = 8;
static const int Newbie_en_Client = 12;
static const int Newbie_en_Client_RegisteringWithLocationService = 12;
static const int Newbie_en_Consensus = 1;
static const int Newbie_en_Consensus_StartingLocationService = 6;
static const int Newbie_en_Consensus_JoiningExistingQuorum = 7;


#line 77 "newbie.rl"

int HAVEN_init_machine_newbie(HAVEN_newbie_machine_t *machine, HAVEN_state_actions_t *actions)
{
    int cs;
    
#line 46 "newbie_machine.c"
	{
	cs = Newbie_start;
	}

#line 82 "newbie.rl"
}

int HAVEN_exec_machine_newbie(HAVEN_newbie_machine_t *machine, HAVEN_ctx_t* ctx)
{
    int *p = NULL;
    int *pe = NULL;
    int *eof = NULL;
    int cs = machine->cs;

    
#line 62 "newbie_machine.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 7: goto st2;
		case 8: goto st6;
		case 9: goto st7;
	}
	goto st0;
tr4:
#line 8 "consensus.rl"
	{
        HAVEN_consensus_error_cb();
    }
	goto st0;
tr9:
#line 51 "newbie.rl"
	{
        HAVEN_newbie_error_cb();
    }
	goto st0;
tr15:
#line 8 "client.rl"
	{
        HAVEN_client_error_cb();
    }
	goto st0;
#line 93 "newbie_machine.c"
st0:
cs = 0;
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
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
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
case 8:
	if ( (*p) == 0 )
		goto tr8;
	goto st0;
tr8:
#line 23 "newbie.rl"
	{
        HAVEN_newbie_setup_cb();
    }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 158 "newbie_machine.c"
	if ( (*p) == 1 )
		goto tr10;
	goto tr9;
tr10:
#line 31 "newbie.rl"
	{
        HAVEN_newbie_handle_cmd_cb();
    }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 172 "newbie_machine.c"
	switch( (*p) ) {
		case 2: goto tr11;
		case 3: goto tr12;
		case 4: goto tr13;
	}
	goto tr9;
tr11:
#line 43 "newbie.rl"
	{
        {goto st7;}
    }
	goto st11;
tr12:
#line 35 "newbie.rl"
	{
        {goto st12;}
    }
	goto st11;
tr13:
#line 39 "newbie.rl"
	{
        {goto st6;}
    }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 201 "newbie_machine.c"
	goto tr9;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 0 )
		goto tr14;
	goto st0;
tr14:
#line 8 "client.rl"
	{
        HAVEN_client_error_cb();
    }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 220 "newbie_machine.c"
	goto tr15;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 9: 
	case 10: 
	case 11: 
#line 51 "newbie.rl"
	{
        HAVEN_newbie_error_cb();
    }
	break;
	case 2: 
	case 3: 
	case 4: 
	case 5: 
	case 6: 
	case 7: 
#line 8 "consensus.rl"
	{
        HAVEN_consensus_error_cb();
    }
	break;
#line 258 "newbie_machine.c"
	}
	}

	_out: {}
	}

#line 92 "newbie.rl"
}

/* EOF */
