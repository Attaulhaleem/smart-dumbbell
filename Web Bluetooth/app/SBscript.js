const controlButton = document.getElementById("controlButton");
const deviceNameInput = document.getElementById("deviceNameInput");
const connectionStatus = document.getElementById("connectionStatus");
const exerciseName = document.getElementById("exerciseName");

controlButton.addEventListener("click", BLEManager);

async function BLEManager() {

    connectionStatus.textContent = "Searching...";

    try {
        const device = await navigator.bluetooth.requestDevice({
            filters: [
                { name: deviceNameInput.value },
                { namePrefix: 'Arduino' },
                { namePrefix: 'BatteryMonitor' },
                { namePrefix: 'Smartbell' },
                { services: ["1762594c-5e6b-4361-b474-3db93ffbc7f9"] }
                // services: ['heart_rate']
            ],
            // optionalServices
        });

        const connectedDevice = await device.gatt.connect()
        connectionStatus.textContent = "Connected";

        // get service
        const smartbellService = await connectedDevice.getPrimaryService("d9fa4402-bd97-4242-8340-65bbdcbb02e6");

        // get characteristic
        const exerciseCharacteristic = await smartbellService.getCharacteristic("1762594c-5e6b-4361-b474-3db93ffbc7f9");

        // read characteristic
        const exerciseValue = await exerciseCharacteristic.readValue();
        const exerciseNumber = exerciseValue.getUint8(0);

        switch (exerciseNumber) {
            case 1:
                exerciseName.textContent = "Biceps Curl";
                break;
            case 2:
                exerciseName.textContent = "Lateral Raise";
                break;
            case 3:
                exerciseName.textContent = "Overhead Press";
                break;
            default:
                exerciseName.textContent = "Cannot Identify"
        }
    }
    catch {
        connectionStatus.textContent = "Not Connected";
    }
}