#include <DHT.h>
#include <HTTP_Method.h>
#include <Uri.h>
#include <WiFi.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <HTTPClient.h>
#include <ThingsBoard.h>
#include <esp_wifi.h>
#include <WebServer.h>
#include <Preferences.h>

const char* ssid = "VISITANTES";
const char* password = "";

//Inicia o Objeto preferences.
Preferences preferences; 
WebServer server(80);

//Aqui estão as informações de conexão do ESP
const char *apSSID = "ESP32_Config"; //Nome da Rede do ESP.
const char *apPassword = "12345678"; //Senha da Rede do ESP.

//Variavel que determina o tempo entre as amostras da Estação.
int delaytime = 1*289*1000000;  //Invervalo entre as amostras
//Definido em 49 Segundos por conta do processo de coleta dos dados demorar 11 Segundos.


//Dados de Conexão com o Servidor.
const char* thingsboardServer = "https://tb.geati.camboriu.ifc.edu.br/";
const char* accessToken = "f5a3iEEZ2iPhdIoVewTk";
const char* deviceId = "f0b973e0-5642-11ee-bdd4-b1244f07481c";

//Aqui se define as entradas que receberão os Reed Switch.
// O Valor numerico se refere ao numero da porta, e o "PIN" é para armazenar a variavel que esta recebendo o sinal.
const int PIN1 = 2;  // Reed Switch Pin
const int PIN2 = 4;  // Reed Switch Pin
const int PIN3 = 5;  // Reed Switch Pin
const int PIN4 = 18;  // Reed Switch Pin
const int PIN5 = 19;  // Reed Switch Pin
const int PIN6 = 21;  // Reed Switch Pin
const int PIN7 = 22;  // Reed Switch Pin
const int PIN8 = 34;  // Reed Switch Pin

const int PLUVIPIN = 23;  // Reed Switch Pin

const int ANEMPIN = 32;  // Reed Switch Pin

//Pin do DHT11
#define DHTPIN 25
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
float h;
float t;
float hic = dht.computeHeatIndex(t, h, false);


//Variaveis dos Sensores
String direcao = "erro"; // Variável para armazenar a direção

//Variaveis do Sensor Pluviometro.
volatile int ciclo = 0;
volatile float mm = 0;
volatile unsigned long lastPluviTime = 0;
const unsigned long debouncePluviDelay = 100; //Segundo o Paulo, essa Variavel serve para desativar o Pluviometor por 100 milisegundos para evitar que quando a bascula caia, não tenha um efeito rebote que reacione o sensor.


//Variaveis do Sensor de Velocidade do Vento.
const float pi = 3.14159265;     //Número de pi
int period = 5000;               //Tempo de medida(miliseconds)
int radius = 65;                //Raio do anemometro(mm)
unsigned int counter = 0;        //Contador para o sensor
unsigned int RPM = 0;            //Rotações por minuto
float speedwind = 0;             //Velocidade do vento (m/s)
float windspeed = 0;             //Velocidade do vento (km/h)

//Função que gera a pagina HTML de configuração da Rede.
void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Configurar Rede WiFi</h1>";
  html += "<form action='/save' method='POST'>";
  html += "SSID: <input type='text' name='ssid'><br>";
  html += "Senha: <input type='text' name='password'><br>";
  html += "<input type='submit' value='Salvar'>";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

// Função para lidar com o salvamento das configurações
void handleSave() {
  String ssid = server.arg("ssid");
  String password = server.arg("password");

//  Caso não tenha nenhuma rede salva, ou a rede salva não possua nenhum caracter, será iniciado o servidor.
  if (ssid.length() > 0 ) {
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);

    String response = "<html><body>";
    response += "<h1>Configurações Salvas</h1>";
    response += "<p>O dispositivo irá reiniciar e tentar conectar-se à rede configurada.</p>";
    response += "</body></html>";

    server.send(200, "text/html", response);
    delay(2000);
    ESP.restart();
  } else {
    server.send(400, "text/html", "<h1>Falha ao salvar as configurações. Por favor, tente novamente.</h1>");
  }
}

