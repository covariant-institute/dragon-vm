#pragma once

#ifdef DVM_DEBUG

#include <cstdio>

#define DVM_LOG(__msg__) {::printf("DVM LOG:In File %s,Line %d,Function %s:%s\n",__FILE__,__LINE__,__func__,__msg__);::fflush(stdout);}
#else
#define DVM_LOG(__msg__)
#endif