# General Notes
- General convention: 
	- refdes will be displayed as **BOLD**
	- net names will be displayed as `NET_NAME`
	- components will be displayed as *Groups*.
		- Many components will be in *Common* if they don't have a dedicated *Group*, such as pull-up resistor.
	- MCU pins will also be displayed as *MCU Pins*.
	- Warning will be displayed as ***LIKE THIS***.
- All measurements will be done with x100 probe.
- For Assembly:
	- Cut stencil up if feasible
	- Assembly block by block from the *groups* according to the testing document.
- ***MINIMIZE*** the amount of interactions you have with live circuits.
	- Avoid probing the circuit, to prevent the chances of an ***ARCS***.
- When soldering on components, make sure the circuit is disconnected.
- ***IF INTERACTING WITH LIVE CIRCUIT, WEAR HIGH VOLTAGE GLOVES***
	- Be wary of any ***ARCS***
		- For example, the micro USB metal shell is a live `GND`.
- ***DO NOT TOUCH CAPACITORS ON THE CIRCUIT***
	- If you need to touch them for any reason, wait for them to discharge (based on the number below).
- ***DO NOT UPLOAD CODE TO THE ESP32 WHEN THE BOARD IS ON.***
	- Take it off, upload the code, and then put it on.
        - Test scripts can be found in the Firmware/Test Scripts directory.

# 1. Input Circuit
1. This is to verify the input circuit does rectify the AC input.
2. Solder **BR1, F1, F2, J1, R15, R16, R99** from the *Input* and test points for `PRI_GND` & `UNREG_DC`.
	- Place a 12A FB fuse across **F1** & **F2**.
3. Test the input:
	- AC output on `UNREG_DC` with rocker switch OFF (no output)
	- AC output on `UNREG_DC` with rocker switch ON (Expected Full-Wave), recording the Vpp.
4. Desolder **BR1** and solder on **D1, D2, D3, D4** from the *Input*.
5. Test the input:
	- AC output on `UNREG_DC` with rocker switch OFF (no output)
	- AC output on `UNREG_DC` with rocker switch ON (Expected Full-Wave), recording the Vpp.
6. If the AC output for `UNREG_DC` has higher Vpp, then keep **D1, D2, D3, D4** on and move to step 6. Otherwise, desolder **D1, D2, D3, D4**, and solder on **BR1**. 
	- Verify the AC Output for `UNREG_DC` is the same as before if using **BR1** before continuing on.
7. Solder on **C7, C8, C9, C10, C11** from the *Input*.
8. Test the filtered input:
	- AC output on `UNREG_DC` (Expected near DC output, record ripple).
		- Can swap out capacitors with bigger value alternative capacitors from JLab to see if the rippling can be reduced significantly.
	- Check capacitor voltage is 0V after the rocker switch is OFF for 120 seconds.
		- This is the ***universal wait time*** for capacitors to discharge.
		- Note: Might need to wait longer if the capacitance was increased. 

# 2. Independent Channel A 
## Channel A LVDD
1. This is to test the `A_5V` & `A_3V3` rail.
2. Solder on **C1, C3, C4, C5, C12, D41, L1, L5, R94, U2** from the *LVDD* and test points for `A_GND`, `A_5V`.
3. Check the `A_5V` rail.
	- DC output on `A_5V` (ripple less than 200mV) & the red LED is ON.
	- Verify `PRI_GND` & `A_GND` are isolated (using a voltmeter). If not, go to step 4.
	- If output is clean, can skip to step 6. If not, go to step 4.
4. Desolder **C1, C3, C4, C5, C12, L1, L5** and solder on **R11, R12** from the *LVDD*.
5. Check the `A_5V` rail.
	- DC output on `A_5V` (ripple less than 200mV).
	- Verify `PRI_GND` & `A_GND` are isolated (using a voltmeter).
	- If output is cleaner, move on. If output is worst compared to before, revisit step 2.
		- If `GND`'s are now disconnected but worse output, LEAVE IT AS IS, and move on.
