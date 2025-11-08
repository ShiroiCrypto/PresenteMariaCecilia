#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
// SPIFFS não é mais necessário - HTML e CSS estão embutidos no código

// Definições da Rede Wi-Fi
const char *ssid = "Presente Para Maria Cecilia"; // Nome da rede Wi-Fi
const char *password = "matheus123"; // Senha da rede Wi-Fi
const IPAddress apIP(192, 168, 4, 1); // IP do Access Point (AP)

// Objetos para Servir a Web e DNS
DNSServer dnsServer;
WebServer server(80); // Servidor na porta 80 (HTTP padrão)

// Configuração da câmera ESP32CAM
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Frame buffer para captura
camera_fb_t * fb = NULL;

String getContentType(String filename){
  if(filename.endsWith(".htm") || filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".jpg") || filename.endsWith(".jpeg")) return "image/jpeg";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else return "text/plain";
}

// HTML completo com CSS inline embutido no código
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>🌟 Celebrando Nossa Amizade</title>
    <style>
/* Paleta de Cores */
:root {
    --primary-color: #0066cc;
    --secondary-color: #4da6ff;
    --accent-color: #b3d9ff;
    --text-color: #333;
    --bg-color: #f0f7ff;
    --success-color: #28a745;
    --error-color: #dc3545;
    --shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
    --shadow-lg: 0 8px 16px rgba(0, 0, 0, 0.15);
}
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}
html {
    width: 100%;
    overflow-x: hidden;
    margin: 0;
    padding: 0;
}
body {
    font-family: 'Arial', sans-serif;
    color: var(--text-color);
    margin: 0;
    padding: 0;
    width: 100%;
    overflow-x: hidden;
    display: flex;
    justify-content: flex-start;
    align-items: center;
    min-height: 100vh;
    padding-left: 5%;
    background-color: var(--bg-color);
    background-image: linear-gradient(135deg, var(--bg-color) 0%, #ffffff 50%, var(--accent-color) 100%);
    background-attachment: fixed;
}
.container {
    background-color: #fff;
    border-radius: 20px;
    box-shadow: var(--shadow-lg);
    padding: 40px;
    max-width: 800px;
    width: 90%;
    text-align: center;
    animation: fadeIn 0.6s ease-in;
    margin: 0;
    flex-shrink: 0;
}
@keyframes fadeIn {
    from { opacity: 0; transform: translateY(20px); }
    to { opacity: 1; transform: translateY(0); }
}
header {
    text-align: center;
    margin-bottom: 2em;
    width: 100%;
}
section {
    text-align: center;
    margin: 0 auto;
    width: 100%;
}
h1 {
    color: var(--primary-color);
    font-size: 2.4em;
    margin: 0 0 10px 0;
    text-align: center;
    text-shadow: 2px 2px 4px rgba(0, 102, 204, 0.1);
    width: 100%;
}
h2 {
    color: var(--secondary-color);
    font-size: 1.6em;
    margin: 0 0 1.5em 0;
    text-align: center;
    width: 100%;
}
h3 {
    text-align: center;
    margin: 0 auto;
    width: 100%;
}
p {
    line-height: 1.6;
    margin: 0 0 20px 0;
    text-align: center;
    color: #445566;
    width: 100%;
}
.message-text {
    text-align: center;
    margin: 1.5em auto;
    max-width: 600px;
}
.camera-section {
    margin: 2em 0;
    text-align: center;
    width: 100%;
}
.camera-container {
    background: linear-gradient(135deg, #f8f9fa 0%, #ffffff 100%);
    border-radius: 16px;
    padding: 20px;
    box-shadow: var(--shadow);
    border: 2px solid var(--accent-color);
    margin: 0 auto;
    text-align: center;
    width: 100%;
}
.camera-status {
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 10px;
    margin-bottom: 15px;
    font-size: 0.9em;
    color: #666;
}
.status-indicator {
    width: 12px;
    height: 12px;
    border-radius: 50%;
    background-color: #ccc;
    transition: all 0.3s ease;
}
.status-indicator.active {
    background-color: var(--success-color);
    box-shadow: 0 0 8px var(--success-color);
    animation: pulse 2s infinite;
}
@keyframes pulse {
    0%, 100% { opacity: 1; }
    50% { opacity: 0.6; }
}
.video-container {
    position: relative;
    width: 100%;
    max-width: 640px;
    margin: 0 auto;
    border-radius: 12px;
    overflow: hidden;
    background: #000;
    box-shadow: var(--shadow-lg);
    text-align: center;
}
.stream-image {
    width: 100%;
    height: auto;
    display: block;
    min-height: 240px;
    object-fit: contain;
    margin: 0 auto;
}
.camera-loading {
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    background: rgba(0, 0, 0, 0.7);
    color: white;
    z-index: 10;
}
.spinner {
    width: 40px;
    height: 40px;
    border: 4px solid rgba(255, 255, 255, 0.3);
    border-top-color: white;
    border-radius: 50%;
    animation: spin 1s linear infinite;
    margin-bottom: 10px;
}
@keyframes spin {
    to { transform: rotate(360deg); }
}
.camera-controls {
    display: flex;
    gap: 12px;
    justify-content: center;
    align-items: center;
    margin: 15px 0 0 0;
    flex-wrap: wrap;
    width: 100%;
}
.camera-btn {
    min-width: 140px;
    font-size: 0.95em;
    padding: 12px 24px;
}
.captured-photo {
    margin: 20px auto 0;
    padding: 20px;
    background: linear-gradient(135deg, var(--accent-color) 0%, #ffffff 100%);
    border-radius: 12px;
    animation: slideDown 0.5s ease-out;
    text-align: center;
    max-width: 100%;
}
@keyframes slideDown {
    from { opacity: 0; transform: translateY(-20px); }
    to { opacity: 1; transform: translateY(0); }
}
.captured-photo h3 {
    color: var(--primary-color);
    margin: 0 auto 15px;
    font-size: 1.2em;
}
.captured-photo img {
    max-width: 100%;
    height: auto;
    border-radius: 8px;
    box-shadow: var(--shadow);
    margin: 0 auto 15px;
    display: block;
}
.subtitle {
    color: #666;
    margin: 6px auto 0;
}
.action {
    text-align: center;
    margin: 2em 0;
    display: flex;
    justify-content: center;
    align-items: center;
    width: 100%;
}
footer {
    text-align: center;
    margin: 30px 0 0 0;
    padding: 20px 0 0 0;
    border-top: 1px solid #eee;
    font-size: 0.9em;
    color: #999;
    width: 100%;
}
.button {
    display: inline-block;
    background-color: var(--primary-color);
    color: white;
    text-decoration: none;
    padding: 15px 30px;
    border-radius: 50px;
    font-weight: bold;
    font-size: 1.1em;
    transition: all 0.3s ease;
    border: none;
    cursor: pointer;
    box-shadow: var(--shadow);
    position: relative;
    overflow: hidden;
    margin: 0 auto;
}
.button::before {
    content: '';
    position: absolute;
    top: 50%;
    left: 50%;
    width: 0;
    height: 0;
    border-radius: 50%;
    background: rgba(255, 255, 255, 0.3);
    transform: translate(-50%, -50%);
    transition: width 0.6s, height 0.6s;
}
.button:hover::before {
    width: 300px;
    height: 300px;
}
.button.secondary {
    background-color: var(--secondary-color);
}
.button:hover {
    background-color: #0052a3;
    transform: translateY(-3px);
    box-shadow: 0 6px 12px rgba(0, 102, 204, 0.3);
}
.button.secondary:hover {
    background-color: #3b88c4;
}
.button:active {
    transform: translateY(-1px);
}
.button:disabled {
    opacity: 0.6;
    cursor: not-allowed;
    transform: none;
}
.button.glow {
    animation: glow 2s ease-in-out infinite alternate;
}
@keyframes glow {
    from { box-shadow: 0 0 10px var(--primary-color), 0 0 20px var(--primary-color); }
    to { box-shadow: 0 0 20px var(--primary-color), 0 0 30px var(--primary-color), 0 0 40px var(--primary-color); }
}
.modal {
    position: fixed;
    inset: 0;
    display: none;
    align-items: center;
    justify-content: center;
    background: rgba(0, 0, 0, 0.6);
    z-index: 9999;
    backdrop-filter: blur(4px);
    animation: fadeIn 0.3s ease;
}
.modal-content {
    background: #fff;
    padding: 32px;
    border-radius: 16px;
    max-width: 600px;
    width: 90%;
    text-align: center;
    box-shadow: 0 10px 40px rgba(0, 0, 0, 0.3);
    position: relative;
    animation: slideUp 0.4s ease;
}
@keyframes slideUp {
    from { opacity: 0; transform: translateY(30px); }
    to { opacity: 1; transform: translateY(0); }
}
.modal .big {
    font-size: 1.3em;
    color: var(--primary-color);
    margin: 10px 0 20px;
    font-weight: bold;
}
.close {
    position: absolute;
    top: 15px;
    right: 20px;
    background: transparent;
    border: none;
    font-size: 2em;
    cursor: pointer;
    color: #999;
    transition: all 0.3s ease;
    width: 40px;
    height: 40px;
    display: flex;
    align-items: center;
    justify-content: center;
    border-radius: 50%;
}
.close:hover {
    color: var(--error-color);
    background: rgba(220, 53, 69, 0.1);
    transform: rotate(90deg);
}
.modal-actions {
    display: flex;
    gap: 12px;
    justify-content: center;
    margin-top: 20px;
    flex-wrap: wrap;
}
.card-preview {
    margin: 20px auto;
    padding: 20px;
    background: linear-gradient(135deg, var(--primary-color) 0%, var(--secondary-color) 100%);
    border-radius: 12px;
    color: white;
    box-shadow: var(--shadow-lg);
    text-align: center;
}
.card-front {
    text-align: center;
}
.card-front h3 {
    font-size: 2em;
    margin: 10px auto;
    text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.2);
}
.card-front .quote {
    font-size: 1.1em;
    font-style: italic;
    margin: 15px auto;
    line-height: 1.6;
}
.card-front .date {
    font-size: 0.9em;
    opacity: 0.9;
    margin: 20px auto 0;
}
#confetti-canvas {
    position: fixed;
    left: 0;
    top: 0;
    width: 100%;
    height: 100%;
    pointer-events: none;
    z-index: 9998;
}
@media (max-width: 768px) {
    body { 
        padding: 20px 0;
        justify-content: center;
        align-items: center;
    }
    .container {
        padding: 25px 20px;
        width: calc(100% - 32px);
        margin: 0 16px;
        max-width: none;
        box-sizing: border-box;
    }
    h1 { font-size: 2em; }
    h2 { font-size: 1.4em; }
    .camera-controls {
        flex-direction: column;
        align-items: center;
        gap: 10px;
    }
    .camera-btn {
        width: 100%;
        max-width: 100%;
        margin: 0;
    }
    .modal-actions {
        flex-direction: column;
        align-items: center;
        gap: 10px;
    }
    .modal-actions .button {
        width: 100%;
        max-width: 100%;
    }
    .message-text { 
        padding: 0 10px;
        text-align: center;
    }
    .video-container {
        max-width: 100%;
    }
}
@media (max-width: 480px) {
    body { 
        padding: 16px 0;
        justify-content: center;
        align-items: center;
    }
    .container {
        padding: 20px 15px;
        border-radius: 16px;
        width: calc(100% - 24px);
        margin: 0 12px;
        max-width: none;
        box-sizing: border-box;
    }
    h1 { 
        font-size: 1.8em;
        margin: 0 0 8px 0;
    }
    h2 { 
        font-size: 1.2em;
        margin: 0 0 1em 0;
    }
    .button {
        padding: 12px 20px;
        font-size: 0.95em;
        width: 100%;
        max-width: 100%;
        margin: 5px 0;
    }
    .camera-container { 
        padding: 15px;
        box-sizing: border-box;
    }
    .video-container { 
        min-height: 200px;
        max-width: 100%;
    }
    .stream-image { 
        min-height: 200px;
    }
    .message-text { 
        padding: 0 5px;
        text-align: center;
        font-size: 0.95em;
    }
    .camera-controls {
        gap: 8px;
    }
    .camera-btn {
        width: 100%;
        max-width: 100%;
        padding: 12px 16px;
        font-size: 0.9em;
    }
    header {
        margin-bottom: 1.5em;
    }
    .camera-section {
        margin: 1.5em 0;
    }
    .action {
        margin: 1.5em 0;
    }
}
    </style>
</head>
<body>
    <canvas id="confetti-canvas" aria-hidden="true"></canvas>
    <canvas id="card-canvas" aria-hidden="true"></canvas>

    <div class="container">
        <header>
            <h1>✨ Olá, Maria Cecília!</h1>
            <p class="subtitle">Que bom ter você como amiga.</p>
        </header>

        <section class="message">
            <h2>Um presente digital para celebrar nossa amizade</h2>
            
            <div class="camera-section">
                <div class="camera-container">
                    <div id="camera-status" class="camera-status">
                        <span class="status-indicator"></span>
                        <span class="status-text">Câmera pronta</span>
                    </div>
                    <div id="video-container" class="video-container">
                        <img id="stream" src="/stream" alt="Streaming de vídeo" class="stream-image">
                        <div id="camera-loading" class="camera-loading">
                            <div class="spinner"></div>
                            <p>Carregando câmera...</p>
                        </div>
                    </div>
                    <div class="camera-controls">
                        <button id="captureBtn" class="button camera-btn" aria-label="Capturar foto">📸 Capturar Foto</button>
                        <button id="toggleStreamBtn" class="button secondary camera-btn" aria-label="Alternar streaming">⏸️ Pausar</button>
                    </div>
                    <div id="captured-photo" class="captured-photo" style="display: none;">
                        <h3>Sua foto capturada:</h3>
                        <img id="photo-preview" src="" alt="Foto capturada">
                        <button id="downloadPhotoBtn" class="button secondary">💾 Baixar Foto</button>
                    </div>
                </div>
            </div>

            <p class="message-text">
                Às vezes a vida nos presenteia com pessoas especiais. Que bom ter você como amiga! 
                Use a câmera acima para capturar um momento especial e clique no botão abaixo para celebrarmos juntos.
            </p>
        </section>

        <section class="action">
            <button id="revealBtn" class="button" aria-label="Celebrar">CELEBRAR NOSSA AMIZADE 🎉</button>
        </section>

        <footer>
            <p>Feito com ❤️ por Matheus Gustavo · Powered by ESP32</p>
        </footer>
    </div>

    <div id="modal" class="modal" role="dialog" aria-modal="true" aria-hidden="true">
        <div class="modal-content celebration">
            <button id="closeModal" class="close" aria-label="Fechar">×</button>
            <h2>🌟 Para você guardar essa lembrança</h2>
            
            <div id="card-preview" class="card-preview">
                <div class="card-front">
                    <h3>Amizade</h3>
                    <p class="quote">"A amizade duplica as alegrias e divide as tristezas."</p>
                    <p class="date">Novembro, 2025</p>
                </div>
            </div>

            <p class="big">Obrigado por fazer parte desta história!</p>
            
            <div class="modal-actions">
                <button id="downloadCard" class="button secondary">Baixar Cartão Digital</button>
                <button id="celebrate" class="button">Celebrar Novamente! 🎉</button>
            </div>
        </div>
    </div>

    <script>
    let streamActive = true;
    let streamImage = document.getElementById('stream');
    let cameraStatus = document.getElementById('camera-status');
    let statusText = cameraStatus.querySelector('.status-text');
    let statusIndicator = cameraStatus.querySelector('.status-indicator');
    let cameraLoading = document.getElementById('camera-loading');
    let toggleStreamBtn = document.getElementById('toggleStreamBtn');
    let captureBtn = document.getElementById('captureBtn');
    let capturedPhoto = document.getElementById('captured-photo');
    let photoPreview = document.getElementById('photo-preview');
    let downloadPhotoBtn = document.getElementById('downloadPhotoBtn');

    async function checkCameraStatus() {
        try {
            const response = await fetch('/status');
            const data = await response.json();
            if (data.camera === 'ok') {
                statusIndicator.classList.add('active');
                statusText.textContent = 'Câmera conectada';
                cameraLoading.style.display = 'none';
            }
        } catch (error) {
            console.error('Erro ao verificar status da câmera:', error);
            statusText.textContent = 'Câmera desconectada';
            statusIndicator.classList.remove('active');
        }
    }

    toggleStreamBtn.addEventListener('click', () => {
        streamActive = !streamActive;
        if (streamActive) {
            streamImage.src = '/stream?t=' + Date.now();
            toggleStreamBtn.textContent = '⏸️ Pausar';
            toggleStreamBtn.classList.remove('paused');
        } else {
            streamImage.src = '';
            toggleStreamBtn.textContent = '▶️ Iniciar';
            toggleStreamBtn.classList.add('paused');
        }
    });

    captureBtn.addEventListener('click', async () => {
        try {
            captureBtn.disabled = true;
            captureBtn.textContent = '📸 Capturando...';
            
            const response = await fetch('/capture?t=' + Date.now());
            if (response.ok) {
                const blob = await response.blob();
                const blobUrl = URL.createObjectURL(blob);
                photoPreview.src = blobUrl;
                capturedPhoto.style.display = 'block';
                capturedPhoto.scrollIntoView({ behavior: 'smooth', block: 'nearest' });
                
                // Store the blob for later use
                photoPreview.dataset.photoBlob = blobUrl;
                
                downloadPhotoBtn.onclick = async () => {
                    try {
                        // Re-fetch the photo to ensure we have fresh data
                        const downloadResponse = await fetch('/capture?t=' + Date.now());
                        if (downloadResponse.ok) {
                            const downloadBlob = await downloadResponse.blob();
                            const downloadUrl = URL.createObjectURL(downloadBlob);
                            const a = document.createElement('a');
                            a.href = downloadUrl;
                            a.download = 'foto-maria-cecilia-' + Date.now() + '.jpg';
                            document.body.appendChild(a);
                            a.click();
                            document.body.removeChild(a);
                            URL.revokeObjectURL(downloadUrl); // Clean up the URL
                        } else {
                            throw new Error('Falha ao baixar a foto');
                        }
                    } catch (error) {
                        console.error('Erro ao baixar a foto:', error);
                        alert('Erro ao baixar a foto. Tente novamente.');
                    }
                };
            } else {
                alert('Erro ao capturar foto. Tente novamente.');
            }
        } catch (error) {
            console.error('Erro ao capturar foto:', error);
            alert('Erro ao capturar foto. Verifique se a câmera está funcionando.');
        } finally {
            captureBtn.disabled = false;
            captureBtn.textContent = '📸 Capturar Foto';
        }
    });

    streamImage.addEventListener('error', () => {
        cameraLoading.style.display = 'flex';
        statusText.textContent = 'Erro ao carregar stream';
        statusIndicator.classList.remove('active');
    });

    streamImage.addEventListener('load', () => {
        cameraLoading.style.display = 'none';
        statusText.textContent = 'Câmera conectada';
        statusIndicator.classList.add('active');
    });

    checkCameraStatus();
    setInterval(checkCameraStatus, 5000);
    
    function playMelody() {
        try {
            const ctx = new (window.AudioContext || window.webkitAudioContext)();
            const notes = [261.63, 329.63, 392.00, 523.25];
            let t = ctx.currentTime;
            notes.forEach((freq, i) => {
                const o = ctx.createOscillator();
                const g = ctx.createGain();
                o.type = 'sine';
                o.frequency.value = freq;
                g.gain.value = 0.0001;
                o.connect(g);
                g.connect(ctx.destination);
                g.gain.setValueAtTime(0.0001, t);
                g.gain.exponentialRampToValueAtTime(0.1, t + 0.02);
                g.gain.exponentialRampToValueAtTime(0.0001, t + 0.5);
                o.start(t);
                o.stop(t + 0.6);
                t += 0.2;
            });
        } catch (e) {
            console.log('WebAudio não disponível', e);
        }
    }

    function launchConfetti() {
        const canvas = document.getElementById('confetti-canvas');
        const ctx = canvas.getContext('2d');
        let W = canvas.width = window.innerWidth;
        let H = canvas.height = window.innerHeight;

        window.addEventListener('resize', () => {
            W = canvas.width = window.innerWidth;
            H = canvas.height = window.innerHeight;
        });

        const colors = ['#0066cc','#4da6ff','#80c1ff','#b3d9ff','#e6f2ff'];
        const pieces = Array(150).fill().map(() => ({
            x: Math.random() * W,
            y: Math.random() * H - H,
            r: Math.random() * 10 + 5,
            d: Math.random() * 16 - 8,
            color: colors[Math.floor(Math.random() * colors.length)],
            tilt: Math.random() * 10 - 5,
            opacity: 1
        }));

        let confettiActive = true;
        function update() {
            if (!confettiActive) return;
            
            ctx.clearRect(0, 0, W, H);
            pieces.forEach(p => {
                p.y += 3;
                p.tilt = Math.sin(p.d) * 15;
                p.x += Math.sin(p.d);
                p.opacity -= 0.001;
                ctx.fillStyle = p.color;
                ctx.globalAlpha = p.opacity;
                ctx.beginPath();
                ctx.ellipse(p.x, p.y, p.r, p.r * 0.6, p.tilt, 0, Math.PI * 2);
                ctx.fill();

                if (p.y > H || p.opacity <= 0) {
                    p.y = -20;
                    p.opacity = 1;
                }
            });
            requestAnimationFrame(update);
        }

        confettiActive = true;
        update();

        setTimeout(() => {
            confettiActive = false;
        }, 8000);
    }

    function generateCard() {
        const canvas = document.getElementById('card-canvas');
        const ctx = canvas.getContext('2d');
        canvas.width = 600;
        canvas.height = 400;

        const gradient = ctx.createLinearGradient(0, 0, 600, 400);
        gradient.addColorStop(0, '#0066cc');
        gradient.addColorStop(1, '#b3d9ff');
        ctx.fillStyle = gradient;
        ctx.fillRect(0, 0, 600, 400);

        ctx.fillStyle = 'rgba(255,255,255,0.1)';
        for(let i = 0; i < 5; i++) {
            ctx.beginPath();
            ctx.arc(
                Math.random() * 600,
                Math.random() * 400,
                Math.random() * 100 + 50,
                0,
                Math.PI * 2
            );
            ctx.fill();
        }

        ctx.textAlign = 'center';
        ctx.fillStyle = 'white';
        ctx.shadowColor = 'rgba(0,0,0,0.2)';
        ctx.shadowBlur = 10;
        
        ctx.font = 'bold 40px Arial';
        ctx.fillText('Amizade', 300, 100);

        ctx.shadowBlur = 5;
        ctx.font = '20px Arial';
        ctx.fillText('"A amizade duplica as alegrias', 300, 180);
        ctx.fillText('e divide as tristezas."', 300, 210);

        ctx.font = 'italic 16px Arial';
        ctx.fillText('Novembro, 2025', 300, 350);

        return canvas.toDataURL('image/png');
    }

    document.getElementById('revealBtn').addEventListener('click', () => {
        modal.style.display = 'flex';
        modal.style.opacity = '0';
        setTimeout(() => {
            modal.style.opacity = '1';
        }, 10);
        playMelody();
        launchConfetti();
    });

    document.getElementById('celebrate').addEventListener('click', () => {
        playMelody();
        launchConfetti();
    });

    document.getElementById('downloadCard').addEventListener('click', () => {
        const link = document.createElement('a');
        link.download = 'cartao-amizade.png';
        link.href = generateCard();
        link.click();
    });

    document.getElementById('closeModal').addEventListener('click', () => {
        modal.style.opacity = '0';
        setTimeout(() => {
            modal.style.display = 'none';
        }, 300);
    });
    </script>
</body>
</html>
)rawliteral";

