#ifndef FIREBASE_H
#define FIREBASE_H
#include <stdbool.h>
void firebase_init(const char* url, const char* token);
void firebase_put(const char* path, const char* data);
void firebase_get(const char* path, char* response_buf, int buf_size);
void firebase_patch(const char* path, const char* data);
void firebase_delete(const char* path);
void firebase_cleanup();
bool firebase_test_connection();

#endif