6. Solder on **C2, C6, D40, U1, R93** & test point for `A_3V3` from the *LVDD*.
7. Check the `A_3V3` rail.
	- DC output on `A_3V3` (ripple less than 33mV) & the yellow LED is ON.

## Channel A ESP32
1. This is to test the ESP32 is powered on. 
2. Solder on **J7, J8**, or the header pins for the ESP32 MCU A, from the *MCU*.
3. Place the ESP32 on following the orientation on the board.
	- Verify the LED on the ESP32 module turns on.

## Channel A External Connections Part 1
1. This is testing the input button & LCD screen for Channel A.
2. Solder on external JST connectors for Channel A labelled **LCD_A, BUTTON_A**.
3. Test to make sure these connections work.
	- Run the Channel_A_Ext_Conn_1.cpp file.
 		- Check to make sure the input from Button A is read properly on the LCD screen.

## Channel A Temperature Sensor
1. This is to test the temperature sensor.
2. Solder on **C37, R46, R47, U7**.
3. Solder on $50k\Omega$ for **R54**.
	- Run the Channel_A_Temp_Sense.cpp file.
		- Make sure the temperature sensor sends HIGH to *A_TMP_SNSE* at room temperature and this is displayed on the LCD.
		- Make sure the temperature sensor sends a LOW signal with hot air gun at $55^{\circ}C$ aimed at the temperature sensor and this is displayed on the LCD.
			- The temperature sensor should go off at around $53^{\circ}C$.
4. Determine what an appropiate resistor/temperature cut off is.
	- Few choices here. Recommend using a lower temperature threshold first before moving onto higher threshold.
		- $50k\Omega$ for $53^\circ C$ threshold
		- $30k\Omega$ for $79^{\circ}C$ threshold
		- $15k\Omega$ for $100^{\circ}C$ threshold

## Channel A Transformers
1. This is to test the flyback & feedback transformer.
2. Solder on **C24, C25, C26, T2, T4, Q29, Q35, Q38, Q39, R61, R70, R101** from the *Transformer* and test points `FLY_OUT_A`, `PRI_FET_A` & `SEC_FET_A`.
3. Check the flyback transformer outputs VDC at `FLY_OUT_A`.
	- Run the Channel_A_Transformer1.cpp file.
		- With the MCU, send a fixed PWM at 10kHz with a duty cycle of 25% on *A_DRIVE* and record the VDC at `FLY_OUT_A` (should be approximately 10.9V $\pm$ 2V) & record how long it takes for output to settle (call this *time1*).
			- ***MAKE SURE THE DUTY CYCLE IS BELOW 50% ALWAYS***.
  			- Verify the frequency used has the expected VDC output, if not repeat with a different frequency increasing order of 10kHz.
				- ***DO NOT GO ABOVE 50kHz!***.
		- Verify one last time that `PRI_GND` and `A_GND` are isolated from each other (using a voltmeter).
		- Check capacitor voltage is 0V after the rocker switch is OFF after the ***universal wait time***. 
		- If the ripple is above 50mV, change the **C24, C25, C26** to a higher capacitance after ***universal wait time*** and redo step 3.
4. Record PWM duty cycle vs. VDC output.
	- Run the Channel_A_Transformer2.cpp file.
		- Measure the duty cycle from 21V to 9.5V in 0.10V intervals after waiting *time1* seconds.
			- If duty cycle to voltage is nonlinear, use a line of best fit to estimate.
		- Some extra notes
			- The 12V number will be used for LV circuit.
			- These ***won't*** be the final numbers used to associate output for Channel A and PWM duty cycle, since there will be a voltage drop (i.e. there's a diode in line to prevent reverse polarity). 
			- The flyback transformer is expected to output 20V to 10V in normal operation. The 21V and 9.5V values helps to ensure that all boundary conditions are tested and an upper/lower duty cycle can be found for the firmware (+ to account for voltage drop).

