/*
 * ADC.h
 *
 *  Created on: Apr 13, 2022
 *      Author: natha
 */

#ifndef ADC_H_
#define ADC_H_

    int A0_result;
    int A0_value();

    void ADC();
    void ADC_1();
    void ADC_2();
    //void PWM_State(int val);
    //void LED_Feedback(int inch);

    void init_PWM();

    int Brightness;




#endif /* ADC_H_ */
