#include "LedControl.h"

// 4개 차레대로 lc 배열
LedControl lc[4]=[LedControl(2, 4, 3, 1),LedControl(5, 7, 6, 1),LedControl(8, 10, 9, 1),LedControl(11, 13, 12, 1)];


// 전역변수
const int analog_cnt=8;
const int ldrPin[analog_count] = [A0,A1,A2,A3,A4,A5,A6,A7];    
int init_value[analog_count]=[]; //초기 조도 센서 측정(한번 실행)
int ldrValue[analog_count]=[]; //조도센서 아웃풋 (실행 중)

const int ANIMATION_SPEED = 500; //애니메이션 실행 속도


// 기본 구 배열 
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


// 이미지의 총 개수를 자동으로 계산합니다.
const int IMAGES_LEN = sizeof(IMAGES) / sizeof(IMAGES[0]);


void setup() {
  Serial.begin(9600);
  // 모든 매트릭스를 초기화합니다.
  for(int i=0;i<4;i++){
    lc[i].shutdown(0,false);
    lc[i].setIntensity(0, 8);
    lc[i].clearDisplay(0);
  }


  for(int i=0;i<analog_cnt;i++){
    init_value.append(analogRead(ldrPin[i]));
  }
 
}


void loop() {
  
  for (int i = 0; i < 4; i++) {
    drawImage(i,IMAGES[i]); //그리기
  }

  int dif[analog_cnt]=[];
  for(int i=0;i<analog_cnt;i++){
      ldrValue[i] = analogRead(ldrPin[i]);  //  각 핀의 아날로그 값 읽기
      Serial.print("Sensor Value : ");
      Serial.println(ldrValue);
      dif[i]=(init_value[i]-ldrValue[i]); // 변화값 저장
  }
  
  for(int i=0;i<analog_cnt;i++){
    //일정 수준 이상 변하면 그리기 <- 알고리즘 설계 !! 제일 중요
  }
  }

  delay(500);
};

/**
 * @brief 하나의 이미지 데이터를 4개의 모든 LED 매트릭스에 출력하는 함수
 * @param image 8x8 이미지 데이터 (8바이트 배열)
 */
void drawImage(int i,const uint8_t image[8]) {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      // 이미지 데이터의 각 비트를 읽어 LED의 ON/OFF 상태를 결정합니다.
      // (7-col)을 사용하여 배열에 그려진 모양 그대로 출력되도록 합니다.
      bool pixelOn = bitRead(image[row], 7 - col);
      lc[i].setLed(0,row,col,pixelOn);
      
      }
  }

}