/*
 * circularbuffer.c
 *
 *  Created on: Feb 24, 2026
 *      Author: yusuf
 */
#include "circular_buffer.h"

void 	 Circular_Buffer_Init 		 (Circular_Buffer_t *circularbuffer)
{
	// 1. REVİZYON: memset sadece ilk 512 byte'ı değil, 512 elemanın tamamını (512 * 6 byte) temizleyecek şekilde güncellendi.
	memset(circularbuffer->buffer, 0, CIRCULAR_BUFFER_SIZE * sizeof(Sensor_Data_t));
	circularbuffer->head = 0;
	circularbuffer->tail = 0;
}

// 2. REVİZYON: Parametre uint8_t yerine Sensor_Data_t olarak güncellendi.
bool  Circular_Buffer_Dequeue     (Circular_Buffer_t *circularbuffer , Sensor_Data_t *data)
{
	if(Circular_Buffer_Is_Empty(circularbuffer))
		return false;

	*data = circularbuffer->buffer[circularbuffer->tail] ;
	circularbuffer->tail = (circularbuffer->tail + 1) % CIRCULAR_BUFFER_SIZE;
	return true;

}
// 3. REVİZYON: Parametre uint8_t yerine Sensor_Data_t olarak güncellendi.
bool 	 Circular_Buffer_Enqueue     (Circular_Buffer_t *circularbuffer , Sensor_Data_t data)
{
	if(Circular_Buffer_Is_Fully(circularbuffer))
		return false;

	 circularbuffer->buffer[circularbuffer->head] = data;
	 circularbuffer->head = (circularbuffer->head + 1) % CIRCULAR_BUFFER_SIZE;
	 return true;
}
//Is Buffer Empty ? | Is Buffer Fully ?
bool 	 Circular_Buffer_Is_Empty 	 (Circular_Buffer_t *circularbuffer)
{
	return (circularbuffer->head == circularbuffer->tail) ? true : false;
}
bool 	 Circular_Buffer_Is_Fully	 (Circular_Buffer_t *circularbuffer)
{
	int div = circularbuffer->head - circularbuffer->tail;
	if(div < 0)
		div = div + CIRCULAR_BUFFER_SIZE;

	return(div == CIRCULAR_BUFFER_SIZE	-	1) ? true : false;
}

uint16_t Circular_Buffer_Count	     (Circular_Buffer_t *circularbuffer)
{
	if(circularbuffer->head >= circularbuffer->tail)
		return(circularbuffer->head - circularbuffer->tail);

	else
		return(circularbuffer->head - circularbuffer->tail + CIRCULAR_BUFFER_SIZE);
}
