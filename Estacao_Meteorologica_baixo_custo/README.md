
# 🌦️ Estação Meteorológica 3D de Baixo Custo – IFC Camboriú

![Estação montada](IMAGES/estacao_montada.jpeg)

##  Sobre o Projeto

Este projeto apresenta a **evolução de um protótipo de estação meteorológica de baixo custo**, desenvolvido no **Instituto Federal Catarinense - Campus Camboriú**, com o objetivo de funcionar de forma contínua em ambientes externos. 

A proposta é criar uma estação **acessível, precisa e replicável**, utilizando **impressão 3D**, **componentes eletrônicos de baixo custo** e **tecnologias IoT**, permitindo o monitoramento climático em tempo real.

---

##  Equipe de Desenvolvimento

- **Rafael Luiz C. Santos Pereira**
- **Cauã da Costa Silva**
- **Rafael de Moura Speroni** (Orientador)
- **Angelo Augusto Frozza** (Colaborador)
- **Joice Seleme Mota** (Colaboradora)

📍 *Instituto Federal Catarinense – Campus Camboriú*

---

## ⚙️ Componentes Principais

| Componente | Função |
|-------------|--------|
| ESP32 | Microcontrolador principal |
| BME280 | Temperatura, umidade e pressão |
| AS5600 | Sensor de direção do vento |
| Reed Switch | Pluviômetro tipo báscula |
| Painel solar + bateria | Alimentação autônoma |
| PCB personalizada | Integra ESP32, sensores e monitoramento de carga |

---

##  Estrutura Física

A estrutura foi impressa em 3D e desenhada para:
- Resistir à chuva e ventos;
- Ser montada por encaixe e rosca;
- Reduzir o número de partes e o tempo de impressão;
- Permitir fixação em haste de suporte;

Arquivos STL disponíveis em [`3d_models`](HARDWARE/3D_MODELS).

---

## 📡 Comunicação

A estação utiliza comunicação **Wi-Fi** para envio de dados ao **ThingsBoard** por meio de requisições HTTP.  
Futuramente, está prevista a integração com **LoRa (SX1278)**, visando maior alcance e menor consumo energético.

Mais informações em [`docs/comunicacao.md`](FIRMWARE/Thingsboard/thingsboard.http).

---

## 🧠 Firmware

O código-fonte está em [`firmware/src`](FIRMWARE/main/codigo_estacao.ino).

Funções principais:
- Leitura dos sensores (BME280, AS5600, reed switch);
- Cálculo de chuva e vento;
- Envio de dados via HTTP;
- Modo de sono para economia de energia (deepsleep).

---

## 📊 Exemplo de Dados

| Data | Temperatura (°C) | Umidade (%) | Pressão (hPa) | Chuva (mm) |
|------|------------------|--------------|----------------|-------------|
| 15/10/2025 10:00 | 27.3 | 68 | 1012 | 0.0 |
| 15/10/2025 11:00 | 28.1 | 66 | 1011 | 0.2 |

Mais exemplos em [`data/exemplos_medicoes.csv`](DATA/painel_thingsboard.png)

---

## 🤝 Contribuindo

Este projeto é de caráter **acadêmico e experimental**, parte da Feira de Iniciação Científica e Extensão (FICE 2025).  
Contribuições futuras são bem-vindas por meio de *pull requests* ou *issues*.

---

## 📸 Créditos

Projeto desenvolvido pelos alunos e professores do **IFC – Campus Camboriú**, no contexto do projeto *Plataforma georreferenciada VigiAA - Vigilância do Aedes Aegypti* (Edital FAPESC 37/2024).
