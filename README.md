# smart-dumbbell
Real-time workout classification using Arduino Nano 33 BLE Sense, Edge Impulse, and Web Bluetooth.

## Details

The Smart Dumbbell can classify four types of movements namely Idle, Biceps Curl, Lateral Raise, and Overhead Press. A confidence score is also displayed showing the model's confidence in the prediction. The only hardware required is an Arduino Nano 33 BLE Sense and a battery attached to a dumbbell. For more details, see the <a href="https://github.com/Attaulhaleem/smart-dumbbell/blob/main/doc/Report.pdf">documentation</a>.

---

### Edge Impulse

The data collection, model training and testing was done using Edge Impulse Studio. The Arduino code relies on the `smartbell_inferencing` library which contains the TinyML model. The library and other project data can be accessed [here](https://studio.edgeimpulse.com/public/161663/latest).

---

### Dashboard

:warning: Web Bluetooth is an experimental technology and will only work on <a href="https://developer.mozilla.org/en-US/docs/Web/API/Web_Bluetooth_API#browser_compatibility">compatible browsers</a>.

<a href="https://github.com/Attaulhaleem/smart-dumbbell/blob/main/Web Bluetooth/app/index.html">
  <img src="https://github.com/Attaulhaleem/smart-dumbbell/blob/main/doc/dashboard.png" align="center"/>
</a>

---

### Acknowledgements

The idea for the Smart Dumbbell came from [this project](https://microchipdeveloper.com/machine-learning:smartbell-with-edge-impulse).
