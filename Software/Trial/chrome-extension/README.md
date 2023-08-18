## Shift Register (74HC595N):

- 74HC595N Shift Register is an 8-bit serial-in, parallel out shift register that feeds an 8-bit D-type storage register.
- The storage register has parallel 3- state outputs that are High, Low and High impedance state.
- Separate clocks are provided for both the shift and storage register.
- The shift register has a direct overriding clear (SRCLR) input, serial (SER) input where the data is to be sent along with a clock signal with the help of serial clock (SRCLK).
- When the output-enable (OE) input is high, the outputs are in the high-impedance state.
- As mentioned previously these shift registers contain 2 registers that is D-type storage register and shift register.
- The data in shift register is only sent to the storage register when the Storage Latch (RCLK according to datasheet) is high.

### Features:

- 8-bit serial-in, parallel-out shift
- Wide operating voltage range of 2 V to 6 V
- Low power consumption: 80-μA (maximum)
- ICC tpd = 13 ns (Propagation time delay) ±6-mA output drive at 5 V
- Low input current: 1 μA (maximum) Shift register has direct clear.

## CD74HC4067 16-Channel Multiplexer:

- The CD74HC4067 is a highly efficient high-speed CMOS logic integrated circuit designed specifically as a 16-channel analog multiplexer/demultiplexer.
- This sophisticated IC presents a powerful solution for seamlessly routing and switching multiple analog signals in a diverse range of professional electronic applications.
- By leveraging its compact form factor and reliable performance, the CD74HC4067 empowers project teams to streamline signal consolidation and distribution processes.
- The inhibit pin (Ē) of the multiplexer is a control pin which is used to disable or enable the operation of the IC.
- When the Ē is held high the IC is disabled, and it effectively disconnects all analog channels, preventing any signal routing or switching. Whereas when the pin is held low the IC is active and all the IC will work as intended.
- The inhibit pin temporarily halts the CD74HC4067's operation without external circuitry, allowing control by a microcontroller or digital source.

- The CD74HC4067 has 4 selection lines S0, S1, S2, S3 at pins 10, 11, 14, 13 of the IC respectively which are used to select the channel for input in case of demultiplexing and output in case of multiplexing.
- The below truth table shows the selection of channel with respect to selection lines.

### Features:

- Wide voltage range: Operates with voltages between 2V and 6V
- Low ON resistance
- Fast switching speed (typical delays of 10 ns)
- Break-before-make switching: Prevents momentary signal shorting during switching.
- Low power consumption
- Wide temperature range: Designed to operate reliably in temperatures from -40°C to 85°C.

---

    Functional Diagram 					Pin Layout

---

## IRF540 MOSFET

- The IRF540 is a n channel MOSFET which offers an excellent balance of fast switching, ruggedized device design, low on-resistance, and cost-effectiveness.
- It is a suitable choice for commercial and industrial applications with power dissipation levels of up to approximately 50 W.
- The IRF540 MOSFET is commonly housed in the TO-220AB package, which is widely preferred due to its low thermal resistance and cost-effectiveness.

### Features:

- Dynamic dV/dt rating
- Repetitive avalanche rated
- 175 °C operating temperature
- Fast switching
- Ease of paralleling
- Simple drive requirement

### Schematics:

- A schematic is a graphical representation of a circuit design that shows the relationships between different components and their connections. 
- In a printed circuit board (PCB), a schematic is used as the starting point for creating the physical layout of the board.

## Design:

- We have designed this project to be modular this is only possible as we are able to daisy chain shift registers and expand the outputs. 
- In the main control board, there are 8 shift registers, 2 for each colour and ground with a total of 4 channels. 
- With this design we can control 8x8 RGB matrix as this design is modular, we can attach more shift registers which can be further daisy chained to make a bigger matrix. 
- Each channel has 2 shift registers for 16 digital outputs. 
- But shift registers can only output 3-6V of power in each digital pin with 65mA max current hence we are using an external power supply which will trigger the LEDs with the help of IRF540 MOSFETs.

###### TODO: High Side Switching Low Side Switching

