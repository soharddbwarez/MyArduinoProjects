/*
 * LEDseq.h
 *
 *  Created on: 26.12.2014
 *      Author: andrey
 */

#ifndef LEDSEQ_H_
#define LEDSEQ_H_

#include <Arduino.h>

//#define USE_TIMER1
#define USE_TIMER2

#if defined(__AVR_ATmega2560__)
//#define USE_TIMER3
//#define USE_TIMER4
//#define USE_TIMER5
#endif

#if defined(USE_TIMER1) || defined(USE_TIMER3) || defined(USE_TIMER4) || defined(USE_TIMER5)
//#define DEF_PRESCALER   (TimerW::Prescaler_1)
//#define DEF_PRESCALER   (TimerW::Prescaler_8)
//#define DEF_PRESCALER   (TimerW::Prescaler_64)
#define DEF_PRESCALER   (TimerW::Prescaler_256)
//#define DEF_PRESCALER   (TimerW::Prescaler_1024)
#elif defined(USE_TIMER2)
//#define DEF_PRESCALER   (Prescaler2::Prescaler_1)
//#define DEF_PRESCALER   (Prescaler2::Prescaler_8)
//#define DEF_PRESCALER   (Prescaler2::Prescaler_32)
//#define DEF_PRESCALER   (Prescaler2::Prescaler_64)
#define DEF_PRESCALER   (Prescaler2::Prescaler_128)
//#define DEF_PRESCALER   (Prescaler2::Prescaler_256)
//#define DEF_PRESCALER   (Prescaler2::Prescaler_1024)
#else
#error "LEDseq library TODO: Add support to the timer here."
#endif

class LEDseq
{
    unsigned char  m_pinNo;
    const char   **m_states;
    char           m_stateCount;
    const short   *m_delays;
    unsigned char  m_delayCount;
    char  		   m_curState;
    LEDseq        *m_previousSeq;
    static LEDseq *s_ledSeq;
    volatile short m_counter;
    short          m_statePtr;
    const char    *m_charactersUsed;
    bool           m_ledOn;
    short          m_curDelay;
public:
#if defined(USE_TIMER1) || defined(USE_TIMER3) || defined(USE_TIMER4) || defined(USE_TIMER5)
    typedef TimerW::ClkSelect ClkSelect;
#elif defined(USE_TIMER2)
    typedef Prescaler2::ClkSelect ClkSelect;
#else
#error "LEDseq library TODO: Add support to the timer here."
#endif
    typedef unsigned short Param;
    typedef void (*CallbackFunc)(LEDseq* obj, Param param);
protected:
    static ClkSelect s_prescaler;
    CallbackFunc     m_onSeqEnd;
    CallbackFunc     m_onSeqStop;
    Param            m_seqEndParam;
    Param            m_seqStopParam;
public:
	LEDseq(unsigned char pinNo,             // LED pin number.
		   const char *states[],        // Sequences of states
		   char stateCount,             // Count of states
		   const short delays[],        // Delays
		   unsigned char delayCount,    // Count of delays
		   const char charactersUsed[], // Characters that used
		   LEDseq *previousSeq = 0);    // Link to previous LEDseq object
	/*
	 *  Return current state.
	 */
        inline unsigned char state() const {
            return m_curState;
        }
	/*
	 *  Select new current state.
	 */
	void setState(unsigned char state);
	/*
	 *  Loop iteration hadler.
	 *  Should be called from loop() function.
	 */
	static void loop();
	/*
	 *  Switch on the LED and stop blinking.
	 */
	void ledOn();
	/*
	 *  Switch off the LED and stop blinking.
	 */
	void ledOff();
        /*
         *  Return Sequence End callback function pointer.
         */
        inline CallbackFunc onSeqEnd() const {
            return m_onSeqEnd;
        }
        /*
         *  Set new Sequence End callback function.
         *  Parameter will be sent in callback function call.
         */
        void setOnSeqEnd(CallbackFunc callback, Param param = 0);
        /*
         *  Return Sequence Stop callback function pointer.
         */
        inline CallbackFunc onSeqStop() const {
            return m_onSeqStop;
        }
        /*
         *  Set new Sequence Stop callback function.
         *  Parameter will be sent in callback function call.
         */
        void setOnSeqStop(CallbackFunc callback, Param param = 0);
        /*
         *  Return LEDseq timer prescaler value.
         */
	inline static ClkSelect clockSelect() {
	    return s_prescaler;
	}
        /*
         *  Set LEDseq timer prescaler value.
         */
	static void setClockSelect(ClkSelect prescaler);
	/*
	 *  TIMER1 Overflow Interrupt handler.
	 *  Should be called from ISR(TIMER1_OVF_vect) function.
	 */
	static inline void handleISR() {
		LEDseq *p = s_ledSeq;
		while (p) {
			--(p->m_counter);
			p = p->m_previousSeq;
		}
	}
protected:
	void loopHandler();
};

#endif /* LEDSEQ_H_ */
