
Things to prepare

-Download mounriver studio from
https://mounriver.com/download
Remember to select between Windows Linux or Mac
-Example code can be downloaded from 
https://github.com/openwch/ch570
(Optional)
-Download code projects from my GitHub
The most relevant one to download is Earth-Computer
Earth Reset is the version that does not have warm-booting

To open a project file in Mounriver Studio IDE, click the "earth-computar.wvproj" and the setup should be automatic.

To compile we can press F7 or Shift-F7 (to recompile)

To flash, we can use two methods

1)USB-C 
-First ensure the chip is not connected to power via UART. It should only be powered by USB, also sometimes USB flashing fails if theres power input from other sources, ie the nordic power profiler
-In top bar go into Tools > WCH In-system programmer 
-Ensure chip series and chip model is correct (CH57X and CH570)
-Port choosen is USB
-Make sure the hex file is correct (usually found in the obj subfolder in the project directory, only available after compiling)
-While holding the download button, turn the chip off and on by flicking the switch
-It should now appear in the device list, then click flash 

2)UART
You need the Link-E USB to UART debugger
You might need to download additional drivers to use this (I didnt have to though but look online if theres issues with this method)
Plug the 3v3 and ground from the UART to the board, plug the SoftwareClock (SWCLK) to PA1 and SoftwareDevelopmentInputOutput (SWDIO) to SysGround to PA0 on the CH570 (this might differ for other controller models)
Press shift + f8, make sure target unit type is correct, target file is correct. Might have to first erase code flash via off/on then try to query device again.
Make sure the following are checked:
Erase All, Program, Verify, Reset and Run, ClearCodeFlash by power off, and if required Disable MCU Code-protect
Then exit and press f8 to flash.
