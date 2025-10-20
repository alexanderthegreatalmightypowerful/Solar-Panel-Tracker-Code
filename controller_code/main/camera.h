#define UART_PORT UART_NUM_2
#define TXD_PIN GPIO_NUM_17
#define RXD_PIN GPIO_NUM_16
#define UART_BUFFER_SIZE 1024
#define portMAX_DELAY 100

uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_APB,
};

QueueHandle_t uart_queue;
void init_UART(){
//uart_driver_install(UART_PORT, UART_BUFFER_SIZE, UART_BUFFER_SIZE, 0, NULL, 0);
//uart_set_pin(UART_PORT, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
uart_driver_install(UART_NUM_2, UART_BUFFER_SIZE, UART_BUFFER_SIZE, 10, &uart_queue, 0);
//uart_driver_install(UART_PORT, UART_BUFFER_SIZE, 0, 0, NULL, 0);
//uart_set_pin(UART_PORT, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
uart_param_config(UART_PORT, &uart_config);
uart_set_pin(UART_NUM_2, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}
