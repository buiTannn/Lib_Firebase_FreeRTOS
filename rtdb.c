#include "rtdb.h"
#include "firebase.h"
#include <stdio.h>
#include "cJSON.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

// get int value from Firebase
bool firebase_get_int(const char* path, int* output) {
    char response[128];
    firebase_get(path, response, sizeof(response));
    
    cJSON* root = cJSON_Parse(response);
    if (!root) return false;
    
    bool success = false;
    if (cJSON_IsNumber(root)) {
        *output = root->valueint;
        success = true;
    }
    
    cJSON_Delete(root);
    return success;
}

// get float value from Firebase
bool firebase_get_float(const char* path, float* output) {
    char response[128];
    firebase_get(path, response, sizeof(response));
    
    cJSON* root = cJSON_Parse(response);
    if (!root) return false;
    
    bool success = false;
    if (cJSON_IsNumber(root)) {
        *output = (float)root->valuedouble;
        success = true;
    }
    
    cJSON_Delete(root);
    return success;
}

// get bool value from Firebase
bool firebase_get_bool(const char* path, bool* output) {
    char response[128];
    firebase_get(path, response, sizeof(response));
    
    cJSON* root = cJSON_Parse(response);
    if (!root) return false;
    
    bool success = false;
    if (cJSON_IsBool(root)) {
        *output = cJSON_IsTrue(root);
        success = true;
    }
    
    cJSON_Delete(root);
    return success;
}

// Get strinhg value from Firebase
bool firebase_get_string(const char* path, char** output) {
    char response[512];
    firebase_get(path, response, sizeof(response));
    
    cJSON* root = cJSON_Parse(response);
    if (!root) return false;
    
    bool success = false;
    if (cJSON_IsString(root)) {
        *output = strdup(root->valuestring); 
        success = true;
    }
    
    cJSON_Delete(root);
    return success;
}


// Put data to Firebase with type
void firebase_put_data(const char* path, fb_data_type_t type, ...) {
    char buffer[256];
    va_list args;
    va_start(args, type);

    switch(type) {
        case FB_INT: {
            int val = va_arg(args, int);
            sprintf(buffer, "%d", val);
            break;
        }
        case FB_FLOAT: {
            double val = va_arg(args, double);
            snprintf(buffer, sizeof(buffer), "%.2f", val);
            break;
        }
        case FB_BOOL: {
            int val = va_arg(args, int); 
            strcpy(buffer, val ? "true" : "false");
            break;
        }
        case FB_STRING: {
            char temp[33];
            va_list args_copy;
            va_copy(args_copy, args);
            // Get the format string and the actual string
            // Assuming the first argument is a format string
            // and the second is the actual string to be formatted
            const char* fmt = va_arg(args_copy,const char*);
            vsniprintf(temp, sizeof(temp), fmt , args_copy);
            va_end(args_copy);
            snprintf(buffer, sizeof(buffer), "\"%s\"", temp);  
            break;
        }
        case FB_JSON: {
            const char* json = (const char*)va_arg(args, const char*);
            strncpy(buffer, json, sizeof(buffer));
            break;
        }
    }

    va_end(args);

    firebase_put(path, buffer);
}



