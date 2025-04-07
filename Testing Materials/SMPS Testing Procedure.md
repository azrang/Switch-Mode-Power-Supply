# General Notes
- General convention: 
	- Refdes will be displayed as **BOLD**
	- Test points (which are also typically net names) will be displayed as `NET_NAME`
	- MCU pins will also be displayed as *MCU Pins*.
	- Warning will be displayed as ***LIKE THIS***.
- All measurements will be done with 100x probe.
- For Assembly, cut stencil up based on assembly order:
	- ***IF INTERACTING WITH LIVE CIRCUIT, WEAR HIGH VOLTAGE GLOVES***
- ***MINIMIZE*** the amount of interactions you have with live circuits and be wary of ***ARCS***.
- When soldering on components, make sure the circuit is disconnected.
- ***DO NOT TOUCH CAPACITORS ON THE CIRCUIT***
	- If you need to touch them for any reason, wait for them to discharge (based on the ***universal wait time*** which is determined below).
- ***DO NOT UPLOAD CODE TO THE ESP32 WHEN THE BOARD ROCKER SWITCH IS ON.***
	- Best practice is to take off the ESP32, upload the code, and then put it on.
        - Test scripts can be found in the Firmware/Test Scripts directory.

# 1. Input Circuit
1. This is to verify the input circuit does rectify the AC input.
2. Solder **BR1, F1, F2, J1, R15, R16, R99** and test points for `PRI_GND` & `UNREG_DC`.
	- Place a 12A FB fuse across **F1** & **F2**.
3. Test the input:
	- AC output on `UNREG_DC` with rocker switch OFF (no output)
	- AC output on `UNREG_DC` with rocker switch ON (Expected Full-Wave), recording the Vpp.
4. Desolder **BR1** and solder on **D1, D2, D3, D4**.
5. Test the input:
	- AC output on `UNREG_DC` with rocker switch OFF (no output)
	- AC output on `UNREG_DC` with rocker switch ON (Expected Full-Wave), recording the Vpp.
6. If the AC output for `UNREG_DC` has higher Vpp, then keep **D1, D2, D3, D4** on and move to step 6. Otherwise, desolder **D1, D2, D3, D4**, and solder on **BR1**. 
	- Verify the AC Output for `UNREG_DC` is the same as before if using **BR1** before continuing on.
7. Solder on **C7, C8, C9, C10, C11**.
8. Test the filtered input:
	- AC output on `UNREG_DC` (Expected near DC output, record ripple).
		- Can swap out capacitors with bigger value alternative capacitors from JLab to see if the rippling can be reduced significantly.
	- Check capacitor voltage is 0V after the rocker switch is OFF for 120 seconds.
		- This is the ***universal wait time*** for capacitors to discharge.
		- Note: Might need to wait longer if the capacitance was increased. 

# 2. Independent Channel A 
## Channel A LVDD
1. This is to test the `A_5V` & `A_3V3` rail.
2. Solder on **C1, C2, C3, C4, C5, C12, D41, L1, L5, R94, U2** and test points for `A_GND`, `A_5V`.
3. Check the `A_5V` rail.
	- DC output on `A_5V` (ripple less than 200mV) & the red LED is ON.
	- Verify `PRI_GND` & `A_GND` are isolated (using an ohmmeter). If not, go to step 4.
	- If output is clean, can skip to step 6. If not, go to step 4.
4. Desolder **C1, C3, C4, C5, C12, L1, L5** and solder on **R11, R12**.
5. Check the `A_5V` rail.
	- DC output on `A_5V` (ripple less than 200mV).
	- Verify `PRI_GND` & `A_GND` are isolated (using a voltmeter).
	- If output is cleaner, move on. If output is worst compared to before, revisit step 2.
		- If `GND`'s are now disconnected but worse output, LEAVE IT AS IS, and move on.
6. Solder on **C6, D40, U1, R93** & test point for `A_3V3`.
7. Check the `A_3V3` rail.
	- DC output on `A_3V3` (ripple less than 33mV) & the yellow LED is ON.

## Channel A ESP32
1. This is to test the ESP32 is powered on. 
2. Solder on **J7, J8**, or the header pins for the ESP32 MCU A.
3. Place the ESP32 on following the orientation on the board.
	- Verify the LED on the ESP32 module turns on.
4. Solder on **Q5, Q7, R17, R18** and test points `BUCK_A_nPWM`, `BUCK_A_PWM`, & `OUT_A_LV`.
5. Determine the accuracy of PWM duty cycle & logical level shift voltage levels.
	- Run the Channel_A_ESP32_PWM.cpp file.
		- Connect the `OUT_A_LV` to `A_5V` & measure this voltage.
		- Set the PWM to 10kHz with 10% duty cycle and measure how accurate the duty cycle is on the `BUCK_A_PWM` and `BUCK_A_nPWM` (should be 1 - duty cycle) & record the difference. This accuracy will be denoted as the *percent accuracy*.
			- Make sure that the logical levels for these circuits switches between `A_GND` & `A_5V`. 
		- Run these tests again in increasing order of 10kHz until you reach 50kHz and measure the *percent accuracy* for each frequency.

