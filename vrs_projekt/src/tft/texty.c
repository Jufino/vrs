/*
 * texty.c
 *
 *  Created on: 6. 12. 2016
 *      Author: Juraj
 */

#include <tft/texty.h>

volatile char isShowtextPozadovana = 0;
volatile char isShowtextAktualna = 0;
volatile char isShowtextAkcnyZasah = 0;
volatile float lastShowedValuePozadovana = -100;
volatile float lastShowedValueAktualna = -100;
volatile float lastShowedValueZasah = -100;

void showPozadovanaHodnota(float temp) {
	const int riadok = 0;
	if(temp<-20 || temp > 100){
		lcdPutS("--.--", lcdTextX(14), lcdTextY(riadok),
						decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	}
	else if (temp != lastShowedValuePozadovana) {
		char buffer[20];
		const int decimalTemp = (int) temp;
		const int fragmetTemp = round((temp - decimalTemp) * 10);
		sprintf(buffer, "%d.%d", decimalTemp, fragmetTemp);

		if (!isShowtextPozadovana) {
			lcdPutS("Pozadovana:", lcdTextX(1), lcdTextY(riadok),
					decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		}
		lcdPutS("00.00", lcdTextX(14), lcdTextY(riadok),
				decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		lcdPutS(buffer, lcdTextX(14), lcdTextY(riadok),
				decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		lastShowedValuePozadovana = temp;
	}

}

void showAktualnaHodnota(float temp) {
	const int riadok = 1;
	if(temp<-20 || temp > 100){
		lcdPutS("--.--", lcdTextX(14), lcdTextY(riadok),
				decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	}
	else if (temp != lastShowedValueAktualna) {
		char buffer[20];
		const int decimalTemp = (int) temp;
		const int fragmetTemp = round((temp - decimalTemp) * 10);

		sprintf(buffer, "%d.%d", decimalTemp, fragmetTemp);

		if (!isShowtextPozadovana) {
			lcdPutS("Aktualna:", lcdTextX(1), lcdTextY(riadok),
					decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		}
		lcdPutS("00.00", lcdTextX(14), lcdTextY(riadok),
				decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		lcdPutS(buffer, lcdTextX(14), lcdTextY(riadok),
				decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		lastShowedValueAktualna = temp;
	}
}

void showAkcnyZasah(int status) {
	if (lastShowedValueZasah != status) {
		const int riadok = 2;
		if (!isShowtextPozadovana) {
			lcdPutS("Zasah:", lcdTextX(1), lcdTextY(riadok),
					decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		}
		if (status == 0)
			lcdPutS("0", lcdTextX(14), lcdTextY(riadok),
					decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		else
			lcdPutS("1", lcdTextX(14), lcdTextY(riadok),
					decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		lastShowedValueZasah = status;
	}
}
