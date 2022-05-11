#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "misc.h"
#include "hashdb.h"

//uncomment the next line when you want to use your routine
#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * split_input
 *
 * usage:   input buffer csv format, builds table of pointers to each column
 * assumes: input buffer has at least one char in it (not counting the '\0')
 * returns:  0 if all the columns were found,  -1 otherwise
 */
int
split_input(char *buf, char delim, int cnt, char **table, unsigned long lineno,
            char **argv)
{
    int idx = 0;    /* index for table */
    int fldof = 0;  /* indicates if we have an overflow of fields */

    /*
     * walk through buffer, validating fields and directing table pointers to
     * them
     */
    while (*buf != '\0') {
        /* checks that we are not trying to add more fields than allowed */
        if (idx >= cnt) {
            fldof = 1;
            break;
        }

        /*
         * point next text table slot to start of next field before validating
         * that this field is valid. If it is not valid, record is dropped from
         * output
         */
        *(table + idx++) = buf;

        if (*buf == '"') {
            /*
             * if we have a quoted field, loop over field looking for invalid
             * character sequences. Invalid character sequences includes \0
             * before a terminating "; a " not followed by the delimiter, \n, or
             * another ".
             */
            while (*buf != '\0') {
                /* update buffer */
                buf++;

                /*
                 * if we have \0 before closing ", the quoted field was not
                 * properly terminated
                 */
                if (*buf == '\0') {
                    dropmsg("Quoted field missing final quote", lineno,
                                argv);
                    return -1;
                }

                /*
                 * if we hit a ", we must have it followed by the delimiter, a
                 * \n, or another ". Otherwise, it is an invalid field
                 */
                if (*buf == '"') {
                    if ((*(buf + 1) == delim) || (*(buf + 1) == '\n')) {
                        buf++;
                        *buf = '\0';
                        break;

                    } else if (*(buf + 1) == '"') {
                        buf++;

                    } else {
                        dropmsg("Quoted field not terminated properly", lineno,
                                    argv);
                        return -1;
                    }
                }
            }

        } else {
            /*
             * if not a quoted field first make sure it isn't an elmpty field.
             * If it is an empty field, then add null terminator and continue to
             * next field
             */
            if (*buf == delim || *buf == '\n') {
                *buf = '\0';

            } else {
                /*
                 * if we have an unquoted field, loop over field looking for
                 * invalid character sequences. The only invalid character
                 * sequence in this case is having " anywhere in the field
                 */
                while (*(buf++) != '\0') {
                    if (*buf == '"') {
                        dropmsg("A \" is not allowed inside unquoted field",
                                    lineno, argv);
                        return -1;

                    } else if (*buf == '\n' || *buf == delim) {
                        *buf = '\0';
                        break;
                    }
                }
            }
        }

        /* moves to next field */
        buf++;
    }

    /*
     * make sure we have the correct number of fields read. If we have too many
     * or too few, prints corresponding error message
     */
    if (idx < cnt) {
        dropmsg("too few columns", lineno, argv);
        return -1;

    } else if (fldof) {
        dropmsg("too many columns", lineno, argv);
        return -1;
    }

    return 0;
}
#endif