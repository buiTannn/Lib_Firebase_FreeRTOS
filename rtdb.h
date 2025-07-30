#ifndef RTDB_H
#define RTDB_H
#include <stdbool.h>

bool firebase_get_int(const char* path, int* output);
bool firebase_get_float(const char* path, float* output);
bool firebase_get_bool(const char* path, bool* output);
bool firebase_get_string(const char* path, char** output);

typedef enum {
    FB_INT,
    FB_FLOAT,
    FB_BOOL,
    FB_STRING,
    FB_JSON
} fb_data_type_t;

void firebase_put_data(const char* path, fb_data_type_t type, ...);

#endif
