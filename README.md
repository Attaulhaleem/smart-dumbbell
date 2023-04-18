# smart-dumbbell
Real-time workout classification using Arduino Nano 33 BLE Sense, Edge Impulse, and Web Bluetooth.

## Details

The Smart Dumbbell can classify four types of movements namely Idle, Biceps Curl, Lateral Raise, and Overhead Press. A confidence score is also displayed showing the model's confidence in the prediction. The only hardware required is an Arduino Nano 33 BLE Sense and a battery attached to a dumbbell. For more details, see the [documentation](https://github.com/Attaulhaleem/smart-dumbbell/blob/main/docs/Report.pdf).

---

### Edge Impulse

The data collection, model training and testing was done using Edge Impulse Studio. The project can be accessed [here](https://studio.edgeimpulse.com/public/161663/latest).

---

### Dashboard

:warning: Web Bluetooth is an experimental technology and will only work on [compatible browsers](https://developer.mozilla.org/en-US/docs/Web/API/Web_Bluetooth_API#browser_compatibility).

![Smartbell Dashboard](https://github.com/Attaulhaleem/smart-dumbbell/blob/main/docs/dashboard.png)

---

### Acknowledgements

The idea for the Smart Dumbbell came from [this project](https://microchipdeveloper.com/machine-learning:smartbell-with-edge-impulse).
