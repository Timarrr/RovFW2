# Copyright (c) 2020 Arduino CMake Toolchain

###############################################################################
# This is an automatically generated template file for board options.
# You may edit it to comment/uncomment selected board and board options.
# However do not change the structure of this template, which is fixed and 
# any change to the structure gets overwritten.

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Arduino boards.
# Platform: Arduino AVR Boards
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# set(ARDUINO_BOARD "Arduino Yn [avr.yun]") # Arduino Yn
# set(ARDUINO_BOARD "Arduino Uno [avr.uno]") # Arduino Uno
# set(ARDUINO_BOARD "Arduino Duemilanove or Diecimila [avr.diecimila]") # Arduino Duemilanove or Diecimila
# set(ARDUINO_BOARD "Arduino Nano [avr.nano]") # Arduino Nano
# set(ARDUINO_BOARD "Arduino Mega or Mega 2560 [avr.mega]") # Arduino Mega or Mega 2560
# set(ARDUINO_BOARD "Arduino Mega ADK [avr.megaADK]") # Arduino Mega ADK
# set(ARDUINO_BOARD "Arduino Leonardo [avr.leonardo]") # Arduino Leonardo
# set(ARDUINO_BOARD "Arduino Leonardo ETH [avr.leonardoeth]") # Arduino Leonardo ETH
# set(ARDUINO_BOARD "Arduino Micro [avr.micro]") # Arduino Micro
# set(ARDUINO_BOARD "Arduino Esplora [avr.esplora]") # Arduino Esplora
# set(ARDUINO_BOARD "Arduino Mini [avr.mini]") # Arduino Mini
# set(ARDUINO_BOARD "Arduino Ethernet [avr.ethernet]") # Arduino Ethernet
# set(ARDUINO_BOARD "Arduino Fio [avr.fio]") # Arduino Fio
# set(ARDUINO_BOARD "Arduino BT [avr.bt]") # Arduino BT
# set(ARDUINO_BOARD "LilyPad Arduino USB [avr.LilyPadUSB]") # LilyPad Arduino USB
# set(ARDUINO_BOARD "LilyPad Arduino [avr.lilypad]") # LilyPad Arduino
# set(ARDUINO_BOARD "Arduino Pro or Pro Mini [avr.pro]") # Arduino Pro or Pro Mini
# set(ARDUINO_BOARD "Arduino NG or older [avr.atmegang]") # Arduino NG or older
# set(ARDUINO_BOARD "Arduino Robot Control [avr.robotControl]") # Arduino Robot Control
# set(ARDUINO_BOARD "Arduino Robot Motor [avr.robotMotor]") # Arduino Robot Motor
# set(ARDUINO_BOARD "Arduino Gemma [avr.gemma]") # Arduino Gemma
# set(ARDUINO_BOARD "Adafruit Circuit Playground [avr.circuitplay32u4cat]") # Adafruit Circuit Playground
# set(ARDUINO_BOARD "Arduino Yn Mini [avr.yunmini]") # Arduino Yn Mini
# set(ARDUINO_BOARD "Arduino Industrial 101 [avr.chiwawa]") # Arduino Industrial 101
# set(ARDUINO_BOARD "Linino One [avr.one]") # Linino One
# set(ARDUINO_BOARD "Arduino Uno WiFi [avr.unowifi]") # Arduino Uno WiFi

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Arduino boards.
# Platform: Arduino SAMD Boards (32-bits ARM Cortex-M0+)
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# set(ARDUINO_BOARD "Arduino Zero (Programming Port) [samd.arduino_zero_edbg]") # Arduino Zero (Programming Port)
# set(ARDUINO_BOARD "Arduino Zero (Native USB Port) [samd.arduino_zero_native]") # Arduino Zero (Native USB Port)
# set(ARDUINO_BOARD "Arduino MKR1000 [samd.mkr1000]") # Arduino MKR1000
# set(ARDUINO_BOARD "Arduino MKRZERO [samd.mkrzero]") # Arduino MKRZERO
# set(ARDUINO_BOARD "Arduino MKR WiFi 1010 [samd.mkrwifi1010]") # Arduino MKR WiFi 1010
# set(ARDUINO_BOARD "Arduino NANO 33 IoT [samd.nano_33_iot]") # Arduino NANO 33 IoT
# set(ARDUINO_BOARD "Arduino MKR FOX 1200 [samd.mkrfox1200]") # Arduino MKR FOX 1200
# set(ARDUINO_BOARD "Arduino MKR WAN 1300 [samd.mkrwan1300]") # Arduino MKR WAN 1300
# set(ARDUINO_BOARD "Arduino MKR WAN 1310 [samd.mkrwan1310]") # Arduino MKR WAN 1310
# set(ARDUINO_BOARD "Arduino MKR GSM 1400 [samd.mkrgsm1400]") # Arduino MKR GSM 1400
# set(ARDUINO_BOARD "Arduino MKR NB 1500 [samd.mkrnb1500]") # Arduino MKR NB 1500
# set(ARDUINO_BOARD "Arduino MKR Vidor 4000 [samd.mkrvidor4000]") # Arduino MKR Vidor 4000
# set(ARDUINO_BOARD "Adafruit Circuit Playground Express [samd.adafruit_circuitplayground_m0]") # Adafruit Circuit Playground Express
# set(ARDUINO_BOARD "Arduino M0 Pro (Programming Port) [samd.mzero_pro_bl_dbg]") # Arduino M0 Pro (Programming Port)
# set(ARDUINO_BOARD "Arduino M0 Pro (Native USB Port) [samd.mzero_pro_bl]") # Arduino M0 Pro (Native USB Port)
set(ARDUINO_BOARD "Arduino M0 [samd.mzero_bl]") # Arduino M0
# set(ARDUINO_BOARD "Arduino Tian [samd.tian]") # Arduino Tian

