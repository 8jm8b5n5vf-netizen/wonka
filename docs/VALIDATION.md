# Validação e pendências — 28/10/2026

Atualização visual: nome WONKA no produto/builds/exportação e painel com relevo, acabamento metálico, knobs com iluminação radial procedural e controles em baixo/alto relevo.

## Executado nesta entrega

Ambiente: Linux x86-64, GCC 14.2, CMake 4.4.4, JUCE 8.0.6 no commit `51a8a6d7aeae7326956d747737ccf1575e61e209`. Dependências locais extraídas pelo dpkg de pacotes baixados com índices Debian assinados e verificação de checksums pelo APT; TLS preservado. Sem instalação de administrador.

- Configuração e compilação Release concluídas, incluindo wrapper VST3 e seu `moduleinfo.json`.
- Biblioteca VST3 Linux: `build/WONKA_artefacts/Release/VST3/WONKA.vst3/Contents/x86_64-linux/WONKA.so`. `ldd` não encontrou dependências ausentes.
- CTest: **2 suítes passaram**, nenhuma falhou ou foi pulada na execução sem display. A parte gráfica é condicional e fica fora dessa execução sem DISPLAY.
- `volta_tests`: **30 checks DSP passaram**. Abrange duração, fades, Line/Swell diferentes e não silenciosos, cauda, 3/4 e 3/8, pontuadas e tercinas, timing exato dos ecos em 60/120 BPM, ping-pong mono, feedback a 0,94, inclusão da fonte, conversão de sample rate, redução de aliasing, seleção, normalização explícita, fonte estéreo em oposição de fase e proteção contra NaNs.
- `volta_integration`: passou importação WAV mono/AIFF estéreo, render no worker, prévia até o fim, gravação estéreo, bypass, exportação WAV PCM24 e float32 a 44,1 kHz com leitura de volta dos canais/duração/formato. Sessão restaurada com parâmetros/seleção e áudio idêntico após apagar o WAV original. Bloco de áudio incorporado inválido rejeitado.
- Playhead simulado verificou BPM e compasso do host, fallback de BPM manual mantendo compasso disponível, e override manual de compasso. Isso não é validação em uma DAW real.
- Execução adicional do teste de integração com Xvfb verificou desenho real da interface e redimensionamento. Imagem gerada em `build/WONKA-preview.png` e copiada para `docs/WONKA-preview.png`. O teste substitui o arquivo anterior para não reutilizar evidência antiga.
- O script de configuração/compilação/testes salvo no ambiente foi executado com sucesso. Não existe serviço de backend a iniciar.

Os testes usam sinais sintéticos, não uma avaliação auditiva com gravações vocais reais. A prévia foi exercitada por chamadas do processador nos testes, sem dispositivo de áudio físico. Não houve medição automática de todas as alocações/locks da thread de áudio; a separação foi verificada por revisão do código e testes funcionais. A gravação funciona quando o host entrega blocos; não basta abrir a janela do editor.

## Ainda não executado

- Compilar VST3/AU no macOS (Intel ou Apple Silicon) e VST3 no Windows. O script macOS e a matriz de CI foram fornecidos, mas não executados nestes sistemas.
- Abrir, escanear e usar WONKA no FL Studio, testar arraste externo de/para a playlist, teclado espaço sob interceptação do host, automação em sessões reais e reabertura de projetos FLP. Não há alegação de compatibilidade confirmada com FL Studio.
- `auval`, pluginval, assinatura Developer ID, notarização, instaladores e testes de distribuição.
- Testes prolongados de stress em todas as sample rates, automação contínua, várias instâncias e pressão de memória. Validação sonora subjetiva com vocais reais.

## Limitações de projeto

- O Reverse ajusta duração por resampling, alterando também pitch/formantes. Não há algoritmo de time-stretch com preservação de pitch.
- Entrada de até 60 s, 1/2 canais, 8–192 kHz. Duração principal/cauda Throw de até 30 s; a cauda Reverse opcional pode adicionar até 6 s. O visor separa duração principal e WAV total. Reverb é truncado suavemente em uma janela finita baseada no T60 estimado.
- A imagem estéreo do Throw é transformada intencionalmente; canais de entrada não são descartados ou somados com cancelamento de fase, mas o resultado não mantém a panorâmica original.
- Na seleção automática há apenas análise de envelope/silêncio; revise os marcadores. A seleção salva da sessão tem prioridade na restauração.
- Só caminhos reais WAV/AIFF são importados por drag/drop. Não há suporte genérico a regiões proprietárias das DAWs nem posicionamento automático na timeline.
- Arquivos de arraste são temporários. A sessão preserva o áudio fonte incorporado, não os temporários; pare a gravação e aguarde o render antes de salvar. Sessões podem ficar grandes.
- Presets iniciais estão incluídos; não há navegador de presets externos do usuário. Parâmetros e escolhas de interface/exportação ficam no estado do host.
- Proteção é clipping rígido opcional, não um limiter de mastering. Não há normalização automática por padrão.

## Artefatos

- Código e instruções: `/workspace/wonka`.
- Pacote de fontes: `/workspace/WONKA-source.zip`.
- VST3 Linux compilado: `/workspace/wonka/build/WONKA_artefacts/Release/VST3/WONKA.vst3`.
- Preview: `/workspace/wonka/docs/WONKA-preview.png`.
- Nenhum binário macOS ou Windows foi gerado nesta máquina Linux.