## Channel A External Connections Part 1
1. This is testing the input button & LCD screen for Channel A.
2. Solder on external JST connectors, **R5 & R7** for Channel A labelled **LCD_A, BUTTON_A**.
3. Test to make sure these connections work.
	- Run the Channel_A_Ext_Conn_1.cpp file.
		- Check to make sure the input from Button A is read properly on the LCD screen.

## Channel A Temperature Sensor
1. This is to test the temperature sensor.
2. Solder on **C37, R46, R47, U7**.
3. Solder on $50k\Omega$ for **R54**.
	- Run the Channel_A_Temp_Sense.cpp file.
		- Make sure the temperature sensor sends HIGH to *A_TMP_SNSE* at room temperature with this displayed on the LCD.
		- Make sure the temperature sensor sends a LOW signal with hot air gun at $55^{\circ}C$ (can set it up to $65^{\circ}C$) aimed at the temperature sensor and this is displayed on the LCD.
			- The temperature sensor should go off at around $53^{\circ}C$.
4. Determine what an appropriate resistor value for **R54** is to determine a temperature cut off.
	- Few choices here. Recommend using a lower temperature threshold first before moving onto higher threshold. Note this resistor value (will be used for Channel B later on).
		- $50k\Omega$ for $53^\circ C$ threshold
		- $30k\Omega$ for $79^{\circ}C$ threshold
		- $15k\Omega$ for $100^{\circ}C$ threshold

## Channel A Transformers
1. This is to test the flyback & feedback transformer.
2. Solder on **C24, C25, C26, Q29, Q35, Q38, Q39, R61, R70, R101, T2, T4** and test points `FLY_OUT_A`, `PRI_FET_A` & `SEC_FET_A`.
3. Check the flyback transformer outputs VDC at `FLY_OUT_A`.
	- Run the Channel_A_Transformer1.cpp file.
		- With the MCU, send a fixed PWM at 10kHz with a duty cycle of 25% on *A_DRIVE* and record the VDC at `FLY_OUT_A` (should be approximately 10.9V $\pm$ 2V) & record how long it takes for the output to settle (call this *transformer settle time*).
			- ***MAKE SURE THE DUTY CYCLE IS BELOW 50% ALWAYS!!!***
			- Verify the frequency used has the expected VDC output, if not, repeat with a different frequency in increasing order of 10kHz.
				- Feel free to test in smaller increments like 5kHz, 1kHz, or 500Hz, but nothing smaller than 500Hz.
				- ***DO NOT GO ABOVE 50kHz!***
		- Verify one last time that `PRI_GND` and `A_GND` are isolated from each other (using an ohmmeter).
		- Check capacitor voltage is 0V after the rocker switch is OFF after the ***universal wait time***. 
		- If the ripple is above 50mV, change the **C24, C25, C26** to a higher capacitance after ***universal wait time*** and redo step 3.
4. Determine the duty cycle needed for 10V, 12V, 20V at `FLY_OUT_A`.
	- Edit and run the Channel_A_Transform1.cpp file.
		- Based on the previous step, determine at what duty cycle 10V, 12V, and 20V occurs at. Record these values as *minimum duty cycle*, *LV duty cycle*, & *maximum duty cycle*. 
5. Record PWM duty cycle vs. VDC output.
	- Run the Channel_A_Transformer2.cpp file.
		- Measure the output voltage, `FLY_OUT_A`, with the duty cycle sweeping across from *minimum duty cycle* & *maximum duty cycle* in the *percent accuracy* increments so the total interval is 50 seconds.
			- To measure the full sweeping waveform: 
				- Set the horizontal time div to 5s per block.
				- Use the trigger function so the entire waveform is captured in a 50 second window.
				- Measure each voltage by zooming into the horizontal waveform for each block or using a cursor.
				- This procedure will be used multiple times and will be referred to as a *sweep measurement*.
			- If duty cycle to voltage is nonlinear, use a line of best fit to estimate.
		- Some extra notes
			- The flyback transformer is expected to output 20V to 10V in normal operation. 
			- The 12V number will be used for LV circuit and WON'T change.
			- These ***won't*** be the final numbers for the min and max duty cycle for Channel A and PWM duty cycle, since there will be a voltage drop (i.e. there's a diode in line to prevent reverse polarity). 

