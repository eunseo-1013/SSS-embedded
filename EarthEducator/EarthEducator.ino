#include "LedControl.h"


LedControl lc[4] = {
    LedControl(2, 4, 3, 1),
    LedControl(5, 7, 6, 1),
    LedControl(11, 13, 12, 1),
    LedControl(8, 10, 9, 1)
};


const int analog_cnt = 8;


const int change=50; // 조도 변화량 기준
const int change_div=2; // 조도 변화량 나눗셈 2 == 100 이상일때
const int ldrPin[analog_cnt] = {A0, A1, A2, A3, A4, A5, A6, A7}; 
int init_value[analog_cnt]; // 조도 센서 값(초기)
int ldrValue[analog_cnt];   // 조도 센서 값 (현재)

const int ANIMATION_SPEED = 500; // 애니메이션 실행 속도 


const uint8_t IMAGES[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000011,
  0b00000111,
  0b00001111,
  0b00011111,
  0b00011111
};


const uint8_t IMAGES_up_2[8] = {
  0b00000001,
  0b00000001,
  0b00000011,
  0b00000111,
  0b00001111,
  0b00001111,
  0b00011111,
  0b00011111
};


const uint8_t IMAGES_up_1[8] = {
 0b00000000,
  0b00000001,
  0b00000011,
  0b00000111,
  0b00001111,
  0b00001111,
  0b00011111,
  0b00011111
};


const uint8_t IMAGES_side_1[8] = {
  0b00000000,
  0b00000000,
  0b00010100,
  0b00001111,
  0b00011111,
  0b00001111,
  0b00011111,
  0b00011111
};


const uint8_t IMAGES_side_2[8] = {
  0b00000000,
  0b00100000,
  0b00010100,
  0b00111111,
  0b00011111,
  0b00001111,
  0b00011111,
  0b00011111
};

void drawImage_0(int i, const uint8_t image[8]) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            bool pixelOn = bitRead(image[row], 7 - col);
          
            lc[i].setLed(0, row, col, pixelOn);
        }
    }
}

void drawImage_180(int i, const uint8_t image[8]) {  
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            bool pixelOn = bitRead(image[row], col);
            lc[i].setLed(0, 7-row, 7-col, pixelOn);
        }
    }
}


void drawImage_270_0(int i, const uint8_t image[8]) {  
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            // 7 - col: 이미지 배열을 비트의 오른쪽부터 읽어 매트릭스에 맞게 출력
            bool pixelOn = bitRead(image[row], 7-col);
            // setLed(address, row, col, state) - address는 0
            lc[i].setLed(0, col, row, pixelOn);
        }
    }
}

void drawImage_270_180(int i, const uint8_t image[8]) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            bool pixelOn = bitRead(image[row], col);
            lc[i].setLed(0, col, row, pixelOn);
        }
    }
}



int find_change_index(int dif[]){
    int max = 0;
    int max_index = 0; // 최대 변화 값의 인덱스

    for(int i = 0; i < analog_cnt; i++){
        if(dif[i] > max){
            max = dif[i];
            max_index = i; 
        }
    }
    return max_index; 
}


int find_change_value(int dif[]){
  int change_value = dif[find_change_index(dif)]/change;
  return change_value;
}



void setup() {
    Serial.begin(9600);
    
    // 모든 매트릭스를 초기화
    for(int i = 0; i < 4; i++){
        lc[i].shutdown(0, false); 
        lc[i].setIntensity(0, 5);  // 밝기 설정 (0-15)
        lc[i].clearDisplay(0);     
    }

    // 초기 조도 값 측정 
    for(int i = 0; i < analog_cnt; i++){
        init_value[i] = analogRead(ldrPin[i]);
    }
    
    // 초기 측정값 시리얼 출력
    /*
    Serial.println("Initial LDR Values:");
    for(int i = 0; i < analog_cnt; i++){
        Serial.print("A");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(init_value[i]);
    }*/
    
}


void loop() {
    
    // 4개의 매트릭스에 그리기

    drawImage_0(0, IMAGES); 
    drawImage_0(2, IMAGES); 
    drawImage_180(1, IMAGES); 
    drawImage_180(3, IMAGES); 
  

    int dif[analog_cnt]; // 변화값을 저장할 배열
    
    for(int i = 0; i < analog_cnt; i++){
        ldrValue[i] = analogRead(ldrPin[i]);    // 현재 아날로그 값 읽기
        
       /* Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(" Current Value : ");
        Serial.println(ldrValue[i]); // 배열 요소 [i] 출력*/
      
        // 변화값 저장 (현재값 - 초기값) : 밝아질수록 커짐 <- 양수일때 변화해야함 (기준 양수 값 = change)
        dif[i] = - init_value[i] + ldrValue[i]; 
        dif[i]=(dif[i]);
        Serial.print("Difference : ");
        Serial.println(dif[i]);
    }
  


  int index=find_change_index(dif);
  int value = find_change_value(dif);

  if(value>=change_div){
    switch (index){
    case 0:
        drawImage_180(3,IMAGES_up_2);
        drawImage_0(0,IMAGES_up_2);
      break;
    case 1:
      
        drawImage_0(0,IMAGES_side_2);
    break;
    case 2:
        drawImage_270_180(1,IMAGES_up_2);
        drawImage_270_0(0,IMAGES_up_2);
      break;
    case 3:
      
        drawImage_180(1,IMAGES_side_2);
    break;

    case 4:
        drawImage_180(1,IMAGES_up_2);
        drawImage_0(2,IMAGES_up_2);
      break;
    case 5:
       
        drawImage_0(2,IMAGES_side_2);
      break;
    case 6:
        drawImage_270_180(3,IMAGES_up_2);
        drawImage_270_0(2,IMAGES_up_2);
      break;
    case 7:
       
        drawImage_180(3,IMAGES_side_2);
    break;

  }
  }
  else if(value >= change_div-1){
    switch (index){
    case 0:
        drawImage_180(3,IMAGES_up_1);
        drawImage_0(0,IMAGES_up_1);
      
      break;
    case 1:

        drawImage_0(0,IMAGES_side_1);
      
    break;
    case 2:
        drawImage_270_180(1,IMAGES_up_1);
        drawImage_270_0(0,IMAGES_up_1);
      
      break;
    case 3:
  
        drawImage_180(1,IMAGES_side_1);
      
    break;

    case 4:
  
 
        drawImage_180(1,IMAGES_up_1);
        drawImage_0(2,IMAGES_up_1);
      
      break;
    case 5:
  
        drawImage_0(2,IMAGES_side_1);
      
      break;
    case 6:
      
        drawImage_270_180(3,IMAGES_up_1);
        drawImage_270_0(2,IMAGES_up_1);
      
      break;
    case 7:
      
        drawImage_180(3,IMAGES_side_1);
    break;

  }

  }
  else{
     drawImage_0(0, IMAGES); 
    drawImage_0(2, IMAGES); 
    drawImage_180(1, IMAGES); 
    drawImage_180(3, IMAGES); 
  }

    delay(ANIMATION_SPEED); // 애니메이션 속도만큼 딜레이
}