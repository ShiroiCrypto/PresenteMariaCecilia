# 📤 Guia de Upload dos Arquivos para o SPIFFS do ESP32CAM

Este guia explica como fazer upload dos arquivos HTML e CSS para o sistema de arquivos SPIFFS do ESP32CAM usando a **versão mais recente do Arduino IDE (2.x)**.

## 📋 Pré-requisitos

1. **Arduino IDE 2.x** (versão mais recente) instalado
2. **ESP32 Board Manager** instalado no Arduino IDE
3. **Plugin SPIFFS Data Upload** (já incluído no Arduino IDE 2.x)

## 🔧 Instalação do ESP32 Board Manager (Arduino IDE 2.x)

1. Abra o **Arduino IDE 2.x**
2. Vá em **Arquivo → Preferências** (ou pressione `Ctrl+,`)
3. Na seção **Gerenciadores de Placas Adicionais (URLs)**, clique no ícone de **+** e adicione:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Clique em **OK**
5. Vá em **Ferramentas → Placa → Gerenciador de Placas**
6. Procure por **"esp32"** e instale a versão mais recente (clique em **Instalar**)
7. Aguarde a instalação completar

**Nota:** O plugin **ESP32 Sketch Data Upload** já vem incluído no Arduino IDE 2.x, não é necessário instalar separadamente!

## 📁 Estrutura de Pastas

**IMPORTANTE:** A pasta `data` deve estar **dentro** da pasta do sketch (`presente_esp32`).

Certifique-se de que a estrutura está assim:

```
codigo_esp32/
└── presente_esp32/
    ├── data/
    │   ├── index.html
    │   └── style.css
    └── presente_esp32.ino
```

✅ **Estrutura Correta:** A pasta `data` está dentro de `presente_esp32`  
❌ **Estrutura Incorreta:** A pasta `data` está no mesmo nível de `presente_esp32`

## 🚀 Passo a Passo para Upload

### 1. Preparar os Arquivos

Certifique-se de que todos os arquivos estão na pasta `data/`:
- ✅ `index.html`
- ✅ `style.css`

### 2. Abrir o Sketch no Arduino IDE 2.x

1. Abra o **Arduino IDE 2.x**
2. Vá em **Arquivo → Abrir** (ou pressione `Ctrl+O`)
3. Navegue até `codigo_esp32/presente_esp32/presente_esp32.ino`
4. Abra o arquivo

### 3. Configurar a Placa (Arduino IDE 2.x)

1. No painel lateral direito, clique em **"Selecionar outra placa e porta"** ou vá em **Ferramentas → Placa**
2. Selecione **ESP32 Arduino → AI Thinker ESP32-CAM** (ou **ESP32 Wrover Module**)
3. Configure as opções no painel lateral direito:
   - **Porta:** Selecione a porta COM do seu ESP32CAM
   - **Upload Speed:** 115200
   - **CPU Frequency:** 240MHz (WiFi/BT)
   - **Flash Frequency:** 80MHz
   - **Flash Mode:** QIO
   - **Flash Size:** 4MB (32Mb)
   - **Partition Scheme:** **Default 4MB with spiffs** ⚠️ **IMPORTANTE!**
   - **PSRAM:** Enabled (se disponível)
   - **Core Debug Level:** Nenhum (ou Info)

### 4. Fazer Upload do Código

1. Conecte o ESP32CAM ao computador via USB
2. Verifique se a porta COM está selecionada corretamente
3. Clique no botão **Upload** (seta →) na barra de ferramentas superior
   - Ou pressione `Ctrl+U`
   - Ou vá em **Sketch → Upload**
4. Aguarde a compilação e upload
5. Você verá mensagens de progresso na parte inferior do IDE

### 5. Fazer Upload dos Arquivos para SPIFFS (Arduino IDE 2.x)

1. **Após o upload do código**, vá em **Ferramentas → ESP32 Sketch Data Upload**
   - Ou use o atalho: procure no menu **Ferramentas** pela opção **ESP32 Sketch Data Upload**