## Channel A HV/LV 
1. This is to test the HV & LV relays + buck converter.
2. Solder on **C23, D5, D6, K1, K2, L3, Q1, Q2, Q3, R21** and test points `OUT_A_HV`,`OUT_A_BUCK`, and `OUT_A_CURR`.
3. Test HV side (through line).
	- Run the Channel_A_HV_SW.cpp file.
		- Send a LOW signal to *A_HV_LV_OUT*. 
		- Confirm that the input from `FLY_OUT_A` appears at `OUT_A_HV` and `OUT_A_CURR`. 
			- `OUT_A_LV` should be floating.
4. Test LV side (buck converter).
	- Run the Channel_A_LV_Buck1.cpp file.
		- Send a HIGH signal to *A_HV_LV_OUT* with `FLY_OUT_A` set to 12VDC and *A_BUCK* is set to 50% duty cycle at 10kHz.
			- Confirm you hear the relays click as the *A_HV_LV_OUT* changes.
			- Feel free to adjust the PWM frequency from 5kHz to 50kHz, but note longer frequencies will need more time for the transient to settle. 
		- Confirm that `FLY_OUT_A` and `OUT_A_LV` are both 12V. 
			- `OUT_A_HV` should be floating.
		- Confirm `OUT_A_BUCK` and `OUT_A_CURR` are the expected voltage (around 6V $\pm$ 1V).
			- If this isn't the case, move onto step 5. If it does work, move onto step 7.
5. Desolder **Q1, Q3** and solder on **Q12, Q56** (these are on the back). 
6. Run the tests from step 4 again.
7. Determine the duty cycle needed for 1V, 10V at `OUT_A_BUCK`.
	- Edit and run the Channel_A_LV_Buck1.cpp file.
		- Based on the previous step, determine at what duty cycle 1V and 10V occurs at. Record these values as *minimum buck duty cycle* & *maximum buck duty cycle*. 
		- If the ripple for the output of the buck converter is significant (more than 1%), then replace the **C23** capacitor. 
			- It might be necessary to use a 100uF/1mF through hole capacitor to get the voltage filtering desired.
	- Note: If the buck converter is not working properly, most of the voltage range can still be achieved by only using the flyback transformer (although the efficiency will be much worst compared to using a buck converter). 
8. Testing buck converter duty cycle vs. voltage output.
	- Run the Channel_A_LV_Buck2.cpp file.
		- Similar to the transformer, record the output voltage at `OUT_A_BUCK` with the duty cycle set from *minimum buck duty cycle* to *maximum buck duty cycle* in increments of *percent accuracy* so the total interval is 50 seconds and using the *sweep measurement*. 
			- If duty cycle to voltage is nonlinear, use a line of best fit to estimate.
		- Some extra notes
			- These ***won't*** be the final numbers used to associate output for Channel A and PWM duty cycle, since there will be a voltage drop (i.e. there's a diode in line to prevent reverse polarity). 
			- The buck converter is expected to output 10V to 1V in normal operation. Be wary when the output voltage is at the lower range since the buck converter can operate strange in the lower voltage ends.

## Channel A Current Sense & OVP
1. This is to test the current sense & OVP circuit.
2. Solder on **C31, C32, C33, D9, D10, D11, D12, D13, J4, Q13, Q15, Q44, R25, R26, R27, R30, R31, R33** and test points `OUT_A` and `A_PROT_OUT`.
3. Connect a $100\Omega$ resistor between `A_PROT_OUT` and `A_GND` and an ammeter between the fuse pins for **J4**.
4. Testing the current sense.
	- Run the Channel_A_CURR_SENSE.cpp file.
		- Set `OUT_A_CURR` to be 18V, 12V, 6V in 5s intervals.
		- Make sure the MCU displays the same current as the ammeter within a $\pm$ 0.10A tolerance (should be in the range of 0.18A, 0.12A, and 0.06A).
5. Replace the ammeter with a 3A fuse for **J4**.
6. Testing the OVP circuit.
	- Run the Channel_A_OVP.cpp file.
		- Set `OUT_A_CURR` to 20V by setting the duty cycle at the *maximum duty cycle*. 
			- Make sure that the output stays ON according to the current sense reading on the LCD screen. 
		- Set `OUT_A_CURR` to a voltage greater than 20V by setting the duty cycle to be *maximum duty cycle* + *percent accuracy*. 
			- Make sure that the output stays OFF according to the current sense reading on the LCD screen.

