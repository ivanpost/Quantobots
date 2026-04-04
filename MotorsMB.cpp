/*
Блок управления моторами с плавным подмешиванием minPWM
Логика: исключает рывки при переходе через порог страгивания.
*/

if (<PIDtakt>) {
	// 1. Рассчитываем динамическую базовую скорость
	float speedReduction = abs(<PID_Out>) * <brakeSens.Name>;
	int dynamicSpeed = <avgSpeed.Name> - (int)speedReduction;
	
	// Ограничение снизу для сохранения управляемости
	if (dynamicSpeed < (<minPWM.Name> / 2)) dynamicSpeed = <minPWM.Name> / 2;
	
	// 2. Распределяем мощность ПИД
	float leftRaw = (float)dynamicSpeed + <PID_Out>;
	float rightRaw = (float)dynamicSpeed - <PID_Out>;
	
	// --- 3. ОБРАБОТКА ЛЕВОГО МОТОРА ---
	if (abs(leftRaw) < 3) { // "Мертвая зона" чуть меньше, для чувствительности
		 <LeftPWM.Name> = 0;
	} else {
		 <LeftForward.Name> = (leftRaw >= 0);
		 int val = constrain(abs((int)leftRaw), 0, 255);
		 // Плавное подмешивание: растягиваем рабочий диапазон от minPWM до 255
		 <LeftPWM.Name> = map(val, 0, 255, <minPWM.Name>, 255);
	}
	
	// --- 4. ОБРАБОТКА ПРАВОГО МОТОРА ---
	if (abs(rightRaw) < 3) {
		 <RightPWM.Name> = 0;
	} else {
		 <RightForward.Name> = (rightRaw >= 0);
		 int val = constrain(abs((int)rightRaw), 0, 255);
		 // Плавное подмешивание
		 <RightPWM.Name> = map(val, 0, 255, <minPWM.Name>, 255);
	}
}

// Принудительная остановка
if (!<Run>) {
	<LeftPWM.Name> = 0;
	<RightPWM.Name> = 0;
}
