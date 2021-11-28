## Alimentador automático de pets

### Objetivo do projeto
Automatizar o processo de fornecimento de ração para animais de estimação, empregando comunicação por protocolo MQTT.

### Recursos utilizados
Abaixo temos os principais recursos empregados na execução do projeto:

* Placa NodeMcu ESP8266 com Wi-Fi V3
* Arduino IDE para programação
* MQTTBox para testes e operação

### Comunicação com o MQTTBox
O MQTTBox foi usado para gerar um client com dois tópicos: um subscribe e um publish, responsáveis pelo acionamento das portas digitais do NodeMcu remotamente (conectado através da rede wireless doméstica).
