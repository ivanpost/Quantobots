
float err_arr[5] = {0,0,0,0,0}; // Уменьшили размер
int err_p = 0;
unsigned long lastPIDTime = 0;

<PIDtakt> = false;
if (millis() - lastPIDTime >= 2) {
    lastPIDTime = millis();

    err_p = (err_p + 1) % 5; // Цикл теперь по 5
    err_arr[err_p] = <currentError>;

    float P = err_arr[err_p] * <kP>;

    // D-составляющая (dStep теперь от 1 до 4)
    int past_p = (err_p + 5 - constrain(<dStep5.Name>,1,4)) % 5; 
    float D = (err_arr[err_p] - err_arr[past_p]) * <kD>;

    // I-составляющая (среднее по 5 точкам)
    float err_sum = 0;
    for (int i = 0; i < 5; i++) {
        err_sum += err_arr[i];
    }
    float I = (err_sum / 5.0) * <kI>; 

    float result = P + I + D;
      // Ограничиваем, чтобы не "срывало" моторы
    float limit = <avgSpeed.Name> * 1.8;
    <PID_Out> = constrain(result, -limit, limit);
    <PIDtakt> = true;
}