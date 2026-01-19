# GPS Tracker
Это прототип GPS-трекера на ESP32-CAM (без использования камеры) и GPS-модуле NEO-6M.

Схема подключения:
```bah
ESP32-CAM → GPS NEO-6M
3V3 → VCC
GND → GND
IO13 → TX
IO12 → RX
```


<img width="950" height="629" alt="Схема подключения" src="https://github.com/user-attachments/assets/86b6694d-b846-4ad1-8eaf-babbd4b3b5aa" />


# Получение Bot Token и Chat ID


1. Получение Bot Token
   
  1.1 Найдите в Telegram @BotFather
  
  1.2 Нажмите Start → введите команду /newbot
  
  1.3 Укажите:
  
    1. Имя бота (например: Мой GPS Трекер)
    
    2. Юзернейм (латиница, должен заканчиваться на bot, например: my_gps_tracker_bot)
    
  1.4 Скопируйте токен из ответа BotFather (формат: цифры:буквы_цифры)

3. Получение Chat ID
Найдите в Telegram @userinfobot

Нажмите Start

Бот автоматически отправит ваш Chat ID

Примечание: Для групповых чатов или каналов Chat ID будет отрицательным.

Быстрый старт (Quick Start)
Получите Bot Token и Chat ID (инструкция выше)

Подготовьте данные вашей Wi-Fi сети:

SSID (название сети)

Пароль от сети

В файле кода найдите и замените следующие значения:

cpp
// Вставьте ваши данные:
const char* ssid = "ВАШ_SSID";
const char* password = "ВАШ_ПАРОЛЬ";
const char* BOT_TOKEN = "ВАШ_BOT_TOKEN";
const char* CHAT_ID = "ВАШ_CHAT_ID";
Загрузите код на ESP32-CAM

Устройство подключится к Wi-Fi и GPS, начнет отправлять координаты в Telegram
