/*
 * INTset.h
 *
 * Created: 2018-11-05 오전 6:23:45
 *  Author: njg71
 */ 


#ifndef INTSET_H_
#define INTSET_H_

#include <avr/interrupt.h>

void INT_disableSENSOR();
void INT_enableSENSOR();
void INT_init();

#endif /* INTSET_H_ */