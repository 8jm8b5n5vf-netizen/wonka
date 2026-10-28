# WONKA — vocal motion

Plugin de efeito C++17/JUCE 8.0.6 com identidade própria, para preparar transições vocais Reverse/Throw. Projeto independente: não usa marca, logotipo, recursos nem algoritmos do Boomerang. O áudio é processado de verdade; não há reconhecimento de palavras ou serviços externos.

**Destino do usuário: FL Studio no macOS.** VST3 e AU são configurados no Mac; Windows/Linux compilam VST3. Um binário Linux não pode ser instalado no Mac. Veja [validação e limitações](docs/VALIDATION.md) antes de usar em uma sessão importante.

## Interface WONKA

Painel grafite com textura de metal escovado, borda chanfrada e encaixes rebaixados. Knobs têm reflexos radiais de metal usinado, aro serrilhado, sombra projetada e marcações turquesa/violeta. Botões e menus recebem iluminação e relevo; o seletor Reverse/Throw usa dois botões. O acabamento é desenhado diretamente na interface JUCE e acompanha o redimensionamento.

## Compilar no seu Mac

1. Instale as ferramentas da Apple: abra o Terminal e execute `xcode-select --install`.
2. Instale [CMake 3.22 ou superior](https://cmake.org/download/). Se já usa Homebrew: `brew install cmake`.
3. Coloque a pasta deste projeto no Mac. No Terminal, entre nela e execute:

```sh
cmake -S . -B build-mac -DCMAKE_BUILD_TYPE=Release \
  '-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64' -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0
cmake --build build-mac --config Release --parallel 2
ctest --test-dir build-mac -C Release --output-on-failure
```

Alternativa: execute `scripts/build-mac.command` no Terminal. A primeira configuração baixa o JUCE oficial no commit fixado pelo CMake. Apple Silicon e Intel são solicitados na compilação universal; disponibilidade real depende do SDK instalado. O fluxo de CI em `.github/workflows/build.yml` também prepara builds macOS/Windows, mas só roda depois de enviar o projeto ao GitHub. Ele ainda não foi executado por esta entrega.

### Instalar no FL Studio no Mac

Feche o FL Studio. Depois de compilar, copie a **pasta inteira** `WONKA.vst3` de `build-mac/WONKA_artefacts/Release/VST3/` para `~/Library/Audio/Plug-Ins/VST3/`. Crie esse destino se necessário. Para AU, copie `AU/WONKA.component` para `~/Library/Audio/Plug-Ins/Components/`.

Abra o FL Studio, vá a **Options → Manage plugins** e faça uma nova busca (**Find installed plugins**). Adicione WONKA a um slot de efeito do Mixer. Os nomes dos menus podem variar por versão. O plugin precisa receber chamadas de áudio da DAW para prévia e gravação; se o host suspender plugins ociosos, mantenha a reprodução/monitoramento ativo.

Para distribuir a terceiros, trate licenciamento JUCE (AGPLv3/comercial conforme sua licença), assinatura Developer ID e notarização Apple. Não há assinatura nem notarização nesta entrega. Para validar AU no Mac: `auval -v aufx Wnk1 Wnka`; valide também no FL Studio e com pluginval antes de distribuir.

### Windows

Instale Visual Studio 2022 com **Desktop development with C++** e CMake. No Developer PowerShell:

```powershell
cmake -S . -B build-win -A x64
cmake --build build-win --config Release --parallel 2
ctest --test-dir build-win -C Release --output-on-failure
```

Copie `build-win/WONKA_artefacts/Release/VST3/WONKA.vst3` para `C:\Program Files\Common Files\VST3\` e faça a busca de plugins na DAW. AU é exclusivo do macOS.

### Linux / desenvolvimento na nuvem

Debian/Ubuntu: CMake, g++, make, pkg-config, libasound2-dev, libx11-dev, libxext-dev, libxinerama-dev, libxrandr-dev, libxcursor-dev, libfreetype-dev, libfontconfig1-dev e libgl1-mesa-dev. Não é necessário WebKit/CURL; ambos estão desativados no plugin.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
```

Na máquina da entrega, as bibliotecas foram extraídas de pacotes Debian verificados para `/workspace/tools/sysroot`, sem acesso de administrador. Ferramentas em `/workspace/tools/python`; JUCE em `/workspace/deps/JUCE`. Use:

```sh
. /workspace/tools/activate-volta.sh
cmake -S /workspace/wonka -B /workspace/wonka/build -DCMAKE_BUILD_TYPE=Release \
  -DFETCHCONTENT_SOURCE_DIR_JUCE=/workspace/deps/JUCE \
  -DCMAKE_PREFIX_PATH=/workspace/tools/sysroot/usr
cmake --build /workspace/wonka/build --parallel 2
ctest --test-dir /workspace/wonka/build --output-on-failure
```

Para testar apenas os algoritmos, sem JUCE nem bibliotecas gráficas:

```sh
cmake -S . -B build-core -DWONKA_PLUGIN=OFF
cmake --build build-core
ctest --test-dir build-core --output-on-failure
```

## Usar

- **Abrir voz** ou arrastar WAV/AIFF mono/estéreo. Limite: 60 segundos, 8–192 kHz. A seleção usa os dois marcadores na forma de onda da fonte. A fonte inteira é preservada na sessão.
- **Gravar** captura a entrada do slot de efeito, não abre diretamente o microfone do sistema. Roteie a voz ao Mixer, ative a reprodução/monitoramento e pare a gravação antes de salvar a sessão. A parada automática ocorre em 60 s. O áudio passa normalmente pela entrada, exceto durante a prévia.
- **Reverse / Line** ou **Swell**; **Throw** repete o trecho selecionado. **Sugerir final** procura a última região ativa pelo envelope/silêncio; revise os marcadores. Não identifica palavras com garantia.
- **Ouvir fonte / Ouvir resultado** escolhe a comparação e a forma de onda. **Ouvir / Parar** e espaço (quando o editor tem foco e o host entrega a tecla) controlam a prévia.
- **Space**, **Length**, abas **Tone / Shape / Tremolo / Bars**, feedback e divisão ajustam o render. Mudanças interrompem a prévia e disparam render em segundo plano após breve debounce. Os valores de cada aba são independentes.
- **Usar Bars** troca a autoridade da duração para Bars e desabilita Length; desligado, Length define segundos. A faixa atual é de 0,05 a 30 s para a duração principal/cauda Throw; durações musicais acima de 30 s são limitadas e o visor mostra a duração efetiva. BPM/compasso vêm do host quando disponíveis e habilitados, ou dos controles manuais. A indicação mostra segundos e compassos efetivos. O BPM suportado é 30–300.
- A aba Tremolo oferece 1/4, 1/8, 1/16 sincronizados, ou 4 Hz livre. A intensidade fica no knob menor. **Volume** altera só a prévia. **Bypass** preserva a entrada da DAW, interrompendo a prévia. **Proteger clipping** usa teto rígido de ±0,999 no efeito, prévia e WAV; não é um limiter transparente. **Normalizar** é opcional e fica desligado por padrão (pico alvo 0,98).
- Escolha 44,1/48/88,2/96 kHz e PCM24 ou float32. **Exportar WAV** abre o destino. **Arrastar WAV** é habilitado quando o arquivo do resultado terminou de ser gerado em segundo plano. Arraste o botão para a DAW; o host pode recusar. Use Exportar como alternativa. Formatos proprietários de regiões internas da DAW não são decodificados; file drops são suportados quando a DAW fornece caminhos reais WAV/AIFF.
- Para Reverse, alinhe **o fim do arquivo completo** ao início da voz original na playlist. A duração WAV no visor inclui a cauda escolhida. Nenhum posicionamento automático na timeline é prometido.

## Arquitetura e estado

- `Source/Engine.*`: DSP offline independente de JUCE; algoritmos e conversão sinc com janela.
- `Source/AudioIO.*`: importação, exportação atômica, picos da forma de onda e codificação do áudio da sessão.
- `Source/Plugin.*`: integração JUCE, parâmetros, captura pré-alocada, jobs serializados e publicação de buffers imutáveis com hazard pointer. A thread de áudio não lê arquivos, não renderiza efeitos, não aloca buffers nem usa mutex. Volume tem rampa de 20 ms. Os outros controles são aplicados a um novo render enquanto a prévia está parada.
- `Source/Editor.cpp`: interface JUCE redimensionável grafite/turquesa/violeta, knobs metálicos próprios, forma de onda real e drag/drop.
- `tests/`: regressão DSP e integração do processador/IO/persistência. Não substituem validação de DAW.

O estado da DAW incorpora o áudio fonte como PCM float em Base64, junto de modo, seleção, parâmetros e formato de exportação. Assim, excluir o arquivo original ou os temporários de drag não quebra a sessão. O resultado é reconstruído a partir da fonte e dos parâmetros ao reabrir. Isso aumenta o tamanho da sessão (até cerca de 123 MB em Base64 para 60 s estéreo a 192 kHz, além do XML); salvar pode custar tempo fora da thread de áudio. Capturas ainda em andamento não são incorporadas: pare e aguarde **Pronto** antes de salvar. Não há arquivos externos obrigatórios para relocalizar.

Os WAVs de arraste ficam no diretório temporário do sistema, subpasta `WONKA-drag`; podem ser limpos depois de importar/copiar o áudio para o projeto da DAW. Não são a estratégia de persistência da sessão.

[Algoritmos e parâmetros](docs/DSP.md) · [Validação e pendências](docs/VALIDATION.md)
