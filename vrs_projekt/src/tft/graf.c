/*
 * graf.c
 *
 *  Created on: 6. 12. 2016
 *      Author: Juraj
 */
#include <tft/graf.h>

volatile int hodnotyGrafu[GRAFSIRKA];
volatile int indexAktualnaHodnota = -1;
volatile float lastShowedValuePozadovanaGraf = -100;
volatile char firstTimeFullGraf = 0;

void vykresliOsiGrafu() {
	lcdLine(GRAFOFFSETX, GRAFOFFSETY + GRAFVYSKA, GRAFOFFSETX, GRAFOFFSETY,
			decodeRgbValue(31, 31, 31)); //os y
	lcdLine(GRAFOFFSETX, GRAFOFFSETY + GRAFVYSKA, GRAFOFFSETX + GRAFSIRKA,
	GRAFOFFSETY + GRAFVYSKA, decodeRgbValue(31, 31, 31)); //os x
}

void pridajPozadovanuHodnotuDoGrafu(float temp) {
	float hodnota = (int) ((float) temp * k + (float) q);
	if (hodnota != lastShowedValuePozadovanaGraf) {
		lcdLine(GRAFOFFSETX + 1, lastShowedValuePozadovanaGraf,
		GRAFOFFSETX + GRAFSIRKA, lastShowedValuePozadovanaGraf,
				decodeRgbValue(0, 0, 0));
		float pozad = (int) ((float) temp * k + (float) q);
		lcdLine(GRAFOFFSETX + 1, pozad, GRAFOFFSETX + GRAFSIRKA, pozad,
				decodeRgbValue(31, 0, 0));
		lastShowedValuePozadovanaGraf = hodnota;
	}
}

void pridajAktualnuHodnotuDoGrafu(float temp) {
	int index = 0;
	float hodnota = (int) ((float) temp * k + (float) q);
	if (firstTimeFullGraf) {
		for (int i = indexAktualnaHodnota + 1; i < GRAFSIRKA+GRAFOFFSETX; i++) {
			lcdPlot(GRAFOFFSETX + index + 1, hodnotyGrafu[i],
					decodeRgbValue(0, 0, 0));
			index++;
		}
	}
	for (int i = 0; i <= indexAktualnaHodnota; i++) {
		lcdPlot(GRAFOFFSETX + index + 1, hodnotyGrafu[i],
				decodeRgbValue(0, 0, 0));
		index++;
	}
	if (++indexAktualnaHodnota >= GRAFSIRKA) {
		indexAktualnaHodnota = 0;
		firstTimeFullGraf = 1;
	}
	hodnotyGrafu[indexAktualnaHodnota] = hodnota;
	index = 0;
	if (firstTimeFullGraf) {
		for (int i = indexAktualnaHodnota + 1; i < GRAFSIRKA; i++) {
			lcdPlot(GRAFOFFSETX + index + 1, hodnotyGrafu[i],
					decodeRgbValue(0, 31, 31));
			index++;
		}
	}
	for (int i = 0; i <= indexAktualnaHodnota; i++) {
		lcdPlot(GRAFOFFSETX + index + 1, hodnotyGrafu[i],
				decodeRgbValue(0, 31, 31));
		index++;
	}
}
