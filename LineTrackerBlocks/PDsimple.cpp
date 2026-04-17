/*
<tInterval> - параметр блока (int) - частота обработки этого регулятора 3-10 мс 
<filterK.Name> - параметр, переменная проекта (float) - фильтрация ошибки
<avgSpeed.Name> - параметр, переменная проекта (int) - средняя скорость PWM

<currentError> - вход блока - текущая ошибка из блока считывания датчиков.
<kP> - вход блока - коэффициент kP
<kD> - вход блока - коэффициент kD
<PIDtakt> - выход блока - разрешение на корректировку скорости двигателей
<PID_Out> - выход блока - результат работы регулятора
*/

float prevError = 0.0;
unsigned long lastProcessTime =0;
float smoothError = 0.0;

void loop() {
	<PIDtakt> = false;
	// 2. Выполняем расчеты и управление строго каждые 3-10 мс
	if (millis() - lastProcessTime >= <tInterval>) {
		lastProcessTime = millis();
		// Добавьте коэффициент фильтрации (0.1 - 0.5)
		smoothError = (<currentError> * <filterK.Name>) + (smoothError * (1.0 - <filterK.Name>));
		// И дальше в ПИД используйте smoothError вместо currentError
		float P = smoothError * <kP>;
		float D = (smoothError - prevError) * <kD>;
		// Ограничиваем воздействие, чтобы оно не превышало разумных пределов ШИМ
		int maxTurn = <avgSpeed.Name> * 1.8; // Коэффициент 1.5 - 2.0 оптимален
		if (maxTurn > 250) maxTurn = 250; // Но не выше предела Arduino
		<PID_Out> = constrain((P+D), -maxTurn, maxTurn);;
		prevError = smoothError;
		<PIDtakt> = true;
	}
}