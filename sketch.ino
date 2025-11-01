// كود Stepper Motor مبسط لـ Wokwi
// ESP32 مع محرك Stepper مباشرة (بدون Driver)

// تعريف منافذ المحرك
#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17

// تسلسل الخطوات (Half-Step Sequence)
const int stepSequence[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

int currentStep = 0;
const int stepsPerRevolution = 2048; // لمحرك 28BYJ-48 (استخدم 200 للمحركات الثانية)

void setup() {
  // تهيئة المنافذ
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(115200);
  Serial.println("=== Stepper Motor على Wokwi ===");
  Serial.println("استعد للتشغيل...\n");
  delay(2000);
}

void loop() {
  // ═══════════════════════════════════════
  // Task 1: دورة كاملة في كل اتجاه
  // ═══════════════════════════════════════
  
  Serial.println("→ الدوران مع عقارب الساعة (360°)");
  rotateDegrees(360, 2, true);  // 360 درجة، سرعة 2ms، مع عقارب الساعة
  
  delay(1000);
  
  Serial.println("← الدوران عكس عقارب الساعة (360°)");
  rotateDegrees(360, 2, false);  // 360 درجة، سرعة 2ms، عكس عقارب الساعة
  
  Serial.println("✓ Task 1 مكتمل!\n");
  delay(2000);
  
  // ═══════════════════════════════════════
  // Task 2: التحكم في السرعة
  // ═══════════════════════════════════════
  
  Serial.println("\n⚡ Task 2: بدء التسارع...");
  
  // 1. التسارع التدريجي
  for(int speed = 10; speed >= 1; speed--) {
    rotateDegrees(90, speed, true);
    Serial.print("السرعة: ");
    Serial.println(11 - speed);
  }
  
  // 2. التشغيل بأقصى سرعة
  Serial.println("🚀 أقصى سرعة لمدة ثانيتين...");
  unsigned long startTime = millis();
  while(millis() - startTime < 2000) {
    stepMotor(true, 1);
  }
  
  Serial.println("⏸️  توقف لمدة ثانيتين");
  stopMotor();
  delay(2000);
  
  // 3. التباطؤ التدريجي
  Serial.println("🐌 التباطؤ التدريجي...");
  for(int speed = 1; speed <= 10; speed++) {
    rotateDegrees(90, speed, true);
    Serial.print("السرعة: ");
    Serial.println(11 - speed);
  }
  
  stopMotor();
  Serial.println("✓ Task 2 مكتمل!\n");
  delay(3000);
}

// دالة تدوير المحرك بزاوية معينة
void rotateDegrees(int degrees, int speedDelay, bool clockwise) {
  // حساب عدد الخطوات المطلوبة
  long steps = (long)degrees * stepsPerRevolution / 360;
  
  for(long i = 0; i < steps; i++) {
    stepMotor(clockwise, speedDelay);
  }
}

// دالة تحريك المحرك خطوة واحدة
void stepMotor(bool clockwise, int delayTime) {
  // تحديد الاتجاه
  if(clockwise) {
    currentStep++;
    if(currentStep >= 8) currentStep = 0;
  } else {
    currentStep--;
    if(currentStep < 0) currentStep = 7;
  }
  
  // تطبيق التسلسل
  digitalWrite(IN1, stepSequence[currentStep][0]);
  digitalWrite(IN2, stepSequence[currentStep][1]);
  digitalWrite(IN3, stepSequence[currentStep][2]);
  digitalWrite(IN4, stepSequence[currentStep][3]);
  
  delay(delayTime);
}

// دالة إيقاف المحرك
void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}