#pragma once

#define dvm_offset_of(TYPE, MEMBER) \
    ((size_t) &((TYPE *) nullptr)->MEMBER)
