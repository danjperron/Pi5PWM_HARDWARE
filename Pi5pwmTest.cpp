#include "Pi5PWM.hpp"
#include <unistd.h>
int main()
{
  Pi5PWM * pwm12 = new Pi5PWM(12);
  Pi5PWM * pwm18 = new Pi5PWM(18);
  //Pi5PWM * pwm23 = new Pi5PWM(23);   //it will throw  out_of_range
  pwm12->duty(1500);
  pwm12->enable(true);
  pwm18->duty(1500);
  pwm18->enable(true);
  usleep(1000000);
  // hey play with the rc servo
  for(int loop=0;loop<10;loop++)
  {
    for(int v = -1000;v<=1000;v+=50)
     {
      pwm12->duty(1500+v);
      pwm18->duty(1500-v);
      usleep(20000);
     }
    for(int v = 1000;v>=-1000;v-=50)
     {
      pwm12->duty(1500+v);
      pwm18->duty(1500-v);
      usleep(20000);
     }
  }
  // return to hone
      pwm12->duty(1500);
      pwm18->duty(1500);
      usleep(1000000);


   delete pwm12;
   delete pwm18;
  Pi5PWM::releaseIOMap();
  return 0;
}
