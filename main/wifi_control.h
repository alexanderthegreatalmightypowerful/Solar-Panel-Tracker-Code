#define EXAMPLE_ESP_WIFI_SSID      "ESP32-AP"
#define EXAMPLE_ESP_WIFI_PASS      "12345678" // Must be at least 8 characters
#define EXAMPLE_MAX_STA_CONN       4

static const char *STAG = "webserver-ap";

static esp_err_t http_handler(httpd_req_t *req) {
    const char* resp_str = "<h1>Hello from ESP32 Access Point!</h1>";
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    printf("SENDING REQUEST TO CLIENT!");

    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac); 
    
    char id_string[18];
    sprintf(id_string, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    ESP_LOGI(STAG, "Serving ID: %s", id_string);
    httpd_resp_send(req, id_string, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
    
        //const char* resp_str = "<h1>Hello from ESP32 Access Point!</h1>";
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    printf("SENDING REQUEST TO CLIENT!");
    
    return ESP_OK;
}

static const httpd_uri_t uri_handler = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = http_handler,
    .user_ctx  = NULL
};

static httpd_handle_t start_webserver(void) {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(STAG, "Starting web server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(STAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &uri_handler);
        return server;
    }

    ESP_LOGI(STAG, "Error starting server!");
    return NULL;
}

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data) {
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        //ESP_LOGI(STAG, "station " MACSTR " join, AID=%d", MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        //ESP_LOGI(STAG, "station " MACSTR " leave, AID=%d", MAC2STR(event->mac), event->aid);
    }
}

static void wifi_init_softap(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_register(WIFI_EVENT,
                                        ESP_EVENT_ANY_ID,
                                        &wifi_event_handler,
                                        NULL,
                                        NULL);

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .channel = 1,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(STAG, "wifi_init_softap finished. SSID:%s password:%s", EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
}


static esp_err_t echo_get_handler(httpd_req_t *req) {
    char*  buf;
    size_t buf_len;

    // Get the length of the query string
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = (char*)malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(STAG, "Found URL query string: %s", buf);

            // Parse the query string to find a specific key-value pair
            char param[32];
            if (httpd_query_key_value(buf, "message", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(STAG, "Found URL query parameter 'message': %s", param);
                //httpd_resp_sendstr_printf(req, "GET message received: %s", param);
            } else {
                httpd_resp_sendstr(req, "GET request received, but no 'message' parameter found.");
            }
        }
        free(buf);
    } else {
        httpd_resp_sendstr(req, "GET request received, but no query parameters.");
    }
    return ESP_OK;
}




//DIFFERENT WIFI SETUP;
void setup_server(){
    nvs_flash_init();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    uint8_t mac_address[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac_address);
    
    //ESP_LOGI(STAG, "Receiver MAC Address: " MACSTR, MAC2STR(mac_address));
}

