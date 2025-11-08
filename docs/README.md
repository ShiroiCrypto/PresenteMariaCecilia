# 🎁 Presente para Maria Cecília — Projeto ESP32 (Portal Cativo)

Um projeto simples e personalizável que usa um ESP32 para criar uma rede Wi‑Fi temporária (portal cativo) e exibir uma página de presente personalizada quando alguém se conecta.

Este repositório contém o código do ESP32 e arquivos estáticos (HTML/CSS) usados como a mensagem/página do presente.

## ✨ Recursos

- Rede Wi‑Fi criada pelo ESP32 contendo uma página de boas-vindas personalizada.
- Conteúdo servível a partir de `codigo_esp32/data/index.html` (HTML/CSS) ou embutido no sketch.
- Fácil personalização da mensagem, estilo e funcionalidades (animações, áudio, imagens).

## Estrutura do repositório

- `codigo_esp32/` — código para o ESP32 (sketch(es) e pasta `data` com arquivos estáticos).
    - `codigo_esp32/presente_esp32.ino` — sketch principal (nome pode variar).
    - `codigo_esp32/data/index.html` — HTML usado como página do presente (pode ser servido via LittleFS/SPIFFS).
- `docs/README.md` — este arquivo de documentação.

> Observação: os nomes de arquivos/paths podem variar; ajuste conforme o que estiver no seu projeto.

## Requisitos

- Placa ESP32 (qualquer modelo compatível).
- Arduino IDE (com suporte ao ESP32) ou VS Code + PlatformIO.
- Biblioteca padrão do ESP32: `WiFi.h`, `WebServer.h`, `DNSServer.h` (normalmente inclusas).

## Como personalizar a mensagem

1. Se o projeto usa `data/index.html` (recomendado): edite `codigo_esp32/data/index.html` com o conteúdo que você quiser (texto, imagens, CSS, JS).
2. Se o HTML estiver embutido no sketch (`.ino`), edite a variável que contém o HTML (ex.: `const char* html = R"rawliteral(... )rawliteral";`).
3. Para imagens maiores, prefira servir arquivos estáticos via LittleFS/SPIFFS em vez de base64 embutido.
4. Altere o SSID da rede no sketch, se quiser um nome de Wi‑Fi diferente (procure por `ssid` ou `Presente`).

## Como testar localmente (rápido)

Se quiser ver o HTML no seu computador antes de gravar no ESP32, abra `codigo_esp32/data/index.html` no navegador ou execute um servidor HTTP simples na pasta `codigo_esp32/data`.

Exemplo usando Python (no Windows PowerShell):

```powershell
cd codigo_esp32/data
python -m http.server 8000
```

Abra http://localhost:8000 no navegador para ver a página.

## Como carregar no ESP32

Opção A — Arduino IDE

1. Abra o sketch (`codigo_esp32/presente_esp32.ino`).
2. Se usar LittleFS/SPIFFS, combine com o upload dos arquivos estáticos conforme sua rotina (plugin de upload de filesystem ou PlatformIO).
3. Selecione a placa ESP32 correta e a porta COM.
4. Faça upload.

Opção B — PlatformIO (VS Code)

1. Abra o projeto no VS Code com PlatformIO.
2. Configure a placa no `platformio.ini`.
3. Faça upload (e, se necessário, faça upload do sistema de arquivos para os arquivos estáticos).

## Dicas e boas práticas

- Teste a página localmente antes de gravar no ESP32.
- Mantenha HTML/CSS simples para reduzir uso de memória no dispositivo.
- Se usar áudio ou imagens grandes, hospede-os externamente ou use LittleFS.
- Ajuste o comportamento do portal cativo com cuidado (DNS + WebServer) para garantir redirecionamento confiável.

## Exemplo rápido de personalização

- Editar o título da rede (ssid): abra o sketch e procure por `ssid`.
- Trocar o texto de boas‑vindas: edite `index.html` ou a string HTML no sketch.

## Licença

Este projeto está licenciado sob a licença MIT. O arquivo completo da licença está disponível em `../LICENSE` (arquivo `LICENSE` na raiz do repositório).

Resumo rápido:

- Copyright (c) 2025 Shiroi | Crypto
- Permissão é concedida gratuitamente para usar, copiar, modificar e distribuir o software, contanto que o aviso de copyright e esta permissão sejam incluídos.

Se você preferir outra licença ou quiser incluir termos específicos para uso pessoal/comercial, me avise que eu atualizo o `LICENSE` e o README.

---

Se quiser, eu posso:

1. Gerar um HTML de exemplo bem bonito (com estilo, fotos e animação) pronto em `codigo_esp32/data/index.html`.
2. Ajudar a configurar o upload de arquivos estáticos para LittleFS/SPIFFS no Arduino IDE ou PlatformIO.
3. Personalizar a mensagem para a Maria Cecília — se você me passar nome, fotos (ou links), cores e tom da mensagem.

Diga qual dos itens acima você quer que eu faça em seguida que eu cuido do resto.