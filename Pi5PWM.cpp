#include "Pi5PWM.hpp"
#include <iostream>
#include <system_error>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <sys/mman.h>

using namespace std;


void * Pi5PWM::base=NULL;
uint32_t * Pi5PWM::pwm0=NULL;
uint32_t * Pi5PWM::iobank0=NULL;
uint32_t * Pi5PWM::iobank0pads=NULL;
uint32_t * Pi5PWM::sys_clock=NULL;
uint32_t * Pi5PWM::rio0=NULL;


int Pi5PWM::setIOMap()
{
   const char *fname = "/sys/bus/pci/devices/0000:01:00.0/resource1";
   int fd;
   if (-1 == (fd = open(fname, O_RDWR | O_SYNC)))
      {
       throw system_error(EIO,generic_category());
       return(0);
      }
   // 0x400000 is the size in bytes of the "resource1" sysfs file
   base =  mmap(NULL, 0x400000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   if (base == MAP_FAILED)
      {
       throw system_error(EIO,generic_category());
       return(0);
      }
   if (-1 == close(fd))
     {
       throw system_error(EIO,generic_category());
      return(0);
     }
   pwm0 = (uint32_t *)((uint8_t *) base + OFFSET_PWM0);
   iobank0 = (uint32_t *)((uint8_t *) base + OFFSET_IOBANK0);
   iobank0pads = (uint32_t *)((uint8_t *) base + OFFSET_IOBANK0PADS);
   sys_clock = (uint32_t *)((uint8_t *) base + OFFSET_CLOCK_MAIN);
   rio0 = (uint32_t *)((uint8_t *) base + OFFSET_RIO0); // real gpio output not used
   // set PWM clock
   sys_clock[CLK_PWM0_CTRL/4]=0x11000800;
   sys_clock[CLK_PWM0_DIV_INT/4]=1;
   sys_clock[CLK_PWM0_DIV_FRAC/4]=0;
   sys_clock[CLK_PWM0_SEL/4]=1;
 return 1;
}

void  Pi5PWM::releaseIOMap()
{
  if(base)
    munmap(base,0x400000);
base = NULL;
pwm0 = NULL;
iobank0=NULL;
iobank0pads=NULL;
sys_clock=NULL;
rio0=NULL;
}



Pi5PWM::Pi5PWM(int pin)
{
  currentPWM = getPWMIdx(pin);
  if(currentPWM>=0)
  {
    currentPin=pin;


    if(Pi5PWM::base==NULL)
      setIOMap();
    setPin();
  }
  else
  {
    currentPin=-1;
    throw out_of_range( "Pin " + to_string(pin) + " is not PWM capable");

  }
}

Pi5PWM::~Pi5PWM()
{
   if(currentPWM<0)
      return;
   releasePin();
}



int Pi5PWM::getPWMIdx(int pin)
{
   switch(pin)
	{
	   case 12:  return 0;
	   case 13:  return 1;
	   case 14:
	   case 18:  return 2;
	   case 15:
	   case 19:  return 3;
	}
   return -1;
}



int Pi5PWM::range(uint32_t Range)
{
  if(currentPWM<0) return 0;
  uint32_t  _Range =  (uint32_t) (factor_us * Range);
  pwm0[PWM_CHANNEL[currentPWM]+PWM_CHAN_RANGE]= _Range;
  return 1;
}

int Pi5PWM::duty(uint32_t Duty)
{
  if(currentPWM<0) return 0;
  uint32_t  _Duty =  (uint32_t) (factor_us * Duty);
  pwm0[PWM_CHANNEL[currentPWM]+PWM_CHAN_DUTY]= _Duty;
  return 1;
}

int Pi5PWM::enable(bool Enable)
{
  if(currentPWM<0) return 0;
  uint64_t temp= 1<< currentPWM;
  if(!Enable)
    {
       temp= (~temp) & 0xf;
       temp =(pwm0[0] & temp) | 0x80000000;
    }
    else
    {
      temp = pwm0[0] | temp | 0x80000000;
    }

  pwm0[0]=temp;
  return 1;
}

int Pi5PWM::setPin()
{
  if(currentPWM<0) return 0;
  enable(false);
  //set pin to PWM
  iobank0[1+(currentPin*2)]= (currentPin > 15) ? 3 : 0;
  iobank0pads[1+currentPin]=0x56;

  // set default 1500
    pwm0[PWM_CHANNEL[currentPWM]+PWM_CHAN_CTRL]= 257;
    pwm0[PWM_CHANNEL[currentPWM]+PWM_CHAN_PHASE]=   0;
    pwm0[PWM_CHANNEL[currentPWM]+PWM_CHAN_RANGE]= 615318;
    pwm0[PWM_CHANNEL[currentPWM]+PWM_CHAN_DUTY]= 45455;
  return 1;
}

int Pi5PWM::releasePin()
{
 if(currentPWM<0) return 0;
  enable(false);
  iobank0[1+(currentPin*2)]= 0x9f;
  iobank0pads[1+currentPin]= 0x96;
  return 1;
}

