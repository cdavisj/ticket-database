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
 * free_tickets
 *      tears down the database freeing all allocated memory
 *
 *      go down each hash chain and free all the vehicles and the tickets
 *      for each vehicle
 * args
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 */
void
free_tickets(struct vehicle **hashtab, uint32_t tabsz)
{
    struct vehicle *vptr;   // current vehicle
    struct vehicle *tmpv;   // helper for removing vehicles
    struct ticket *tptr;    // current ticket
    unsigned long tcnt = 0; // tcnt for number of tickets freed

    // iterate over hash chain looking for non-null chains
    for (uint32_t i = 0; i < tabsz; i++) {
        // if this is a non-null chain
        if (*(hashtab + i) != NULL) {
            // store current vehicle as vptr
            vptr = *(hashtab + i);

            while (vptr != NULL) {
                // store first ticket as tptr
                tptr = vptr->head;

                // go through and free all the tickets for this vehicle
                while (tptr != NULL) {
                    // link vptr head to next ticket
                    vptr->head = tptr->next;
                    // remove ticket (tptr)
                    free(tptr);
                    // move tptr to new head (if null, we'll terminate loop)
                    tptr = vptr->head;
                    tcnt++;
                }

                // now that we've freed all tickets, we can remove the vehicle
                tmpv = vptr->next;
                free(vptr->plate);
                free(vptr->state);
                free(vptr);
                vptr = tmpv;
            }
        }
    }

    // free memory allocated for hash table
    free(hashtab);

    // output total tickets freed
    printf("Total tickets freed: %lu\n", tcnt);
}
#endif
