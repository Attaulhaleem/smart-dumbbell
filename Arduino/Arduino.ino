#include <ArduinoBLE.h>

// define custom BLE service
// UUID generated from: https://www.uuidgenerator.net/
BLEService smartbellService("d9fa4402-bd97-4242-8340-65bbdcbb02e6");

// define service characteristic of unsigned short type
// 0 --> Unidentified, 1 --> Biceps Curl, 2 --> Lateral Raise, 3 --> Overhead Press
BLEUnsignedShortCharacteristic exerciseCharacteristic("1762594c-5e6b-4361-b474-3db93ffbc7f9", BLERead | BLENotify);

void setup()
{
  // wait for BLE to start
  while (!BLE.begin());

  // set advertised local name
  BLE.setLocalName("Smartbell");

  // set service UUID
  BLE.setAdvertisedService(smartbellService);

  // add characteristic to service
  smartbellService.addCharacteristic(exerciseCharacteristic);

  // add service
  BLE.addService(smartbellService);

  // set initial value for characteristic
  exerciseCharacteristic.setValue(0);

  // start advertising
  BLE.advertise();
}

void loop()
{
  // listen for central devices
  BLEDevice central = BLE.central();

  // if central connected to peripheral
  if (central)
  {
    // turn LED on to show connected status
    digitalWrite(LED_BUILTIN, HIGH);

    // for duration of connection
    while (central.connected())
    {
      unsigned short x = rand() % 4;
      // infer
      // convert to x
      // x = 0 (none), 1 (bicep), 2 (lateral), 3 (overhead)
      // write to characteristic

      // write characteristic only if it changed
      if (exerciseCharacteristic.value() != x)
        exerciseCharacteristic.writeValue(x);
    }

    // turn LED off to show disconnected status
    digitalWrite(LED_BUILTIN, LOW);
  }
}