## Channel A Enable
1. This is testing Channel A enable.
2. Solder on **C44, Q17, Q19, R24, R37, R62** and test point `CHANNEL_A`.
3. Test the enable signal & voltage drop.
	- Run the Channel_A_Enable1.cpp file.
		- Send a LOW to *A_OUT_EN* and make sure `OUT_A` is giving a VDC output, but `CHANNEL_A` is low.
		- Send a HIGH to *A_OUT_EN* and make sure `OUT_A` and `CHANNEL_A` are nearly equal. 
			- Record the voltage drop across `FLY_OUT_A` and `CHANNEL_A`.
	- Make sure the **C44** is discharged after the ***universal wait time***.
4. Remap all the duty cycle to output voltage.
	- Run the Channel_A_Enable2.cpp file.
		- Determine the appropriate duty cycles needed for the flyback transformer & buck converter to output 1V - 20V at `CHANNEL_A`. 
			- Update the *minimum duty cycle*, *maximum duty cycle*, *minimum buck duty cycle*, and *maximum buck duty cycle*, but NOT the *LV duty cycle*.
		- NOTE: The circuit should switch from HV to LV when the voltage at `CHANNEL_A` reads 10V, not when `FLY_OUT_A` 10V. 

## Channel A Feedback
1. This is to test the feedback signal from Channel A output.
2. Solder on **D29, R67, R68, R69, U9** and test point `A_SENSE_IN`.
3. Test the feedback signal.
	- Run the Channel_A_Feedback.cpp file.
		- Map the output voltages measured at `CHANNEL_A` to the ADC readings (displayed on the LCD screen) at 1V intervals. 
		- Make a line of best fit to determine the relationship between ADC reading and voltage output.
	- With this new mapping, the MCU should use *A_VOL_SENSE* as the actual voltage and adjust *A_DRIVE* (or *A_BUCK*) accordingly.

## Channel A External Connections Part 2
1. This is testing the input potentiometer & XT30 connector for `CHANNEL_A`.
2. Solder on external JST connectors for Channel A labelled **A_V_POT, A_C_POT** and the XT30 connecter, **CHANNEL_A_CONN**.
3. Test to make sure the potentiometers work.
	- Run the Channel_A_Ext_Conn2.cpp file.
		- The pot inputs do adjust their respective voltage (1V-20V) and current values (0A-3A) and it is displayed on the LCD screen.
4. Test to make sure the `CHANNEL_A` XT30 connector works.
	- Run the Channel_A_Ext_Conn3.cpp file.
		- Set the `CHANNEL_A` output to be 1V, 6V, 12V, and 20V. 
		- Measure the `CHANNEL_A` output at the end of the XT30 connector and verify the ripple is below 20mV. 
			- If the ripple is greater than 20mV, increase the capacitance for **C44** and test again.

