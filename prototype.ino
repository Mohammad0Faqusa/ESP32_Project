void task1 (void * parameters) {
    for (;;) {
        // arduino cloud execution 
        vTaskDelay(1000 / portTICK_PERIOD_MS) ; 
    }
}

void task2 (void * parameters) {
    for (;;) {
        // arduino cloud execution 
        vTaskDelay(1000 / portTICK_PERIOD_MS) ; 
    }
}

void setup() {
    xTaskCreate(
        task1 , 
        "Arduino Cloud", 
        1000 , 
        NULL , 
        2, 
        NULL 
    )
    xTaskCreate(
        task2 , 
        "read sensors", 
        1000 , 
        NULL , 
        1, 
        NULL 
    )
}