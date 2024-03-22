#include <TuyaWifi.h>
#include <SoftwareSerial.h>

TuyaWifi my_device;

/* Estados */
unsigned char connection_led = 0;
unsigned char power_state = 0;
unsigned char fan_speed = 0;

// configuração de pinos
unsigned const char relay_pin_array[3] = {4,3,2};

unsigned const int res_pin = 5;
unsigned const int power_off_led_pin = 13;
unsigned const int power_off_btn_pin = 12;
unsigned const char speed_btn_pin_array[3] = {10,8,6};
unsigned const char speed_led_pin_array[3] = {11,9,7};

/* Data point define */
#define DPID_POWER 1
#define DPID_FAN_SPEED 3

/* Stores all DPs and their types. PS: array[][0]:dpid, array[][1]:dp type. 
 *                                     dp type(TuyaDefs.h) : DP_TYPE_RAW, DP_TYPE_BOOL, DP_TYPE_VALUE, DP_TYPE_STRING, DP_TYPE_ENUM, DP_TYPE_BITMAP
*/
unsigned char dp_array[][2] =
{
  {DPID_POWER, DP_TYPE_BOOL},
  {DPID_FAN_SPEED, DP_TYPE_VALUE}
};

unsigned char pid[] = {"pid here"};
unsigned char mcu_ver[] = {"3.1.4"};

/* last time */
unsigned long last_time = 0;

void setup() 
{
  Serial.begin(9600);

  //Initialize led port, turn off led. (res led === power off led)
  pinMode(power_off_led_pin, OUTPUT);
  digitalWrite(power_off_led_pin, HIGH);

  //Initialize networking keys.
  pinMode(res_pin, INPUT_PULLUP);
  pinMode(power_off_btn_pin, INPUT_PULLUP);
  
  pinMode(power_off_led_pin, OUTPUT);
  digitalWrite(power_off_led_pin, !power_state);

  //initialize relays
  for(unsigned int i = 0; i < sizeof(relay_pin_array); i++) {
     pinMode(relay_pin_array[i], OUTPUT);
     digitalWrite(relay_pin_array[i], HIGH);
  }

  //initialize speed buttons
  for(unsigned int i = 0; i < sizeof(speed_btn_pin_array); i++) {
     pinMode(speed_btn_pin_array[i], INPUT_PULLUP);
  }

  //initialize speed buttons
  for(unsigned int i = 0; i < sizeof( speed_led_pin_array); i++) {
     pinMode(speed_led_pin_array[i], OUTPUT);
      
  }

  //Enter the PID and MCU software version
  my_device.init(pid, mcu_ver);
  //incoming all DPs and their types array, DP numbers
  my_device.set_dp_cmd_total(dp_array, 1);
  //register DP download processing callback function
  my_device.dp_process_func_register(dp_process);
  //register upload all DP callback function
  my_device.dp_update_all_func_register(dp_update_all);
  
  last_time = millis();

}

void loop() 
{
  my_device.uart_service();
  if (digitalRead(res_pin) == LOW) {
    delay(1000);
    if (digitalRead(res_pin) == LOW) {
      my_device.mcu_set_wifi_mode(SMART_CONFIG);
    }
  }
  else if (digitalRead(power_off_btn_pin) == LOW) {
    set_fan_speed(0, 4);
    set_power(0, 1);
  }
  else {
    for(unsigned int i = 0; i < sizeof(speed_btn_pin_array); i++) {
       if (digitalRead(speed_btn_pin_array[i]) == LOW) {
         if (power_state == 0) {
           set_power(1, 1); 
         } 
         set_fan_speed(i+1, 4);
         break;
       }
    }
  }
  
  /* LED blinks when network is being connected */
  if ((my_device.mcu_get_wifi_work_state() != WIFI_LOW_POWER) && (my_device.mcu_get_wifi_work_state() != WIFI_CONN_CLOUD) && (my_device.mcu_get_wifi_work_state() != WIFI_SATE_UNKNOW)) {
    if (millis()- last_time >= 500) {
      last_time = millis();

      connection_led = !connection_led;
      digitalWrite(power_off_led_pin, connection_led);
    }
  }
  else {
    digitalWrite(power_off_led_pin, !power_state);
  }

  delay(10);
}

/**
 * @description: DP download callback function.
 * @param {unsigned char} dpid
 * @param {const unsigned char} value
 * @param {unsigned short} length
 * @return {unsigned char}
 */
unsigned char dp_process(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  int received_power_state;
  int received_fan_speed;
  switch(dpid) {
    case DPID_POWER:
      received_power_state = my_device.mcu_get_dp_download_data(DPID_POWER, value, length);
      if (received_power_state == 0) {
        set_fan_speed(0, length);
        set_power(0, length);
      }
      else {
        set_power(1, length);
        set_fan_speed(1, length);
      }
      
    break;
      
    case DPID_FAN_SPEED:

      received_fan_speed = my_device.mcu_get_dp_download_data(DPID_FAN_SPEED, value, length);
      
      if (power_state == 0) {
        set_power(1, length);
        set_fan_speed(received_fan_speed, length);
      }
      else {
        set_fan_speed(received_fan_speed, length);
        
        if (received_fan_speed == 0) {
          set_power(0, length);
        }
        
      }
      
    break;
          
    default:break;
  }
  
  return TY_SUCCESS;
}

void set_power(char value, short length) {
  power_state = value;
  my_device.mcu_dp_update(DPID_POWER, power_state, length);
}

void set_fan_speed(char value, short length){
  fan_speed = value; 
  my_device.mcu_dp_update(DPID_FAN_SPEED, fan_speed, length);
  powerRelay();
}

void powerRelay() {
  
  for(unsigned int i = 0; i < sizeof(relay_pin_array); i++) {
     digitalWrite(relay_pin_array[i], HIGH);
     digitalWrite(speed_led_pin_array[i], LOW);
  }
  delay(10);
  
  if (fan_speed > 0) {
    digitalWrite(relay_pin_array[fan_speed-1], LOW);
    digitalWrite(speed_led_pin_array[fan_speed-1], HIGH);
  }
}

/**
 * @description: Upload all DP status of the current device.
 * @param {*}
 * @return {*}
 */
void dp_update_all(void)
{
  my_device.mcu_dp_update(DPID_POWER, power_state, 1);
  my_device.mcu_dp_update(DPID_FAN_SPEED, fan_speed, 4);
}
