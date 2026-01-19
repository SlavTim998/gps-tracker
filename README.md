# gps-tracker
Это прототип GPS трекера на esp32-cam (без камеры) и gps модуле neo-6m

Схема подключения:
Esp32-cam ------> GPS (neo-6m)

3V3       ------> VCC

GND       ------> GND

IO13      ------> TX

IO12      ------> RX

<img width="950" height="629" alt="image" src="https://github.com/user-attachments/assets/86b6694d-b846-4ad1-8eaf-babbd4b3b5aa" />

# Получение Bot Token и Chat Id

Инструкция по созданию Telegram бота и получению Bot token:
1. Найдите в Telegram @BotFather
2. Нажмите Start → затем команда /newbot
3. Укажите:
   · Имя бота (например: Мой Бот)
   · Юзернейм (латиница, заканчивается на bot, например: my_test_bot)
4. Скопируйте токен из ответа (формат: цифры:буквы_цифры)

Инструкция по получению Chat Id:
1. Найдите в Telegram @userinfobot
2. Нажмите Start
3. Бот автоматически покажет ваш Chat ID

## QUICK START


