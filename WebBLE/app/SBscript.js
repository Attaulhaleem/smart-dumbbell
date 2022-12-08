const controlButton = document.getElementById("controlButton");
const deviceNameInput = document.getElementById("deviceNameInput");
const connectionStatus = document.getElementById("connectionStatus");

controlButton.addEventListener("click", BLEManager);

async function BLEManager() {

    connectionStatus.textContent = "Searching...";

    try {
        const device = await navigator.bluetooth.requestDevice({
            filters: [
                { name: deviceNameInput.value },
                { namePrefix: 'Arduino' },
                { namePrefix: 'BatteryMonitor' },
                { namePrefix: 'Smartbell' }
                // services: ['heart_rate']
            ],
            // optionalServices: ['battery_service']
            // mention any services needed
        });

        const connectedDevice = await device.gatt.connect()
        connectionStatus.textContent = "Connected";

        // get services
        // const batteryService = await connectedDevice.getPrimaryService("battery_service");
        // const heartRateService = await connectedDevice.getPrimaryService("heart_rate");

        // or
        // const batteryLevelCharacteristic = await batteryService.getCharacteristic(0x2A19);
        // const heartRateCharacteristic = await heartRateService.getCharacteristic(0x2A37);

        // read characteristic
        // const batteryLevel = await batteryLevelCharacteristic.readValue();
        // const batteryPercent = batteryLevel.getUint8(0);
        // batteryCharge.textContent = batteryPercent.toString() + "%";
    }
    catch {
        if (typeof device !== 'undefined') {
            connectionStatus.textContent = "Connection Failed";
        }
        else {
            connectionStatus.textContent = "Cancelled"
        }
    }
}