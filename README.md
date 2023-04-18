# smart-dumbbell
Real-time workout classification using Arduino Nano 33 BLE Sense, Edge Impulse, and Web Bluetooth.

## Details

The Smart Dumbbell can classify four types of movements namely Idle, Biceps Curl, Lateral Raise, and Overhead Press. A confidence score is also displayed showing the model's confidence in the prediction. The only hardware required is an Arduino Nano 33 BLE Sense and a battery attached to a dumbbell. For more details, see the <a href="https://github.com/Attaulhaleem/smart-dumbbell/blob/main/docs/Report.pdf">documentation</a>.

---

### Edge Impulse

The data collection, model training and testing was done using Edge Impulse Studio. The project can be accessed <a href="https://studio.edgeimpulse.com/public/161663/latest">here</a>.

---

### Dashboard

:warning: Web Bluetooth is an experimental technology and will only work on <a href="https://developer.mozilla.org/en-US/docs/Web/API/Web_Bluetooth_API#browser_compatibility">compatible browsers</a>.

<a href="https://github.com/Attaulhaleem/smart-dumbbell/blob/main/Web Bluetooth/app/index.html">
  <img src="https://github.com/Attaulhaleem/smart-dumbbell/blob/main/docs/dashboard.png" align="center"/>
</a>

---

### Acknowledgements

The Smart Dumbbell was largely inspired by <a href="https://microchipdeveloper.com/machine-learning:smartbell-with-edge-impulse">this project</a>.
<a href="https://www.coursera.org/learn/introduction-to-embedded-machine-learning">This course</a> was instrumental in completing this project.
