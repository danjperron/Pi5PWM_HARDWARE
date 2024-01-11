# Pi5PWM_HARDWARE
Raspberry Pi5 using hardware PWM

Using mmap to find hardware peripheral pwm0, RIO, GPIO and system clock.

The python folder contains a version in python using /sys/class/pwm with os.command()

N.B. On python you need to set the dtoverlay=pwm. The C++ doesn't care since I set the register directly.


On the main root you have the c++ class using directly the hardware.


Pi5PWM is a class which provide hardware PWM on GPIO pin 12,13,14,15,18 and 19.
You could select the range and the duty. The step are in micro-second.

Look at the example  Pi5pwmTest.cpp


To compile

  g++ -o pi5pwmTest pi5pwmTest.cpp Pi5PWM.cpp


to run

  sudo ./pi5pwmTest



