# 1. Independent Channel A 
## Input Circuit

1. Soldered **BR1, F1-F2, J1, R15-R16, R99** from the *Input* and test points for `PRI_GND` & `UNREG_DC`. Place 12A fuse.
   	- AC output on `UNREG_DC` with rocker switch OFF: No Output
   	- AC output on `UNREG_DC` with rocker switch ON:
   	  ![A_InputCircuit_BR1](Images/A_InputCircuit_BR1.png)
2. Desoldered **BR1** and solder on **D1-D4** from the *Input*.
	- AC output on `UNREG_DC` with rocker switch OFF:
	- AC output on `UNREG_DC` with rocker switch ON:
3. **BR1** vs **D1-D4** results:



7. If the AC output for `UNREG_DC` has higher Vpp, then keep **D1-D4** on and move to step 6. Otherwise, desolder **D1-D4**, and solder on **BR1**. 
	- Verify the AC Output for `UNREG_DC` is the same as before if using **BR1** before continuing on.
8. Solder on **C7-C11** from the *Input*.
9. Test the filtered input:
	- AC output on `UNREG_DC` (Expected near DC output, record ripple).
		- Can swap out capacitors with bigger value alternative capacitors from JLab to see if the rippling can be reduced significantly.
	- Check capacitor voltage is 0V after the rocker switch is OFF for 120 seconds.
		- This is the ***universal wait time*** for capacitors to discharge.
		- Note: Might need to wait longer if the capacitance was increased
