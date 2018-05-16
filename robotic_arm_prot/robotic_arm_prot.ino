
//*************************************************************//
/*                                                             /*
 * @author Taha Topal                                           *
 */                                                             */
 //************************************************************//
 
#include <ResponsiveAnalogRead.h> // potansiyometre değerlerini sabit alabilmek için uygulayacagımız filtrenin bulundugu kütüphane
#include<Servo.h>   //Servo objelerinin ve fonksiyonların kullanılması için gerekli kütüphane 
Servo servo_alt;    //alt servo objesi oluşturuldu
Servo servo_orta;   //orta servo objesi oluşturuldu
Servo servo_ust;    //üst servo objesi oluşturuldu
Servo servo_agiz;   //agiz servo objesi oluşturuldu
const int agiz_pot = A0; // ağiz servosuna gidecek verinin alınacağı potansiyometre analog 0 pinine atandı
const int alt_pot = A1; // alt servosuna gidecek verinin alınacagı potansiyometre analog 1 pinine atandı
const int orta_pot = A2; // orta servosuna gidecek verinin alınacagı potansiyometre analog 2 pinine atandı
const int ust_pot = A3; // üst servosuna gidecek verinin alınacagı potansiyometre analog 3 pinine atandı
ResponsiveAnalogRead analog_alt(alt_pot,true); // potansiyometreden gelen verileri filitrelemek için kullanılan kütüphanıenin objesi
ResponsiveAnalogRead analog_agiz(agiz_pot,true);
ResponsiveAnalogRead analog_ust(ust_pot,true);
ResponsiveAnalogRead analog_orta(orta_pot,true);
int p_agiz;                           //potansiyometre değerlerinin tutulması için gerekli değerler
int p_alt;
int p_ust;
int p_orta;
int mp_agiz, mp_alt, mp_ust, mp_orta; // map fonksiyonuyla belili değer aralıklarının tutulması için gerekli olan değerler tanımlandı
const int k_button =  10;// kayıt ve play buttonları tanımlandı
const int p_button = 11;
int rec[100][4]; //100 kayda kadar alabilir
int k_sayisi = 0;
void setup() {
   Serial.begin(9600);        //Serial haberleşmeyi 9600 baud rate den baslatır
  pinMode(agiz_pot, INPUT);   //pinmode fonksiyonlarıyla bağladıgımız pinlerden veri geleceğini arduinoya söylüyoruz
  pinMode(alt_pot, INPUT);
  pinMode(orta_pot, INPUT);
  pinMode(ust_pot, INPUT);
  pinMode(k_button, INPUT);
  pinMode(p_button, INPUT);
  servo_alt.attach(2);        //attach fonksiyonuyla Servolarımızın sinyal pinlerini belirliyoruz
  servo_ust.attach(4);
  servo_agiz.attach(5);
  servo_orta.attach(3);
  servo_alt.write(90);        //Servolarımızın sistemde ki default değerlerini başlangıç konumuna göre belirliyoruz
  servo_ust.write(0);
  servo_orta.write(180);
  servo_agiz.write(180);
 
  delay(1000);
}

void loop() {

  // put your main code here, to run repeatedly:
  analog_alt.update();//filitrelenen potansiyometre verilerini döngü başa geldikçe güncelle
  analog_ust.update();
  analog_orta.update();
  analog_agiz.update();
  p_agiz=analog_agiz.getValue();// filitrelenen potansiyometre değerleri çekildi
  p_orta=analog_orta.getValue();
  p_ust=analog_ust.getValue();
  p_alt=analog_alt.getValue();
  Serial.print("agiz : ");Serial.print(p_agiz);Serial.print("\t");
  Serial.print("orta : ");Serial.print(p_orta);Serial.print("\t");
  Serial.print("ust : ");Serial.print(p_ust);Serial.print("\t");
  Serial.print("alt : ");Serial.print(p_alt);Serial.print("\t");
  Serial.println();
  mp_agiz = map(p_agiz, 0, 1023, 180, 120); // potansiyometreden gelen 0-1023 arsı sinyal değerlerini 0 - 180 arası açı değerlerine sığdır
  mp_ust = map(p_ust, 0, 1023, 0, 85);
  mp_orta = map(p_orta, 0, 1023, 180, 45);
  mp_alt = map(p_alt, 0, 1023, 0, 180);
  servo_alt.write(mp_alt); //Servolara potansiyometreden gelen ve açı aralıgına sıgdırılan değerler gönderildi
  servo_ust.write(mp_ust);
  servo_orta.write(mp_orta);
  servo_agiz.write(mp_agiz);
  if (digitalRead(k_button)) { // eger kayıt buttonunaa basılırsa
    Serial.println("k_button basıldı ::");
    rec[k_sayisi][0] = mp_alt; //yaratılan arrayin içinde potansiyometreden gelen değerler kaydedildi
    rec[k_sayisi][1] = mp_orta;
    rec[k_sayisi][2] = mp_ust;
    rec[k_sayisi][3] = mp_agiz;
    k_sayisi = k_sayisi + 1;  //daha sonra play kısmında kullanılmak üzere kayıt sayısı bir değerin içinde saklandı
    delay(500);
  } else if (digitalRead(p_button)){ // eğer play buttonuna basılmışsa alınan kayıtları oynatmaya başla
    int q = 0;// kayıdı oynatmak için 0 dan başlayıp kayır sayisina kadar giden değer
    Serial.println("play button basıldı ::");
    while (1) {                  // sonsuza kadar çalıştır
      servo_alt.write(rec[q][0]); //kayıtları oynat
      delay(100);
      servo_ust.write(rec[q][2]);
      delay(100);
      servo_orta.write(rec[q][1]);
      delay(100);
      servo_agiz.write(rec[q][3]);
      delay(100);
      Serial.print(rec[q][0]);Serial.print("\t"); // Seri porttan onatılan değerlerin görülmesi için output
      Serial.print(rec[q][1]);Serial.print("\t");
      Serial.print(rec[q][2]);Serial.print("\t");
      Serial.print(rec[q][3]);Serial.print("\t");
      Serial.println();
      delay(400);               // servoların konumlara gelmeleri için 400 ms bekle
      if (digitalRead(k_button)) { // eger kayıt butonuna tekrar basılırsa oynatma kısmından çıkıp real time kısmına geç
        for(int i = 0; i<100;i++){// eğer tekrar kayıt almak istiyosa rec arrayinin içini 0 a eşitle
          for(int j=0;j<4;j++){
            rec[i][j]=0;
            }
          }
        break;
      }
      q++;
      if (k_sayisi == q) {      // eğer toplam kayıt sayısına gelmişse tekrar başa sarmak için q değerini sıfıra eşitle
        q = 0;
      }
    }

  }

}

