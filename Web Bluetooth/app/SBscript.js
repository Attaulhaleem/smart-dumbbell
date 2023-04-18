const controlButton = document.getElementById("controlButton");
const deviceNameInput = document.getElementById("deviceNameInput");
const connectionStatus = document.getElementById("connectionStatus");
const exerciseName = document.getElementById("exerciseName");
const confidencePercentage = document.getElementById("confidencePercentage");

const serviceUUID = "d9fa4402-bd97-4242-8340-65bbdcbb02e6";
const labelCharacteristicUUID = "1762594c-5e6b-4361-b474-3db93ffbc7f9";
const confidenceCharacteristicUUID = "2fa176ff-77a1-46f0-8837-53278fe15fbc";

controlButton.addEventListener("click", BLEManager);

async function BLEManager() {

    connectionStatus.textContent = "Searching...";

    try {
        const device = await navigator.bluetooth.requestDevice({
            filters: [
                { name: deviceNameInput.value },
                { namePrefix: 'Arduino' },
                { namePrefix: 'Smartbell' },
                { services: [serviceUUID] }
            ],
            // optionalServices
        });

        const connectedDevice = await device.gatt.connect()
        connectionStatus.textContent = "Connected";

        const smartbellService = await connectedDevice.getPrimaryService(serviceUUID);

        const labelCharacteristic = await smartbellService.getCharacteristic(labelCharacteristicUUID);
        labelCharacteristic.startNotifications();
        labelCharacteristic.addEventListener('characteristicvaluechanged', handleLabelNotifications);

        const confidenceCharacteristic = await smartbellService.getCharacteristic(confidenceCharacteristicUUID);
        confidenceCharacteristic.startNotifications();
        confidenceCharacteristic.addEventListener('characteristicvaluechanged', handleConfidenceNotifications);
    }
    catch {
        connectionStatus.textContent = "Not Connected";
    }
}

function handleLabelNotifications(event) {
    let labelValue = event.target.value;
    const labelId = labelValue.getUint8(0);

    switch (labelId) {
        case 1:
            exerciseName.textContent = "Bicep Curl";
            break;
        case 2:
            exerciseName.textContent = "Lateral Raise";
            break;
        case 3:
            exerciseName.textContent = "Overhead Press";
            break;
        case 4:
            exerciseName.textContent = "Idle";
            break;
        default:
            exerciseName.textContent = "Cannot Identify";
    }
}

function handleConfidenceNotifications(event) {
    let confidenceValue = event.target.value;
    const confidenceNumber = confidenceValue.getUint8(0);

    if (confidenceNumber == 0)
        confidencePercentage.textContent = "-";
    else
        confidencePercentage.textContent = confidenceNumber.toString() + "%";
}