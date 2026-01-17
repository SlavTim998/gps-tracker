#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "Название вашей сети";
const char* password = "Пароль от вашей сети";
#define BOT_TOKEN "Токен вашего бота"
#define CHAT_ID "Ваш chat id (получаем здесь @userinfobot)"

#include <TinyGPS++.h>
#include <HardwareSerial.h>

TinyGPSPlus gps;

HardwareSerial gpsSerial(2);

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
unsigned long botLastTime = 0;
const unsigned long botDelay = 1000;
bool gpsFixed = false;
unsigned long lastGpsUpdate = 0;
const unsigned long gpsTimeout = 30000;

String getLocationString() {
  if (!gps.location.isValid() || !gps.location.isUpdated()) {
    return "📍 GPS ожидает фиксации...\nСпутников: " + String(gps.satellites.value());
  }
  
  String location = "📍 Координаты:\n";
  location += "Широта: " + String(gps.location.lat(), 6) + "\n";
  location += "Долгота: " + String(gps.location.lng(), 6) + "\n";
  location += "Спутников: " + String(gps.satellites.value()) + "\n";
  location += "Точность: " + String(gps.hdop.hdop() / 100.0, 1) + " м\n";
  
  if (gps.date.isValid() && gps.time.isValid()) {
    char dateTime[30];
    sprintf(dateTime, "%02d.%02d.%04d %02d:%02d:%02d",
            gps.date.day(), gps.date.month(), gps.date.year(),
            gps.time.hour() + 3,
            gps.time.minute(), gps.time.second());
    location += "Время: " + String(dateTime) + "\n";
  }
  
  location += "\n🗺 Google Maps:\n";
  location += "https://maps.google.com/?q=";
  location += String(gps.location.lat(), 6);
  location += ",";
  location += String(gps.location.lng(), 6);
  
  return location;
}

String getMapsLink() {
  if (!gps.location.isValid()) {
    return "❌ GPS не готов. Подождите фиксации спутников.";
  }
  
  String link = "📍 Текущее местоположение:\n";
  link += "https://maps.google.com/?q=";
  link += String(gps.location.lat(), 6);
  link += ",";
  link += String(gps.location.lng(), 6);
  link += "&z=17";
  
  return link;
}


String getSystemStatus() {
  String status = "📊 Статус системы:\n";
  status += "--------------------------------\n";
  status += "📶 WiFi: ";
  if (WiFi.status() == WL_CONNECTED) {
    status += "Подключен\n";
    status += "  RSSI: " + String(WiFi.RSSI()) + " dBm\n";
    status += "  IP: " + WiFi.localIP().toString() + "\n";
  } else {
    status += "Отключен\n";
  }

  status += "🛰 GPS:\n";
  status += "  Фиксация: " + String(gps.location.isValid() ? "ЕСТЬ ✓" : "НЕТ ✗") + "\n";
  status += "  Спутников: " + String(gps.satellites.value()) + "\n";
  status += "  HDOP: " + String(gps.hdop.hdop() / 100.0, 1) + "\n";
  
  if (gps.location.isValid()) {
    status += "  Широта: " + String(gps.location.lat(), 6) + "\n";
    status += "  Долгота: " + String(gps.location.lng(), 6) + "\n";
    status += "  Скорость: " + String(gps.speed.kmph(), 1) + " км/ч\n";
    status += "  Курс: " + String(gps.course.deg(), 0) + "°\n";
  }

  status += "--------------------------------\n";
  status += "💻 Система:\n";
  status += "  Аптайм: " + String(millis() / 1000) + " сек\n";
  status += "  Свободная память: " + String(esp_get_free_heap_size() / 1024) + " КБ\n";
  
  return status;
}

