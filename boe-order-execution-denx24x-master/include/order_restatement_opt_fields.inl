#ifndef FIELD
#error You need to define FIELD macro
#else

FIELD(active_volume, uint32, 4, 5, 2)
FIELD(secondary_order_id, base36, 8, 6, 1)

#undef FIELD
#endif
