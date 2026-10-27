// Pini L298N
#define IN1 7   // motor stânga
#define IN2 8
#define IN3 9   // motor dreapta
#define IN4 10
#define BUZZER 6

// Pini senzor HC-SR04
#define TRIG 11
#define ECHO 12

#define DIST_OBSTACOL 20

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  opreste();
  randomSeed(analogRead(A0));
  delay(1000);   // pauză la pornire
}

// ---------- Motoare individuale ----------
void stanga(bool fata) {
  digitalWrite(IN1, fata ? HIGH : LOW);
  digitalWrite(IN2, fata ? LOW : HIGH);
}

void dreapta(bool fata) {
  digitalWrite(IN3, fata ? LOW : HIGH);
  digitalWrite(IN4, fata ? HIGH : LOW);
}

// ---------- Mișcări ----------
void inainte()     { stanga(true);  dreapta(true);  }
void inapoi()      { stanga(false); dreapta(false); }
void viraStanga()  { stanga(false); dreapta(true);  }
void viraDreapta() { stanga(true);  dreapta(false); }

void opreste() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ---------- Senzor ----------
long masoaraDistanta() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long durata = pulseIn(ECHO, HIGH, 30000);  // timeout 30 ms
  if (durata == 0) return 400;               // fără ecou = liber
  return durata * 0.034 / 2;                 // cm
}

// Medie din 3 citiri
long distantaFiltrata() {
  long suma = 0;
  for (int i = 0; i < 3; i++) {
    suma += masoaraDistanta();
    delay(10);
  }
  return suma / 3;
}

// ---------- Evitare obstacol ----------
void eviteObstacol() {
  opreste();

  // buzzer pulsat: 3 bipuri
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(150);
    digitalWrite(BUZZER, LOW);
    delay(100);
  }

  inapoi();
  delay(400);
  opreste();
  delay(200);

  // alege random stânga sau dreapta
  if (random(2) == 0) viraStanga();
  else                viraDreapta();
  delay(random(400, 800));   // unghi de viraj variabil
  opreste();
  delay(200);
}

// ---------- Program principal ----------
void loop() {
  long d = distantaFiltrata();
  Serial.println(d);   // DEBUG: șterge după ce merge

  if (d < DIST_OBSTACOL) {
    eviteObstacol();
  } else {
    inainte();
  }
}