void handleRoot(){
  // Serve HTML embutido diretamente do código
  server.send_P(200, "text/html", HTML_PAGE);
}

// Handler para streaming de vídeo
void handleStream() {
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  server.sendContent(response);

  while (true) {
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Falha na captura da câmera");
      break;
    }

    response = "--frame\r\n";
    response += "Content-Type: image/jpeg\r\n\r\n";
    server.sendContent(response);
    server.sendContent_P((char *)fb->buf, fb->len);
    server.sendContent("\r\n");
    
    esp_camera_fb_return(fb);
    fb = NULL;
    
    delay(100); // Ajuste a taxa de frames (10 FPS)
  }
}

// Handler para captura de foto única
void handleCapture() {
  fb = esp_camera_fb_get();
  if (!fb) {
    server.send(500, "text/plain", "Falha na captura da câmera");
    return;
  }

  server.sendHeader("Content-Disposition", "inline; filename=capture.jpg");
  server.send_P(200, "image/jpeg", (const char *)fb->buf, fb->len);
  esp_camera_fb_return(fb);
  fb = NULL;
}

// Handler para status da câmera
void handleStatus() {
  String json = "{";
  json += "\"camera\":\"ok\",";
  json += "\"resolution\":\"" + String(fb ? "captured" : "ready") + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

void handleNotFound(){
  // Redireciona tudo para a página principal (comportamento de portal cativo)
  // Como HTML e CSS estão embutidos, não precisa verificar SPIFFS
  String uri = server.uri();
  if(uri == "/") { 
    handleRoot(); 
    return; 
  }
  // Redireciona tudo para a página principal
  handleRoot();
}

void setup(){
  Serial.begin(115200);
  delay(1000);

  // Configuração da câmera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // Configuração de qualidade baseada na memória disponível
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA; // 640x480
    config.jpeg_quality = 10; // 0-63, menor = melhor qualidade
    config.fb_count = 2;
    Serial.println("PSRAM encontrado - usando resolução VGA");
  } else {
    config.frame_size = FRAMESIZE_CIF; // 400x296 - menor resolução sem PSRAM
    config.jpeg_quality = 12;
    config.fb_count = 1;
    Serial.println("PSRAM não encontrado - usando resolução CIF");
  }

  // Inicializa a câmera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Falha na inicialização da câmera com erro 0x%x", err);
    return;
  }
  Serial.println("Câmera inicializada com sucesso!");

  // SPIFFS não é mais necessário - HTML e CSS estão embutidos no código
  Serial.println("HTML e CSS embutidos no codigo - SPIFFS nao necessario");

  // Configura o IP estático para o Access Point
  WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));

  // Inicia o Access Point com senha
  WiFi.softAP(ssid, password);
  Serial.print("Access Point Iniciado: ");
  Serial.println(ssid);
  Serial.print("Senha: ");
  Serial.println(password);
  Serial.print("IP do AP: ");
  Serial.println(WiFi.softAPIP());

  // Inicia o Servidor DNS na porta 53, redirecionando tudo para o IP do AP (192.168.4.1)
  dnsServer.start(53, "*", apIP);

  // Rotas do servidor
  server.on("/", HTTP_GET, handleRoot);
  server.on("/stream", HTTP_GET, handleStream);
  server.on("/capture", HTTP_GET, handleCapture);
  server.on("/status", HTTP_GET, handleStatus);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Servidor Web Iniciado");
  Serial.println("Acesse: http://192.168.4.1");
}

void loop(){
  dnsServer.processNextRequest();
  server.handleClient();
}