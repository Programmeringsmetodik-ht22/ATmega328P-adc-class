/********************************************************************************
* main.cpp: Demonstration av klass för implementering av AD-omvandling
*           för analoga signaler. I detta fall läses analog pin A0 / PORTC0
*           för PWM-styrning av tre lysdioder med en periodtid på 10 ms.
********************************************************************************/
#include "led.hpp"
#include "button.hpp"
#include "led_vector.hpp"
#include "adc.hpp"

/********************************************************************************
* main: Ansluter tre lysdioder till pin 8 - 10, en tryckknapp till pin 13 samt 
*       en potentiometer till analog pin A0. Lysdioderna lagras i en dynamisk
*       vektor för kollektiv PWM-styrning. Vid nedtryckning av tryckknappen
*       sker avläsning av den analoga insignalen från potentiometern och används 
*       för att kontrollera ljusstyrkan på lysdiodenrna, övrig tid hålls de
*       släckta. En PWM-period på 10 000 us används.
********************************************************************************/
int main(void)
{
   led l1(8);
   led l2(9);
   led l3(10);
   
   button b1(13); 
   adc a1(A0);

   led_vector v1;

   v1.push(l1);
   v1.push(l2);
   v1.push(l3);

   while (1)
   {
      if (b1.is_pressed())
      {
         a1.get_pwm_values();

         v1.on();
         misc::delay_us(a1.pwm_on_us());

         v1.off();
         misc::delay_us(a1.pwm_off_us());
      }
   }

   return 0;
}

