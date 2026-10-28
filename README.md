# WONKA 0.2.2 — vocal transitions

Plugin C++17/JUCE 8.0.6 de efeito para **carregar voz → Reverse/Throw → ajustar → ouvir → arrastar à DAW**. Interface compacta de **360 × 640 pontos lógicos**, própria, inspirada na organização das referências. Não usa imagens, logo, marca ou algoritmos proprietários do Boomerang, nem promete som idêntico.

**Destino: VST3 no FL Studio/macOS.** AU também é configurado no Mac; Windows/Linux têm VST3. Consulte [o que realmente foi testado](docs/VALIDATION.md) e [a revisão das falhas](docs/REVIEW-0.2.md). A versão aparece no rodapé. A versão branca 4K da logo do usuário está incorporada ao plugin; veja [Assets](Assets/README.md).

## Atualizar no Mac, passo a passo

1. Salve a sessão e **feche o FL Studio**.
2. Baixe o ZIP atualizado do repositório e descompacte. Use os scripts **dessa nova pasta**, não os da versão anterior.
3. Abra o Terminal. Digite `bash`, dê um espaço, arraste **scripts/build-mac.command** para o Terminal e aperte Enter.
4. O script procura o CMake em Aplicativos, identifica o Mac (inclusive Apple Silicon com Terminal em Rosetta) e solicita um build **universal arm64 + x86_64**, cobrindo o FL nativo ou em Rosetta. Isso mantém as arquiteturas que compilaram na versão anterior. Aguarde a compilação e os testes. Pare se houver erro.
5. Com o FL fechado, digite `bash`, espaço, arraste **scripts/install-mac.command** e aperte Enter. Ele instala o VST3 no usuário atual, sem sudo, preservando a versão anterior em backup.
6. Abra o FL. Em **Options → Manage plugins**, ative a verificação e a nova busca dos plugins já verificados (**Verify plugins / Rescan previously verified plugins**, conforme versão), então **Find installed plugins**.
7. Abra uma **nova instância** de WONKA no Mixer e confirme **v0.2.2** no rodapé. Se ainda mostrar a versão antiga, feche/reabra o FL e confira a pasta instalada antes de recompilar. Não altere o identificador do plugin nem apague seu projeto para forçar a busca.

