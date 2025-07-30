Config change: 
---------------------------------------------------------------------------------------------------------
CONFIG_ESP_HTTP_CLIENT_ENABLE_HTTPS=y
CONFIG_ESP_MAIN_TASK_STACK_SIZE=8192
CONFIG_COMPILER_CXX_EXCEPTIONS=y
CONFIG_ESPTOOLPY_FLASHSIZE_4MB=y
CONFIG_ESPTOOLPY_FLASHSIZE="4MB"
---------------------------------------------------------------------------------------------------------

platform ini: add this line
---------------------------------------------------------------------------------------------------------
lib_deps = 
    https://github.com/buiTannn/Lib_Firebase_FreeRTOS.git
board_build.embed_files = .pio/libdeps/esp32doit-devkit-v1/Lib_Firebase_FreeRTOS/firebase_root_cert.pem
---------------------------------------------------------------------------------------------------------

Add bellow to CMakeList folder src 
---------------------------------------------------------------------------------------------------------
FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)

idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
     EMBED_TXTFILES "../.pio/libdeps/esp32doit-devkit-v1/Lib_Firebase_FreeRTOS/firebase_root_cert.pem"
)
---------------------------------------------------------------------------------------------------------


