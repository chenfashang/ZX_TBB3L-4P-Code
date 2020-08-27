  solution "TBB3L-4P-B-Code"
  configurations { "RS485" }
  project"TBB3L-4P-B-Code"
  kind "ConsoleApp"
  language "C"
  filter "configurations:RS485"
  sysincludedirs  {"$(VC_IncludePath)"}
  defines { "STM32F10X_HD", "USE_STDPERIPH_DRIVER", "_IAR_", "__ICCARM__", "__CC_ARM", "_Pragma(x)=", "__interrupt=", "__packed=__attribute__((__packed__))", "__weak=__attribute__((weak))" }
  forceincludes { "" }
  includedirs { "../HARDWARE/LED", "../SYSTEM/delay", "../SYSTEM/sys", "../SYSTEM/usart", "../USER", "../STM32F10x_FWLib/inc", "../CORE", "../HARDWARE/KEY", "../HARDWARE/LCD", "../HARDWARE/RTC", "../HARDWARE/RTC", "../HARDWARE/WKUP", "../HARDWARE/ADC", "../HARDWARE/DAC", "../HARDWARE/DMA", "../HARDWARE/IIC", "../HARDWARE/24CXX", "../HARDWARE/SPI", "../HARDWARE/FLASH", "../HARDWARE/TOUCH", "../HARDWARE/RS485", "../HARDWARE/LEAK_TEST", "C:/Keil_v5/ARM/RV31" }
  files { "./main.c", "./stm32f10x_it.c", "./system_stm32f10x.c", "../HARDWARE/LED/led.c", "../HARDWARE/KEY/key.c", "../HARDWARE/LCD/lcd.c", "../HARDWARE/RS485/rs485.c", "../HARDWARE/LEAK_TEST/LeakTest.c", "../SYSTEM/delay/delay.c", "../SYSTEM/sys/sys.c", "../SYSTEM/usart/usart.c", "../CORE/core_cm3.c", "../CORE/startup_stm32f10x_hd.s", "../STM32F10x_FWLib/src/misc.c", "../STM32F10x_FWLib/src/stm32f10x_fsmc.c", "../STM32F10x_FWLib/src/stm32f10x_gpio.c", "../STM32F10x_FWLib/src/stm32f10x_rcc.c", "../STM32F10x_FWLib/src/stm32f10x_usart.c", "../README.TXT"}
  vpaths {["USER"] = { "./main.c" , "./stm32f10x_it.c" , "./system_stm32f10x.c" } , ["HARDWARE"] = { "../HARDWARE/LED/led.c" , "../HARDWARE/KEY/key.c" , "../HARDWARE/LCD/lcd.c" , "../HARDWARE/RS485/rs485.c" , "../HARDWARE/LEAK_TEST/LeakTest.c" } , ["SYSTEM"] = { "../SYSTEM/delay/delay.c" , "../SYSTEM/sys/sys.c" , "../SYSTEM/usart/usart.c" } , ["CORE"] = { "../CORE/core_cm3.c" , "../CORE/startup_stm32f10x_hd.s" } , ["FWLib"] = { "../STM32F10x_FWLib/src/misc.c" , "../STM32F10x_FWLib/src/stm32f10x_fsmc.c" , "../STM32F10x_FWLib/src/stm32f10x_gpio.c" , "../STM32F10x_FWLib/src/stm32f10x_rcc.c" , "../STM32F10x_FWLib/src/stm32f10x_usart.c" } , ["README"] = { "../README.TXT" } }
