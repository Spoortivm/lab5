#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <tm4c123gh6pm.h>

int clock = 16000000;       // global variable

void GPIO_PORTF_INIT(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20;  //enable clock to portf

    //GPIO_PORTF_LOCK_R = 0X4C4F434B;
    //GPIO_PORTF_CR_R = 0X1F;

    GPIO_PORTF_DIR_R = 0x0E;    //set correct directions

    GPIO_PORTF_DEN_R = 0x1F;    //enable digital operation at pins
    GPIO_PORTF_PUR_R = 0x11;    //enable pullups for switches
    // GPIO_PORTF_DATA_R =  0x0;    // put no data on GPIO



    /* ..........................................below part for interrupts..................................................... */


   /* GPIOIS
    GPIOIBE
    GPIOIEV
    GPIOIM
    GPIORIS
    GPIOMIS
    GPIOICR*/


    GPIO_PORTF_IM_R  = 0x00;

     GPIO_PORTF_IS_R  |= (1<<4) ;   // sw2 level detection only
     GPIO_PORTF_IBE_R &= ~(1<<4);   // int gen controlled by IEV



     GPIO_PORTF_IEV_R &= ~(1<<4); // low level or falling edge triggers int

    //GPIO_PORTF_ICR_R = 0x01;   // clear int // not needed for level sensitive app

     GPIO_PORTF_IM_R  |= (1<<4);  // send int to controller




     //GPIO_PORTF_RIS_R this is set when interrupt occurs.

     //GPIO_PORTF_MIS_R this is set when int has reached int ctrll

    // GPIO_PORTF_ICR_R for level detect this has no effect



}


void delay(float time, int clock)
{

    float val;
    val = (time*clock)/1000;
    NVIC_ST_RELOAD_R = val;        // reload value
    NVIC_ST_CURRENT_R  = 0x0;         // current value
    NVIC_ST_CTRL_R = 0x05;          // enable and choice of clk

    while((NVIC_ST_CTRL_R & (1<<16)) == 0)
      {
            //do nothing
      }
     NVIC_ST_CTRL_R = 0x0;


}

void GPIO_PORTF_Handler(void)
{


            int state0  = GPIO_PORTF_DATA_R ;
            delay(25,clock);                   // arg1 is required delay in ms
            int state1 = GPIO_PORTF_DATA_R;

            if(state0 != state1){                // for  debouncing

            GPIO_PORTF_DATA_R ^= (0x02);     // turn on led
            //GPIO_PORTF_ICR_R |= (1<<0);     // int cleared ??: not required for level sensitive stuff

            }
}


void main(void)
{
    NVIC_EN0_R = (1<<30); // enabling int on PORTF
    GPIO_PORTF_INIT();
    while(1)
    {
       // GPIO_PORTF_DATA_R = 0x0E;

        //GPIO_PORTF_DATA_R &= ~(1<<1);
    }

}

