#include "LedControl.h"

// 사용자 요청에 따라 핀 설정은 고정됩니다.
// LedControl(DataIn, Clock, CS, numDevices)
LedControl lc1 = LedControl(2, 4, 3, 1);
LedControl lc2 = LedControl(5, 7, 6, 1);
LedControl lc3 = LedControl(8, 10, 9, 1);
LedControl lc4 = LedControl(11, 13, 12, 1);
const int ldrPin = A0;    // LDR 연결 핀     // LED 제어 핀
int ldrValue = 0; 



// 애니메이션에 사용할 이미지 데이터 배열
const uint8_t IMAGES[][8] = {
  {
   0b00000000,
  0b00000000,
  0b00000000,
  0b00000011,
  0b00000111,
  0b00001111,
  0b00011111,
  0b00011111
},{
  0b00011111,
  0b00011111,
  0b00001111,
  0b00000111,
  0b00000011,
  0b00000000,
  0b00000000,
  0b00000000
},{
  0b00011111,
  0b00011111,
  0b00001111,
  0b00000111,
  0b00000011,
  0b00000000,
  0b00000000,
  0b00000000
},{
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000011,
  0b00000111,
  0b00001111,
  0b00011111,
  0b00011111
}
};
const uint8_t IMAGES2[][8] = {
  {
  0b00000000,
  0b01000000,
  0b01110000,
  0b01111111,
  0b00111111,
  0b00111111,
  0b00011111,
  0b00011111
},{
  0b00011111,
  0b00011111,
  0b00001111,
  0b00000111,
  0b00000011,
  0b00000000,
  0b00000000,
  0b00000000
},{
  0b00011111,
  0b00011111,
  0b00001111,
  0b00000111,
  0b00000011,
  0b00000000,
  0b00000000,
  0b00000000
},{
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000011,
  0b00000111,
  0b00001111,
  0b00011111,
  0b00011111
}
};

// 이미지의 총 개수를 자동으로 계산합니다.
const int IMAGES_LEN = sizeof(IMAGES) / sizeof(IMAGES[0]);

// 애니메이션 속도 (밀리초 단위, 1000 = 1초)
const int ANIMATION_SPEED = 500;

void setup() {
  Serial.begin(9600);
  // 모든 매트릭스를 초기화합니다.
  lc1.shutdown(0, false);
  lc2.shutdown(0, false);
  lc3.shutdown(0, false);
  lc4.shutdown(0, false);

  // 모든 매트릭스의 밝기를 중간값(8)으로 설정합니다. (0~15)
  lc1.setIntensity(0, 8);
  lc2.setIntensity(0, 8);
  lc3.setIntensity(0, 8);
  lc4.setIntensity(0, 8);

  // 모든 매트릭스의 화면을 지웁니다.
  lc1.clearDisplay(0);
  lc2.clearDisplay(0);
  lc3.clearDisplay(0);
  lc4.clearDisplay(0);
}

void loop() {
  // 0번 이미지부터 마지막 이미지까지 순서대로 반복합니다.
  for (int i = 0; i < 4; i++) {
    // 현재 순서(i)의 이미지를 모든 매트릭스에 그립니다.
    drawImage(i,IMAGES[i]);
    // 설정된 속도만큼 잠시 기다립니다.
  }

  ldrValue = analogRead(ldrPin);  // 아날로그 값 읽기
  Serial.print("Sensor Value : ");
  Serial.println(ldrValue);
  if(ldrValue<300){
     for (int i = 0; i < 4; i++) {
    // 현재 순서(i)의 이미지를 모든 매트릭스에 그립니다.
    drawImage(i,IMAGES2[i]);
    // 설정된 속도만큼 잠시 기다립니다.
  }
  }


  delay(500);
};

/**
 * @brief 하나의 이미지 데이터를 4개의 모든 LED 매트릭스에 출력하는 함수
 * @param image 8x8 이미지 데이터 (8바이트 배열)
 */
void drawImage(int i,const uint8_t image[8]) {
  if(i==0){
     for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      // 이미지 데이터의 각 비트를 읽어 LED의 ON/OFF 상태를 결정합니다.
      // (7-col)을 사용하여 배열에 그려진 모양 그대로 출력되도록 합니다.
      bool pixelOn = bitRead(image[row], 7 - col);

      // 4개의 매트릭스에 동일한 픽셀을 설정합니다.
      lc1.setLed(0, row, col, pixelOn);
      
    }

  }
 
  }
  if(i==1){
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      // 이미지 데이터의 각 비트를 읽어 LED의 ON/OFF 상태를 결정합니다.
      // (7-col)을 사용하여 배열에 그려진 모양 그대로 출력되도록 합니다.
      bool pixelOn = bitRead(image[row], 7 - col);

      // 4개의 매트릭스에 동일한 픽셀을 설정합니다.
     
      lc2.setLed(0, row, col, pixelOn);
     
    }
  }
  }
if(i==2){
for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      // 이미지 데이터의 각 비트를 읽어 LED의 ON/OFF 상태를 결정합니다.
      // (7-col)을 사용하여 배열에 그려진 모양 그대로 출력되도록 합니다.
      bool pixelOn = bitRead(image[row], 7 - col);

      // 4개의 매트릭스에 동일한 픽셀을 설정합니다.
      
      lc3.setLed(0, row, col, pixelOn);
      
    }
  }
}
  
  if(i==3){
 for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      // 이미지 데이터의 각 비트를 읽어 LED의 ON/OFF 상태를 결정합니다.
      // (7-col)을 사용하여 배열에 그려진 모양 그대로 출력되도록 합니다.
      bool pixelOn = bitRead(image[row], 7 - col);

      // 4개의 매트릭스에 동일한 픽셀을 설정합니다.
   
      lc4.setLed(0, row, col, pixelOn);
    }
  }
  }
 
}