#ifndef FIELD
#error You need to define FIELD macro
#else
FIELD(symbol, str, 8, 2, 1)
FIELD(last_mkt, str, 4, 7, 128)
FIELD(fee_code, str, 2, 8, 1)
#undef FIELD
#endif