- As we can see in the below schematic the serial data out QH\* is the input of the second shift register and this is how daisy chaining is achieved. Serial clock, Latch, Master Clear, and Output Enable of each channel (in this case 2 shift registers) are common.
- This design support 7 colours as mentioned in below table a total of 8 LED states including off state.

###### TODO: IMAGE 

- Here Red colour is used to signify a dead node which is common for all nodes, In the initial version green colour is used to represent Wi-SUN FAN 1.0 FSK nodes, blue colour is used to represent Wi-SUN FAN 1.1 OFDM nodes, purple colour is used to represent Wi-SUN FAN 1.1 FSK nodes. 
- This ensures there is ability to add more verticals on the physical dashboard. 
- All of this can be controlled with 9 GPIO pins of the microcontroller (in this case ESP32 devkit module). 
- 4 GPIO pins are used to control the selection lines for the multiplexer IC.
We have designed another smaller module to add two more shift registers in an existing channel of any colour. We have designed this project to be modular and future-proof.

## Layout Guidelines:

- Place components carefully: Place components in a logical, organized manner that makes sense for the circuit. 
- Group related components together, such as power supply components, signal processing components, etc.
Keep traces as short as possible: short traces reduce the risk of interference and crosstalk, as well as signal loss. Plan the layout to minimize the length of the traces.
- Avoid sharp angles: Avoid sharp angles in traces whenever possible. Sharp angles can lead to reflections and cause signal quality issues.
- Use the right trace widths: Use appropriate trace widths for the signals being carried. 
- Narrower traces are sufficient for low-current signals, while wider traces are necessary for higher-current signals.
- Keep high-frequency traces short: High-frequency signals should be kept as short as possible to minimize the risk of interference and signal degradation.
- Keep analog and digital signals separate: Separate analog and digital signal paths to reduce noise coupling.
- Minimize ground loops: Minimize ground loops by keeping the ground plane as continuous as possible, and by routing high-current ground paths separately from low-current paths.

## Software Architecture:

- <b>[OneM2M]()</b> is a global organization that develops technical standards for machine-to-machine (M2M) and Internet of Things (IoT) communications. 
- It provides a common platform for various devices and applications to communicate with each other and share data in a standardized and interoperable manner, regardless of the underlying network technology. We use OM2M an open-source implementation of oneM2M standard.
- All the deployed nodes send data to OM2M and OM2M has a feature called Subscription which sends HTTP POST request to ESP32 Dev Module Microcontroller upon any new updates in OM2M and the ESP32 processes the POST request data. 
- In this data is then parsed to get the “con” field and the driver code finds the location of the node ID in node map which is a 2D matrix with node IDs and their corresponding position on the LED Matrix. This data from “con” contains node ID and node status (node status can contain 3 responses “on”, “off”, “dead”). 
- When the nodes status is “on” the corresponding LED will light up to corresponding colour, When the node status is “off” the corresponding LED will be turned off and finally when the node status is “dead” the correspond LED will show RED colour. 
- All the deployed nodes send the captured data to OM2M which is a middleware layer it also has database connectivity.

## Hardware Architecture:
The hardware architecture for a printed circuit board (PCB) refers to the physical design and arrangement of the components on the board, as well as the electrical and mechanical connections between them.

- Each 74HC595N Shift Register can provide 8 digital outputs (can only trigger between HIGH or LOW) each of digital outputs can only provide a maximum of 80mA. Hence, we are using a MOSFET as switch with a 24V power supply to provide current for all the 64 RGB LEDs.

---

Top View Bottom View

---


- The above PCB is logic board which houses all the ICs and microcontroller in this case ESP32. 
- The logic board consists of total 9 IC headers of which 8 IC headers are used to hold 74HC595N Shift registers and 1 IC header is used to hold CD74HC4067 16 Channel Multiplexer. 
- The output enable in the shift register can control the brightness of the leds upon sending PWM to it from the microcontroller. 
- Otherwise, we can just ground it (as it is a active low trigger) for this a small two way switch is used to toggle between PWM and ground.
The outputs of this logic board are connected to the led matrix board via 8 pin ribbon cable connected to 8 pin headers on each of boards. 
- Additionally, 7 pin headers on the top of the logic boards serve to extend the outputs of the shift registers to make use of the daisy chaining feature of the 74HC595N shift register to expand more outputs.

