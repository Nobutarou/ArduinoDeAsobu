// Work: D0, D1, D8-D11, D13
// Dead: D2-D7, D12

const int LED_PIN = D2;

void setup(){
    pinMode( LED_PIN, OUTPUT );
}

void loop(){
    digitalWrite( LED_PIN, HIGH );
}