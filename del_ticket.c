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
 * del_ticket
 *      remove ticket (summons) from the database
 *      look up the summons for a vehicle and pay it (remove the ticket)
 *
 *      Find the vehicle by the state and plate. if not found return -1
 *
 *      convert the summ string to a long long int using
 *      using strtosumid. This long long int is how the summons number
 *      is stored in the database in struct ticket.
 *      After being converted you can use it find a summons id match
 *      in the linked list of tickets attached to the strcut vehicle.
 *
 *      After you have the vehicle, find the summons on the ticket chain
 *      that matches the converted summons string (summid in the example)
 *      example:
 *        unsigned long long summid
 *        if (strtosumid(summ, &summid, argv) != 0)
 *           return -1;
 *        now search the ticket chain to find that summons
 *        struct ticket *tptr;
 *        if (tptr->summons == summid)
 *          found it
 *
 *      If found, delete it from the ticket chain. If the vehicle
 *      no longer has any tickets, then remove the vehicle from the hash chain
 *      Otherwise you must update the tot_fine and cnt_tickets fields in
 *      the struct vehicle so the are accurate after removing the summons
 *      Make sure to delete all space that malloc()'d
 *
 * Inputs:
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *  fineTab table mapping fine code to description and fine
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  summ    summon id string to be found
 *  argv    for error printing
 *
 * returns 0 if ok -1 if not found or error
 */

int
del_ticket(struct vehicle **hashtab, uint32_t tabsz, struct fine *fineTab,
    char *plate, char *state, char *summ, char **argv)
{
    struct vehicle *vptr;       // current vehicle
    struct vehicle *prvv;       // previous vehicle (for removing vehicle)
    struct ticket *tptr;        // current ticket
    struct ticket *prvt;        // previous ticket (for removing tickets)
    unsigned long long sumid;   // summons id for ticket to remove

    // convert summ to unsigned long long
    if (strtosumid(summ, &sumid, argv) != 0)
        return -1;

    // get hash chain for vehicle by license plate
    vptr = *(hashtab + hash(plate, argv) % tabsz);

    // search through hash chain for vehicle (by state and plate)
    while (vptr != NULL) {
        // if we found the correct vehicle pay fee by summons id
        if (strcmp(vptr->plate, plate) == 0 &&
            strcmp(vptr->state, state) == 0) {
            // search for ticket by summons id
            tptr = vptr->head;
            while (tptr != NULL) {
                // if the ticket matches the summons id, remove that ticket
                if (tptr->summons == sumid) {
                    // update tot_fine and cnt_ticket for this vehicle
                    vptr->tot_fine -= (fineTab + tptr->code)->fine;
                    vptr->cnt_ticket--;

                    // remove ticket
                    // if the ticket is the first ticket in the list
                    if (tptr == vptr->head) {
                        // if it is the only ticket
                        if (tptr->next == NULL) {
                            // simply free and relink vptr head to NULL
                            free(tptr);
                            tptr = NULL;
                            vptr->head = NULL;
                        } else {
                            // there are more tickets in list
                            // relink head to next node, and remove tptr
                            vptr->head = tptr->next;
                            free(tptr);
                            tptr = NULL;
                        }
                    } else {
                        // tptr is not the head of the list
                        // find previous ticket to tptr
                        prvt = vptr->head;
                        while (prvt->next != tptr)
                            prvt = prvt->next;

                        // relink prevous ticket to pass over vptr in list
                        prvt->next = tptr->next;
                        // remove current ticket
                        free(tptr);
                        tptr = NULL;
                    }

                    // if there are no more tickets, remove vehicle
                    if (vptr->cnt_ticket == 0) {
                        // store head of vehicle chain
                        prvv = *(hashtab + hash(plate, argv) % tabsz);

                        // if the vehicle is the first in the chain
                        if (vptr == prvv) {
                            // if it is the only vehicle
                            if (vptr->next == NULL) {
                                // simply remove vptr and relink chain to NULL
                                free(vptr->state);
                                free(vptr->plate);
                                free(vptr);
                                vptr = NULL;
                                *(hashtab + hash(plate, argv) % tabsz) = NULL;
                            } else {
                                // not the only vehicle in chain
                                // relink chain to next vehicle and remove vptr
                                *(hashtab + hash(plate, argv) % tabsz) =
                                    vptr->next;
                                free(vptr->state);
                                free(vptr->plate);
                                free(vptr);
                                vptr = NULL;
                            }
                        } else {
                            // this is not the first vehicle in the chain
                            // find previous vehicle in chain
                            while (prvv->next != vptr)
                                prvv = prvv->next;

                            // relink previous vehicle to pass over vptr
                            prvv->next = vptr->next;

                            // remove current vehicle
                            free(vptr->state);
                            free(vptr->plate);
                            free(vptr);
                            vptr = NULL;
                        }
                    }

                    // since we found the correct ticket to remove, return 0
                    return 0;
                }

                // progress to next ticket in ticket chain
                tptr = tptr->next;
            }
        }

        // progress vptr to next vehicle in chain
        vptr = vptr->next;
    }

    // if we reach this, the vehicle and/or ticket was not found, return -1
    return -1;

}
#endif