## Channel A HV/LV 
1. This is to test the HV & LV relays + buck converter.
2. Solder on **C23, D5, D6, K1, K2, L3, Q1, Q2, Q3, Q5, Q7, R17, R18, R21** from the *HV/LV* and test points `OUT_A_HV`, `OUT_A_LV`,`BUCK_A_\P\W\M`, `BUCK_A_PWM`,`OUT_A_BUCK`, and `OUT_A_CURR`.
3. Test HV side (through line)
	- Send a LOW signal to *A_HV_LV_OUT*. 
	- Confirm that the input from `FLY_OUT_A` appears at `OUT_A_HV` and `OUT_A_CURR`. 
		- `OUT_A_LV` should be floating.
4. Test LV side (buck converter)
	- Send a HIGH signal to *A_HV_LV_OUT* with `A_FLY_OUT_A` set to 12VDC and *A_BUCK* is set to 50% duty cycle.
		- Confirm you hear the relays click as the *A_HV_LV_OUT* changes.
	- Confirm that `FLY_OUT_A` and `OUT_A_LV` are both 12V. 
		- `OUT_A_HV` should be floating.
	- Confirm `OUT_A_BUCK` and `OUT_A_CURR` are the expected voltage (around 6V). 
		- If this isn't the case, move onto step 5. If it does work, move onto step 7.
5. Desolder **Q1, Q3** and solder on **Q12, Q56** (these are on the back). 
6. Run the tests from step 4 again.
7. Testing buck converter duty cycle vs. voltage output.
	- Similar to the transformer, record the duty cycle to output voltage of the buck converter from 10V to 2V in 0.25V intervals.
		- If duty cycle to voltage is nonlinear, use a line of best fit to estimate.
	- Some extra notes
		- These ***won't*** be the final numbers used to associate output for Channel A and PWM duty cycle, since there will be a voltage drop (i.e. there's a diode in line to prevent reverse polarity). 
		- The buck converter is expected to output 10V to 1V in normal operation. The 2V lower end is because the buck converter can operate strange in the lower voltage ends, so the 1V should be tested more thoroughly later on.
### Get the interval for voltage checked by cult.

## Channel A Current Sense
1. This is to test the current sense.
2. Solder on **C31, C32, C33, D10, D11, R25, R30** from the *Current Sense* and test points `OUT_A`.
3. Connect a $100\Omega$ resistor between `OUT_A` and `A_GND`.
4. Testing current sense.
	- Set the transformer output voltage to be 18V, 12V, 6V.
	- Make sure the MCU reads 0.18A, 0.12A, and 0.06A within a ±0.01A tolerance.

## Channel A OVP
1. This is for the OVP circuit.
2. Solder on **D9, D12, D13, J4, Q13, Q15, Q44, R26, R27, R31, R33** from *OVP* and test points `A_PROT_OUT`.
	- Place a 3A on the fuse holder, J4.
3. Testing the voltage drop
	- Record the voltage drop across `OUT_A` & `A_PROT_OUT` (should be approximately 0.7V).

## Channel A Enable
1. This is testing Channel A enable.
2. Solder on **Q17, Q19, R24, R37** from *Enable* and test points `CHANNEL_A`.
3. Test the enable signal.
	- Send a LOW to *A_OUT_EN* and make sure `OUT_A` is giving a VDC output, but `CHANNEL_A` is low.
	- Send a HIGH to *A_OUT_EN* and make sure `OUT_A` and `CHANNEL_A` are nearly equal. If there is a voltage drop, record it.
4. Remap all the duty cycle to output voltage.
	- Redo the calculations for duty cycles to output voltage from the Channel A Transformer and Channel A HV/LV where the output voltage is measured at `CHANNEL_A` instead of from `FLY_OUT_A`.
	- Make sure to get the 12V level measured!
