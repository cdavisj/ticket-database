#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "hashdb.h"

//uncomment the next line when you want to use your routine
#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * vehicle_lookup
 *          look for vehicle in the database
 *          vehicle must match both plate and state strings
 * args
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  argv    for error printing
 *
 * returns  pointer to vehicle if found NULL otherwise
 */

struct vehicle *
vehicle_lookup(struct vehicle **hashtab, uint32_t tabsz, char *plate,
    char *state, char **argv)
{
    struct vehicle *vptr; // vehicle pointer for searching through chains

    // store head of chain
    vptr = *(hashtab + hash(plate, argv) % tabsz);

    // iterate over current chain searching for vehicle
    while (vptr != NULL) {
        // if vptr node matches vehicle data, return vehicle
        if (strcmp(vptr->plate, plate) == 0 &&
            strcmp(vptr->state, state) == 0) {
            return vptr;
        }

        // progress vptr to next vehicle in chain
        vptr = vptr->next;
    }

    // if this point is reached, the vehicle info was not found, return NULL
    return NULL;
}
#endif
