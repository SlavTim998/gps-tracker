<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>GPS Tracker - ESP32-CAM + NEO-6M</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            line-height: 1.6;
            color: #333;
            background-color: #f5f5f5;
            padding: 20px;
            max-width: 1200px;
            margin: 0 auto;
        }
        
        header {
            background: linear-gradient(135deg, #2c3e50, #3498db);
            color: white;
            padding: 2rem;
            border-radius: 10px;
            margin-bottom: 2rem;
            text-align: center;
        }
        
        h1 {
            font-size: 2.5rem;
            margin-bottom: 0.5rem;
        }
        
        .subtitle {
            font-size: 1.2rem;
            opacity: 0.9;
        }
        
        main {
            display: grid;
            grid-template-columns: 1fr;
            gap: 2rem;
        }
        
        @media (min-width: 768px) {
            main {
                grid-template-columns: 1fr 1fr;
            }
        }
        
        section {
            background: white;
            padding: 1.5rem;
            border-radius: 10px;
            box-shadow: 0 4px 6px rgba(0,0,0,0.1);
        }
        
        h2 {
            color: #2c3e50;
            border-bottom: 3px solid #3498db;
            padding-bottom: 0.5rem;
            margin-bottom: 1rem;
        }
        
        h3 {
            color: #3498db;
            margin: 1rem 0 0.5rem 0;
        }
        
        .schema {
            text-align: center;
            grid-column: 1 / -1;
        }
        
        .schema img {
            max-width: 100%;
            height: auto;
            border-radius: 8px;
            border: 2px solid #ddd;
        }
        
        .connection-table {
            width: 100%;
            border-collapse: collapse;
            margin: 1rem 0;
        }
        
        .connection-table th, .connection-table td {
            padding: 12px;
            text-align: left;
            border-bottom: 1px solid #ddd;
        }
        
        .connection-table th {
            background-color: #f8f9fa;
            color: #2c3e50;
        }
        
        .connection-table tr:hover {
            background-color: #f1f8ff;
        }
        
        .instruction-steps {
            list-style-type: none;
            counter-reset: step-counter;
        }
        
        .instruction-steps li {
            counter-increment: step-counter;
            margin-bottom: 1.5rem;
            padding-left: 2.5rem;
            position: relative;
        }
        
        .instruction-steps li:before {
            content: counter(step-counter);
            background: #3498db;
            color: white;
            width: 28px;
            height: 28px;
            border-radius: 50%;
            display: inline-flex;
            align-items: center;
            justify-content: center;
            position: absolute;
            left: 0;
            top: 0;
            font-weight: bold;
        }
        
        .code-block {
            background: #2c3e50;
            color: #ecf0f1;
            padding: 1rem;
            border-radius: 5px;
            font-family: 'Courier New', monospace;
            margin: 1rem 0;
            overflow-x: auto;
        }
        
        .highlight {
            background: #fff3cd;
            padding: 0.2rem 0.4rem;
            border-radius: 3px;
            font-weight: bold;
        }
        
        .warning {
            background: #f8d7da;
            color: #721c24;
            padding: 1rem;
            border-radius: 5px;
            border-left: 4px solid #dc3545;
            margin: 1rem 0;
        }
        
        .note {
            background: #d1ecf1;
            color: #0c5460;
            padding: 1rem;
            border-radius: 5px;
            border-left: 4px solid #17a2b8;
            margin: 1rem 0;
        }
        
        .section-link {
            display: inline-block;
            background: #3498db;
            color: white;
            padding: 0.5rem 1rem;
            border-radius: 5px;
            text-decoration: none;
            margin-top: 1rem;
            transition: background 0.3s;
        }
        
        .section-link:hover {
            background: #2980b9;
        }
        
        footer {
            text-align: center;
            margin-top: 3rem;
            padding-top: 1rem;
            border-top: 1px solid #ddd;
            color: #666;
        }
    </style>
</head>
<body>
    <header>
        <h1>GPS Tracker</h1>
        <p class="subtitle">Прототип GPS-трекера на ESP32-CAM (без камеры) и GPS-модуле NEO-6M</p>
    </header>
    
    <main>
        <section class="schema">
            <h2>Схема подключения</h2>
            <p><strong>ESP32-CAM</strong> → <strong>GPS NEO-6M</strong></p>
            <img src="https://github.com/user-attachments/assets/86b6694d-b846-4ad1-8eaf-babbd4b3b5aa" 
                 alt="Схема подключения ESP32-CAM и GPS NEO-6M" 
                 width="950" height="629">
        </section>
        
        <section>
            <h2>Таблица подключения</h2>
            <table class="connection-table">
                <thead>
                    <tr>
                        <th>ESP32-CAM</th>
                        <th>GPS NEO-6M</th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td><span class="highlight">3V3</span></td>
                        <td>VCC</td>
                    </tr>
                    <tr>
                        <td><span class="highlight">GND</span></td>
                        <td>GND</td>
                    </tr>
                    <tr>
                        <td><span class="highlight">IO13</span></td>
                        <td>TX</td>
                    </tr>
                    <tr>
                        <td><span class="highlight">IO12</span></td>
                        <td>RX</td>
                    </tr>
                </tbody>
            </table>
        </section>
        
        <section>
            <h2>Получение Bot Token</h2>
            <ol class="instruction-steps">
                <li>Найдите в Telegram <strong>@BotFather</strong></li>
                <li>Нажмите <span class="highlight">Start</span> → введите команду <span class="highlight">/newbot</span></li>
                <li>Укажите:
                    <ul style="margin-top: 0.5rem; margin-left: 1rem;">
                        <li><strong>Имя бота</strong> (например: Мой GPS Трекер)</li>
                        <li><strong>Юзернейм</strong> (латиница, должен заканчиваться на <span class="highlight">bot</span>, например: my_gps_tracker_bot)</li>
                    </ul>
                </li>
                <li>Скопируйте токен из ответа BotFather (формат: <span class="highlight">цифры:буквы_цифры</span>)</li>
            </ol>
            
            <div class="warning">
                <strong>Важно:</strong> Bot Token является конфиденциальной информацией — не публикуйте его в открытом доступе!
            </div>
        </section>
        
        <section>
            <h2>Получение Chat ID</h2>
            <ol class="instruction-steps">
                <li>Найдите в Telegram <strong>@userinfobot</strong></li>
                <li>Нажмите <span class="highlight">Start</span></li>
                <li>Бот автоматически отправит ваш <strong>Chat ID</strong></li>
            </ol>
            
            <div class="note">
                <strong>Примечание:</strong> Для групповых чатов или каналов Chat ID будет отрицательным.
            </div>
        </section>
        
        <section style="grid-column: 1 / -1;">
            <h2>Быстрый старт (Quick Start)</h2>
            
            <h3>1. Подготовка данных</h3>
            <p>Получите и подготовьте следующие данные:</p>
            <ul style="margin-left: 1.5rem; margin-bottom: 1rem;">
                <li><strong>Bot Token</strong> (инструкция выше)</li>
                <li><strong>Chat ID</strong> (инструкция выше)</li>
                <li><strong>SSID</strong> (название вашей Wi-Fi сети)</li>
                <li><strong>Пароль</strong> от Wi-Fi сети</li>
            </ul>
            
            <h3>2. Настройка кода</h3>
            <p>В файле кода найдите и замените следующие значения:</p>
            
            <div class="code-block">
// Вставьте ваши данные:<br>
const char* ssid = "<span style="color:#e74c3c">ВАШ_SSID</span>";<br>
const char* password = "<span style="color:#e74c3c">ВАШ_ПАРОЛЬ</span>";<br>
const char* BOT_TOKEN = "<span style="color:#e74c3c">ВАШ_BOT_TOKEN</span>";<br>
const char* CHAT_ID = "<span style="color:#e74c3c">ВАШ_CHAT_ID</span>";
            </div>
            
            <h3>3. Запуск системы</h3>
            <ol style="margin-left: 1.5rem;">
                <li>Загрузите код на ESP32-CAM</li>
                <li>Устройство подключится к Wi-Fi сети</li>
                <li>GPS-модуль начнет поиск спутников</li>
                <li>После получения координат данные будут отправлены в Telegram</li>
            </ol>
            
            <div class="note">
                <strong>Совет:</strong> Для первых запусков GPS может потребоваться несколько минут для получения сигнала. Убедитесь, что модуль находится под открытым небом.
            </div>
        </section>
    </main>
    
    <footer>
        <p>© 2024 GPS Tracker Project | ESP32-CAM + NEO-6M</p>
        <p style="font-size: 0.9rem; margin-top: 0.5rem;">Инструкция по созданию Telegram бота для GPS-трекера</p>
    </footer>
</body>
</html>