### Get the step 4 checked by cult cause we use feedback signal on section below. NVM it's needed for LCD screen!

## Channel A Feedback
1. This is to test the feedback signal from Channel A output.
2. Solder on **D29, R67, R68, R69, U9** from *Feedback* and test points `A_SENSE_IN`.
3. Test the feedback signal.
	- Measure the feedback signal from the MCU, *A_VOL_SENSE*, map it to the calculated output voltage from the OpAmp circuit, and see how it compares to the actual `CHANNEL_A` output (should be accurate within 50mV).
	- With this new mapping, the MCU should use *A_VOL_SENSE* as the actual voltage and adjust *A_DRIVE* (or *A_BUCK*) accordingly.

## Channel A External Connections Part 2
1. This is testing the input button for Channel A.
2. Solder on external JST connectors for Channel A labelled **LCD_A, BUTTON_A, A_V_POT, A_C_POT** and the XT30 connecter, **CHANNEL_A_CONN**, from the *External*.
3. Test to make sure these connections work.
	- The LCD screen is on and is changing as appropriately.
	- The pot inputs do adjust their respective voltage and current values to pins *A_V_KNOB* & *A_C_KNOB*.
	- The button does send a HIGH signal to *A_BUTT*.
	- The XT30 connector does have the same output as `CHANNEL_A`. 
		- If the XT30 connector output has ripples, go to step 4, otherwise move onto the next section.
4. Solder on **R62, C44**.
	- Make sure the **C44** is discharged after the universal wait time.

## Channel A Testing
1. This is to make sure Channel A is working properly independently.
2. Make sure the voltage can be set from 1V-20V.
	- Set the voltage to 1V, 6V, 12V, 18V, 20V and that voltage appears at `CHANNEL_A` and the XT30 connector (no load).
3. Make sure the current can be set from 0-3A.
	- Set a current limit to 0.10A with the voltage to 6V and connect the output to a $100\Omega$ resistor to make sure the output current is correct.
	- Set a current limit to 0.03A with the voltage to 6V and connect the output to a $100\Omega$ resistor to make sure the current limit turns on.
3. Leave the power supply connected to a load for 30mins-60mins. 
	- Make sure it is still working properly and there isn't any significant change (i.e. it doesn't become hot).

# 2. Independent Channel B
<u>Note:</u> This will be mostly identical to independent channel A except the refdes for the components will change to their channel B counterparts and some minor tweaks to the testing.

## Channel B LVDD
1. This is to test the `B_5V` & `B_3V3` rail.
2. Solder on the same counterparts from Channel A (either EMC filter, **C13, C15, C16, C17, C19, L2, L6**, OR 0 Ohm resistors, **R13, R14**) and **D44, R97, U4** from *LVDD*, as well as test points for `B_GND`, `B_5V`.
3. Check the `B_5V` rail.
	- DC output on `B_5V` (ripple less than 200mV) and the red LED is ON.
	- Verify `PRI_GND` & `B_GND` are isolated (using a voltmeter). 
	- Also verify that `A_GND` & `B_GND` are isolated.
4. Solder on **C14, C18, D43, U3, R96** from the *LVDD* and test point for `B_3V3`.
5. Check the `B_3V3` rail.
	- DC output on `B_3V3` (ripple less than 33mV) and the yellow LED is ON.

## Channel B ESP32
1. This is to test the ESP32 is powered on. 
2. Solder on **J10, J11**, or the header pins for the ESP32 MCU A, from the *MCU*.
3. Place the ESP32 on following the orientation on the board.
	- Verify the LED on the ESP32 module turns on.

## Channel B Temperature Sensor
1. This is to test the temperature sensor.
2. Solder on **C38, R59, R60, U8**.
3. Solder on $68k\Omega+5k\Omega$ or $73k\Omega$, for **R66**. 
	- Make sure the temperature sensor sends HIGH to *B_TMP_SNSE* at room temperature.
	- Make sure the temperature sensor sends a LOW signal with human touch.