#### DATA

                    Top View 		               	       Bottom View

---

The above images are led matrix board which houses all the connectors for the LEDs and their driving circuit. 
- IRF540 MOSFETs are used for switching LED power and a 31-ohm resistance is used to control the current which is connected in common for each line and it also has a capacitor which help with switching times. 
- Additionally, we added four of 8-pin headers for extending their outputs for each control lines i.e., ground, red, blue, green. 
- We are implementing 2 types of switching i.e., High Side Switching and Low Side Switching with the help of IRF540 MOSFET as mentioned above.

- SR0_1 is the input for the MOSFET for Low side switching, signal is received from shift register output for ground channel, RED_M1 is the input for the MOSFET for high side switching received from shift register output for red channel, GR_M1 is the input for the MOSFET for high side switching received from shift register output for green channel, BL_M1 is the input for the MOSFET for high side switching received from shift register output for blue channel.

## Cost and Time Approximation:
### For Wi-SUN dashboard: -
- The LED Matrix Board has dimensions of 162.87 x 141.66 mm and consists of two layers and the approximate cost is Rs. 5367 INR from pcbpower.com. The PCB preparation time is about 1 week (7 days) and approximate delivery time is 2-3 days.
- The Logic board has dimensions of 97.88 x 91.16 mm and consists of two layers and the approximate cost is Rs. 3136 INR from pcbpower.com. The PCB preparation time is about 1 week (7 days) and approximate delivery time is 2-3 days.
### For All nodes deployment dashboard: -

- The number of LEDs and header pins would be increased to 300. So, we would be needed to use 4 LED matrix boards and 4 Shift Register Extender Modules to increase the outputs. And the printing of the all-nodes dashboard would cost around Rs. 3000 INR (based on current dashboard costs, could be more according to design) and would take 2-3 days for printing and delivery.
- The LED matrix board has dimensions of 162.87 x 141.66 mm and consists of two layers and the approximate cost is Rs. 6580 INR from pcbpower.com. The PCB preparation time is about 1 week (7 days) and approximate delivery time is 2-3 days. We would require 4 of these Modules to control 256 LEDs. We would require 4 of these Modules to control 256 LEDs.
- The Logic board has dimensions of 97.88 x 91.16 mm and consists of two layers and the approximate cost is Rs. 3136 INR from pcbpower.com. The PCB preparation time is about 1 week (7 days) and approximate delivery time is 2-3 days.

Estimated Assembly time for completion the project is 3-4 days.

## Bill of Materials Required: -
| S.No | Component name | Operating Voltage | Quantity | Price | REF |
| ------ | ---------------- | ------------------ | ----------- | -------- | ----- |
| 1 | 74HC595N | 2V - 6V | 8 |  |  
| 2 | 16 pin IC header | NA | 8 |  |  
| 3 | CD74HC4067 IC | 2V - 6V | 1 |  |  
| 4 | 24 pin header for CD74HC4067 | NA | 1 |  |  
| 5 | RGB LEDs | 3.3V / 10-20mA | 64 |  |  | 6 | 8 pin male header | NA | 16 |  |  
| 7 | 7 pin male header | NA | 12 |  |  
| 8 | 4 pin headers for leds | NA | 64 | SV1-SV64 |  
| 9 | 4 pin female to female connectors | NA |  
| 10 | 8 pin female to female connectors | NA | 8 |  |  
| 11 | 7 pin female to female connectors | NA | 4 |  |  
| 12 | Resistors | 35 ohms | 24 | R1- R24 |  
| 13 | 19 pin header female | NA | 2 |  
| 14 | 0.1uF capacitor | 24 |  | C1 - C24 |  
| 15 | 2 pin male header | NA | 1 |  |  
| 16 | Power Supply 5V 2Amps | 5V 2A | 1 
| 17 | IRF540 MOSFETS 100V/24A | 24 |  | Q1-Q32 |  
| 18 | PCB (logic board) | 2 |  | 2667 |  
| 19 | PCB (led matrix ) | 2 |  | 4180 |  
| 20 | all deployments Dashboard Print | 1 |  | 3500 |