#==============================================================================
# Menu options.
# Board: Arduino Yn [avr.yun]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Uno [avr.uno]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Duemilanove or Diecimila [avr.diecimila]
#==============================================================================

# Option: Processor
set(ARDUINO_AVR_DIECIMILA_MENU_CPU_ATMEGA328 TRUE) # ATmega328P
# set(ARDUINO_AVR_DIECIMILA_MENU_CPU_ATMEGA168 TRUE) # ATmega168

#==============================================================================
# Menu options.
# Board: Arduino Nano [avr.nano]
#==============================================================================

# Option: Processor
set(ARDUINO_AVR_NANO_MENU_CPU_ATMEGA328 TRUE) # ATmega328P
# set(ARDUINO_AVR_NANO_MENU_CPU_ATMEGA328OLD TRUE) # ATmega328P (Old Bootloader)
# set(ARDUINO_AVR_NANO_MENU_CPU_ATMEGA168 TRUE) # ATmega168

#==============================================================================
# Menu options.
# Board: Arduino Mega or Mega 2560 [avr.mega]
#==============================================================================

# Option: Processor
set(ARDUINO_AVR_MEGA_MENU_CPU_ATMEGA2560 TRUE) # ATmega2560 (Mega 2560)
# set(ARDUINO_AVR_MEGA_MENU_CPU_ATMEGA1280 TRUE) # ATmega1280

#==============================================================================
# Menu options.
# Board: Arduino Mega ADK [avr.megaADK]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Leonardo [avr.leonardo]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Leonardo ETH [avr.leonardoeth]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Micro [avr.micro]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Esplora [avr.esplora]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Mini [avr.mini]
#==============================================================================

# Option: Processor
set(ARDUINO_AVR_MINI_MENU_CPU_ATMEGA328 TRUE) # ATmega328P
# set(ARDUINO_AVR_MINI_MENU_CPU_ATMEGA168 TRUE) # ATmega168

#==============================================================================
# Menu options.
# Board: Arduino Ethernet [avr.ethernet]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Fio [avr.fio]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino BT [avr.bt]
#==============================================================================

# Option: Processor
set(ARDUINO_AVR_BT_MENU_CPU_ATMEGA328 TRUE) # ATmega328P
# set(ARDUINO_AVR_BT_MENU_CPU_ATMEGA168 TRUE) # ATmega168

#==============================================================================
# Menu options.
# Board: LilyPad Arduino USB [avr.LilyPadUSB]
#==============================================================================

#==============================================================================
# Menu options.
# Board: LilyPad Arduino [avr.lilypad]
#==============================================================================

# Option: Processor
set(ARDUINO_AVR_LILYPAD_MENU_CPU_ATMEGA328 TRUE) # ATmega328P
# set(ARDUINO_AVR_LILYPAD_MENU_CPU_ATMEGA168 TRUE) # ATmega168

#==============================================================================
# Menu options.
# Board: Arduino Pro or Pro Mini [avr.pro]
#==============================================================================

# Option: Processor
set(ARDUINO_AVR_PRO_MENU_CPU_16MHZATMEGA328 TRUE) # ATmega328P (5V, 16 MHz)
# set(ARDUINO_AVR_PRO_MENU_CPU_8MHZATMEGA328 TRUE) # ATmega328P (3.3V, 8 MHz)
# set(ARDUINO_AVR_PRO_MENU_CPU_16MHZATMEGA168 TRUE) # ATmega168 (5V, 16 MHz)
# set(ARDUINO_AVR_PRO_MENU_CPU_8MHZATMEGA168 TRUE) # ATmega168 (3.3V, 8 MHz)