4. Desolder the resistors and solder on the proper **R66** using the same value as the **R54**.

## Channel B Transformers
1. This is to test the flyback & feedback transformer.
2. Solder on **C28, C29, C30, T3, T5, Q40, Q41, Q45, Q48, R71, R72, R102** from the *Transformer* and test points `FLY_OUT_B`, `PRI_FET_B` & `SEC_FET_B`.
3. Check the flyback transformer outputs VDC.
	- With the MCU, send a fixed PWM on *B_DRIVE* and record the VDC. 
		- ***MAKE SURE THE DUTY CYCLE IS BELOW 50% ALWAYS***.
	- Verify one last time that `PRI_GND`, `A_GND`, & `B_GND` are all isolated from each other (using a voltmeter).
	- Check capacitor voltage is 0V after the rocker switch is OFF after the ***universal wait time***. 
	- If the ripple is above 50mV, change the **C28, C29, C30** to a higher capacitance after waiting the ***universal wait time***. 
4. Record PWM duty cycle vs. VDC output.
	- Measure the duty cycle from 21V to 10V in 1V intervals.
		- If duty cycle to voltage is nonlinear, use a line of best fit to estimate.
		- These numbers should be reasonably close to the numbers recorded by Channel A. ***IF THEY'RE NOT DO NOT PROCEED BEFORE DETERMINING WHY THIS IS THE CASE!!!***.
### Get the interval for voltage checked by cult.

---
## Channel B HV/LV 
1. This is to test the HV & LV relays + buck converter.
2. Solder on **C23, D5, D6, K1, K2, L3, Q1, Q2, Q3, Q5, Q7, R17, R18, R21** from the *HV/LV* and test points `OUT_A_HV`, `OUT_A_LV`,`BUCK_A_\P\W\M`, `BUCK_A_PWM`,`OUT_A_BUCK`, and `OUT_A_CURR`.
3. Test HV side (through line)
	- Send a LOW signal to *A_HV_LV_OUT*. 
	- Confirm that the input from `FLY_OUT_A` appears at `OUT_A_HV` and `OUT_A_CURR`. 
		- `OUT_A_LV` should be floating.
4. Test LV side (buck converter)
	- Send a HIGH signal to *A_HV_LV_OUT* with `A_FLY_OUT_A` set to 12VDC and *A_BUCK* is set to 50% duty cycle.
		- Confirm you hear the relays click as the *A_HV_LV_OUT* changes.
	- Confirm that `FLY_OUT_A` and `OUT_A_LV` are both 12V. 
		- `OUT_A_HV` should be floating.
	- Confirm `OUT_A_BUCK` and `OUT_A_CURR` are the expected voltage (around 6V). 
		- If this isn't the case, move onto step 5. If it does work, move onto step 7.
5. Desolder **Q1, Q3** and solder on **Q12, Q56** (these are on the back). 
6. Run the tests from step 4 again.
7. Testing buck converter duty cycle vs. voltage output.
	- Similar to the transformer, record the duty cycle to output voltage of the buck converter from 10V to 2V in 0.25V intervals.
		- If duty cycle to voltage is nonlinear, use a line of best fit to estimate.
	- Some extra notes
		- These ***won't*** be the final numbers used to associate output for Channel A and PWM duty cycle, since there will be a voltage drop (i.e. there's a diode in line to prevent reverse polarity). 
		- The buck converter is expected to output 10V to 1V in normal operation. The 2V lower end is because the buck converter can operate strange in the lower voltage ends, so the 1V should be tested more thoroughly later on.
### Get the interval for voltage checked by cult.

