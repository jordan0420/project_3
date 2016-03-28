/*
*  HELLO
* 
*  Copyright (C) 1995 by Sun Microsystems, Inc.
*  All rights reserved.
* 
*  This file is a product of SunSoft, Inc. and is provided for
*  unrestricted use provided that this legend is included on all
*  media and as a part of the software program in whole or part.
*  Users may copy, modify or distribute this file at will.
* 
*  THIS FILE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING
*  THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
*  PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
* 
*  This file is provided with no support and without any obligation on the
*  part of SunSoft, Inc. to assist in its use, correction, modification or
*  enhancement.
* 
*  SUNSOFT AND SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT
*  TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY THIS
*  FILE OR ANY PART THEREOF.
* 
*  IN NO EVENT WILL SUNSOFT OR SUN MICROSYSTEMS, INC. BE LIABLE FOR ANY
*  LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
*  DAMAGES, EVEN IF THEY HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
*  DAMAGES.
* 
*  SunSoft, Inc.
*  2550 Garcia Avenue
*  Mountain View, California  94043
*/

/*
% cc -Xa -v -o hello hello.c -lpthread -lthread
 *
 * HELLO - a threaded variant.
 *
 */

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

/* External Declarations        */

/* hello_thread - print the "hello" part.               */

        void *
hello_thread( void *arg ) {
        printf( "hello " );
        return( 0 );
}

/* world_thread - print the "world\n" part.             */

        void *
world_thread( void *arg ) {
        int             n;
        pthread_t       tid     = (pthread_t) arg;

        if ( n = pthread_join( tid, NULL ) ) {
                fprintf( stderr, "pthread_join: %s\n", strerror( n ) );
                exit( 1 );
        }

        printf( "world" );
        pthread_exit( 0 );
/* NOTREACHED */
        return( 0 );
}

/* main - start here.                                   */

main( int argc, char *argv[] ) {
        int             n;
        pthread_t       htid, wtid;

        assert( sizeof( pthread_t ) <= sizeof( void * ) );

        if ( n = pthread_create( &htid, NULL, hello_thread, NULL ) ) {
                fprintf( stderr, "pthread_create: %s\n", strerror( n ) );
                exit( 1 );
        }

        if ( n = pthread_create( &wtid, NULL, world_thread, (void *) htid ) ) {
                fprintf( stderr, "pthread_create: %s\n", strerror( n ) );
                exit( 1 );
        }

        if ( n = pthread_join( wtid, NULL ) ) {
                fprintf( stderr, "pthread_join: %s\n", strerror( n ) );
                exit( 1 );
        }

        printf( "\n" );

        return( 0 );
}

