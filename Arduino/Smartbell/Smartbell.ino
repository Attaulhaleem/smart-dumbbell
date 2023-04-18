#include <smartbell_inferencing.h>
#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>

static bool displaySerial = false;

// custom UUIDs generated from: https://www.uuidgenerator.net/
BLEService smartbellService("d9fa4402-bd97-4242-8340-65bbdcbb02e6");
BLEUnsignedShortCharacteristic labelCharacteristic("1762594c-5e6b-4361-b474-3db93ffbc7f9", BLERead | BLENotify);  // 0 -> Cannot Identify, 1 -> Biceps Curl, 2 -> Lateral Raise, 3 -> Overhead Press, 4 -> Idle
BLEUnsignedShortCharacteristic confidenceCharacteristic("2fa176ff-77a1-46f0-8837-53278fe15fbc", BLERead | BLENotify);  // confidence percentage of label (0-100)

// Status of sensor
enum sensorStatus 
{
    NOT_INIT,
    INIT,
    SAMPLED
};

// CONSTANTS
#define N_SENSORS             3         // number of sensor axes used (ax, ay, az)
#define CONVERT_G_TO_MS2      9.80665f
#define MAX_ACCEPTED_RANGE    2.0f
#define CONFIDENCE_THRESHOLD  0.7f

// Global variables
static float data[N_SENSORS];
unsigned long previousMillis = 0;
sensorStatus accelerometerStatus = NOT_INIT;

// Function declarations
void startBLE();
float getSign(float number);
void pollAccelerometer();
unsigned short getConfidencePercentage(float f);
unsigned short getIdFromLabel(String s);

void setup()
{
  if (displaySerial)
  {
    Serial.begin(115200); // for debugging
    while (!Serial);
  }
  
  startBLE();

  if(IMU.begin())
    accelerometerStatus = INIT;
  else if (displaySerial)
    ei_printf("Failed to start IMU.\r\n");
}

void loop()
{
  BLEDevice central = BLE.central();  // listen for central devices

  while (central.connected()) // while central connected to peripheral
  {
    digitalWrite(LED_BUILTIN, HIGH);  // turn LED on to show connected status

    if (displaySerial)
      ei_printf("Starting inference in 2 seconds...\r\n");

    if (millis() - previousMillis > 2000)
    {      
      previousMillis = millis();
  
      if (displaySerial)
        ei_printf("Sampling...\r\n");
      
      float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 }; // Allocate buffer for values from sensor

      for (size_t i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i += EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME) 
      {
        int64_t next_tick = (int64_t)micros() + ((int64_t)EI_CLASSIFIER_INTERVAL_MS * 1000);  // Determine the next tick (and then sleep later)
    
        if (accelerometerStatus == INIT)
        {
          pollAccelerometer();
          accelerometerStatus = SAMPLED;
        }
    
        if (accelerometerStatus == SAMPLED)
        {
          for (int j = 0; j < N_SENSORS; j++)
            buffer[i + j] = data[j];
            
          accelerometerStatus = INIT;
        }
    
        int64_t wait_time = next_tick - (int64_t)micros();
    
        if(wait_time > 0)
          delayMicroseconds(wait_time);
      }

      signal_t signal;
      int err = numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal); // Turn the raw buffer in a signal which we can the classify
      
      if (err) 
      {
        if (displaySerial)
          ei_printf("Error: %d", err);
          
        return;
      }
  
      ei_impulse_result_t result = { 0 };
      err = run_classifier(&signal, &result); // Run the classifier
      
      if (err != EI_IMPULSE_OK) 
      {
        if (displaySerial)
          ei_printf("Error: %d", err);
          
        return;
      }
  
      // Print the predictions
      if (displaySerial)
        ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.):\r\n",
                  result.timing.dsp, result.timing.classification, result.timing.anomaly);

      bool labelFound = false;
  
      for (size_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++)
      {
        const char* label = result.classification[i].label;

        if (displaySerial)
          ei_printf("%s: %.5f\r\n", result.classification[i].label, result.classification[i].value);
  
        if (result.classification[i].value > CONFIDENCE_THRESHOLD)
        {
          labelFound = true;
          labelCharacteristic.writeValue(getIdFromLabel(result.classification[i].label));
          confidenceCharacteristic.writeValue(getConfidencePercentage(result.classification[i].value));
        }
      }

      if (!labelFound)
      {
        labelCharacteristic.writeValue(0);  // Cannot Identify
        confidenceCharacteristic.writeValue(0);
      }
    }
  }
  digitalWrite(LED_BUILTIN, LOW); // turn LED off to show disconnected status
}

float getSign(float number)
{
  return (number >= 0.0) ? 1.0 : -1.0;
}

void pollAccelerometer()
{  
  if (IMU.accelerationAvailable()) 
  {
    IMU.readAcceleration(data[0], data[1], data[2]);

    for (int i = 0; i < 3; i++)
      if (fabs(data[i]) > MAX_ACCEPTED_RANGE)
        data[i] = getSign(data[i]) * MAX_ACCEPTED_RANGE;

    data[0] *= CONVERT_G_TO_MS2;
    data[1] *= CONVERT_G_TO_MS2;
    data[2] *= CONVERT_G_TO_MS2;
  }
}

unsigned short getIdFromLabel(String s)
{
  if (s == "Bicep Curl")
    return 1;
  
  if (s == "Lateral Raise")
    return 2;
  
  if (s == "Overhead Raise")
    return 3;

  if (s == "Idle")
    return 4;

  return 0;
}

unsigned short getConfidencePercentage(float f)
{
  return round(f * 100);
}

void startBLE()
{
  while (!BLE.begin()); // wait for BLE to start
  BLE.setLocalName("Smartbell");  // set advertised local name
  BLE.setAdvertisedService(smartbellService);  // set service UUID
  // add characteristics to service
  smartbellService.addCharacteristic(labelCharacteristic);
  smartbellService.addCharacteristic(confidenceCharacteristic);
  BLE.addService(smartbellService);  // add service
  // set initial values for characteristic
  labelCharacteristic.setValue(0);
  confidenceCharacteristic.setValue(0);
  BLE.advertise();  // start advertising
}