## Channel A Current Sense
1. This is to test the current sense.
2. Solder on **C31, C32, C33, D10, D11, R25, R30** from the *Current Sense* and test points `OUT_A`.
3. Connect a $100\Omega$ resistor between `OUT_A` and `A_GND`.
4. Testing current sense.
	- Set the transformer output voltage to be 18V, 12V, 6V.
	- Make sure the MCU reads 0.18A, 0.12A, and 0.06A within a ±0.01A tolerance.

## Channel A OVP
1. This is for the OVP circuit.
2. Solder on **D9, D12, D13, J4, Q13, Q15, Q44, R26, R27, R31, R33** from *OVP* and test points `A_PROT_OUT`.
	- Place a 3A on the fuse holder, J4.
3. Testing the voltage drop
	- Record the voltage drop across `OUT_A` & `A_PROT_OUT` (should be approximately 0.7V).

## Channel A Enable
1. This is testing Channel A enable.
2. Solder on **Q17, Q19, R24, R37** from *Enable* and test points `CHANNEL_A`.
3. Test the enable signal.
	- Send a LOW to *A_OUT_EN* and make sure `OUT_A` is giving a VDC output, but `CHANNEL_A` is low.
	- Send a HIGH to *A_OUT_EN* and make sure `OUT_A` and `CHANNEL_A` are nearly equal. If there is a voltage drop, record it.
4. Remap all the duty cycle to output voltage.
	- Redo the calculations for duty cycles to output voltage from the Channel A Transformer and Channel A HV/LV where the output voltage is measured at `CHANNEL_A` instead of from `FLY_OUT_A`.
	- Make sure to get the 12V level measured!
### Get the step 4 checked by cult cause we use feedback signal on section below. NVM it's needed for LCD screen!

## Channel A Feedback
1. This is to test the feedback signal from Channel A output.
2. Solder on **D29, R67, R68, R69, U9** from *Feedback* and test points `A_SENSE_IN`.
3. Test the feedback signal.
	- Measure the feedback signal from the MCU, *A_VOL_SENSE*, map it to the calculated output voltage from the OpAmp circuit, and see how it compares to the actual `CHANNEL_A` output (should be accurate within 50mV).
	- With this new mapping, the MCU should use *A_VOL_SENSE* as the actual voltage and adjust *A_DRIVE* (or *A_BUCK*) accordingly.

## Channel A External Connections
1. This is testing the input button for Channel A.
2. Solder on external JST connectors for Channel A labelled **LCD_A, BUTTON_A, A_V_POT, A_C_POT** and the XT30 connecter, **CHANNEL_A_CONN**, from the *External*.
3. Test to make sure these connections work.
	- The LCD screen is on and is changing as appropriately.
	- The pot inputs do adjust their respective voltage and current values to pins *A_V_KNOB* & *A_C_KNOB*.
	- The button does send a HIGH signal to *A_BUTT*.
	- The XT30 connector does have the same output as `CHANNEL_A`. 
		- If the XT30 connector output has ripples, go to step 4, otherwise move onto the next section.
4. Solder on **R62, C44**.
	- Make sure the **C44** is discharged after the universal wait time.

## Channel A Testing
1. This is to make sure Channel A is working properly independently.
2. Make sure the voltage can be set from 1V-20V.
	- Set the voltage to 1V, 6V, 12V, 18V, 20V and that voltage appears at `CHANNEL_A` and the XT30 connector (no load).
3. Make sure the current can be set from 0-3A.
	- Set a current limit to 0.10A with the voltage to 6V and connect the output to a $100\Omega$ resistor to make sure the output current is correct.
	- Set a current limit to 0.03A with the voltage to 6V and connect the output to a $100\Omega$ resistor to make sure the current limit turns on.
3. Leave the power supply connected to a load for 30mins-60mins. 
	- Make sure it is still working properly and there isn't any significant change (i.e. it doesn't become hot).

# 3. USB-C Channel C



# 4. Parallel Mode Testing




# 5. Series Mode Testing



# ESP32 Scripts
- Script 1: 
	- what this does
