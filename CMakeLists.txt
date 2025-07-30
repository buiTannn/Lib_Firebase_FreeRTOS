
idf_component_register(
    SRCS 
        "firebase.c"
        "rtdb.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES 
        esp_http_client 
        esp-tls
)