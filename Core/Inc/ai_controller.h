/*
 * ai_controller.h
 *
 *  Created on: 8 Eki 2026
 *      Author: yusuf
 */

#ifndef INC_AI_CONTROLLER_H_
#define INC_AI_CONTROLLER_H_

#ifndef AI_CONTROLLER_H
#define AI_CONTROLLER_H

#include "circular_buffer.h"
#include "main.h" // (GPIO) tanıyabilmesi için

// Yapay zekayı tetikleyecek tek fonksiyonumuz
void AI_Process_Buffer(Circular_Buffer_t *kuyruk);

#endif /* AI_CONTROLLER_H */

#endif /* INC_AI_CONTROLLER_H_ */
