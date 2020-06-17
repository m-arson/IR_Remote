#define RECV_PIN  5
#define BTN00     0xD130B09F
#define BTN01     0x20DF08F7
#define BTN10     0x2ADE08A3
#define BTN11     0x20DF8877
#define BTN20     0x7F613A3B
#define BTN21     0x20DF48B7
#define BTN30     0x1A46B33F
#define BTN31     0x20DFC837
#define BTN40     0x9F3E1063
#define BTN41     0x20DF28D7
#define BTN50     0x2FB2625F
#define BTN51     0x20DFA857
#define BTN60     0x4CEB59FF
#define BTN61     0x20DF6897
#define BTN70     0xE7D0D303
#define BTN71     0x20DFE817
#define BTN80     0xC984587B
#define BTN81     0x20DF18E7
#define BTN90     0x2331B07F
#define BTN91     0x20DF9867
#define UP0       0x6F019A9F
#define UP1       0x20DFA25D
#define DOWN0     0x8C3A923F
#define DOWN1     0x20DF629D
#define LEFT0     0x7A71E203
#define LEFT1     0x20DFE21D
#define RIGHT0    0x6F43E8BB
#define RIGHT1    0x20DF12ED
#define VOLUP0    0x9E0A10FF
#define VOLUP1    0x20DF40BF
#define VOLDOWN0  0x8EEF4B83
#define VOLDOWN1  0x20DFC03F
#define CHUP0     0x86288A23
#define CHUP1     0x20DF00FF
#define CHDOWN0   0x169CDC1F
#define CHDOWN1   0x20DF807F
#define MENU0     0xACE7C23F
#define MENU1     0x20DFC23D
#define POWER0  0x180BD9FF
#define POWER1  0x20DF10EF

#include <IRremote.h>

unsigned long list_arrl;
unsigned long timer = 0;
unsigned long last_timer = 0;
unsigned long data = 0;
char last_data = 0;
byte data_size = 0;
char char_data = 0;
bool mode = 0;
byte active_mode = 0;
char passwd[5] = "1234";
char temp_passwd[5];
bool enter_passmode = 0;
int count = 0;
bool led1_cond = 0, led2_cond = 0;
IRrecv rec(RECV_PIN);
decode_results r;
void indicator(byte timez, int looong = 50) {
  for (byte i = 0; i < timez; i++) {
    if (i == timez - 1) {
      digitalWrite(2, HIGH);
      delay(looong);
      digitalWrite(2, LOW);
      return;
    }
    digitalWrite(2, HIGH);
    delay(looong);
    digitalWrite(2, LOW);
    delay(looong);
  }
}
char recv_f(unsigned long din) {
  char recv_data = 0;
  switch (din) {
    case BTN00 : case BTN01 :
      recv_data = '0';
      break;
    case BTN10 : case BTN11 :
      recv_data = '1';
      break;
    case BTN20 : case BTN21 :
      recv_data = '2';
      break;
    case BTN30 : case BTN31 :
      recv_data = '3';
      break;
    case BTN40 : case BTN41 :
      recv_data = '4';
      break;
    case BTN50 : case BTN51 :
      recv_data = '5';
      break;
    case BTN60 : case BTN61 :
      recv_data = '6';
      break;
    case BTN70 : case BTN71 :
      recv_data = '7';
      break;
    case BTN80 : case BTN81 :
      recv_data = '8';
      break;
    case BTN90 : case BTN91 :
      recv_data = '9';
      break;
    case UP0 : case UP1 :
      recv_data = 'u';
      break;
    case DOWN0 : case DOWN1 :
      recv_data = 'd';
      break;
    case LEFT0 : case LEFT1 :
      recv_data = 'l';
      break;
    case RIGHT0 : case RIGHT1 :
      recv_data = 'r';
      break;
    case VOLUP0 : case VOLUP1 :
      recv_data = 'U';
      break;
    case VOLDOWN0 : case VOLDOWN1 :
      recv_data = 'D';
      break;
    case CHUP0 : case CHUP1 :
      recv_data = 'L';
      break;
    case CHDOWN0 : case CHDOWN1 :
      recv_data = 'R';
      break;
    case MENU0 : case MENU1 :
      recv_data = 'M';
      break;
    case POWER0 : case POWER1 :
      recv_data = 'P';
      break;
  }
  return recv_data;
}
void mainloop() {
  if (rec.decode(&r) && mode == 0) {
    last_timer = millis();
    data = r.value;
    rec.resume();
    data_size = ((String) data).length();
    mode = 1;
  }
  if ((unsigned long) millis() - last_timer > 150) {
    if (data == 0xFFFFFFFF && mode == 1) {
      mode = 0;
      return;
    }
    else {
      if (data != 0x0 && mode == 1) {
        if (data_size > 7) {
          char_data = recv_f(data);
          if (char_data == 'P') {
            indicator(1, 500);
            active_mode = 0;
            char_data = 0;
            last_data = 0;
            mode = 0;
            return;
          }
          else if (char_data == '1') {
            led1_cond = !led1_cond;
            digitalWrite(3, led1_cond);
          }
          else if (char_data == '2') {
            led2_cond = !led2_cond;
            digitalWrite(4, led2_cond);
          }
          else {
            last_data = char_data;
            indicator(1);
          }
        }
        else {
          indicator(1);
        }
      }
      mode = 0;
    }
  }
}
void passenter_mode() {
  if (rec.decode(&r) && mode == 0) {
    last_timer = millis();
    data = r.value;
    rec.resume();
    data_size = ((String) data).length();
    mode = 1;
  }
  if ((unsigned long) millis() - last_timer > 150) {
    if (data == 0xFFFFFFFF && mode == 1) {
      mode = 0;
      return;
    }
    else {
      if (data != 0x0 && mode == 1) {
        if (data_size > 7) {
          char_data = recv_f(data);
          last_data = char_data;
          indicator(1);
        }
        else {
          indicator(1);
        }
          temp_passwd[count] = last_data;
        if (count == 3) {
          if (!strcmp(passwd, temp_passwd)) {
            indicator(2, 100);
            strcpy(temp_passwd, "0000");
            count = 0;
            char_data = 0;
            last_data = 0;
            mode = 0;
            active_mode = 2;
            return;
          }
          else {
            indicator(3);
            strcpy(temp_passwd, "0000");
            count = -1;
          }
        }
        count++;
      }
      mode = 0;
    }
  }
}
void enter_mode() {
  if (rec.decode(&r) && mode == 0) {
    last_timer = millis();
    data = r.value;
    rec.resume();
    data_size = ((String) data).length();
    mode = 1;
  }
  if ((unsigned long) millis() - last_timer > 150) {
    if (data == 0xFFFFFFFF && mode == 1) {
      mode = 0;
      return;
    }
    else {
      if (data != 0x0 && mode == 1) {
        if (data_size > 7) {
          char_data = recv_f(data);
          if (char_data == 'P') {
            indicator(1, 500);
            last_data = 0;
            char_data = 0;
            mode = 0;
            active_mode = 1;
            return;
          }
          else {
            indicator(4);
          }
        }
      }
      mode = 0;
    }
  }
}
void setup() { 
  Serial.begin(115200);
  rec.enableIRIn();
  pinMode(2, OUTPUT);
}
void loop() {
  if (active_mode == 0) enter_mode();
  else if (active_mode == 1) passenter_mode();
  else if (active_mode == 2) mainloop();
}