2. O Arduino IDE 2.x irá:
   - Procurar automaticamente a pasta `data` dentro de `presente_esp32`
   - Fazer upload de todos os arquivos da pasta `data/` para o SPIFFS
3. Aguarde o processo de upload (pode levar alguns segundos)
4. Você verá mensagens no **Monitor Serial** indicando o progresso
5. Quando concluir, você verá uma mensagem de sucesso

**IMPORTANTE:** 
- O ESP32 precisa estar conectado via USB
- O upload do SPIFFS deve ser feito **após** o upload do código
- Se você modificar os arquivos HTML/CSS, faça upload novamente do SPIFFS

## 🔍 Verificação

Após o upload, abra o **Monitor Serial** no Arduino IDE 2.x:

1. Clique no ícone **Monitor Serial** na barra de ferramentas superior (ícone de lupa)
   - Ou pressione `Ctrl+Shift+M`
   - Ou vá em **Ferramentas → Monitor Serial**
2. Configure a velocidade: **115200 baud**
3. Você deve ver as seguintes mensagens:

```
SPIFFS montado com sucesso
Câmera inicializada com sucesso!
Access Point Iniciado: Presente Para Maria Cecilia
Senha: matheus123
IP do AP: 192.168.4.1
Servidor Web Iniciado
Acesse: http://192.168.4.1
```

✅ Se você ver essas mensagens, tudo está funcionando corretamente!

## 📱 Testar o Site

1. Conecte seu celular/computador à rede Wi-Fi: **"Presente Para Maria Cecilia"**
2. Digite a senha: **matheus123**
3. Abra o navegador e acesse: **http://192.168.4.1**
4. Você deve ver o site funcionando com streaming de vídeo!

## ⚠️ Problemas Comuns

### Erro: "SPIFFS montado com sucesso" mas página não carrega
- **Solução:** Verifique se os arquivos foram enviados corretamente. Faça upload novamente do SPIFFS.

### Erro: "Falha ao montar SPIFFS"
- **Solução:** 
  1. Verifique se o Partition Scheme está configurado para incluir SPIFFS
  2. Tente formatar o SPIFFS: **Ferramentas → ESP32 Sketch Data Upload → Erase Flash**

### Arquivos não aparecem
- **Solução:** Certifique-se de que a pasta `data` está **dentro** da pasta do sketch (`presente_esp32`), não no mesmo nível
- Verifique a estrutura: `presente_esp32/data/index.html` e `presente_esp32/data/style.css`

### Streaming de vídeo não funciona
- **Solução:** 
  1. Verifique se a câmera está conectada corretamente
  2. Verifique as mensagens no Serial Monitor para erros da câmera
  3. Certifique-se de que o ESP32CAM está selecionado como placa

## 🛠️ Solução de Problemas no Arduino IDE 2.x

### Não encontro a opção "ESP32 Sketch Data Upload"
- **Solução:** 
  1. Certifique-se de que o ESP32 Board Manager está instalado
  2. Certifique-se de que uma placa ESP32 está selecionada
  3. A opção só aparece quando uma placa ESP32 está selecionada

### Erro ao fazer upload do SPIFFS
- **Solução:**
  1. Certifique-se de que o **Partition Scheme** está configurado como **"Default 4MB with spiffs"**
  2. Tente fazer upload do código novamente primeiro
  3. Depois faça upload do SPIFFS

### A pasta data não é encontrada
- **Solução:**
  1. Certifique-se de que a pasta `data` está **dentro** de `presente_esp32`
  2. A estrutura deve ser: `presente_esp32/data/index.html`
  3. Feche e reabra o sketch no Arduino IDE

## 📝 Notas Importantes

- O tamanho máximo do SPIFFS depende da configuração do Partition Scheme
- Arquivos grandes podem demorar para fazer upload
- Sempre faça upload do código primeiro, depois do SPIFFS
- Se modificar HTML/CSS, faça upload novamente apenas do SPIFFS

---

**Boa sorte com seu projeto! 🎉**

