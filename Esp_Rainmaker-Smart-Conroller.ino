#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
#include <SimpleTimer.h>
#include <wifi_provisioning/manager.h>

// Set Defalt Values
#define DEFAULT_LED_MODE true
#define DEFAULT_LOCK_ONE_MODE true
#define DEFAULT_LOCK_TWO_MODE true

// BLE Credentils
const char *service_name = "RainMakerBLE";
const char *pop = "1234567";

// GPIO
static uint8_t gpio_reset = 0;
static uint8_t led = 12;
static uint8_t lock_one = 14;
static uint8_t lock_two = 13;
bool lock_one_state = true;
bool lock_two_state = true;
bool led_state = true;

bool wifi_connected = 0;


SimpleTimer Timer;

//------------------------------------------- Declaring Devices -----------------------------------------------------//

//The framework provides some standard device types like switch, lightbulb, fan, temperature sensor.
static Switch my_switch("LED", &led);
static Switch my_switch_one("BEDROOM", &lock_one);
static Switch my_switch_two("KITCHEN", &lock_two);

void sysProvEvent(arduino_event_t *sys_event)
{
  switch (sys_event->event_id) {
    case ARDUINO_EVENT_PROV_START:
#if CONFIG_IDF_TARGET_ESP32
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on BLE\n", service_name, pop);
      printQR(service_name, pop, "ble");
#else
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on SoftAP\n", service_name, pop);
      printQR(service_name, pop, "softap");
#endif
      break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.printf("\nConnected to Wi-Fi!\n");
      wifi_connected = 1;
      delay(500);
      break;
    case ARDUINO_EVENT_PROV_CRED_RECV: {
        Serial.println("\nReceived Wi-Fi credentials");
        Serial.print("\tSSID : ");
        Serial.println((const char *) sys_event->event_info.prov_cred_recv.ssid);
        Serial.print("\tPassword : ");
        Serial.println((char const *) sys_event->event_info.prov_cred_recv.password);
        break;
      }
  }
}

void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx)
{
  const char *device_name = device->getDeviceName();
  Serial.println(device_name);
  const char *param_name = param->getParamName();

  if (strcmp(device_name, "LED") == 0)
  {
    if (strcmp(param_name, "Power") == 0)
    {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      led_state = val.val.b;
      (led_state == false) ? digitalWrite(led, HIGH) : digitalWrite(led, LOW);
      param->updateAndReport(val);
    }
  }
  if (strcmp(device_name, "BEDROOM") == 0)
  {
    if (strcmp(param_name, "Power") == 0)
    {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      lock_one_state = val.val.b;
      (lock_one_state == false) ? digitalWrite(lock_one, HIGH) : digitalWrite(lock_one, LOW);
      param->updateAndReport(val);
    }
  }
  if (strcmp(device_name, "KITCHEN") == 0)
  {
    if (strcmp(param_name, "Power") == 0)
    {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      lock_two_state = val.val.b;
      (lock_two_state == false) ? digitalWrite(lock_two, HIGH) : digitalWrite(lock_two, LOW);
      param->updateAndReport(val);
    }
  }
  
}

void setup()
{

  Serial.begin(115200);

  // Configure the input GPIOs
  pinMode(gpio_reset, INPUT);
  pinMode(led, OUTPUT);
  pinMode(lock_one, OUTPUT);
  pinMode(lock_two, OUTPUT);
  digitalWrite(led, DEFAULT_LED_MODE);
  digitalWrite(lock_one, DEFAULT_LOCK_ONE_MODE);
  digitalWrite(lock_two, DEFAULT_LOCK_TWO_MODE);

  
  

  //------------------------------------------- Declaring Node -----------------------------------------------------//
  Node my_node;
  my_node = RMaker.initNode("ESP RainMaker");

  //Standard switch device
  my_switch.addCb(write_callback);
  my_switch_one.addCb(write_callback);
  my_switch_two.addCb(write_callback);

  //------------------------------------------- Adding Devices in Node -----------------------------------------------------//
  my_node.addDevice(my_switch);
  my_node.addDevice(my_switch_one);
  my_node.addDevice(my_switch_two);

  //This is optional
  RMaker.enableOTA(OTA_USING_PARAMS);
  //If you want to enable scheduling, set time zone for your region using setTimeZone().
  //The list of available values are provided here https://rainmaker.espressif.com/docs/time-service.html
  // RMaker.setTimeZone("Asia/Shanghai");
  // Alternatively, enable the Timezone service and let the phone apps set the appropriate timezone
  RMaker.enableTZService();
  RMaker.enableSchedule();

  Serial.printf("\nStarting ESP-RainMaker\n");
  RMaker.start();

  // Timer for Sending Sensor's Data
  Timer.setInterval(3000);

  WiFi.onEvent(sysProvEvent);

#if CONFIG_IDF_TARGET_ESP32
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
#else
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
#endif

}

void loop()
{

  


  //-----------------------------------------------------------  Logic to read button

  // Read GPIO0 (external button to reset device
  if (digitalRead(gpio_reset) == LOW) 
  { //Push button pressed
    Serial.printf("Reset Button Pressed!\n");
    // Key debounce handling
    delay(100);
    int startTime = millis();
    while (digitalRead(gpio_reset) == LOW) delay(50);
    int endTime = millis();
  

    if ((endTime - startTime) > 10000) 
    {
      // If key pressed for more than 10secs, reset all
      Serial.printf("Reset to factory.\n");
      wifi_connected = 0;
      RMakerFactoryReset(2);
    } 
    else if ((endTime - startTime) > 3000) {
      Serial.printf("Reset Wi-Fi.\n");
      wifi_connected = 0;
      // If key pressed for more than 3secs, but less than 10, reset Wi-Fi
      RMakerWiFiReset(2);
    } 
    else 
    {
      // Toggle device state
      led_state = !led_state;
      Serial.printf("Toggle State to %s.\n", led_state ? "true" : "false");
      my_switch.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, led_state);
      (led_state == false) ? digitalWrite(led, HIGH) : digitalWrite(led, LOW);
    }
  }
  delay(100);
}

