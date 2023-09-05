#if !defined(FIELD) || !defined(VAR_FIELD)
#error You need to define FIELD and VAR_FIELD macro
#else

VAR_FIELD(cl_ord_id, 18, 20)
FIELD(reason, restatement_reason, 46)

#undef FIELD
#undef VAR_FIELD

#endif
