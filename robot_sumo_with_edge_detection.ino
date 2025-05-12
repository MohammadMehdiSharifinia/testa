#define EchoPin 3          // پایه Echo سنسور اولتراسونیک
#define TriggerPin 4       // پایه Trigger سنسور اولتراسونیک

#define ENA 6              // PWM موتور A
#define IN1 9              // جهت موتور A
#define IN2 10             // جهت موتور A

#define ENB 5              // PWM موتور B
#define IN3 7              // جهت موتور B
#define IN4 8              // جهت موتور B

#define detectionThresholdMin 0.5   // حداقل فاصله برای توقف (سانتی‌متر)
#define detectionThresholdMax 200   // حداکثر فاصله برای تشخیص دشمن (سانتی‌متر)
#define stepIncrease 20             // افزایش برد تشخیص هنگام گم کردن دشمن
#define switchInterval 200          // زمان تناوب خاموش و روشن شدن موتور A در مرحله جستجوی اولیه

float detectionRange = detectionThresholdMax;  // برد تشخیص فعلی
bool foundTarget = false;   // آیا دشمن پیدا شده است؟
bool initialSearch = true;  // مرحله جستجوی اولیه
unsigned long lastSwitchTime = 0;  // زمان آخرین تغییر وضعیت موتور A
unsigned long lastAngleSwitch = 0;  // زمان آخرین تغییر زاویه
int currentAngle = 10;  // زاویه فعلی برای جستجوی زاویه‌ای
bool toRight = true;  // جهت چرخش به راست یا چپ

void setup() {
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  long distance = measureDistance(); // اندازه‌گیری فاصله با سنسور اولتراسونیک
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance >= detectionThresholdMin && distance <= detectionRange) {
    // دشمن شناسایی شده است
    foundTarget = true;
    initialSearch = false;
    attackEnemy();  // به دشمن حمله می‌کند
  } else {
    if (foundTarget) {
      // دشمن گم شده است
      foundTarget = false;
      detectionRange += stepIncrease;  // افزایش برد تشخیص
      if (detectionRange > detectionThresholdMax) detectionRange = detectionThresholdMax;
      searchAngular();  // جستجوی زاویه‌ای
    } else if (initialSearch) {
      // مرحله جستجوی اولیه
      searchInitial();
    } else {
      // جستجوی زاویه‌ای
      searchAngular();
    }
  }
}

// اندازه‌گیری فاصله با استفاده از سنسور اولتراسونیک
long measureDistance() {
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);

  long duration = pulseIn(EchoPin, HIGH, 20000); // timeout 20ms
  if (duration == 0) return -1; // چیزی پیدا نشد

  return duration * 0.034 / 2.0;
}

// تابع جستجوی اولیه
void searchInitial() {
  unsigned long currentTime = millis();
  static bool motorOn = false;

  if (currentTime - lastSwitchTime >= switchInterval) {
    lastSwitchTime = currentTime;
    motorOn = !motorOn;

    if (motorOn) {
      analogWrite(ENA, 200);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    } else {
      analogWrite(ENA, 0);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
    }

    // موتور B خاموش است
    analogWrite(ENB, 0);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}

// تابع حمله به دشمن
void attackEnemy() {
  // حرکت با تمام سرعت به سمت دشمن
  analogWrite(ENA, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  analogWrite(ENB, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  Serial.println("Attacking the enemy with full speed!");
}

// تابع توقف موتورها
void stopMotors() {
  analogWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  analogWrite(ENB, 0);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  Serial.println("Motors stopped.");
}

// تابع جستجوی زاویه‌ای
void searchAngular() {
  unsigned long now = millis();

  if (now - lastAngleSwitch >= currentAngle * 10) {
    lastAngleSwitch = now;

    // تغییر زاویه و جهت
    currentAngle += 10;
    toRight = !toRight;

    if (currentAngle > 60) currentAngle = 10;  // بازنشانی زاویه پس از 60 درجه
  }

  if (toRight) {
    analogWrite(ENA, 200);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    analogWrite(ENB, 200);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    Serial.println("Rotating to the right...");
  } else {
    analogWrite(ENA, 200);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    analogWrite(ENB, 200);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    Serial.println("Rotating to the left...");
  }
}
