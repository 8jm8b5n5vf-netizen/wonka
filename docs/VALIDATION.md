# WONKA 0.2.0 — validação de 28/10/2026

## Executado nesta revisão

Linux x86-64, GCC 14.2, CMake 4.4.4, JUCE 8.0.6 no commit `51a8a6d7aeae7326956d747737ccf1575e61e209`. Dependências locais Debian verificadas, TLS/checksums preservados.

- **Build Release VST3 Linux concluído**, incluindo wrapper e `moduleinfo.json`. `ldd` não encontrou bibliotecas ausentes. Isso não constitui validação do wrapper dentro de uma DAW.
- **CTest: 2 suítes passaram**, nenhuma falhou. A execução padrão sem DISPLAY não exercita as etapas gráficas condicionais.
- **DSP: 43 verificações passaram.** Duração, seleção/fades, inversão temporal de um marcador conhecido, crescimento de Swell, canais, cauda, 3/4 e 3/8, timing pontuado/tercinado e em BPMs diferentes, alternância dos ecos, estabilidade do feedback sem depender da proteção de clipping, resposta real de Space/Length/Tone/Shape/Tremolo/Bars, sinal estéreo em oposição de fase, resampling/anti-aliasing, amostras inválidas e normalização explícita. A opção fonte + efeito preserva a fonte seca antes do primeiro eco.
- **Integração sem display: 84 verificações passaram.** WAV mono/AIFF estéreo; sample rates/duração/canais; render no worker; sessão com áudio idêntico após apagar a fonte original; prévia e fim da reprodução; PCM24/float32 com leitura de volta; captura estéreo; parada exatamente aos 60 s e aviso; bypass; BPM/compasso por playhead simulado e overrides manuais.
- Os 17 presets estabeleceram baseline sem indicação de alteração e preservaram a seleção. Alterar um parâmetro marcou o preset como modificado.
- Alterações invalidaram o render imediatamente, antes do debounce. Tentativas de exportar/ouvir/arrastar um resultado antigo foram recusadas. Falha ao importar um novo arquivo invalidou o resultado anterior, e o timer não o recriou silenciosamente a partir da fonte antiga.
- WAV float32 preparado para arraste foi lido e teve suas amostras comparadas integralmente ao buffer renderizado, na mesma sample rate. A prévia em ganho unitário foi comparada por canal ao buffer depois das rampas explícitas de ganho, ao longo do restante do trecho; tolerância 1e-6.
- Cache foi criado antes do gesto de drag, validado e permaneceu no disco após substituir o estado da sessão. Não há rotina de limpeza automática dos WAVs usados pela DAW.
- **Probe de heap Linux:** interceptações de new/malloc/calloc/realloc/free não detectaram operações de heap no callback de prévia ensaiado. Isso não é uma medição exaustiva de todos os caminhos do host, de alocadores externos ou de todas as situações de stress.
- **Execução adicional com Xvfb: 223 verificações no executável de integração passaram.** Esse total inclui a repetição das verificações comuns e etapas gráficas, não representa 223 testes adicionais independentes. Verificou editor 360 × 640, alvo direto do display e fallback de file drop do editor, file URLs, erro explícito para conteúdo não suportado, rótulo de preset modificado, independência das quatro abas, escalas 70/85/100/125 com um único transform e controles dentro dos limites, painel técnico recolhível e restauração da largura/tamanho. Gerou imagens atuais dos dois painéis.
- Scripts macOS passaram em `bash -n` (sintaxe); não foram executados no macOS nesta revisão. O script de build faz detecção local da arquitetura e solicita binários universais arm64/x86_64, como na compilação anterior do usuário.

Sinais de teste são sintéticos. Não foi feita avaliação auditiva com voz real, dispositivo físico ou comparação sonora A/B com Boomerang. Os vídeos foram usados para analisar o fluxo/organização/proporção, não para inferir um algoritmo proprietário.

## Pendente de teste no ambiente de destino

- **Compilar 0.2.0 no Mac e abrir essa versão no FL Studio.** O log fornecido pelo usuário comprovou build VST3/AU e dois testes da versão anterior, e o usuário relatou sua abertura no FL. Isso não comprova funcionamento da revisão atual.
- Seletor nativo macOS (foco, callback e cancelamento no host), drop nativo Finder → display, regiões da playlist → plugin e waveform → playlist. Os testes locais chamaram os alvos JUCE com dados compatíveis; não simularam uma sessão real do FL/macOS.
- Transporte parado/Smart disable, espaço interceptado pelo host, roteamento de áudio, automação em projetos FLP, múltiplas instâncias, restauração real de sessão FLP e tela Retina física. O teste de prévia chamou processBlock sem exigir transporte em reprodução; o host ainda precisa continuar chamando o plugin.
- AU nativo, Windows, `auval`, pluginval, assinatura Developer ID, notarização, instaladores assinados e distribuição. A disponibilidade ou resultado dos jobs GitHub Actions não foi confirmado nesta revisão.
- PNG real da logo: **não fornecido**. A incorporação condicional como BinaryData e o espaço proporcional no cabeçalho foram preparados, mas não houve compilação/validação com o arquivo real.
- Stress prolongado, mudanças contínuas de tempo/rate, gravação interrompida por reconfiguração do host e pressão de memória em máquinas com pouca RAM.

## Limites documentados

- Importação/gravação até 60 s, mono/estéreo, 8–192 kHz. Duração principal/cauda Throw até 30 s; duração musical acima disso é limitada e indicada. Cauda Reverse pode acrescentar até 6 s. Reverb é cortado suavemente numa janela finita baseada no T60 estimado.
- Length no Reverse usa resampling e muda pitch/formantes; não é time-stretch com preservação de pitch. Interpolação da prévia entre rates é linear; render/WAV usam sinc com janela.
- Imagem estéreo do Throw é transformada intencionalmente: canal principal alterna e o segundo acompanha em ganho reduzido no lado oposto. Não se promete manter a panorâmica original do efeito. A fonte seca opcional preserva os canais.
- Não há reconhecimento de palavras/seleção automática no fluxo. A seleção é manual. Não há suporte genérico a formatos proprietários de regiões da DAW nem posicionamento automático na timeline.
- Normalização é opcional e desligada por padrão. Proteção é teto rígido, não limiter de mastering. Volume/rampas são da prévia e não alteram o WAV.
- Captura em andamento não é incorporada ao estado: pare e aguarde antes de salvar. Áudio incorporado aumenta o XML da sessão. Cache persistente precisa ser copiado/consolidado pela DAW para mover projetos entre máquinas; não há migração automática de WAVs temporários antigos da versão anterior.
- Renders paramétricos são atualizados em segundo plano, com breve debounce. A prévia é interrompida suavemente e precisa ser reiniciada; não existe morph entre renders nem DSP de delay/reverb contínuo na entrada ao vivo. É uma ferramenta de preparação/exportação de trechos vocais.

## Arquivos e evidências

- Projeto: `/workspace/wonka`.
- VST3 Linux: `build/WONKA_artefacts/Release/VST3/WONKA.vst3`.
- Testes: `build/volta_tests`, `build/volta_integration`; relatório CTest em `build/Testing/Temporary/LastTest.log`.
- Imagens: `docs/WONKA-preview.png`, `docs/WONKA-options.png`.
- Fontes empacotadas: `/workspace/WONKA-0.2.0-source.zip`. Não contém binários macOS.
- Atualização local: `scripts/build-mac.command`, depois `scripts/install-mac.command`. Confirme v0.2.0 no rodapé após o rescan do FL.