#==============================================================================
# Menu options.
# Board: Arduino NG or older [avr.atmegang]
#==============================================================================

# Option: Processor
set(ARDUINO_AVR_ATMEGANG_MENU_CPU_ATMEGA168 TRUE) # ATmega168
# set(ARDUINO_AVR_ATMEGANG_MENU_CPU_ATMEGA8 TRUE) # ATmega8

#==============================================================================
# Menu options.
# Board: Arduino Robot Control [avr.robotControl]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Robot Motor [avr.robotMotor]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Gemma [avr.gemma]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Adafruit Circuit Playground [avr.circuitplay32u4cat]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Yn Mini [avr.yunmini]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Industrial 101 [avr.chiwawa]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Linino One [avr.one]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Uno WiFi [avr.unowifi]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Zero (Programming Port) [samd.arduino_zero_edbg]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Zero (Native USB Port) [samd.arduino_zero_native]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino MKR1000 [samd.mkr1000]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino MKRZERO [samd.mkrzero]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino MKR WiFi 1010 [samd.mkrwifi1010]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino NANO 33 IoT [samd.nano_33_iot]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino MKR FOX 1200 [samd.mkrfox1200]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino MKR WAN 1300 [samd.mkrwan1300]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino MKR WAN 1310 [samd.mkrwan1310]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino MKR GSM 1400 [samd.mkrgsm1400]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino MKR NB 1500 [samd.mkrnb1500]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino MKR Vidor 4000 [samd.mkrvidor4000]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Adafruit Circuit Playground Express [samd.adafruit_circuitplayground_m0]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino M0 Pro (Programming Port) [samd.mzero_pro_bl_dbg]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino M0 Pro (Native USB Port) [samd.mzero_pro_bl]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino M0 [samd.mzero_bl]
#==============================================================================

#==============================================================================
# Menu options.
# Board: Arduino Tian [samd.tian]
#==============================================================================

#******************************************************************************
# Arduino programmers.
# Platform: Arduino AVR Boards
#******************************************************************************
# set(ARDUINO_PROGRAMMER "AVR ISP [avr.avrisp]") # AVR ISP
# set(ARDUINO_PROGRAMMER "AVRISP mkII [avr.avrispmkii]") # AVRISP mkII
# set(ARDUINO_PROGRAMMER "USBtinyISP [avr.usbtinyisp]") # USBtinyISP
# set(ARDUINO_PROGRAMMER "ArduinoISP [avr.arduinoisp]") # ArduinoISP
# set(ARDUINO_PROGRAMMER "ArduinoISP.org [avr.arduinoisporg]") # ArduinoISP.org
# set(ARDUINO_PROGRAMMER "USBasp [avr.usbasp]") # USBasp
# set(ARDUINO_PROGRAMMER "Parallel Programmer [avr.parallel]") # Parallel Programmer
# set(ARDUINO_PROGRAMMER "Arduino as ISP [avr.arduinoasisp]") # Arduino as ISP
# set(ARDUINO_PROGRAMMER "Arduino as ISP (ATmega32U4) [avr.arduinoasispatmega32u4]") # Arduino as ISP (ATmega32U4)
# set(ARDUINO_PROGRAMMER "Arduino Gemma [avr.usbGemma]") # Arduino Gemma
# set(ARDUINO_PROGRAMMER "BusPirate as ISP [avr.buspirate]") # BusPirate as ISP
# set(ARDUINO_PROGRAMMER "Atmel STK500 development board [avr.stk500]") # Atmel STK500 development board
# set(ARDUINO_PROGRAMMER "Atmel JTAGICE3 (ISP mode) [avr.jtag3isp]") # Atmel JTAGICE3 (ISP mode)
# set(ARDUINO_PROGRAMMER "Atmel JTAGICE3 (JTAG mode) [avr.jtag3]") # Atmel JTAGICE3 (JTAG mode)
# set(ARDUINO_PROGRAMMER "Atmel-ICE (AVR) [avr.atmel_ice]") # Atmel-ICE (AVR)

#******************************************************************************
# Arduino programmers.
# Platform: Arduino SAMD Boards (32-bits ARM Cortex-M0+)
#******************************************************************************
# set(ARDUINO_PROGRAMMER "Atmel EDBG [samd.edbg]") # Atmel EDBG
# set(ARDUINO_PROGRAMMER "Atmel-ICE [samd.atmel_ice]") # Atmel-ICE
# set(ARDUINO_PROGRAMMER "Atmel SAM-ICE [samd.sam_ice]") # Atmel SAM-ICE
# set(ARDUINO_PROGRAMMER "Segger J-Link [samd.jlink]") # Segger J-Link
