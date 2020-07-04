#include "CRandomGroup.h"


void CRandomGroup::SetRangeRandom() {

	int currentTotalValue = 0;
	int tempValue = 0;

	for (int j = 0; j < listRandomItem.size() ; j++) {


		int tempValue = listRandomItem.at(j)->GetRange() * 2;

		currentTotalValue += tempValue;

		listRandomItem.at(j)->beginRan = currentTotalValue - tempValue + 1;

		listRandomItem.at(j)->endRan = currentTotalValue;


	}

	totalValue = currentTotalValue;

}




int CRandomGroup::Random() {


	int tempValue = 0;

	while (tempValue == 0) {

		tempValue = rand() % totalValue + 1;

	}




	for (int i = 0; i < listRandomItem.size(); i++) {

		if (tempValue >= listRandomItem.at(i)->beginRan && tempValue <= listRandomItem.at(i)->endRan) {

			return listRandomItem.at(i)->GetValue();
		}

	}


}



void CRandomGroup::EditRangeRandomItem(int Value, int Range) {


	for (int i = 0; i < listRandomItem.size(); i++) {


		if (listRandomItem.at(i)->GetValue() == Value) {

			listRandomItem.at(i)->SetRange(Range);

			return;
		}

	}

}