void handleNewMessages(int numNewMessages) {
  Serial.println("📨 Получено сообщений: " + String(numNewMessages));
  
  for (int i = 0; i < numNewMessages; i++) {
    String chatId = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String fromName = bot.messages[i].from_name;
    
    Serial.println("👤 От: " + fromName);
    Serial.println("💬 Текст: " + text);

    if (chatId != CHAT_ID) {
      bot.sendMessage(chatId, "⛔️ Неавторизованный доступ!", "");
      continue;
    }

    if (text == "/start") {
      String welcome = "Здраствуйте, " + fromName + "...\n";
      welcome += "Это GPS-трекер\n\n";
      welcome += "📋 Доступные команды:\n";
      welcome += "/location - Текущие координаты\n";
      welcome += "/maps - Ссылка на карту\n";
      welcome += "/status - Статус системы\n";
      welcome += "/help - Справка\n";
      
      bot.sendMessage(chatId, welcome, "Markdown");
    }
    
    else if (text == "/help") {
      String help = "Помощь по командам:\n\n";
      help += "/location - Полные координаты\n";
      help += "/maps - Только ссылка на карту\n";
      help += "/status - Статус системы и GPS\n";
      
      bot.sendMessage(chatId, help, "");
    }
    
    else if (text == "/location") {
      bot.sendMessage(chatId, "⏳ Получаю координаты...", "");
      delay(500);
      String location = getLocationString();
      bot.sendMessage(chatId, location, "");
    }
    
    else if (text == "/maps") {
      String mapsLink = getMapsLink();
      bot.sendMessage(chatId, mapsLink, "");
    }
    
    else if (text == "/status") {
      String status = getSystemStatus();
      bot.sendMessage(chatId, status, "");
    }
    
    else {
      bot.sendMessage(chatId, "❓ Неизвестная команда. Введите /help для получения списка команд.", "");
    }
  }
}


void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=================================");
  Serial.println("     ESP32-CAM GPS Telegram Bot");
  Serial.println("        GPS модуль: NEO-6M");
  Serial.println("=================================\n");
  
  gpsSerial.begin(9600, SERIAL_8N1, 13, 12);
  
  Serial.println("✅ GPS Serial2 инициализирован (RX=GPIO16, TX=GPIO17)");
  Serial.println("⏳ Ожидание данных GPS...");

  Serial.println("\n📡 Подключение к WiFi: " + String(ssid));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    delay(500);
    Serial.print(".");
    wifiAttempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi подключен!");
    Serial.print("📡 IP адрес: ");
    Serial.println(WiFi.localIP());
    Serial.print("📶 Сила сигнала: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\n❌ Ошибка подключения к WiFi!");
  }

  client.setInsecure();

  if (WiFi.status() == WL_CONNECTED) {
    delay(2000);
    bot.sendMessage(CHAT_ID, "GPS Bot запущен!\n"
                             "📡 WiFi: " + String(WiFi.RSSI()) + " dBm\n"
                             "💻 Готов к работе!", "");
  }
  
  Serial.println("\n✅ Система инициализирована");
  Serial.println("🤖 Бот ожидает команд в Telegram...");
  Serial.println("=================================\n");
}

void loop() {
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);

  }

  static unsigned long lastGpsPrint = 0;
  if (millis() - lastGpsPrint > 5000) {
    lastGpsPrint = millis();
    
    Serial.print("🛰 GPS: ");
    if (gps.location.isValid()) {
      Serial.print("Фиксация ✓ | ");
      Serial.print("Спутников: " + String(gps.satellites.value()));
      Serial.print(" | Lat: " + String(gps.location.lat(), 6));
      Serial.print(" | Lng: " + String(gps.location.lng(), 6));
      Serial.print(" | HDOP: " + String(gps.hdop.hdop() / 100.0, 1));
    } else {
      Serial.print("Поиск... | Спутников: " + String(gps.satellites.value()));
    }
    Serial.println();
  }

  if (millis() - botLastTime > botDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    
    botLastTime = millis();
  }

  static unsigned long lastAutoSend = 0;
  static bool wasFixed = false;
  
  if (gps.location.isValid() && gps.location.isUpdated()) {
    if (!wasFixed) {
      wasFixed = true;
      if (WiFi.status() == WL_CONNECTED) {
        bot.sendMessage(CHAT_ID, "✅ GPS фиксация получена!\n" + getLocationString(), "");
      }
    }
    
    if (millis() - lastAutoSend > 300000) {
      lastAutoSend = millis();
      if (WiFi.status() == WL_CONNECTED) {
        bot.sendMessage(CHAT_ID, "📍 Авто-отчет:\n" + getLocationString(), "");
      }
    }
  }
  delay(10);
}