#include <stdint.h>

class Pi5PWM
{
 public:
    Pi5PWM(int pin);
    ~Pi5PWM();
    int range(uint32_t range);
    int duty(uint32_t duty);
    int enable(bool Enable);
    static int setIOMap();
    static void releaseIOMap();

 private:
    int currentPin;
    int currentPWM;
    int getPWMIdx(int pin);
    int setPin();
    int releasePin();

    static void *base;
    static uint32_t *pwm0;
    static uint32_t *iobank0;
    static uint32_t *iobank0pads;
    static uint32_t *sys_clock;
    static uint32_t *rio0;


    const uint32_t CLOCK = 30303030;
    const double factor_us =  ((double) CLOCK + 500000) / 1000000.0;
    static const uint32_t OFFSET_PWM0= 0x98000;
    static const uint32_t OFFSET_PWM1= 0x98000;
    static const uint32_t OFFSET_IOBANK0=0xd0000;
    static const uint32_t OFFSET_IOBANK0PADS=0xf0000;
    static const uint32_t PWM_GLOBAL_CTRL=0;
    static const uint32_t PWM_FIFO_CTRL=1;
    static const uint32_t OFFSET_CLOCK_MAIN=0x18000;
    static const uint32_t OFFSET_RIO0=0xe0000;

    static const uint32_t CLK_PWM0_CTRL=0x00074;
    static const uint32_t CLK_PWM0_DIV_INT=0x00078;
    static const uint32_t CLK_PWM0_DIV_FRAC=0x0007c;
    static const uint32_t CLK_PWM0_SEL=0x00080;
    static const uint32_t PWM_MAX=4;

    const uint32_t PWM_CHAN0=0x14;
    const uint32_t PWM_CHAN1=0x24;
    const uint32_t PWM_CHAN2=0x34;
    const uint32_t PWM_CHAN3=0x44;
    const uint32_t PWM_CHANNEL[4]={PWM_CHAN0/4,PWM_CHAN1/4,PWM_CHAN2/4,PWM_CHAN3/4};

    const uint32_t PWM_CHAN_CTRL=0;
    const uint32_t PWM_CHAN_RANGE=1;
    const uint32_t PWM_CHAN_PHASE=2;
    const uint32_t PWM_CHAN_DUTY=3;


};

