#include "ESP8266.h"
#include "SoftwareSerial.h"
#include <U8glib.h>
//配置ESP8266WIFI设置
#define SSID "HUAWEI GE"    //填写2.4GHz的WIFI名称，不要使用校园网
#define PASSWORD "gebaojie200428"//填写自己的WIFI密码
#define HOST_NAME "api.heclouds.com"  //API主机名称，连接到OneNET平台，无需修改
#define DEVICE_ID "1099831083"       //填写自己的OneNet设备ID
#define HOST_PORT (80)                //API端口，连接到OneNET平台，无需修改
String APIKey = "Ngie8by96a7PZhV6M8xbF3QK2zE="; //与设备绑定的APIKey
#define INTERVAL_SENSOR 10//定义传感器采样及发送时间间隔
#define LED 10
#define BUTTON 7
#define INTERVAL_LCD 20
unsigned long Icd_time = millis();
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
#define setFont_L u8g.setFont (u8g_font_7x13)
#define setFont_M u8g.setFont (u8g_font_fixed_v0r)
#define setFont_S u8g.setFont (u8g_font_fixed_v0r)
#define setFont_SS u8g.setFont (u8g_font_fub25n)
SoftwareSerial mySerial(3, 2);
ESP8266 wifi(mySerial);
int val = 0;
int old_val = 0;
int state = 0;
char t[9]={'0','0',':','0','0',':','0','0','\0'};
int h=19,m=38,s=10;//输入当前时间
void setup()
{
  pinMode(8, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);

  mySerial.begin(115200); //初始化软串口
  Serial.begin(9600);     //初始化串口
  Serial.print("setup begin\r\n");
  //以下为ESP8266初始化的代码
  Serial.print("FW Version: ");
  Serial.println(wifi.getVersion().c_str());
  if (wifi.setOprToStation()) {
    Serial.print("to station ok\r\n");
  } else {
    Serial.print("to station err\r\n");
  }
  //ESP8266接入WIFI
  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }
  mySerial.println("AT+UART_CUR=9600,8,1,0,0");
  mySerial.begin(9600);
  Serial.println("setup end\r\n");
}
unsigned long net_time1 = millis(); //数据上传服务器时间
void loop(){
  int n=analogRead(A3);
  val = digitalRead(BUTTON);
    if (val == HIGH && old_val == LOW) { 
    	state = 1 - state;
    	delay(10); // 加入延迟以克服杂波
  	}
  	old_val = val;
    if (state == 1) {
    	analogWrite(LED,n/4);
  	} else {
   		analogWrite(LED,0);
  	}
  if (net_time1 > millis())
    net_time1 = millis();
  if (millis() - net_time1 > INTERVAL_SENSOR) //发送数据时间间隔
  {
    int n=analogRead(A3);
    Serial.print("n : ");
    Serial.println(n);
    Serial.print("state : ");
    Serial.println(state);
    Serial.println("");
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
      Serial.print("create tcp ok\r\n");
      char buf[10];
      //拼接发送data字段字符串
      String jsonToSend = "{\"n\":";
      dtostrf(n, 1, 2, buf);
      jsonToSend += "\"" + String(buf) + "\"";
      jsonToSend += ",\"state\":";
      dtostrf(state, 1, 2, buf);
      jsonToSend += "\"" + String(buf) + "\"";
      jsonToSend += "}";
      //拼接POST请求字符串
      String postString = "POST /devices/";
      postString += DEVICE_ID;
      postString += "/datapoints?type=3 HTTP/1.1";
      postString += "\r\n";
      postString += "api-key:";
      postString += APIKey;
      postString += "\r\n";
      postString += "Host:api.heclouds.com\r\n";
      postString += "Connection:close\r\n";
      postString += "Content-Length:";
      postString += jsonToSend.length();
      postString += "\r\n";
      postString += "\r\n";
      postString += jsonToSend;
      postString += "\r\n";
      postString += "\r\n";
      postString += "\r\n";
      const char *postArray = postString.c_str(); //将str转化为char数组
      Serial.println(postArray);
      wifi.send((const uint8_t *)postArray, strlen(postArray)); //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
      Serial.println("send success");      
      if (wifi.releaseTCP()) { //释放TCP连接
        Serial.print("release tcp ok\r\n");
      } else {
        Serial.print("release tcp err\r\n");
      }
      postArray = NULL; //清空数组，等待下次传输数据
    } else {
      Serial.print("create tcp err\r\n");
    }
    Serial.println("");
    net_time1 = millis();
    s++;
  if(s==60){
    s=0;m++;
    if(m==60){
      m=0;h++;
      if(h==24){
        h=0;
      }
    }
  }
  t[0]=h/10+'0';t[1]=h%10+'0';
  t[3]=m/10+'0';t[4]=m%10+'0';
  t[6]=s/10+'0';t[7]=s%10+'0';
  u8g.firstPage();
    do{
    setFont_L;
      u8g.setPrintPos(0,15);
      u8g.print("Time:");
      u8g.setPrintPos(0,25);
      u8g.print(t);
    } while (u8g.nextPage());
  }
  
}

