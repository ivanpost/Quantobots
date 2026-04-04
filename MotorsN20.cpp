/*
<PIDtakt> - вход блока, когда его нужно обработать.
<PID_Out> - вход блока - выход с ПИД регулятора.
<Run> - вход блока - полная остановка (когда 0)
<LeftForward.Name> - параметр, переменная проекта (bool) - направление лев мотора, 1-вперед
<LeftPWM.Name> - параметр, переменная проекта (int) - pwm лев мотора, 255 max
<RightForward.Name> - параметр, переменная проекта (bool) - направление прав мотора, 1-вперед
<RightPWM.Name> - параметр, переменная проекта (int) - pwm прав мотора, 255 max
<minPWM.Name> - параметр, переменная проекта (int) - pwm минимальный, при котором мотор страгивается
<brakeSens.Name> - параметр, переменная проекта (float) - резкость торможения исходный 0.8
*/

if (<PIDtakt>) {
	// 1. Рассчитываем динамическую базовую скорость (зависит от крутизны поворота)
	float speedReduction = abs(<PID_Out>) * <brakeSens.Name>;
	int dynamicSpeed = <avgSpeed.Name> - (int)speedReduction;
	
	// Не даем базовой скорости упасть ниже половины порога страгивания
	if (dynamicSpeed < (<minPWM.Name> / 2)) dynamicSpeed = <minPWM.Name> / 2;
	
	// 2. Распределяем мощность ПИД на левый и правый борта
	float leftRaw = (float)dynamicSpeed + <PID_Out>;
	float rightRaw = (float)dynamicSpeed - <PID_Out>;
	
	// 3. ОБРАБОТКА ЛЕВОГО МОТОРА
	if (leftRaw >= 0) {
		 <LeftForward.Name> = 1; // Едем вперед
		 // Если сигнал слишком слабый (< 5), выключаем, иначе — не ниже minPWM
		 <LeftPWM.Name> = (leftRaw < 5) ? 0 : constrain((int)leftRaw, <minPWM.Name>, 255);
	} else {
		 <LeftForward.Name> = 0; // РЕВЕРС для крутого разворота (торможение инерции)
		 <LeftPWM.Name> = constrain(abs((int)leftRaw), <minPWM.Name>, 255);
	}
	
	// 4. ОБРАБОТКА ПРАВОГО МОТОРА
	if (rightRaw >= 0) {
		 <RightForward.Name> = 1; // Едем вперед
		 <RightPWM.Name> = (rightRaw < 5) ? 0 : constrain((int)rightRaw, <minPWM.Name>, 255);
	} else {
		 <RightForward.Name> = 0; // РЕВЕРС
		 <RightPWM.Name> = constrain(abs((int)rightRaw), <minPWM.Name>, 255);
	}
	}
	if (!<Run>) {
	  <LeftPWM.Name> = 0;
	  <RightPWM.Name> = 0;
	}