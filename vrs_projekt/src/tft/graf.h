/*
 * graf.h
 *
 *  Created on: 6. 12. 2016
 *      Author: Juraj
 */

#ifndef TFT_GRAF_H_
#define TFT_GRAF_H_

#include <tft/ili9163.h>

#define GRAFOFFSETX lcdTextX(1)
#define GRAFOFFSETY (lcdTextY(2)+20)
#define GRAFVYSKA 80
#define GRAFSIRKA 100
#define MINYGRAF 20
#define MAXYGRAF 80
#define k (float)(GRAFVYSKA/(float)(MINYGRAF-MAXYGRAF))
#define q GRAFOFFSETY-(MAXYGRAF*k)

void vykresliOsiGrafu();
void pridajPozadovanuHodnotuDoGrafu(float temp);
void pridajAktualnuHodnotuDoGrafu(float temp);

#endif /* TFT_GRAF_H_ */
