# Revisão WONKA 0.2.0

## Referências examinadas

As nove imagens do pedido mostram a organização e a atmosfera; os três MP4 foram abertos, seus metadados verificados e quadros distribuídos por suas durações analisados:

- `cde52c306f4b44699478594180703d52.MP4`, 19,5 s: mostra importar voz, display, prévia e arrastar resultado à DAW.
- `1b78cb629c354126af7ee5e974410ac3.MP4`, 13,1 s: demonstra o plugin estreito dentro de uma DAW, deixando a sessão visível.
- `d02378d7a7934b49bca1033878f381a2.MP4`, 15,8 s: mostra ajustes, Tone com frequência, display e organização dos knobs/abas.

São referências de fluxo e proporção, não especificações de código/DSP. Os 17 presets foram solicitados pelo usuário e implementados com nomes e valores próprios. Nenhum quadro de vídeo virou textura da interface. Os arquivos de referência e quadros extraídos não estão incorporados ao produto.

## Problemas confirmados no código anterior

- Janela base 900 × 1020 e mínimo 760 × 860: incompatível com o objetivo compacto. Agora 360 × 640, escalável por um único transform, com limites 252 × 448 a 540 × 960. A migração ignora o tamanho gigante legado, sem alterar parâmetros de áudio.
- Área vazia da forma de onda ignorava clique; dependia do botão separado Abrir voz. Agora o display abre o seletor e é alvo explícito de file drops/descrições compatíveis.
- Arraste do resultado existia em um botão, não no display. Agora o gesto parte do corpo do waveform processado; marcadores pertencem à visualização Vocal.
- Indicador WAV era inicializado em 0,00 sem distinguir ausência de fonte/render. Agora não existe duração fictícia para fonte inexistente: Sem áudio/Carregando/Gravando/Processando/Pronto/Reproduzindo/Erro são diferenciados. A fonte aparece no display enquanto o worker prepara o resultado; erros mantêm a causa no status/tooltip.
- Resultado antigo podia permanecer exportável no intervalo entre um ajuste e o timer. Agora cada resultado tem uma assinatura dos parâmetros, seleção, sample rate e estado de sincronização. Exportação, cache e prévia do resultado verificam a assinatura no próprio pedido. Importação nova invalida o resultado anterior mesmo se a leitura falhar.
- WAV de arraste era criado em diretório temporário e seu preparo sobrescrevia a mensagem principal. Agora existe preparação dedicada no processador, em armazenamento persistente, com validação do formato/resultado. Falha de escrita não provoca loop automático de tentativas. O arquivo só entra no gesto depois de estar concluído.
- Thread de áudio fazia buscas por nome no APVTS, inclusive por amostra. Agora volume/proteção/bypass usam ponteiros atômicos estáveis; leitura de arquivos, render, exportação, Base64 e alocações de buffers ficam fora do callback. Um probe Linux verifica operações de heap em uma chamada de áudio nos testes.
- Havia cinco presets e seleção automática ao entrar no Throw. Agora são 17 presets próprios, com indicação de alterações; a seleção é manual e preservada ao trocar preset/modo. Não há reconhecimento de palavras nem nova seleção automática no fluxo.

## Seletor de arquivos e limite do diagnóstico no Mac

O seletor antigo já era mantido por um membro do editor; portanto, o código não comprova uma destruição prematura desse objeto. Nesta revisão ele continua retido durante o callback, bloqueia lançamentos concorrentes e recebe explicitamente o componente pai para o diálogo nativo. SafePointer protege fechamento do editor. File URLs são convertidas para paths sem perder espaços/unicode.

Não foi possível reproduzir a falha do seletor nativo ou do arraste de uma região do FL/macOS neste executor Linux. As demonstrações anexadas são do Boomerang, não capturas do defeito do WONKA. Por isso, não se atribui a falha a um mecanismo interno do host sem evidência. A compatibilidade exata de regiões internas segue pendente; Finder WAV/AIFF e importação por diálogo são os caminhos previstos, com mensagens para falhas de leitura/seleção/render.

## Escolhas próprias

- Vocal compara a fonte e habilita a edição dos marcadores; é uma interpretação do botão visível na referência, não uma alegação sobre sua implementação original.
- Tone é cutoff final 200 Hz–20 kHz, limitado a 45% da sample rate, com filtro de um polo. No Throw há também amortecimento cumulativo dos ecos.
- Shape mostra o expoente de crescimento; no Throw modifica o envelope de repetições.
- Length/Bars têm autoridade exclusiva, indicada por s/bars; cauda Reverse opcional e duração total ficam explícitas.
- Botão `...` revela as opções secundárias no espaço dos knobs, sem aumentar a altura. Nada se sobrepõe ao display; fechar o painel restaura os controles principais.
- Medidores são picos reais dBFS com decaimento visual e clipping retido até clique. O waveform aplica apenas ganho visual de até 8× para legibilidade; isso não modifica amostras, volume, normalização ou WAV.
- Prévia tem rampas de 10 ms no início/parada e volume suavizado em 20 ms. Mudanças de render interrompem a prévia; não há promessa de morph contínuo entre renders diferentes.
- A pasta persistente de WAVs não é limpa automaticamente, pois o host pode manter referências aos arquivos após fechar o plugin. A portabilidade do projeto exige coletar/copiar samples pela DAW.

## Logo incorporada na revisão 0.2.1

O usuário enviou posteriormente o PNG original pelo GitHub. O arquivo RGBA de
1921 × 819 tem transparência; foi renomeado para `Assets/WONKA-logo.png`, sem
alterar seus bytes. O recurso é incorporado por juce_add_binary_data, mantém a
cor preta e é ajustado proporcionalmente no cabeçalho claro. O nome WONKA e os
identificadores de plugin foram mantidos. A versão 0.2.1 identifica essa revisão.

## Atualização visual 0.2.2

A pedido do usuário, a logo foi convertida para branco, usando sua transparência original, e exportada em 4096 × 1746. O original preto permanece intacto em Assets. O cabeçalho agora é grafite para oferecer contraste à versão branca. A ampliação raster usa Lanczos; não há alegação de vetorização nem de novos detalhes originais em 4K.
