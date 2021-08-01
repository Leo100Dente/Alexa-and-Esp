#include <Arduino.h>
#include <WiFi.h>
#include <fauxmoESP.h>

// Identificação para Alexa (Porta e Nome do Dispositivo)
#define RELAY_PIN_1 2
#define LAMP_1 "LED 1"

// Criando Classes (Orientação a objetos)
fauxmoESP fauxmo;

// Identificação da Rede WiFi
const char *ssid = "***"; // Nome da Sua Rede WiFi
const char *passwd = "***";      // Senha da Sua Rede WiFi

// Variavel para teste
int i = 0;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, passwd);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) // Tentando conectar ao WiFi
  {
    delay(500);
    Serial.println("Conectando ao WiFi...");
  }
  pinMode(RELAY_PIN_1, OUTPUT);   // Setando o pin RELAY-PIN-1 como uma saida
  digitalWrite(RELAY_PIN_1, LOW); // Jogando um sinal baixo para o RELAY-PIN-1
  Serial.println("Conectado a rede WiFi!");
  Serial.print("O IP do ESP é: ");
  Serial.println(WiFi.localIP());

  // Por padrão, fauxmoESP cria seu próprio servidor web na porta definida
  // A porta TCP deve ser 80 para dispositivos gen3 (o padrão é 1901)
  // Isso deve ser feito antes da chamada enable()
  fauxmo.createServer(true); // não é necessário, este é o valor padrão
  fauxmo.setPort(80);        // Isso é necessário para dispositivos gen3

  // Você deve chamar enable(true) assim que tiver uma conexão WiFi
  // Você pode ativar ou desativar a biblioteca a qualquer momento
  // Desativá-lo impedirá que os dispositivos sejam descobertos e trocados
  fauxmo.enable(true);

  // Você pode usar diferentes maneiras de chamar a Alexa para modificar o estado dos dispositivos:
  // "Alexa, ligar LED um"

  // Adiciona os dispositivos
  fauxmo.addDevice(LAMP_1);

  fauxmo.onSetState([](unsigned char device_id, const char *device_name, bool state, unsigned char value)
                    {
                      // Retorno de chamada quando um comando da Alexa é recebido.
                      // Você pode usar device_id ou device_name para escolher o elemento no qual realizar uma ação (relé, LED, ...)
                      // O state é um booleano (ON / OFF) e value um número de 0 a 255 (se você disser "definir a luz da cozinha para 50%", receberá 128 aqui).
                      Serial.println("****************************************************************************");
                      Serial.printf("device_id: #%d \n device_name: %s \n state: %d \n value: %d \n", device_id, device_name, state, value);
                      Serial.println("****************************************************************************");
                      // Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
                      if ((strcmp(device_name, LAMP_1) == 0)) // Ver qual o dispositivo foi ativado
                      {
                        Serial.println("RELAY 1 switched by Alexa");
                        //digitalWrite(RELAY_PIN_1, !digitalRead(RELAY_PIN_1));
                        if (state)
                        {
                          digitalWrite(RELAY_PIN_1, HIGH);
                        }
                        else
                        {
                          digitalWrite(RELAY_PIN_1, LOW);
                        }
                      }
                    });
}

void loop()
{
  fauxmo.handle();

  static unsigned long last = millis();
  if (millis() - last > 5000)
  {
    last = millis();
    Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }
}
