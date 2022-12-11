const controlButton = document.getElementById("controlButton");
const deviceNameInput = document.getElementById("deviceNameInput");
const connectionStatus = document.getElementById("connectionStatus");
const exerciseName = document.getElementById("exerciseName");

const serviceUUID = "d9fa4402-bd97-4242-8340-65bbdcbb02e6";
const characteristicUUID = "1762594c-5e6b-4361-b474-3db93ffbc7f9";

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
                { services: [serviceUUID] }
            ],
            // optionalServices
        });

        const connectedDevice = await device.gatt.connect()
        connectionStatus.textContent = "Connected";

        const smartbellService = await connectedDevice.getPrimaryService(serviceUUID);
        const exerciseCharacteristic = await smartbellService.getCharacteristic(characteristicUUID);
        exerciseCharacteristic.startNotifications();
        exerciseCharacteristic.addEventListener('characteristicvaluechanged', handleNotifications);
    }
    catch {
        connectionStatus.textContent = "Not Connected";
    }
}

function handleNotifications(event) {
    let exerciseValue = event.target.value;
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