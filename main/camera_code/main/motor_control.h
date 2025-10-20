const int motor1b = 12;
const int motor1a = 14;

const int motorspeeda = 27;

const int motor2b = 25;
const int motor2a = 26;

const int motorspeedb = 33;

gpio_config_t m1a = {
    .pin_bit_mask = (1ULL << motor1a),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

gpio_config_t m1b = {
    .pin_bit_mask = (1ULL << motor1b),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

gpio_config_t speeda = {
    .pin_bit_mask = (1ULL << motorspeeda),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

//verticle

gpio_config_t m2a = {
    .pin_bit_mask = (1ULL << motor2a),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

gpio_config_t m2b = {
    .pin_bit_mask = (1ULL << motor2b),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

gpio_config_t speedb = {
    .pin_bit_mask = (1ULL << motorspeedb),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

void init_motor_pins(){ //init motor pins on device startup
    gpio_config(&m1a);
    gpio_config(&m1b);

    gpio_set_level(motor1a, 1);
    gpio_set_level(motor1b, 0);

    gpio_config(&m2a);
    gpio_config(&m2b);

    gpio_set_level(motor2a, 1);
    gpio_set_level(motor2b, 0);

    gpio_config(&speeda);
    gpio_set_level(motorspeeda, 0);

    gpio_config(&speedb);
    gpio_set_level(motorspeedb, 0);
}

int motor_v_direction = 0;
int motor_h_direction = 0;

void move_motor(char* m, int dir, float speed){
    /*moves motors in direction
    variable 'm' is what direction "v for verticle"
    "h" for horizantal

    'dir' specifies direction. -1 for left, 0 for stop and 1 for right

    speed specefies the motor speed. unused at this time as not needed
    */

    int motors[3] = {motor1b, motor1a, motorspeeda}; //backup motors are horizantal

    //we check if a limit switch is hit / clicked.
    int verticle_bottom_state = gpio_get_level(verticle_bottom);
    int verticle_top_state = gpio_get_level(verticle_top);

    int horizotal_right_state = gpio_get_level(horizontal_right);
    int horizotal_left_state = gpio_get_level(horizontal_left);

    if(strcmp(m, "v") == 0){ //set motors be verticle
        motors[1] = motor2b;
        motors[0] = motor2a;
        motors[2] = motorspeedb;
        motor_v_direction = dir;
    }else{
        motor_h_direction = dir;
    }

    //we check if the motor is going to move in the direction of a turned on 
    // switch. If it does, then we force a stop.
    if(verticle_bottom_state == 1 && strcmp(m, "v") == 0 && dir == 1){
        dir = 0;
    }
    if(verticle_top_state == 1 && strcmp(m, "v") == 0 && dir == -1){
        dir = 0;
    }

    if(horizotal_right_state == 1 && strcmp(m, "h") == 0 && dir == -1){
        dir = 0;
    }
    if(horizotal_left_state == 1 && strcmp(m, "h") == 0 && dir == 1){
        dir = 0;
    }
    
    if(dir == 0){ //set motor high / low / off levels
        gpio_set_level(motors[0], 0);
        gpio_set_level(motors[1], 0);
        gpio_set_level(motors[2], 0);
    }else if (dir > 0){
        gpio_set_level(motors[0], 1);
        gpio_set_level(motors[1], 0);
        gpio_set_level(motors[2], 1);
    }
    else if (dir < 0){
        gpio_set_level(motors[0], 0);
        gpio_set_level(motors[1], 1);
        gpio_set_level(motors[2], 1);
    }
}

void move_motor_safe(char* m, int dir, float speed, int time){
    /*This function turns a motor for a set amount of time
    and checks after every pulse that the limit switches havnt been actiavted*/
    for (int t = 0; t <= time; t+=20){
         vTaskDelay(pdMS_TO_TICKS(20));
         move_motor(m, dir, speed);
    }
    move_motor(m, 0, 0.0); //stop after we moved for the specefied amount of time
} 

