#ifndef FAST_RAND_H
#define FAST_RAND_H

//Random Number Generator Macros
#define znew (z=36969*(z&65535)+(z>>16))
#define wnew (w=18000*(w&65535)+(w>>16))
#define MWC ((znew<<16)+wnew )

static unsigned long z=362436069, w=521288629;

/* EXAMPLE USE

  int random = MWC % 20;

*/


#endif /* FAST_RAND_H */