## Channel A Testing
1. This is to make sure Channel A is working properly independently. 
	- The file used to run all these tests should be the final Channel A code used to run the power supply (only needs to support independent channel at this point, doesn't need to support series or parallel mode yet), so they should all be the same.
2. Testing output voltage with load.
	- Verify that the power is able to set an output of 1V, 6V, 12V, and 20V with a 1kOhm resistor connected.
3. Test the voltage regulation
	- Set the output voltage to 12V with (no load).
	- Add a 1kOhm resistor while the power supply is running and measure to see if there is any significant transient changes in the voltage and current. 
	- Set the output voltage to 6V and rerun the same test (no load then attach a 1kOhm resistor while it's running).
4. Testing power supply at maximum and minimum range.
	- Rerun the same test from step 3, but with the output voltage set to 1V and 20V.
5. Make sure the HV/LV relays are robust.
	- Set the voltage to 9.9V, 10V, 10.1V and that voltage appears at `CHANNEL_A` and the XT30 connector (no load).
	- Attach a load while the power supply is running and see if there is any significant transient changes in voltage/current and if it can handle transient changes near the edge of the voltage changes. 
6. Make sure the current can be set from 0-3A.
	- Disconnect the fuse and connect an ammeter in series to make sure the output current matches the `CHANNEL_A` output current LCD screen reading.
		- Set a current limit to 0.10A with the voltage to 6V and connect the output to a $100\Omega$ resistor to make sure the output current is correct within a $\pm$ 0.01A, and there's 6V across Channel A.
		- Set a current limit to 0.03A with the voltage to 6V and connect the output to a $100\Omega$ resistor to make sure the current limit turns on, and there's no output across Channel A.
7. Testing 0V output.
	- It is possible to set the output voltage to a voltage lower than 1V due to the forward drop of the diode. 
	- Lower the duty cycle of the buck converter until you notice what the absolute minimum is based on either:
		- The duty cycle is too low (don't go below 5% duty cycle).
		- The ripple it too large to be filtered and reach voltage ripple spec.
8. Leave the power supply on for 30 mins to make sure no overheating. 
	- For the following tests, make sure the power supply doesn't overheat, and the voltage stays accurate for the entire 60 min duration (check on it every 5-10 minutes).
	- Set the voltage to 20V and connect no load. 
	- Set the voltage to 10V and connect no load.
	- Set the voltage to 20V and connect $100\Omega$ resistor.
	- Set the voltage to 10V and connect $100\Omega$ resistor.
	- Set the voltage to 12V and connect $20\Omega$ resistor (need power resistor).
	- Set the voltage to 10V and connect $20\Omega$ resistor (need power resistor).

---
# 2. Independent Channel B
## Some Notes
- This will be mostly identical to independent channel A except the refdes for the components will change to their channel B counterparts and some minor tweaks to the testing. 
- Most of the test results should yield similar results to Channel A. ***IF THEY DON'T VERIFY WHY BEFORE MOVING ON.***
- Most of the scripts used should be the same logic as the channel A (for independent test at least), but make sure to double check the pins for all tests. Some of the pins are different across the two channels, namely: *B_VOL_SENSE*, *B_V_KNOB*, *B_C_KNOB*, *B_TEMP*, *B_DRIVE*. 

## Channel B LVDD
1. This is to test the `B_5V` & `B_3V3` rail.
2. Solder on the same set of components as Channel A (**C13, C14, C15, C16, C17, C19, D44, L2, L6, R97, U4** OR **C14, R11, R12**) and test points for `B_GND`, `B_5V`. 
3. Check the `B_5V` rail.
	- DC output on `B_5V` (ripple less than 200mV) & the red LED is ON (similar to Channel A).
	- Verify `PRI_GND` & `B_GND` are isolated (using an ohmmeter).
4. Solder on **C18, D43, U3, R96** & test point for `B_3V3`.
5. Check the `B_3V3` rail.
	- DC output on `B_3V3` (ripple less than 33mV) & the yellow LED is ON.

## Channel B ESP32
1. This is to test the ESP32 is powered on. 
2. Solder on **J10, J11**, or the header pins for the ESP32 MCU B.
3. Place the ESP32 on following the orientation on the board.
	- Verify the LED on the ESP32 module turns on.
4. Solder on **Q6, Q8, R19, R20** and test points `BUCK_B_nPWM`, `BUCK_B_PWM`, & `OUT_B_LV`.
5. Determine the accuracy of PWM duty cycle & logical level shift voltage levels.
	- Run the Channel_B_ESP32_PWM.cpp file.
		- Connect the `OUT_B_LV` to `B_5V` & measure this voltage.
		- Set the PWM to 10kHz with 10% duty cycle and measure how accurate the duty cycle is on the `BUCK_B_PWM` and `BUCK_B_nPWM` (should be 1 - duty cycle) & record the difference. This accuracy will be denoted as the *percent accuracy*.
			- Make sure that the logical levels for these circuits switches between `B_GND` & `B_5V`. 
		- Run these tests again in increasing order of 10kHz until you reach 50kHz and measure the *percent accuracy* for each frequency.

## Channel B External Connections Part 1
1. This is testing the input button & LCD screen for Channel B.
2. Solder on external JST connectors for Channel B labelled **LCD_B, BUTTON_B**.
3. Test to make sure these connections work.
	- Run the Channel_B_Ext_Conn_1.cpp file.
		- Check to make sure the input from Button B is read properly on the LCD screen.

## Channel B Temperature Sensor
1. This is to test the temperature sensor.
2. Solder on **C38, R59, R60, U8**.
3. Solder on $50k\Omega$ for **R66**.
	- Run the Channel_B_Temp_Sense.cpp file.
		- Make sure the temperature sensor sends HIGH to *B_TMP_SNSE* at room temperature with this displayed on the LCD.
		- Make sure the temperature sensor sends a LOW signal with hot air gun at $55^{\circ}C$ (can set it up to $65^{\circ}C$) aimed at the temperature sensor and this is displayed on the LCD.
			- The temperature sensor should go off at around $53^{\circ}C$.
4. Solder on the same resistor for **R66** as used in **R54**. 

## Channel B Transformers
1. This is to test the flyback & feedback transformer.
2. Solder on **C28, C29, C30, Q40, Q41, Q45, Q48, R71, R72, R102, T3, T5** and test points `FLY_OUT_B`, `PRI_FET_B` & `SEC_FET_B` (use the same values for **C28, C29, C30** as you did for **C24, C25, C26**).
3. Check the flyback transformer outputs VDC at `FLY_OUT_B`.
	- Run the Channel_B_Transformer1.cpp file.
		- With the MCU, send a fixed PWM at the same frequency for *B_DRIVE* as *A_DRIVE* for the transformer with a 25% duty cycle. Record the VDC at `FLY_OUT_B` (should be approximately 10.9V $\pm$ 2V) & record how long it takes for the output to settle (call this *transformer settle time*).
			- ***MAKE SURE THE DUTY CYCLE IS BELOW 50% ALWAYS!!!***
		- Verify one last time that `PRI_GND`, `A_GND`, and `B_GND` are all isolated from each other (using an ohmmeter).
		- Check capacitor voltage is 0V after the rocker switch is OFF after the ***universal wait time***. 
4. Determine the duty cycle needed for 10V, 12V, 20V at `FLY_OUT_B`.
	- Edit and run the Channel_B_Transform1.cpp file.
		- Based on the previous step, determine at what duty cycle 10V, 12V, and 20V occurs at. Record these values as *minimum duty cycle*, *LV duty cycle*, & *maximum duty cycle*.
		- Make sure these duty cycles are very close to the duty cycles found in Channel A, if not the same. 
5. Record PWM duty cycle vs. VDC output.
	- Run the Channel_B_Transformer2.cpp file.
		- Measure the output voltage, `FLY_OUT_B`, with the duty cycle sweeping across from *minimum duty cycle* & *maximum duty cycle* in the *percent accuracy* increments so the total interval is 50 seconds using the *sweep measurement*.
			- If duty cycle to voltage is nonlinear, use a line of best fit to estimate.
		- Some extra notes
			- The flyback transformer is expected to output 20V to 10V in normal operation. 
			- The 12V number will be used for LV circuit and WON'T change.
			- These ***won't*** be the final numbers for the min and max duty cycle for Channel B and PWM duty cycle, since there will be a voltage drop (i.e. there's a diode in line to prevent reverse polarity). 

## Channel B HV/LV 
1. This is to test the HV & LV relays + buck converter.
2. Solder on **D7, D8, K3, K4, L4, Q4, R22** and test points `OUT_B_HV`,`OUT_B_BUCK`, and `OUT_B_CURR`.
3. Solder on the same set of transistors and capacitors. 
	- Either solder on **Q9, Q10** OR **Q57, Q58**.
	- Solder on the same capacitor as you did for **C23** for **C27**.
4. Test HV side (through line).
	- Run the Channel_B_HV_SW.cpp file.
		- Send a LOW signal to *B_HV_LV_OUT*. 
		- Confirm that the input from `FLY_OUT_B` appears at `OUT_B_HV` and `OUT_B_CURR`. 
			- `OUT_B_LV` should be floating.
3. Test LV side (buck converter).
	- Run the Channel_B_LV_Buck1.cpp file.
		- Send a HIGH signal to *B_HV_LV_OUT* with `FLY_OUT_B` set to 12VDC and *B_BUCK* is set to 50% duty cycle with the same frequency as the Channel A buck converter.
			- Confirm you hear the relays click as the *B_HV_LV_OUT* changes.
		- Confirm that `FLY_OUT_B` and `OUT_B_LV` are both 12V. 
			- `OUT_B_HV` should be floating.
		- Confirm `OUT_B_BUCK` and `OUT_B_CURR` are the expected voltage (around 6V $\pm$ 1V).
4. Determine the duty cycle needed for 1V, 10V at `OUT_B_BUCK`.
	- Edit and run the Channel_B_LV_Buck1.cpp file.
		- Based on the previous step, determine at what duty cycle 1V and 10V occurs at. Record these values as *minimum buck duty cycle* & *maximum buck duty cycle*. 
	- Note: If the buck converter is not working properly, most of the voltage range can still be achieved by only using the flyback transformer (although the efficiency will be much worst compared to using a buck converter). 
6. Testing buck converter duty cycle vs. voltage output.
	- Run the Channel_B_LV_Buck2.cpp file.
		- Similar to the transformer, record the output voltage at `OUT_B_BUCK` with the duty cycle set from *minimum buck duty cycle* to *maximum buck duty cycle* in increments of *percent accuracy* so the total interval is 50 seconds using the *sweep measurement*. 
			- If duty cycle to voltage is nonlinear, use a line of best fit to estimate.
		- Some extra notes
			- These ***won't*** be the final numbers used to associate output for Channel B and PWM duty cycle, since there will be a voltage drop (i.e. there's a diode in line to prevent reverse polarity). 
			- The buck converter is expected to output 10V to 1V in normal operation. Be wary when the output voltage is at the lower range since the buck converter can operate strange in the lower voltage ends.

## Channel B Current Sense & OVP
1. This is to test the current sense & OVP circuit.
2. Solder on **C34, C35, C36, D15, D16, D17, D18, D19, J6, Q21, Q22, Q47, R35, R36, R38, R39, R40, R41** and test points `OUT_B` and `B_PROT_OUT`.
3. Connect a $100\Omega$ resistor between `B_PROT_OUT` and `B_GND` and an ammeter between the fuse pins for **J6**.
4. Testing the current sense.
	- Run the Channel_B_CURR_SENSE.cpp file.
		- Set `OUT_B_CURR` to be 18V, 12V, 6V in 5s intervals.
		- Make sure the MCU displays the same current as the ammeter within a $\pm$ 0.10A tolerance (should be in the range of 0.18A, 0.12A, and 0.06A).
5. Replace the ammeter with a 3A fuse for **J6**.
6. Testing the OVP circuit.
	- Run the Channel_B_OVP.cpp file.
		- Set `OUT_B_CURR` to 20V by setting the duty cycle at the *maximum duty cycle*. 
			- Make sure that the output stays ON according to the current sense reading on the LCD screen. 
		- Set `OUT_B_CURR` to a voltage greater than 20V by setting the duty cycle to be *maximum duty cycle* + *percent accuracy*. 
			- Make sure that the output stays OFF according to the current sense reading on the LCD screen.

## Channel B Enable
1. This is testing Channel B enable.
2. Solder on **C45, Q18, Q20, R28, R32, R98** and test point `CHANNEL_B` (use the same capacitance value as **C44** for **C45**).
3. Test the enable signal & voltage drop.
	- Run the Channel_B_Enable1.cpp file.
		- Send a LOW to *B_OUT_EN* and make sure `OUT_B` is giving a VDC output, but `CHANNEL_B` is low.
		- Send a HIGH to *B_OUT_EN* and make sure `OUT_B` and `CHANNEL_B` are nearly equal. 
			- Record the voltage drop across `FLY_OUT_B` and `CHANNEL_B`.
	- Make sure the **C45** is discharged after the ***universal wait time***.
4. Remap all the duty cycle to output voltage.
	- Run the Channel_B_Enable2.cpp file.
		- Determine the appropriate duty cycles needed for the flyback transformer & buck converter to output 1V - 20V at `CHANNEL_B`. 
			- Update the *minimum duty cycle*, *maximum duty cycle*, *minimum buck duty cycle*, and *maximum buck duty cycle*, but NOT the *LV duty cycle*.
		- These numbers should be very similar to the numbers acquired for Channel A.
		- NOTE: The circuit should switch from HV to LV when the voltage at `CHANNEL_B` reads 10V, not when `FLY_OUT_B` reaches 10V. 

## Channel B Feedback
1. This is to test the feedback signal from Channel B output.
2. Solder on **D31, R78, R79, R80, U10** and test point `B_SENSE_IN`.
3. Test the feedback signal.
	- Run the Channel_B_Feedback.cpp file.
		- Map the output voltages measured at `CHANNEL_B` to the ADC readings (displayed on the LCD screen) at 1V intervals. 
		- Make a line of best fit to determine the relationship between ADC reading and voltage output.
	- With this new mapping, the MCU should use *B_VOL_SENSE* as the actual voltage and adjust *B_DRIVE* (or *B_BUCK*) accordingly.

## Channel B External Connections Part 2
1. This is testing the input potentiometer & XT30 connector for `CHANNEL_B`.
2. Solder on external JST connectors for Channel B labelled **B_V_POT, B_C_POT** and the XT30 connecter, **CHANNEL_B_CONN**.
3. Test to make sure the potentiometers work.
	- Run the Channel_B_Ext_Conn2.cpp file.
		- The pot inputs do adjust their respective voltage (1V-20V) and current values (0A-3A) and it is displayed on the LCD screen.
4. Test to make sure the `CHANNEL_B` XT30 connector works.
	- Run the Channel_B_Ext_Conn3.cpp file.
		- Set the `CHANNEL_B` output to be 1V, 6V, 12V, and 20V. 
		- Measure the `CHANNEL_B` output at the end of the XT30 connector and verify the ripple is below 20mV. 

## Channel B Testing
1. This is to make sure Channel B is working properly independently. 
	- The file used to run all these tests should be the final Channel B code used to run the power supply (only needs to support independent channel at this point, doesn't need to support series or parallel mode yet), so they should all be the same.
2. Testing output voltage with load.
	- Verify that the power is able to set an output of 1V, 6V, 12V, and 20V with a 1kOhm resistor connected.
3. Test the voltage regulation
	- Set the output voltage to 12V with (no load).
	- Add a 1kOhm resistor while the power supply is running and measure to see if there is any significant transient changes in the voltage and current. 
	- Set the output voltage to 6V and rerun the same test (no load then attach a 1kOhm resistor while it's running).
4. Testing power supply at maximum and minimum range.
	- Rerun the same test from step 3, but with the output voltage set to 1V and 20V.
5. Make sure the HV/LV relays are robust.
	- Set the voltage to 9.9V, 10V, 10.1V and that voltage appears at `CHANNEL_B` and the XT30 connector (no load).
	- Attach a load while the power supply is running and see if there is any significant transient changes in voltage/current and if it can handle transient changes near the edge of the voltage changes. 
6. Make sure the current can be set from 0-3A.
	- Disconnect the fuse and connect an ammeter in series to make sure the output current matches the `CHANNEL_B` output current LCD screen reading.
		- Set a current limit to 0.10A with the voltage to 6V and connect the output to a $100\Omega$ resistor to make sure the output current is correct within a $\pm$ 0.01A, and there's 6V across Channel B.
		- Set a current limit to 0.03A with the voltage to 6V and connect the output to a $100\Omega$ resistor to make sure the current limit turns on, and there's no output across Channel B.
7. Testing 0V output.
	- It is possible to set the output voltage to a voltage lower than 1V due to the forward drop of the diode. 
	- Lower the duty cycle of the buck converter until you notice what the absolute minimum is based on either:
		- The duty cycle is too low (don't go below 5% duty cycle).
		- The ripple it too large to be filtered and reach voltage ripple spec.
		- Make sure that these numbers are the same across the two channels.
9. Testing isolation across Channel A & Channel B.
	- Turn on both Channel A and Channel B (setting it to 6V is fine) and make sure that all terminals (positive and negative) are disconnected from each other (using an ohmmeter).
	- Connect $1k\Omega$ between Channel B and A GND and make sure there's no current output.
		- Connect a ammeter in series with the resistor and a voltmeter across the resistor to confirm this.
10. Leave the power supply on for 30 mins to make sure no overheating. 
	- For the following tests, make sure the power supply doesn't overheat, and the voltage stays accurate for the entire 60 min duration (check on it every 5-10 minutes).
	- Set the voltage to 20V and connect no load. 
	- Set the voltage to 10V and connect no load.
	- Set the voltage to 20V and connect $100\Omega$ resistor.
	- Set the voltage to 10V and connect $100\Omega$ resistor.
	- Set the voltage to 12V and connect $20\Omega$ resistor (need power resistor).
	- Set the voltage to 10V and connect $20\Omega$ resistor (need power resistor).

---
# 3. USB-C Channel C
## Some Notes
- Most of the equipment needed to test USB-C and USB related signals aren't available to us so most of this will be solder on the components and hope they work. 
- During the initial test for Channel C, run the tests without using Channel. There'll be a best to run Channel B & Channel C at the same time at the end.
- If Channel C can't be verified to be working, then it is best to completely disable it and move on.

## Channel C Transformers
1. This is to test the flyback & feedback transformer.
2. Solder on **C20, C21, C22, Q42, Q43, Q46, Q49, R73, R74, R100,  T1, T6** and test points `FLY_OUT_C`, `PRI_FET_C`, & `SEC_FET_C`.
3. Determine the duty cycle needed for 5V-5.5V at `FLY_OUT_C`.
	- Run the Channel_C_Transformer2.cpp file.
		- With the MCU, send fixed PWM at 50kHz with the duty cycle ranging from the Channel B transformer *minimum duty cycle* to *maximum duty cycle*.
			- The frequency here can be adjusted from 10kHz to 50kHz. Note that the datasheet recommends frequencies of 250kHz to 550kHz but this would strain the MCU too much (MCU B needs to run two drive circuits), so feel free to go higher if needed, but don't go too high.
		- Determine what the appropriate duty cycle range is to achieve `FLY_OUT_C` to reach a range of 5V to 5.5V. 
			- Ideal value would be somewhere near 5.3V, but also keep in mind lower duty cycle percentage for flyback transformer (lower than 10%) are not ideal for power efficiency.
			- ***MAKE SURE THE DUTY CUCLE IS BELOW 50% ALWAYS!!!***.
		- Check capacitor voltage is 0V after the rocker switch is OFF after the ***universal wait time*** (this isn't as big of a concern since this isn't high voltage).

## Channel C OVP

## Channel C Feedback

## Channel C USB Circuit
1. Solder on everything minus R82, R83, R84.

## Channel C Testing
1. Testing Channel C
	1. Use USB oscope tool
	2. Check to see if the load is running
		1. Link here
	3. If no worky, remove U11, and try R82, R83, R84.
		1. Be careful, this can be a destructive action and U11 might not be able to soldered back on. 
2. Check to see if Channel B & Channel C can work.
	1. Add transient loads, See fluctuations


---
# 4. Parallel Mode Testing
## Some Notes
- Make sure to record all values and testing stuff of previous sections now. This can be a destructive action
- 

## 

---
# 5. Series Mode Testing
## Some Notes
- Make sure to record all values and testing stuff of previous sections now. This can be a destructive action
- 
