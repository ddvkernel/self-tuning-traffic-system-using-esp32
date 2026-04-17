// -------- Sensors --------
int nS=34, sS=35, eS=32, wS=33;

// -------- LEDs --------
int nG=25, nY=26, nR=27;
int sG=14, sY=12,  sR=13;
int eG=18, eY=19, eR=21;
int wG=22, wY=23, wR=5;

// -------- Last Served --------
int lastServed = -1;

// -------- Setup --------
void setup() {
  Serial.begin(115200);

  pinMode(nS, INPUT); pinMode(sS, INPUT);
  pinMode(eS, INPUT); pinMode(wS, INPUT);

  pinMode(nG, OUTPUT); pinMode(nY, OUTPUT); pinMode(nR, OUTPUT);
  pinMode(sG, OUTPUT); pinMode(sY, OUTPUT); pinMode(sR, OUTPUT);
  pinMode(eG, OUTPUT); pinMode(eY, OUTPUT); pinMode(eR, OUTPUT);
  pinMode(wG, OUTPUT); pinMode(wY, OUTPUT); pinMode(wR, OUTPUT);

  allRed();
}


// -------- All Red --------
void allRed(){
  digitalWrite(nR,HIGH); digitalWrite(nG,LOW); digitalWrite(nY,LOW);
  digitalWrite(sR,HIGH); digitalWrite(sG,LOW); digitalWrite(sY,LOW);
  digitalWrite(eR,HIGH); digitalWrite(eG,LOW); digitalWrite(eY,LOW);
  digitalWrite(wR,HIGH); digitalWrite(wG,LOW); digitalWrite(wY,LOW);
}

// -------- Check Priority --------
int checkPriority(){
  if(digitalRead(nS)==0) return 0;
  if(digitalRead(sS)==0) return 1;
  if(digitalRead(eS)==0) return 2;
  if(digitalRead(wS)==0) return 3;
  return -1;
}

// -------- Run Signal --------
void runSignal(int road, int duration){

  allRed();

  int G, Y, R;

  if(road==0){ G=nG; Y=nY; R=nR; }
  if(road==1){ G=sG; Y=sY; R=sR; }
  if(road==2){ G=eG; Y=eY; R=eR; }
  if(road==3){ G=wG; Y=wY; R=wR; }

  // -------- GREEN --------
  digitalWrite(R, LOW);
  digitalWrite(G, HIGH);

  unsigned long start = millis();

  while(millis() - start < duration){

    int p = checkPriority();

    if(p != -1 && p != road){
      Serial.println(">>> INTERRUPT (IMMEDIATE)");
      return;
    }

    delay(20);
  }

  // -------- NORMAL YELLOW --------
  digitalWrite(G, LOW);
  digitalWrite(Y, HIGH);
  Serial.println(">>> YELLOW");
  delay(2000);

  digitalWrite(Y, LOW);
  digitalWrite(R, HIGH);
}

// -------- Loop --------
void loop() {

  int p = checkPriority();

  // -------- SMART MODE --------
  if(p != -1){
    Serial.print("SMART MODE: ");
    Serial.println(p);

    runSignal(p, 5000);

    lastServed = p; 
    return;
  }

  
  Serial.println("NORMAL MODE");

  // North
  if(lastServed != 0){
    runSignal(0, 10000);
    if(checkPriority() != -1) return;
  }

  // East
  if(lastServed != 2){
    runSignal(2, 10000);
    if(checkPriority() != -1) return;
  }

  // South
  if(lastServed != 1){
    runSignal(1, 10000);
    if(checkPriority() != -1) return;
  }

  // West
  if(lastServed != 3){
    runSignal(3, 10000);
    if(checkPriority() != -1) return;
  }

  
  lastServed = -1;
}
