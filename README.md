# Lib_Firebase_FreeRTOS

## Guide to Integrate the Library into ESP-IDF + PlatformIO

### Step 1: Modify `sdkconfig` to enable HTTPS and necessary configurations

Add or edit the following lines in your `sdkconfig` file:

```ini
CONFIG_ESP_HTTP_CLIENT_ENABLE_HTTPS=y
CONFIG_ESP_MAIN_TASK_STACK_SIZE=8192
CONFIG_COMPILER_CXX_EXCEPTIONS=y
CONFIG_ESPTOOLPY_FLASHSIZE_4MB=y
CONFIG_ESPTOOLPY_FLASHSIZE="4MB"
```
### Step 2: Add the library and PEM certificate in platformio.ini
Append these lines to your platformio.ini file:

```ini
lib_deps = 
    https://github.com/buiTannn/Lib_Firebase_FreeRTOS.git

board_build.embed_files = .pio/libdeps/esp32doit-devkit-v1/Lib_Firebase_FreeRTOS/firebase_root_cert.pem
```
### Option 2: 
You can download the Lib_Firebase_FreeRTOS folder by downloading the folder below, put it in the destination address .pio/libdeps/esp32doit-devkit-v1/

### Step 3: Update CMakeLists.txt in the src folder to embed the certificate file
Edit the CMakeLists.txt file located in the src directory and add the following block:

```cmake
FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)

idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    EMBED_TXTFILES "../.pio/libdeps/esp32doit-devkit-v1/Lib_Firebase_FreeRTOS/firebase_root_cert.pem"
)
```

## Library main functions
'Getting' data from FIREBASE:

```c
bool firebase_get_int(const char* path, int* output);
bool firebase_get_float(const char* path, float* output);
bool firebase_get_bool(const char* path, bool* output);
bool firebase_get_string(const char* path, char** output);
```

'Putting' data to FIREBASE:

```c
void firebase_put_data(const char* path, fb_data_type_t type, ...);
```

## Example 
Putting text:
```c
firebase_put_data("/my/path/lcd", FB_STRING, "hello world");
```
Putting text and data: 
```c
int temperature = 25;
firebase_put_data("/my/path/lcd", FB_STRING, "Temp = %d°C", temperature);
```
## Coming Soon
Website dashboard connect to Firebase server : https://iot-kit.vercel.app/
<img width="2835" height="1579" alt="image" src="https://github.com/user-attachments/assets/c30d2cca-5526-47c8-96a3-489d68e7755a" />


