#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "hashdb.h"
#include "readsubs.h"

//uncomment the next line when you want to use your routine
#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * insert_ticket
 *      add a ticket (summons) to the database
 *
 *      if the vehicle (plate state) is not in the database then
 *      a new struct vehicle entry must be made for it in the hash table.
 *      New vehicle entries are made at the the FRONT of the collison chain
 *
 *      If the vehicle is already in the hash chain, you only need to add
 *      a new ticket to the ticket chain for this summons.
 *
 *      Tickets are added to the ticket chain linked to the struct vehicle
 *      at the END of the ticket chain.
 *
 *      Also you must update the tot_fine and cnt_tickets in the struct vehicle
 *      to account for the new summons(ticket) just added
 * args:
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *          example hashval = hash(plate, argv) % tabsz;
 *  fineTab pointer fine table. maps code number to text description & fine cost
 *          you use this table to get the amount of the fine when calculating
 *          the tot_fine field in struct vehicle.
 *          example:
 *              struct vehicle *vhpt;
 *              vhpt->tot_fine += fineTab[code].fine;
 *  summ    summons id string to be added
 *          this string needs to be converted to a number to be stored in the
 *          database. This saves space and is faster to compare than a string
 *          Conversion function is supplied. use example:
 *               long long summid;
 *               if (strtosumid(summ, argv, &summid) != 0)
 *                  error handling
 *               nt->summons = summid;
 *  plate   plate id string to be added
 *  state   state id string to be added
 *  date    date of summons string
 *          this string needs to be converted to a number to be stored in the
 *          database. This saves space and is faster to compare than a string
 *          The encoding into a number uses Linux time format.
 *          Conversion function is supplied. use example:
 *              time_t dateval;
 *              if (strtoDate(date, &dateval, argv) != 0)
 *                  error handling
 *              nt->date = dateval;
 *  code    summons code integer value used an an index into the fines table
 *  argv    for error printing
 *
 * returns 0 if ok -1 for all errors
 */

int
insert_ticket(struct vehicle **hashtab, uint32_t tabsz, struct fine *fineTab, char *summ,
            char *plate, char *state, char *date, int code, char **argv)
{
    struct vehicle *vptr;       // current vehicle
    struct vehicle *nv;         // for new vehicle
    struct ticket *tptr;        // current ticket
    struct ticket *nt;          // for new ticket
    unsigned long long sumid;   // summons id for new ticket
    time_t dateval;             // date value for new ticket

    // convert summ to unsigned long long
    if (strtosumid(summ, &sumid, argv) != 0)
        return -1;

    // convert date to int
    if (strtoDate(date, &dateval, argv) != 0)
        return -1;

    // allocate memory for new ticket
    if ((nt = malloc(sizeof(struct ticket))) == NULL) {
        fprintf(stderr,"%s: malloc of new ticket failed\n", *argv);
        exit(EXIT_FAILURE);
    }
    // initialize member values for new ticket
    nt->code = code;
    nt->date = dateval;
    nt->summons = sumid;
    nt->next = NULL;

    // find proper hash chain for the vehicle in hash table
    vptr = *(hashtab + hash(plate, argv) % tabsz);

    // iterate over hash chain searching for correct vehicle
    while (vptr != NULL) {
        if (strcmp(vptr->state, state) == 0 &&
            strcmp(vptr->plate, plate) == 0) {
            // we found a matching vehicle
            // update cnt_ticket and tot_fine
            vptr->cnt_ticket++;
            vptr->tot_fine += (fineTab + code)->fine;

            // if the head is null, this new ticket will be the head
            if (vptr->head == NULL) {
                vptr->head = nt;
            } else {
                // we need to find the last element in the ticket list
                tptr = vptr->head;
                while (tptr->next != NULL)
                    tptr = tptr->next;

                // make the next element the new ticket
                tptr->next = nt;
            }

            // added ticket to vehicle successfully, return 0
            return 0;
        }
        // progress to next vehicle chain in table
        vptr = vptr->next;
    }

    // we didn't find the vehicle, allocate memory for a new vehicle
    if ((nv = malloc(sizeof(struct vehicle))) == NULL) {
        fprintf(stderr,"%s: malloc of new vehicle failed\n", *argv);
        exit(EXIT_FAILURE);
    }
    // initialize members for new vehicle
    if ((nv->plate = strdup(plate)) == NULL) {
        fprintf(stderr, "%s: malloc of new vehicle plate failed\n", *argv);
        exit(EXIT_FAILURE);
    }
    if ((nv->state = strdup(state)) == NULL) {
        fprintf(stderr, "%s: malloc of new vehicle state failed\n", *argv);
        exit(EXIT_FAILURE);
    }
    nv->cnt_ticket = 1;
    nv->tot_fine = (fineTab + code)->fine;
    nv->next = NULL;
    nv->head = NULL;

    // make new ticket head of this new vehicle's ticket list
    nv->head = nt;

    // insert new vehicle at front of hash chain
    // if head is null, make new vehicle head
    if (*(hashtab + hash(plate, argv) % tabsz) == NULL) {
        *(hashtab + hash(plate, argv) % tabsz) = nv;
    } else {
        // head is not null, insert new vehicle at front
        nv->next = *(hashtab + hash(plate, argv) % tabsz);
        *(hashtab + hash(plate, argv) % tabsz) = nv;
    }

    // successfully added new ticket and/or vehicle, return 0
    return 0;
}
#endif
