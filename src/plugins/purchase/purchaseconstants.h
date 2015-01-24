#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {
//widgets
const char W_PURCHASE_REQUESTS[]        = "Purchase.Requests";

//Request states
const int STATE_HIDDEN      = 0;
const int STATE_PUBLIC      = 1;
const int STATE_ORDERED     = 2;
const int STATE_PAID        = 3;
const int STATE_DELIVERED   = 4;
const int STATE_RECEIVED    = 5;
}

#endif // CONSTANTS_H

