/********************************************************************************
* adc.hpp: Innehåller funktionalitet för enkel implementering av AD-omvandling
*          för analoga signaler via klassen adc. AD-omvandling sker via 
*          inläsning av signaler från analoga pinnar A0 - A5 på  Arduino Uno,
*          vilket motsvarar PORTC0 - PORTC5 på ATmega328P.
*
*          Analoga insignaler mellan 0 - 5 V AD-omvandlas till digitala
*          motsvarigheter mellan 0 - 1023. Duty cycle kan användas för
*          PWM-generering och beräknas enligt nedan:
*
*                         Duty cycle = ADC_result / ADC_MAX,
*
*          där ADC_result är resultat avläst från AD-omvandlaren OCH ADC_MAX
*          utgör högsta möjliga avlästa värde, vilket är 1023.0.
********************************************************************************/
#ifndef ADC_HPP_
#define ADC_HPP_

/********************************************************************************
* adc: Strukt för implementering av AD-omvandlare, som möjliggör avläsning
*      av insignaler från analoga pinnar.
********************************************************************************/
class adc
{
private:
   uint8_t pin_ = 0xFF;                     /* Analog pin som skall användas för avläsning. */
   static constexpr auto ADC_MAX_ = 1023.0; /* Högsta digitala värde vid AD-omvandling. */
public:

   /********************************************************************************
   * adc: Initierar analog pin för avläsning och AD-omvandling av insignaler.
   *
   *      - pin : Analog pin som skall läsas för AD-omvandling.
   ********************************************************************************/
   adc(const uint8_t pin)
   {
      this->pin_ = pin;
      static_cast<void>(this->read());
      return;
   }

   /********************************************************************************
   * ~adc: Nollställer angiven analog pin så att denna inte längre kan användas 
   *       för AD-omvandling.
   ********************************************************************************/
   ~adc(void)
   {
      this->pin_ = -1;
      return;
   }

   /********************************************************************************
   * pin: Returnerar använd analog pin.
   ********************************************************************************/
   uint8_t pin(void) const
   {
      return this->pin_;
   }

   /********************************************************************************
   * max_val: Returnerar högsta möjliga AD-omvandlade värde.
   ********************************************************************************/
   double max_val(void) const
   {
      return this->ADC_MAX_;
   }

   /********************************************************************************
   * read: Läser av en analog insignal och returnerar motsvarande digitala
   *       motsvarighet mellan 0 - 1023.
   ********************************************************************************/
   uint16_t read(void) const
   {
      ADMUX = (1 << REFS0) | this->pin_;
      ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
      while ((ADCSRA & (1 << ADIF)) == 0);
      ADCSRA = (1 << ADIF);
      return ADC;
   }

   /********************************************************************************
   * duty_cycle: Läser av en analog insignal och returnerar motsvarande duty cycle
   *             som ett flyttal mellan 0 - 1.
   ********************************************************************************/
   double duty_cycle(void) const
   {
      return this->read() / this->ADC_MAX_;
   }

   /********************************************************************************
   * get_pwm_values: Läser av en analog insignal och beräknar on- och off-tid för
   *                 för PWM-generering, avrundat till närmaste heltal.
   *
   *                 - pwm_period_ms: PWM-perioden mätt i ms (on-tid + off-tid).
   *                 - pwm_on_ms    : Referens till adress där beräknad on-tid
   *                                  för PWM-generering lagras i enheten ms.
   *                 - pwm_off_ms   : Referens till adress där beräknad off-tid
   *                                  för PWM-generering lagras i enheten ms.
   ********************************************************************************/
   void get_pwm_values(const uint8_t pwm_period_ms,
                       uint8_t& pwm_on_ms,
                       uint8_t& pwm_off_ms) const
   {
      pwm_on_ms = static_cast<uint8_t>(this->duty_cycle() * pwm_period_ms + 0.5);
      pwm_off_ms = pwm_period_ms - pwm_on_ms;
      return;
   }

};

#endif /* ADC_HPP_ */