Pré-requisitos, caso faltem: `xcode-select --install` e [CMake 3.22+](https://cmake.org/download/), instalado como `/Applications/CMake.app`. Internet é necessária para baixar o JUCE na primeira configuração.

O VST3 fica em `build-mac/WONKA_artefacts/Release/VST3/WONKA.vst3`. O instalador copia o **bundle inteiro** para `~/Library/Audio/Plug-Ins/VST3/`. O AU fica em `AU/WONKA.component`, para cópia opcional a `~/Library/Audio/Plug-Ins/Components/`. Não se abre o bundle com dois cliques: ele é carregado pela DAW.

A identidade foi preservada: bundle `audio.wonka.effect`, fabricante `Wnka`, plugin `Wnk1`. Apenas a versão mudou para 0.2.2. A compilação local usa a assinatura ad-hoc provida pelo JUCE quando aplicável; não há Developer ID/notarização de distribuição. Para validar AU no Mac: `auval -v aufx Wnk1 Wnka`.

## Primeiro teste no FL Studio

1. No display, **clique para abrir** um WAV/AIFF local curto, ou arraste um arquivo do **Finder** ao display. A fonte deve aparecer com nome, duração e forma de onda, inclusive enquanto o render é preparado. A faixa animada indica progresso indeterminado durante leitura/render. Falhas aparecem no display; passe o mouse no status para ler a causa completa.
2. Escolha Reverse ou Throw. Use **Vocal** para ouvir a fonte e editar os dois marcadores. Desligue Vocal para voltar ao resultado. Clicar no corpo do display abre o navegador; arrastar os marcadores altera a seleção.
3. Aperte o botão de reprodução no canto inferior esquerdo do display. Espaço funciona apenas quando o plugin tem foco e o host entrega a tecla. A prévia sai pelo próprio slot do Mixer; confira envio ao Master e volume.
4. Se o transporte parado fizer o host suspender o plugin, desative **Smart disable** no wrapper para esse teste, ou mantenha o transporte ligado. Os nomes dos controles variam por versão do FL. WONKA não força o host a chamar `processBlock`. O status avisa quando não observa chamadas de áudio durante a prévia.
5. Quando o render e seu WAV estiverem preparados, **arraste o corpo da forma de onda do resultado** para a playlist. Se o WAV ainda estiver sendo preparado, o status avisa; solte e tente novamente quando pronto. O arquivo é gerado no worker, nunca durante o gesto nem na thread de áudio.
6. Se o host recusar o arraste, abra **... → Salvar WAV** e importe esse WAV na DAW. Para Reverse, alinhe **o fim do arquivo completo** ao início da voz original. Não há posicionamento automático.

Arrastar regiões proprietárias diretamente da playlist depende do que o FL disponibiliza ao plugin. WONKA aceita caminhos reais, file URLs e descrições JUCE compatíveis que apontem para WAV/AIFF. Não decodifica formatos internos desconhecidos de regiões. Nenhum teste desta revisão comprova um arraste direto de região da playlist do FL/macOS.

## Controles e duração

- **Space** ajusta tamanho/difusão/decaimento do reverb; o valor é o T60 estimado em segundos. **Length** controla a duração principal do Reverse, ou a duração da cauda no Throw, sem alterar o intervalo dos ecos.
- **s / bars**, sob Length, escolhe a autoridade: segundos de Length **ou** compassos de Bars. Também há **Usar Bars** nas opções. Quando Bars está ativo, Length fica desabilitado. Nunca são multiplicados entre si.
- Abas **Tone / Shape / Tremolo / Bars** preservam valores separados. Tone mostra o cutoff final em Hz/kHz; Shape mostra o expoente do crescimento; Tremolo mostra porcentagem; Bars mostra compassos. A velocidade do Tremolo fica nas opções.
- O status mostra duração total efetiva, equivalente em compassos e BPM. BPM/compasso vêm do host quando habilitados e disponíveis, com fallback manual independente. A duração principal/cauda Throw é limitada a 0,05–30 s. Cauda Reverse opcional adiciona a janela finita de reverb (até 6 s); o status inclui esse acréscimo.
- Alterações relevantes interrompem suavemente a prévia, invalidam o resultado anterior imediatamente e disparam render em segundo plano após breve debounce. É necessário apertar play novamente. Exportação e arraste recusam resultados que não correspondem às configurações atuais.
- **17 presets próprios** com anterior/próximo; `*` no nome indica parâmetros alterados. Presets não substituem sua seleção, áudio fonte ou BPM/compasso manual.
- **...** recolhe/exibe as opções no espaço dos knobs, sem aumentar a janela: sincronização, compasso, delay/feedback, Tremolo rate, volume, cauda, fonte + efeito, normalização, clipping e WAV.
- Escalas **70%, 85%, 100%, 125%** redimensionam janela e conteúdo juntos. O tamanho é salvo na sessão. A escala do dispositivo Retina é gerida pelo JUCE, sem multiplicação manual adicional. Prefira 100%/125% se 70% ficar pequeno na sua tela.
- Medidores são picos reais de entrada/saída em dBFS; clique para limpar clipping. Volume altera apenas a prévia (mostrado em dB). Bypass preserva a entrada do host. Proteção opcional aplica teto rígido ±0,999; não é um limiter de mastering. Normalização opcional tem pico alvo 0,98 e permanece desligada por padrão.

## Gravação, arquivos e sessão

**REC** captura a entrada do plugin; não abre o microfone do sistema. Roteie a voz ao Mixer. Limite de **60 segundos** com contador/parada/aviso; pare e aguarde o processamento antes de salvar. Importação: WAV/AIFF, mono/estéreo, 8–192 kHz, até 60 s. O render usa conversão sinc com janela para a sample rate da sessão.

O estado da DAW incorpora o áudio fonte como PCM float em Base64, parâmetros, seleção, modo, variante, sincronização, opções, preset e largura da interface. Não depende do arquivo original ou do cache de arraste; a fonte pode ser apagada depois da importação e a sessão ainda a restaura. Captura ainda em andamento não é incorporada. O limite máximo pode acrescentar cerca de 123 MB de áudio Base64 ao XML da sessão; salvar demanda tempo fora do callback de áudio.

WAVs de arraste **não são temporários**. No Mac ficam em:

```text
~/Library/Application Support/WonkaAudio/WONKA/Rendered/
```

Windows usa AppData/WonkaAudio/WONKA/Rendered; Linux usa o diretório de configuração do usuário. Arquivos têm nomes únicos e não são apagados ao fechar o editor, trocar preset ou substituir a fonte. **Não limpe essa pasta se uma sessão da DAW ainda referenciar seus WAVs.** Para mover o projeto a outro computador, copie/consolide os samples pela DAW. Arquivos da versão anterior já arrastados a partir de diretórios temporários não são migrados automaticamente; consolide/copie esses samples na DAW. A pasta pode crescer; não há exclusão automática porque o plugin não sabe quando a DAW deixou de depender de um arquivo.

## Compilar em outros ambientes

Windows: Visual Studio 2022 com Desktop development with C++ e CMake:

```powershell
cmake -S . -B build-win -A x64
cmake --build build-win --config Release --parallel 2
ctest --test-dir build-win -C Release --output-on-failure
```

Linux/Debian: CMake, g++, make, pkg-config, libasound2-dev, libx11-dev, libxext-dev, libxinerama-dev, libxrandr-dev, libxcursor-dev, libfreetype-dev, libfontconfig1-dev, libgl1-mesa-dev. WebKit/CURL não são usados pelo plugin.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
```

Nesta máquina de desenvolvimento, dependências verificadas estão em `/workspace/tools/sysroot`, ferramentas em `/workspace/tools/python` e JUCE em `/workspace/deps/JUCE`:

```sh
. /workspace/tools/activate-volta.sh
cmake -S /workspace/wonka -B /workspace/wonka/build -DCMAKE_BUILD_TYPE=Release \
  -DFETCHCONTENT_SOURCE_DIR_JUCE=/workspace/deps/JUCE \
  -DCMAKE_PREFIX_PATH=/workspace/tools/sysroot/usr
cmake --build /workspace/wonka/build --parallel 2
ctest --test-dir /workspace/wonka/build --output-on-failure
```

Teste só DSP, sem JUCE: `cmake -S . -B build-core -DWONKA_PLUGIN=OFF`, depois build/ctest nesse diretório. O JUCE está fixado ao commit oficial `51a8a6d7aeae7326956d747737ccf1575e61e209`. Observe seu licenciamento AGPLv3/comercial antes de distribuir o produto. CI em `.github/workflows/build.yml` fornece os comandos macOS/Windows; resultado de CI não deve ser presumido.

[Algoritmos próprios](docs/DSP.md) · [Revisão e referências](docs/REVIEW-0.2.md) · [Validação e limites](docs/VALIDATION.md)
