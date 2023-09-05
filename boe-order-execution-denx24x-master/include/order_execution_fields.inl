#if !defined(FIELD) || !defined(VAR_FIELD)
#error You need to define FIELD and VAR_FIELD macro
#else

VAR_FIELD(cl_ord_id, 18, 20)
FIELD(exec_id, base36, 38)
FIELD(filled_volume, uint32, 46)
FIELD(price, binary_price, 50)
FIELD(active_volume, uint32, 58)
FIELD(liquidity_indicator, liquidityIndicator, 62)

#undef FIELD
#undef VAR_FIELD

#endif
