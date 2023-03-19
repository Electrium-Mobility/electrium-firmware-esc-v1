#include <SimpleFOC.h>


// Hall sensor instance
// HallSensor(int hallA, int hallB , int hallC , int pp)
//  - hallA, hallB, hallC    - HallSensor A, B and C pins
//  - pp                     - pole pairs
HallSensor sensor = HallSensor(11, 10, 9, 11); // CHECK NUM OF POLE PAIRS

//  BLDCDriver6PWM( int phA_h, int phA_l, int phB_h, int phB_l, int phC_h, int phC_l, int en)
//  - phA_h, phA_l - A phase pwm pin high/low pair 
//  - phB_h, phB_l - B phase pwm pin high/low pair
//  - phB_h, phC_l - C phase pwm pin high/low pair
//  - enable pin    - (optional input)
BLDCDriver6PWM driver = BLDCDriver6PWM(21,20,19,18,17,16,22);

//  LowsideCurrentSense(shunt_resistance, gain, adc_a, adc_b, adc_c)
LowsideCurrentSense current_sense = LowsideCurrentSense(0.01, 20, A3, A2, A1);

//  BLDCMotor( pole_pairs , ( phase_resistance, KV_rating  optional) )
BLDCMotor motor = BLDCMotor(14);

// Interrupt routine initialization
// channel A and B callbacks
void doA(){sensor.handleA();}
void doB(){sensor.handleB();}
void doC(){sensor.handleC();}


void setup() {
  // put your setup code here, to run once:

  //Hall sensor Setup
  sensor.init();
  // hardware interrupt enable
  sensor.enableInterrupts(doA, doB, doC);

  //Motor Driver Setup
  // pwm frequency to be used [Hz]
  driver.pwm_frequency = 20000;
  // power supply voltage [V]
  driver.voltage_power_supply = 36;
  // Max DC voltage allowed - default voltage_power_supply
  driver.voltage_limit = 42;
  // driver init
  driver.init();

  //Current Sensor Setup
  // link the driver with the current sense
  current_sense.linkDriver(&driver);
  // init current sense
  current_sense.init();

  // Motor Setup
  // link the motor to the sensor
  motor.linkSensor(&sensor);

  // link the motor to the driver
  motor.linkDriver(&driver);
  // link driver and the current sense
  // link the motor to current sense
  motor.linkCurrentSense(&current_sense);
  // set control loop type to be used
  motor.controller = MotionControlType::velocity;
  // initialize motor
  motor.init();

  motor.initFOC();

}

void loop() {
  // put your main code here, to run repeatedly:
  // FOC algorithm function
  motor.loopFOC();

  // velocity control loop function
  // setting the target velocity or 2rad/s
  motor.move(2);
  delay(10000);
  motor.move(-2);
  delay(10000);
  
}
