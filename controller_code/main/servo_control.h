#define PCA9685_I2C_ADDR PCA9685_ADDR_BASE

// Define the GPIO pins for I2C communication
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

#define SERVO_CHANNEL 0
#define SERVO_MIN_PULSE 150
#define SERVO_MAX_PULSE 450  
#define PULSE_STOP 300

#define TIME_PER_360_MS 1000 

int servo_angles[0] = {0, 0};

i2c_dev_t pca9685_dev;

void _init_pca_servos_(){
        ESP_LOGI("PCA9685", "Initializing I2C");
    // Initialize the I2C bus device descriptor
    ESP_ERROR_CHECK(i2cdev_init());

    // Initialize the PCA9685 device descriptor
    ESP_ERROR_CHECK(pca9685_init_desc(&pca9685_dev, PCA9685_I2C_ADDR, 0, I2C_SDA_PIN, I2C_SCL_PIN));
    
    // Initialize the PCA9685 chip
    ESP_ERROR_CHECK(pca9685_init(&pca9685_dev));
    
    // Restart the PCA9685
    ESP_ERROR_CHECK(pca9685_restart(&pca9685_dev));
    
    // Set the PWM frequency (e.g., 50 Hz for standard servos)
    ESP_LOGI("PCA9685", "Setting PWM frequency to 50 Hz");
    ESP_ERROR_CHECK(pca9685_set_pwm_frequency(&pca9685_dev, 50));
}

void move_servo(uint8_t channel, uint16_t angle) {
    if (angle > 180) {
        angle = 180;
    }

    uint16_t pulse_value = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) * angle / 180);
    //pulse_value = pulse_value * 4096 / 2000;
    
    ESP_ERROR_CHECK(pca9685_set_pwm_value(&pca9685_dev, channel, pulse_value));
    servo_angles[channel] = angle;
    //ESP_LOGI("PCA9685", "Set channel %d to pulse value %d", channel, pulse_value);
}

void move_servo_raw(uint8_t channel, int pulse) {
    ESP_ERROR_CHECK(pca9685_set_pwm_value(&pca9685_dev, channel, pulse));
}

void move_servo_stepped(int channel, int fangle, int direction){
    
        if(direction > 0){
            if(servo_angles[channel] > fangle){
                return;
            }

            for (uint16_t angle = servo_angles[channel]; angle <= fangle; angle++) {
                move_servo(channel, angle);
                vTaskDelay(pdMS_TO_TICKS(20));
            }
        }else if(direction < 0){
            if(servo_angles[channel] < fangle){
                return;
            }
            for (int angle = servo_angles[channel]; angle >= fangle; angle--) {
                move_servo(channel, (uint16_t)angle);
                vTaskDelay(pdMS_TO_TICKS(20));
            }
        }else{
            move_servo(channel, 0);
        }
}



void move_servo_timed(int channel, int time, int dir){
    if(dir == -1){
        ESP_ERROR_CHECK(pca9685_set_pwm_value(&pca9685_dev, channel, SERVO_MIN_PULSE));
    }else if(dir == 1){
        ESP_ERROR_CHECK(pca9685_set_pwm_value(&pca9685_dev, channel, SERVO_MAX_PULSE));
    }else{
        ESP_ERROR_CHECK(pca9685_set_pwm_value(&pca9685_dev, channel, PULSE_STOP));
    }
    vTaskDelay(pdMS_TO_TICKS(time));
    ESP_ERROR_CHECK(pca9685_set_pwm_value(&pca9685_dev, channel, PULSE_STOP));
}