/*
Параметры блока: <S1.DI> ... <S6.DI> - входы платы
Параметры вылета: <exitForce.Name> (рекомендуемое 7.0) переменная - форсирование при вылете
Вход: <invertSens>
Выходы: currentError, sensorMask -> rawSensor
*/

const int sensorPins[] = {<S1.DI>, <S2.DI>, <S3.DI>, <S4.DI>, <S5.DI>, <S6.DI>};
// Веса распределены симметрично: -5, -3, -1, 1, 3, 5
const float weights[] = {-5.0, -3.0, -1.0, 1.0, 3.0, 5.0}; 

int sensorMask = 0; 
float currentError = 0;
float lastError = 0;

void loop() {
  currentError = calculateError(); 
}

float calculateError() {
    float sumWeights = 0;
    int activeSensors = 0;
    int currentMask = 0;
 
    for (int i = 0; i < 6; i++) {
      int rawVal = digitalRead(sensorPins[i]);
      // Инверсия: если invertSens=false, считаем активным LOW (0), преобразуя в 1
      int val = (<invertSens>) ? rawVal : !rawVal; 
 
      if (val == 1) {
         sumWeights += weights[i];
         activeSensors++;
         // Формируем 6-битную маску (S1 - старший бит)
         currentMask |= (1 << (5 - i));
      }
    }
    sensorMask = currentMask;
 
    if (activeSensors > 0) {
      // Средневзвешенная ошибка
      float error = sumWeights / (float)activeSensors;
      lastError = error;
      return error;
    } else {
      // Логика вылета: если последняя ошибка была сильно левее/правее центра
      float eForceM = constrain((<exitForce.Name>), 6.0, 15.0);
      if (lastError < -1.0) return -eForceM; 
      if (lastError > 1.0) return eForceM;
      return 0;
    }
}
