/*
 * texty.h
 *
 *  Created on: 6. 12. 2016
 *      Author: Juraj
 */

#ifndef TFT_TEXTY_H_
#define TFT_TEXTY_H_

#include <tft/ili9163.h>

void showPozadovanaHodnota(float temp);
void showAktualnaHodnota(float temp);
void showAkcnyZasah(int status);


#endif /* TFT_TEXTY_H_ */
