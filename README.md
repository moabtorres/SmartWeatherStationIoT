# SmartWeatherStationIoT

## 📖 Descrição do Projeto
O **SmartWeather Station IoT** é uma estação inteligente de monitoramento ambiental baseada no microcontrolador ESP32. O sistema realiza a leitura local de temperatura e umidade através do sensor DHT22 e consome dados meteorológicos em tempo real de APIs REST públicas. Os dados são processados e exibidos em um Display LCD 16x2 e em um TFT ILI9341, acompanhados de alertas visuais (LEDs) e sonoros (Buzzer) para anomalias térmicas.

### ✨ Funcionalidades Bônus Implementadas
* **Nível 1 (Previsão do Tempo):** O sistema extrai e exibe no TFT a previsão de temperaturas Máxima e Mínima do dia via Open-Meteo.
* **Nível 3 (Consulta de CEP):** O ESP32 realiza uma requisição na API do ViaCEP durante a inicialização (Setup) para identificar e exibir dinamicamente o nome da cidade e estado (ex: Recife - PE) com base no CEP configurado.

---

## 🏗️ Arquitetura da Solução
A arquitetura baseia-se em um modelo *Edge-to-Cloud*, onde o ESP32 atua como o nó central de processamento. 
1. **Sensoriamento:** O DHT22 coleta os dados físicos.
2. **Conectividade:** O módulo Wi-Fi nativo do ESP32 estabelece conexão de rede.
3. **Processamento:** Requisições HTTP GET são disparadas a cada 30 segundos. A biblioteca `ArduinoJson` desserializa as respostas.
4. **Atuação/Exibição:** Os dados formatados são enviados via I2C para o LCD e via barramento SPI para o TFT. Lógicas condicionais ativam portas GPIO para acionar os LEDs (Status da Rede) e o Buzzer (Alarme Térmico).

---

## 🔌 Componentes Utilizados
Todos os componentes foram validados em ambiente de simulação online:
* 1x Microcontrolador **ESP32**
* 1x Sensor de Temperatura e Umidade **DHT22**
* 1x Display **LCD 16x2** (com módulo I2C)
* 1x Display **TFT ILI9341** (comunicação SPI)
* 1x **LED Verde** (Indicador de Sucesso de Rede)
* 1x **LED Vermelho** (Indicador de Falha de Rede)
* 1x **Buzzer** (Alarme de limite de temperatura)
* 2x Resistores 220Ω

---

## 🌐 APIs Utilizadas
O projeto consome as seguintes APIs públicas e gratuitas:
1. **[Open-Meteo API](https://open-meteo.com/):** Responsável por fornecer o clima atual e a previsão (Max/Min) baseada em latitude e longitude. Retorna dados no formato JSON.
2. **[ViaCEP](https://viacep.com.br/):** Responsável por resolver o CEP e retornar a Localidade (Cidade) e a UF (Estado) da estação.

---

## 🚀 Instruções de Execução

### Pré-requisitos
Para rodar este projeto, você precisará acessar a plataforma Wokwi.
Nenhuma instalação de software local é necessária.

## 📸 Imagens do Projeto

<img width="992" height="803" alt="image" src="https://github.com/user-attachments/assets/cc472bfe-3df5-4790-a679-52efca6ca3c7" />
<img width="301" height="464" alt="image" src="https://github.com/user-attachments/assets/cd3910ef-5735-44f8-a2e1-452527d96c71" />
<img width="682" height="499" alt="image" src="https://github.com/user-attachments/assets/4efc82c0-d6fc-4e62-9a64-1d9fdc6e6ef5" />
<img width="423" height="230" alt="image" src="https://github.com/user-attachments/assets/9ca4d545-7f8b-4a6e-8fcb-c37ae75dd89c" />
<img width="409" height="488" alt="image" src="https://github.com/user-attachments/assets/ac11f641-b8a8-456d-a256-1d4cfee63d18" />

---

## 🎥 Vídeo Demonstrativo
Assista ao vídeo detalhando o funcionamento do projeto:
👉 https://drive.google.com/file/d/1ymiQaZH4m8YVZDetnXDHAiOamdh35ZnM/view?usp=sharing

---
