# Algoritmos WONKA

Algoritmos próprios, sem alegação de equivalência ao Boomerang. Todo processamento abaixo acontece no worker. Seleções têm fades lineares de até 5 ms em ambas as bordas; resultado recebe fades de 5 ms, filtro Tone (cutoff `min(0,45 sampleRate, 200 × 100^Tone)` Hz, coeficiente `1-exp(-2 pi cutoff/sampleRate)`), Tremolo e proteção/normalização opcionais.

## Reverse

**Line:** seleção → fades → conversão de duração por resampling → inversão → reverb em paralelo (dry 0,45 + wet 0,8) → curva crescente `t^(0,2 + 3,8 Shape)` → filtro/modulação → fade final.

**Swell:** seleção → fades → conversão de duração → reverb 100% wet → inversão de todo o buffer de reverb → mesma curva crescente → filtro/modulação → fade final. A cauda reverberante se torna o começo do swell.

Length/Bars define a duração do trecho principal. **Incluir cauda** adiciona a janela de decaimento do reverb (0,25 a 6 segundos); desligado, o efeito fica restrito à duração principal. O tempo WAV total fica sempre explícito na interface e é o valor a usar para alinhamento. A cauda é finita, até a janela de T60 estimada; um reverb IIR não tem uma cauda matematicamente infinita exportável. Há corte suave na borda final.

A alteração de duração usa resampling sinc com janela Hann (48 taps), com filtragem para reduzir aliasing. Ela também altera altura/formantes: não é time-stretch com preservação de pitch. A interpolação de prévia entre sample rates do host é linear; o render e o WAV usam sinc. Recomenda-se ajustar e renderizar na sample rate da sessão.

**Space:** tamanhos dos seis combs multiplicados por `0,65 + 1,6 Space`; T60 estimado de `0,25 + 5,75 Space` s determina ganho de feedback de cada comb; duas etapas all-pass têm realimentação `0,2 + 0,5 Space`, com atraso escalado por `0,7 + Space`. Mistura entre os canais estéreo aumenta até 30%. O reverb inclui amortecimento por Tone. No Throw, Space adiciona esse ambiente aos ecos em ganho até 0,3, dentro da duração de cauda escolhida.

## Throw

Seleção → fades → repetições nos offsets `k × divisão × 60/BPM` → ganho `feedback^(k-1)` (feedback limitado a 0,94) → filtro passa-baixas cumulativo a cada repetição → decaimento Shape → ambiente Space → filtro/modulação → fade final.

Divisões em beats de semínima: 1/4 = 1; 1/8 = 0,5; 1/8 pontuada = 0,75; 1/8 tercina = 1/3; 1/16 = 0,25; 1/16 pontuada = 0,375; 1/16 tercina = 1/6; 1/2 = 2. O espaçamento não é esticado por Length/Bars. O fim total é tamanho da seleção mais a duração da cauda. Ecos que ultrapassam esse fim são cortados com fade. O primeiro eco chega após um intervalo de delay; a versão só efeito preserva esse silêncio inicial como referência temporal.

Mono vira estéreo para alternar esquerda/direita. Em fonte estéreo, o canal esquerdo é a voz principal do eco e o direito acompanha a 25% na posição oposta; a imagem troca de lado a cada eco. Isso preserva conteúdo de ambos os canais sem cancelar sinais em oposição de fase. Não é preservação exata da imagem estéreo original, porque o efeito a modifica intencionalmente. A inclusão do trecho original preserva seus canais e é somada após a filtragem/modulação do efeito, com os fades da seleção; a fonte seca não passa por Tone/Tremolo nem pela ambiência dos ecos. Normalização e proteção opcionais são aplicadas ao resultado combinado.

A seleção do Throw é manual, usando os marcadores na visualização Vocal. Trocar de modo ou preset não tenta identificar palavras nem altera automaticamente a seleção.

## Duração e compasso

`segundos musicais = Bars × numerador × 4/denominador × 60/BPM`. O usuário escolhe segundos **ou** Bars; não há multiplicação entre os controles. O visor informa a duração efetiva após o limite de 30 s. BPM do host e compasso são lidos em `processBlock`; um timer fora da thread de áudio agenda novos renders mesmo com o editor fechado. Na ausência de BPM válido, usam-se BPM/compasso manuais. Tremolo sincronizado tem taxa 1, 2 ou 4 ciclos por beat; modo livre tem 4 Hz.

## Presets e parâmetros estáveis

Os 17 presets em `Source/Presets.h` têm nomes/valores próprios. Identificadores APVTS da versão 0.1 foram mantidos; Tone continua normalizado 0–1 no host, mas é mostrado como frequência e agora usa o mapeamento documentado. Sessões antigas preservam os valores normalizados, embora a resposta de Tone seja a da revisão 0.2. Mudanças de parâmetros são aplicadas a novos buffers imutáveis no worker; somente volume/proteção/bypass são lidos diretamente no callback por ponteiros atômicos.
