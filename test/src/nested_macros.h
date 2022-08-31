/* This file was created to provide regression for a bug in MetricVisitor::CalcFnLineCnt
   whereby the size of the function cannot be determined & causes a segfault */

#define __extern_inline extern __inline __attribute__((__gnu_inline__))

#define __create_function(DECL_TEMPL)                                                              \
    DECL_TEMPL(major, (void)) {                                                                    \
    }

#define __function_decl(name, proto)                                                               \
    __extension__ __extern_inline void this_is_a_prefix_##name proto

__create_function(__function_decl)