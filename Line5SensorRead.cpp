/*
<S1.DI>, <S2.DI>, <S3.DI>, <S4.DI>, <S5.DI>  - параметры блока - входы Arduino
<exitForce.Name> - параметр блока - переменная проекта  - форсирование вылета - по умолчанию 6.0
<invertSens> - вход блока - инверсия датчиков - по умолчанию false
currentError - текущая ошибка, идет на  одноименный выход блока
sensorMask  - маска состояния датчиков для диагностики - идет на выход блока.
*/

const int sensorPins[] = {<S1.DI>, <S2.DI>, <S3.DI>, <S4.DI>, <S5.DI>};
const float weights[] = {-4.0, -2.0, 0.0, 2.0, 4.0}; // Дробные веса для точности

int sensorMask = 0; // Сюда запишем состояние (битовую маску)
float currentError = 0;
float lastError = 0;

void loop() {   // место вставки блока

 // 1. ОПРОС ДАТЧИКОВ - ВСЕГДА (максимальная частота)
  // Это гарантирует, что мы не пропустим 2-сантиметровую линию
  currentError = calculateError(); 

}  // конец loop()

float calculateError() {
	float sumWeights = 0;
	int activeSensors = 0;
	int currentMask = 0; // Временная маска для этого цикла
 
	for (int i = 0; i < 5; i++) {
	  int rawVal = digitalRead(sensorPins[i]);
 
	  // Если линия черная (invertSens = false), то !rawVal превратит 0 в 1
	  int val = ((<invertSens>)) ? rawVal : !rawVal; 
 
	  if (val == 1) {
		 sumWeights += weights[i];
		 activeSensors++;
		 // 2. Формируем маску: записываем "1" в нужный бит (слева направо)
		 currentMask |= (1 << (4 - i));
	  }
	}
	sensorMask = currentMask; // Передаем готовую маску на выход блока
 
	if (activeSensors > 0) {
	  // Теперь ошибка может быть, например, -3.0, -1.0, 1.0, 3.0 и т.д.
	  float currentError = sumWeights / (float)activeSensors;
	  lastError = currentError;
	  return currentError;
	} else {
	  // "Память" вылета. Возвращаем чуть больше максимума.
	  float eForceM = constrain((<exitForce.Name>), 5.0,12.0);
	  if (lastError < -1.5) return -eForceM; 
	  if (lastError > 1.5) return eForceM;
	  return 0;
	}
 }