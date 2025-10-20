const int verticle_bottom = 19;
const int verticle_top = 18;

const int horizontal_right = 23;
const int horizontal_left = 32;

gpio_config_t verticle_limiter = {
    .pin_bit_mask = (1ULL << verticle_bottom),
    .mode = GPIO_MODE_INPUT,            
    .pull_up_en = GPIO_PULLUP_ENABLE,   
    .pull_down_en = GPIO_PULLDOWN_DISABLE, 
    .intr_type = GPIO_INTR_DISABLE      
};

gpio_config_t verticle_limiter2 = {
    .pin_bit_mask = (1ULL << verticle_top), 
    .mode = GPIO_MODE_INPUT,        
    .pull_up_en = GPIO_PULLUP_ENABLE,  
    .pull_down_en = GPIO_PULLDOWN_DISABLE, 
    .intr_type = GPIO_INTR_DISABLE      
};

gpio_config_t horizontal_limiter = {
    .pin_bit_mask = (1ULL << horizontal_right),
    .mode = GPIO_MODE_INPUT,     
    .pull_up_en = GPIO_PULLUP_ENABLE,    
    .pull_down_en = GPIO_PULLDOWN_DISABLE, 
    .intr_type = GPIO_INTR_DISABLE     
};

gpio_config_t horizontal_limiter2 = {
    .pin_bit_mask = (1ULL << horizontal_left), 
    .mode = GPIO_MODE_INPUT,          
    .pull_up_en = GPIO_PULLUP_ENABLE,    
    .pull_down_en = GPIO_PULLDOWN_DISABLE, 
    .intr_type = GPIO_INTR_DISABLE   
};

void init_limit_pins(){ //init limit pins on startup
    gpio_config(&verticle_limiter);
    gpio_config(&verticle_limiter2);

    gpio_config(&horizontal_limiter);
    gpio_config(&horizontal_limiter2);
}