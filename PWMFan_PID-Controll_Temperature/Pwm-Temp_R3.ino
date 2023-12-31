// R4 用に雑に書いたプログラムが 56kB で R3 に載せるにはピンチ!!!
// T sensor setup
const short TEMP_PIN = A3;

// v0c: output voltage at T=0. [mA]
// d_v: tangent of Volt/Temp. [mA/K]
// vref: the reference voltage input to AREF [V]
const float v0c = 500.0;
const float d_v = 10.0;

// UT7500L-18-T92-B-K gives 1.8V
const float vref = 1800.0;

// PWM setting
// on UNO, D5, D6 have 980Hz and others have 490Hz
const short PWM_PIN = 3;
// on R4, I can use percentage via pwm().
// on R3, I have to use 0 - 255 integer.
const short max_pwm_perc = 255; //100%
const short min_pwm_perc = 26; //10%

//// PID setup
const float crit_temp = 85.0;  // degree of C
const float target_temp = 40.0;
const float dt = 1000.0;  //1sec

// at crit_temp, full power.
const float kp = 255.0 / (crit_temp - target_temp);

// 10 deg C/dt --> full power
const float kd = 255.0 / (10.0 / dt);

// no integrate gain. might be not needed for this case.
// If do, +10deg for 100sec --> full power?
const float ki = 0.0;

//// Read vars
int temp_pin_input;
float temp;
float volt;  // temp sensor voltage

float diff_temp;            // diff to target_temp.
float pre_diff_temp = 0.0;  // diff_temp at previous cycle.
float d_temp;               // temp diff from previous cycle.
float pre_temp = 0.0;       //temp at previous cycle.
float int_diff_temp = 0.0;  // time integration of diff_temp
short pwm_perc;             // pwm value 0-255 not percentage.

void setup() {
  // output to serial for debug.
  Serial.begin(9600);

  // AR_EXTERNAL: Work (AREF pin input V)
  // AR_INTERNAL: Work. Vref might be 1400~1500mV?
  // AR_INTERNAL_1_5V: no output.
  // AR_INTERNAL_2_0V: no output.
  // AR_INTERNAL_2_5V: no output.
  analogReference(EXTERNAL);

  // とりあえずフル回転
  analogWrite( PWM_PIN, 255);
}

void loop() {

  // read T.
  temp_pin_input = analogRead(TEMP_PIN);
  volt = temp_pin_input / 1023.0 * vref;
  temp = (volt - v0c) / d_v;

  diff_temp = temp - target_temp;
  d_temp = temp - pre_temp;

  pwm_perc = (int) kp * diff_temp + kd * d_temp / dt + ki * (int_diff_temp + diff_temp * dt);

  if (pwm_perc > max_pwm_perc) {
    pwm_perc = max_pwm_perc;
  } else if (pwm_perc < min_pwm_perc) {
    pwm_perc = min_pwm_perc;
  };
  
  analogWrite( PWM_PIN, pwm_perc);

  Serial.print("time[ms]: ");
  Serial.print(millis());
  Serial.print(", temp: ");
  Serial.print(temp);
  Serial.print(", 1st term: ");
  Serial.print(kp * diff_temp);
  Serial.print(", 2nd term: ");
  Serial.print(kd * d_temp / dt);
  Serial.print(", pwm: ");
  Serial.println(pwm_perc);

  pre_temp = temp;

  delay(dt);  // maybe for more accuracy, I have to use timer.
}
