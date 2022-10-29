/********************************************************************************
* adc.hpp: Inneh�ller funktionalitet f�r enkel implementering av AD-omvandling
*          f�r analoga signaler via klassen adc. AD-omvandling sker via 
*          inl�sning av signaler fr�n analoga pinnar A0 - A5 p�  Arduino Uno,
*          vilket motsvarar PORTC0 - PORTC5 p� ATmega328P.
*
*          Analoga insignaler mellan 0 - 5 V AD-omvandlas till digitala
*          motsvarigheter mellan 0 - 1023. Duty cycle kan anv�ndas f�r
*          PWM-generering och ber�knas enligt nedan:
*
*                         Duty cycle = ADC_result / ADC_MAX,
*
*          d�r ADC_result �r resultat avl�st fr�n AD-omvandlaren OCH ADC_MAX
*          utg�r h�gsta m�jliga avl�sta v�rde, vilket �r 1023.0.
********************************************************************************/
#ifndef ADC_HPP_
#define ADC_HPP_

/********************************************************************************
* adc: Strukt f�r implementering av AD-omvandlare, som m�jligg�r avl�sning
*      av insignaler fr�n analoga pinnar.
********************************************************************************/
class adc
{
private:
   uint8_t pin_ = 0xFF;                     /* Analog pin som skall anv�ndas f�r avl�sning. */
   static constexpr auto ADC_MAX_ = 1023.0; /* H�gsta digitala v�rde vid AD-omvandling. */
public:

   /********************************************************************************
   * adc: Initierar analog pin f�r avl�sning och AD-omvandling av insignaler.
   *
   *      - pin : Analog pin som skall l�sas f�r AD-omvandling.
   ********************************************************************************/
   adc(const uint8_t pin)
   {
      this->pin_ = pin;
      static_cast<void>(this->read());
      return;
   }

   /********************************************************************************
   * ~adc: Nollst�ller angiven analog pin s� att denna inte l�ngre kan anv�ndas 
   *       f�r AD-omvandling.
   ********************************************************************************/
   ~adc(void)
   {
      this->pin_ = -1;
      return;
   }

   /********************************************************************************
   * pin: Returnerar anv�nd analog pin.
   ********************************************************************************/
   uint8_t pin(void) const
   {
      return this->pin_;
   }

   /********************************************************************************
   * max_val: Returnerar h�gsta m�jliga AD-omvandlade v�rde.
   ********************************************************************************/
   double max_val(void) const
   {
      return this->ADC_MAX_;
   }

   /********************************************************************************
   * read: L�ser av en analog insignal och returnerar motsvarande digitala
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
   * duty_cycle: L�ser av en analog insignal och returnerar motsvarande duty cycle
   *             som ett flyttal mellan 0 - 1.
   ********************************************************************************/
   double duty_cycle(void) const
   {
      return this->read() / this->ADC_MAX_;
   }

   /********************************************************************************
   * get_pwm_values: L�ser av en analog insignal och ber�knar on- och off-tid f�r
   *                 f�r PWM-generering, avrundat till n�rmaste heltal.
   *
   *                 - pwm_period_ms: PWM-perioden m�tt i ms (on-tid + off-tid).
   *                 - pwm_on_ms    : Referens till adress d�r ber�knad on-tid
   *                                  f�r PWM-generering lagras i enheten ms.
   *                 - pwm_off_ms   : Referens till adress d�r ber�knad off-tid
   *                                  f�r PWM-generering lagras i enheten ms.
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