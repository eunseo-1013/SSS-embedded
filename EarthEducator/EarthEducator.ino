#include "LedControl.h"

LedControl lc[4] = {
    LedControl(2, 4, 3, 1),
    LedControl(5, 7, 6, 1),
    LedControl(11, 13, 12, 1),
    LedControl(8, 13, 9, 1)
};


//전역 변수
const int analog_cnt = 8; // 조도 센서 개수
const int change = 50;  // 변화량 1
const int change_div = 2; //변화량 배수 기준
const int ldrPin[analog_cnt] = {A0, A1, A2, A3, A4, A5, A6, A7};
int init_value[analog_cnt];
int ldrValue[analog_cnt];
const int SPEED = 500; // 디스플레이 delay 간격


const int buttonPin = 10; 
bool lastButtonState = HIGH;

const uint8_t IMAGES[8] = { // 초기 원
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000011,
  0b00000111,
  0b00001111,
  0b00011111,
  0b00011111
};

const uint8_t IMAGES_up_2[8] = { // chnage * change_div 이상 변했을 때
  0b00000001,
  0b00000001,
  0b00000011,
  0b00000111,
  0b00001111,
  0b00001111,
  0b00011111,
  0b00011111
};

const uint8_t IMAGES_up_1[8] = { // change 양 이상 변했을 때 (짝수)
  0b00000000,
  0b00000001,
  0b00000011,
  0b00000111,
  0b00001111,
  0b00001111,
  0b00011111,
  0b00011111
};

const uint8_t IMAGES_side_1[8] = { // change 양 이상 변했을 때 (홀수)
  0b00000000,
  0b00000000,
  0b00010100,
  0b00001111,
  0b00011111,
  0b00001111,
  0b00011111,
  0b00011111
};

const uint8_t IMAGES_side_2[8] = {// chnage * change_div 이상 변했을 때(홀수)
  0b00000000,
  0b00100000,
  0b00010100,
  0b00111111,
  0b00011111,
  0b00001111,
  0b00011111,
  0b00011111
};


void drawImage(int i, const uint8_t image[8]) {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      bool pixelOn = bitRead(image[row], 7 - col);
      lc[i].setLed(0, row, col, pixelOn);
    }
  }
}

void drawImage_R(int i, const uint8_t image[8]) {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      bool pixelOn = bitRead(image[row], col);
      lc[i].setLed(0, 7 - row, 7 - col, pixelOn);
    }
  }
}

void drawImage_T(int i, const uint8_t image[8]) {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      bool pixelOn = bitRead(image[row], 7 - col);
      lc[i].setLed(0, col, row, pixelOn);
    }
  }
}

void drawImage_RT(int i, const uint8_t image[8]) {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      bool pixelOn = bitRead(image[row], col);
      lc[i].setLed(0, col, row, pixelOn);
    }
  }
}

int find_change_index(int dif[]) { // 최대 변화량을 가진 조도 센서 찾기
  int max = 0;
  int max_index = 0;
  for (int i = 0; i < analog_cnt; i++) {
    if (dif[i] > max) {
      max = dif[i];
      max_index = i;
    }
  }
  return max_index;
}

int find_change_value(int dif[]) { // 최대 변화량 / change <- 변화 기준에서 몇배 인가? 
  int change_value = dif[find_change_index(dif)] / change;
  return change_value;
}

//  버튼 눌러 센서 초기화하는 함수
void recalibrateSensors() {
  for (int blink = 0; blink < 3; blink++) {
     for (int i = 0; i < 4; i++) {
      lc[i].shutdown(0, false);
      lc[i].setIntensity(0, 5);
      lc[i].clearDisplay(0);
    }
    delay(200);

    drawImage(0, IMAGES);
    drawImage(2, IMAGES);
    drawImage_R(1, IMAGES);
    drawImage_R(3, IMAGES);
    delay(200);
  }

  for (int i = 0; i < analog_cnt; i++) {
    init_value[i] = analogRead(ldrPin[i]);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);

  for (int i = 0; i < 4; i++) {
    lc[i].shutdown(0, false);
    lc[i].setIntensity(0, 5);
    lc[i].clearDisplay(0);
  }

  for (int i = 0; i < analog_cnt; i++) {
    init_value[i] = analogRead(ldrPin[i]);
  }
}

void loop() {
  // 버튼 눌림 감지
  bool buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    recalibrateSensors();
    delay(500); // 중복 입력 방지
  }
  lastButtonState = buttonState;

  // 기본 이미지
  drawImage(0, IMAGES);
  drawImage(2, IMAGES);
  drawImage_R(1, IMAGES);
  drawImage_R(3, IMAGES);

  int dif[analog_cnt];
  for (int i = 0; i < analog_cnt; i++) {
    ldrValue[i] = analogRead(ldrPin[i]);
    dif[i] = -init_value[i] + ldrValue[i];
    Serial.print("Difference: ");
    Serial.println(dif[i]);
  }

  int index = find_change_index(dif); 
  int value = find_change_value(dif);

  if (value >= change_div) { 
    switch (index) { // 조도 센서 번호에 맞는 디스플레이 출력 
      case 0:
        drawImage_R(3, IMAGES_up_2);
        drawImage(0, IMAGES_up_2);
        break;
      case 1:
        drawImage(0, IMAGES_side_2);
        break;
      case 2:
        drawImage_RT(1, IMAGES_up_2);
        drawImage_T(0, IMAGES_up_2);
        break;
      case 3:
        drawImage_R(1, IMAGES_side_2);
        break;
      case 4:
        drawImage_R(1, IMAGES_up_2);
        drawImage(2, IMAGES_up_2);
        break;
      case 5:
        drawImage(2, IMAGES_side_2);
        break;
      case 6:
        drawImage_RT(3, IMAGES_up_2);
        drawImage_T(2, IMAGES_up_2);
        break;
      case 7:
        drawImage_R(3, IMAGES_side_2);
        break;
    }
  } else if (value >= change_div - 1) {
    switch (index) {
      case 0:
        drawImage_R(3, IMAGES_up_1);
        drawImage(0, IMAGES_up_1);
        break;
      case 1:
        drawImage(0, IMAGES_side_1);
        break;
      case 2:
        drawImage_RT(1, IMAGES_up_1);
        drawImage_T(0, IMAGES_up_1);
        break;
      case 3:
        drawImage_R(1, IMAGES_side_1);
        break;
      case 4:
        drawImage_R(1, IMAGES_up_1);
        drawImage(2, IMAGES_up_1);
        break;
      case 5:
        drawImage(2, IMAGES_side_1);
        break;
      case 6:
        drawImage_RT(3, IMAGES_up_1);
        drawImage_T(2, IMAGES_up_1);
        break;
      case 7:
        drawImage_R(3, IMAGES_side_1);
        break;
    }
  } else {
    drawImage(0, IMAGES);
    drawImage(2, IMAGES);
    drawImage_R(1, IMAGES);
    drawImage_R(3, IMAGES);
  }

  delay(SPEED);
}
