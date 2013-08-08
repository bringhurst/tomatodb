/*
 *	This file is a part of Hierarchical Allocator library.
 *	Copyright (c) 2004-2011 Alex Pankratov. All rights reserved.
 *
 *	http://swapped.cc/halloc
 */

/*
 *	The program is distributed under terms of BSD license. 
 *	You can obtain the copy of the license by visiting:
 *
 *	http://www.opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBP_MACROS_H_
#define _LIBP_MACROS_H_

#include <stddef.h>  /* offsetof */
#include <stdint.h>  /* intptr_t */

/*
 	restore pointer to the structure by a pointer to its field
 */
#define structof(p,t,f) ((t*)(- offsetof(t,f) + (intptr_t)(p)))

#endif

