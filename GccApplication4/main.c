#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

#define RELAY_ON 0
#define RELAY_OFF 1
#define RELAY_1 4

const int pirPin = PD5; // PIR sensor Pin

int motionDetected = 0;


void initTimer() {
    TCCR1B |= (1 << WGM12); // Configure Timer1 for CTC mode

    // Set OCR1A value
    setTimerInterrupt(); // Call set timer interrupt function
}

void setTimerInterrupt() {
    
    //OCR1A = (CPU Frequency / Prescaler * Interrupt Frequency) -1;

    OCR1A = 24999; // Set CTC compare value for 50 ms

    TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt
    sei(); // Enable global interrupts
    TCCR1B |= (1 << CS11) | (1 << CS10); // Set Timer1 prescaler to 64
}

void initRelay() {
    DDRD |= (1 << RELAY_1);     // Set RELAY_1 pin as output
    PORTD &= ~(1 << RELAY_1);   // Set initial state of RELAY_1 to off
}

void initPIR() {
    DDRD &= ~(1 << pirPin);     // Set PIR sensor pin as input
}

ISR(TIMER1_COMPA_vect) {
    
    // This ISR runs every 50 ms
    
    // Check PIR sensor state

    if (PIND & (1 << pirPin)) {
      motionDetected = 1;
    } else {
      motionDetected = 0;
    }
}

int main() {
    // Initialize all peripherals    
    initRelay();
    initPIR();
    initTimer();
    
    while (1) {
        if (motionDetected) 
        {
            PORTD |= (1 << RELAY_1); // Turn on relay
        } 
        else 
        {
            PORTD &= ~(1 << RELAY_1); // Turn off relay
        }
            }

    return 0;
}
