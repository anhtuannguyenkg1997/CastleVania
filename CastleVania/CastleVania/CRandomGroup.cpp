#include "CRandomGroup.h"


//Tạo ra begin và end value cho 1 Item Random dựa vào tỉ lệ của nó
void CRandomGroup::SetRangeRandom() {

	//Lưu tạm totalValue
	int currentTotalValue = 0;

	//Vùng giá trị tạo ra với tỉ lệ này
	int tempValue = 0;

	for (int j = 0; j < listRandomItem.size() ; j++) {

		//mặc định là 1% sẽ tương ứng với 2 số kiểu int
		int tempValue = listRandomItem.at(j)->GetRange() * 2;

		//Cộng dồn tempValue ta sẽ có  currentTotalValue
		currentTotalValue += tempValue;

		//Giá trị tối thiểu random ra Item này
		listRandomItem.at(j)->beginRan = currentTotalValue - tempValue + 1;

		//Giá trị tối đa random ra Item này
		listRandomItem.at(j)->endRan = currentTotalValue;


	}

	//Gán lại currentTotalValue cho totalValue
	totalValue = currentTotalValue;

}



//Random giá trị Item của Group này
int CRandomGroup::Random() {

	//Không cho random ra giá trị 0 (vì không nằm trong vùng random của Item nào cả)
	int tempValue = 0;

	while (tempValue == 0) {

		tempValue = rand() % totalValue + 1;

	}



	//Duyệt các Item trong Group
	for (int i = 0; i < listRandomItem.size(); i++) {

		//Nếu phạm vi nằm trong vùng của Item nào thì return ngay về giá trị của Item đó
		if (tempValue >= listRandomItem.at(i)->beginRan && tempValue <= listRandomItem.at(i)->endRan) {

			return listRandomItem.at(i)->GetValue();
		}

	}


}


//Truyền vào giá trị của Item và tỉ lệ mới
void CRandomGroup::EditRangeRandomItem(int Value, int Range) {

	//Duyệt qua tất cả các loại Random đang có
	for (int i = 0; i < listRandomItem.size(); i++) {

		//TÌm đến list chứa Item đó và thay đổi lại giá trị
		if (listRandomItem.at(i)->GetValue() == Value) {

			listRandomItem.at(i)->SetRange(Range);

			return;
		}

	}

}