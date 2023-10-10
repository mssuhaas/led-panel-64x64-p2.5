# Smart Display

The LED Matrix is 64x64 RGB 2.5mm pitch which is interfaced by **HUB75E** connectors.
The display is controlled by a ESP32 dev module. This display provides live information from oneM2M.

NOTE: Display we used is 1/32 scan, so to use implement that we need to use

```cpp
Display.begin(32); // Replace this 32 in case there is different LED Panel
```

The ESP32 dev module is connected to the led display using HUB75E and buzzer when there is a notification.

| PI  | PO  |
| --- | --- |
| R2  | R1  |
| G1  | R2  |
| G2  | G1  |
| B1  | G2  |
| B2  | B1  |

Connect panel input (PI) to the ESP8266 / ESP32 as follows:

| PI          | ESP32 GPIO |
| ----------- | ---------- |
| **A**       | 19         |
| **B**       | 23         |
| **C**       | 18         |
| **D**       | 5          |
| **E**       | 15         |
| **STB/LAT** | 22         |
| **P_OE**    | 21         |
| **CLK**     | 14         |
| **R1**      | 13         |

And buzzer connected to Pin 0 of the ESP32.

### RESOURCES

- [Link to Github of PxMatrix Library](https://github.com/2dom/PxMatrix)
