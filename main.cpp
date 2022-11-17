/********************************************************************************
* main.cpp: Demonstration av klass f�r implementering av AD-omvandling
*           f�r analoga signaler. I detta fall l�ses analog pin A0 / PORTC0
*           f�r PWM-styrning av tre lysdioder med en periodtid p� 10 ms.
********************************************************************************/
#include "led.hpp"
#include "button.hpp"
#include "led_vector.hpp"
#include "adc.hpp"

/********************************************************************************
* main: Ansluter tre lysdioder till pin 8 - 10, en tryckknapp till pin 13 samt 
*       en potentiometer till analog pin A0. Lysdioderna lagras i en dynamisk
*       vektor f�r kollektiv PWM-styrning. Vid nedtryckning av tryckknappen
*       sker avl�sning av den analoga insignalen fr�n potentiometern och anv�nds 
*       f�r att kontrollera ljusstyrkan p� lysdiodenrna, �vrig tid h�lls de
*       sl�ckta. En PWM-period p� 10 000 us anv�nds.
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