void setup() {
  Serial.begin(9600);   // Init serial 9600
  // Iniciar o NVS
  preferences.begin("wifi-config", false);

  esp_sleep_enable_timer_wakeup(delaytime);
  setCpuFrequencyMhz(80);

  // Verificar se há configurações salvas
  String ssid = preferences.getString("ssid", "");
  String password = preferences.getString("password", "");

  //  Caso o nome da rede salva seja maior que 0, Ele tenta se conectar a rede.
  if (ssid.length() > 0 ) {
    // Tentar conectar à rede configurada
    WiFi.begin(ssid.c_str(), password.c_str());

    Serial.print("Conectando a ");
    Serial.println(ssid);

    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED && retryCount < 20) {
      delay(500);
      Serial.print(".");
      retryCount++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Conectado!");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      return;
    } else {
      Serial.println("\nFalha ao conectar. Iniciando modo AP.");
    }
  }

  // Modo AP
  WiFi.softAP(apSSID, apPassword);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  // Configurar as rotas do servidor web
  server.on("/", handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.begin();
  Serial.println("Servidor web iniciado.");

  //Abaixo é o codigo original da Estação, e acima é o codigo do modo AP.

  pinMode(PIN1, INPUT);  // Set pin as INPUT
  pinMode(PIN2, INPUT);  // Set pin as INPUT
  pinMode(PIN3, INPUT);  // Set pin as INPUT
  pinMode(PIN4, INPUT);  // Set pin as INPUT
  pinMode(PIN5, INPUT);  // Set pin as INPUT
  pinMode(PIN6, INPUT);  // Set pin as INPUT
  pinMode(PIN7, INPUT);  // Set pin as INPUT
  pinMode(PIN8, INPUT);  // Set pin as INPUT
  
  pinMode(PLUVIPIN, INPUT);  // Set pin as INPUT
  pinMode(ANEMPIN, INPUT);  // Set pin as INPUT
  pinMode(DHTPIN, INPUT);  // Set pin as INPUT
  
  attachInterrupt(23, pluvi, CHANGE);  // Associa interrupção ao pino 2

  dht.begin();
}

void loop() {
  server.handleClient();

  // Tentar conectar à rede configurada
  WiFi.begin(ssid, password);

  Serial.print("Conectando a ");
  Serial.println(ssid);

  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED && retryCount < 20) {
    delay(500);
    Serial.print(".");
    retryCount++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha ao conectar.");
    return;
  }

  Serial.println("Direc");
  direc();
  Serial.println("Anem");
  anem();
  Serial.println("DHT");
  sensorDHT();

  Serial.print("Direção: ");
  Serial.println(direcao);

  Serial.print("Velocidade do vento: ");
  Serial.print(speedwind);
  Serial.println("km/h");

  Serial.print("Milímetros de chuva: ");
  Serial.print(mm);
  Serial.println("mm");

  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.println("%");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println("°C ");
  Serial.print("Calor: ");
  Serial.print(hic);
  Serial.println("°C ");
  Serial.println("--------------------");

  // Crie uma mensagem JSON com os dados do sensor
  String payload = "{\"temperatura\":" + String(t) + ",\"humidade\":" + String(h) + ",\"calor\":" + String(hic) + ",\"direcao\":\"" + direcao + "\",\"velocidade do vento\":" + String(speedwind) + ",\"milimetros de chuva\":" + String(mm) + "}";


  String url = String(thingsboardServer) + "/api/v1/" + accessToken + "/telemetry";  // Crie a URL para enviar os dados para o ThingsBoard

  // Crie um cliente HTTP e envie os dados
  HTTPClient http;  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(payload);

  // Verifique a resposta do servidor
  if (httpResponseCode > 0) {
    Serial.print("Codigo de Resposta: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Erro no envio, Codigo de Resposta: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  // Após o envio dos dados, desabilita o WiFi
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("Desconectado.");
  
  Serial.flush(); 
  esp_deep_sleep_start();
}

void direc() {
  int reads[8] = {digitalRead(PIN1), digitalRead(PIN2), digitalRead(PIN3), digitalRead(PIN4), digitalRead(PIN5), digitalRead(PIN6), digitalRead(PIN7), digitalRead(PIN8)};
  const char* direcoes[8] = {"Norte", "Nordeste", "Leste", "Sudeste", "Sul", "Sudoeste", "Oeste", "Noroeste"};

  for (int i = 0; i < 8; i++) {
    if (reads[i] == 1) {
      direcao = direcoes[i];
      return;
    }
  }

  direcao = "erro";
}

void pluvi(){
  unsigned long currentPluviMillis = millis();

  if (currentPluviMillis - lastPluviTime >= debouncePluviDelay) {
    lastPluviTime = currentPluviMillis;
    ciclo++;
    mm += 1.729;
  }
}

void anem(){
  speedwind = 0;
  windspeed = 0;
  counter = 0;

  long startTime = millis();
  while (millis() < startTime + period) {
    int readAnem = digitalRead(ANEMPIN);  // Reading pin value

    if (readAnem == 1){
      counter++;
      delay(20);
      while (readAnem == 1) {
        readAnem = digitalRead(ANEMPIN);
        if (readAnem != 1){
          break;
        } else if (millis() >= startTime + period){
          break;
        }
      }
    }
  }

  RPM = ((counter) * 60) / (period / 1000); // Calculate revolutions per minute (RPM)

  speedwind = (((2 * pi * radius * RPM) / 60) / 1000) * 3.6; //Calcula velocidade do vento em km/h
}

void sensorDHT(){
  h = dht.readHumidity();
  t = dht.readTemperature();
  hic = dht.computeHeatIndex(t, h, false);
}
