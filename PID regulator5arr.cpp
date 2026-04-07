/*
ПИД регулятор для легкой (180г) платформы. Линейка - 5 сенсоров линии. База колес - 14 см. Драйвер двигателей - TB6612. Двигатель N20.
Центр тяжести - очень близко к оси колес.
<tInterval> - параметр блока (int) - частота обработки этого регулятора 2-5 мс, по умолчанию - 3.
<avgSpeed.Name> - параметр, переменная проекта (int) - средняя скорость PWM
<dStep5.Name> - параметр, переменная проекта (int) - какое измерение брать для D-составляющей: 2-5, по умолчанию - 2.
<currentError> - вход блока - текущая ошибка из блока считывания датчиков.
<kP> - вход блока - коэффициент kP
<kD> - вход блока - коэффициент kD
<kI> - вход блока - коэффициент kI
<PIDtakt> - выход блока - разрешение на корректировку скорости двигателей
<PID_Out> - выход блока - результат работы регулятора
*/

float err_arr[5] = {0,0,0,0,0}; // Уменьшили размер массива для измерений.
int err_p = 0;
unsigned long lastPIDTime = 0;

<PIDtakt> = false;

if (millis() - lastPIDTime >= <tInterval>) {
    lastPIDTime = millis();

    // 1. Записываем новую ошибку в массив
    err_p = (err_p + 1) % 5;
    err_arr[err_p] = <currentError>;

    // 2. P - Пропорциональная часть
    float P = err_arr[err_p] * <kP>;

    // 3. D - Дифференциальная часть (разница между текущей и прошлой ошибкой)
    // dStep5 (1-4) определяет "глубину" взгляда в прошлое
    int past_p = (err_p + 5 - (int)constrain(<dStep5.Name>, 1, 4)) % 5; 
    float D = (err_arr[err_p] - err_arr[past_p]) * <kD>;

    // 4. I - Интегральная часть (среднее за 5 шагов)
    float err_sum = 0;
    for (int i = 0; i < 5; i++) {
        err_sum += err_arr[i];
    }
    float I = (err_sum / 5.0) * <kI>; 

    // 5. Итог
    float result = P + I + D;
    
    // Ограничение воздействия (чтобы не превышать возможности PWM)
    int maxLimit = constrain((int)(<avgSpeed.Name> * 1.8), 50, 250);
    <PID_Out> = constrain(result, (float)-maxLimit, (float)maxLimit);
    
    <PIDtakt> = true;
}