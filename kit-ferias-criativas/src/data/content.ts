import type { siteConfig } from "../config/site";

export type BonusId = keyof typeof siteConfig.bonuses;

export const deliveryFacts = [
  { title: "22 cadernos em PDF", text: "Seis módulos para baixar e abrir no leitor de PDF." },
  { title: "Atividades para imprimir", text: "Escolha a folha, imprima em casa ou em gráfica." },
  { title: "Pagamento único pela Cakto", text: "R$ 29,90, sem mensalidade." },
];

export const painPoints = [
  {
    title: "Férias em casa pedem ideias novas",
    text: "Sem a rotina da escola, a criança logo procura o que fazer. Ter propostas prontas ajuda a escolher uma atividade para aquele momento.",
  },
  {
    title: "Nem sempre sobra tempo para preparar",
    text: "Entre trabalho e casa, planejar brincadeiras educativas fica difícil. O kit reúne folhas prontas para você imprimir quando precisar.",
  },
  {
    title: "A tela nem sempre é a única saída",
    text: "Celular e tablet acalmam por um momento, mas variar com atividades em papel traz outros tipos de descoberta e brincadeira.",
  },
];

export const benefits = [
  {
    icon: "smartphone",
    title: "Alternativas às telas",
    text: "Mais ideias de diversão para variar a rotina durante as férias.",
  },
  {
    icon: "printer",
    title: "Pronto para imprimir",
    text: "Escolha as páginas, imprima e prepare a atividade.",
  },
  {
    icon: "book",
    title: "Aprender brincando",
    text: "Atividades com letras, números, leitura e habilidades motoras.",
  },
  {
    icon: "calendar",
    title: "Mais ideias, menos planejamento",
    text: "Uma coleção organizada para facilitar a escolha das atividades.",
  },
] as const;

export const steps = [
  {
    title: "Finalize o pagamento",
    text: "A compra acontece no checkout da Cakto, em pagamento único de R$ 29,90.",
  },
  {
    title: "Siga o e-mail de acesso",
    text: "Após a aprovação, você recebe por e-mail as instruções para entrar na área de membros da Cakto e baixar os PDFs.",
  },
  {
    title: "Baixe, escolha e imprima",
    text: "Abra o caderno, selecione as páginas e imprima em casa ou em uma gráfica. Um adulto acompanha recortes e peças pequenas.",
  },
];

export type Module = {
  name: string;
  description: string;
  covers: string[];
  example: string;
};

export const modules: Module[] = [
  {
    name: "Primeiras descobertas",
    description: "Traçados, coordenação motora e primeiros reconhecimentos visuais.",
    covers: ["/capa-coordenacao.webp", "/capa-vogais.webp"],
    example: "Folhas de traçado e vogais para quem está começando a explorar lápis e letras.",
  },
  {
    name: "Alfabeto divertido",
    description: "Reconhecimento de letras, traçado e atividades visuais com o alfabeto.",
    covers: ["/capa-alfabeto.webp"],
    example: "Letras maiúsculas e minúsculas para reconhecer e escrever.",
  },
  {
    name: "Brincando com sílabas",
    description: "Sílabas simples e combinações para formar pequenas palavras.",
    covers: ["/capa-silabas.webp"],
    example: "Junção de sílabas com imagens para associar som e escrita.",
  },
  {
    name: "Números e desafios",
    description: "Contagem, números e operações introdutórias com desafios.",
    covers: ["/capa-numeros.webp"],
    example: "Contagem de figuras e pequenas operações para praticar.",
  },
  {
    name: "Pequenas leituras",
    description: "Textos curtos para ler conforme o nível de cada criança.",
    covers: ["/capa-leitura.webp"],
    example: "Frases e pequenos textos para acompanhar a leitura.",
  },
  {
    name: "Jogos e quebra-cabeças",
    description: "Atividades lúdicas de associação e raciocínio.",
    covers: ["/capa-quebra-cabeca.webp"],
    example: "Quebra-cabeças e jogos de associação para resolver.",
  },
];

export type Sample = { src: string; alt: string; caption: string };

export const samples: Sample[] = [
  { src: "/amostra-alfabeto.webp", alt: "Página de exemplo da atividade de alfabeto para imprimir", caption: "Alfabeto" },
  { src: "/amostra-vogais.webp", alt: "Página de exemplo da atividade de vogais para imprimir", caption: "Vogais" },
  { src: "/amostra-silabas.webp", alt: "Página de exemplo da atividade de sílabas para imprimir", caption: "Sílabas" },
  { src: "/amostra-matematica.webp", alt: "Página de exemplo da atividade de números e matemática para imprimir", caption: "Números e matemática" },
  { src: "/amostra-coordenacao.webp", alt: "Página de exemplo da atividade de coordenação motora para imprimir", caption: "Coordenação motora" },
  { src: "/amostra-quebra-cabeca.webp", alt: "Página de exemplo da atividade de quebra-cabeça para imprimir", caption: "Quebra-cabeças" },
];

export const resources: { id: BonusId; title: string; text: string }[] = [
  {
    id: "calendar30Days",
    title: "Calendário de 30 Dias de Férias",
    text: "Sugestões organizadas para facilitar a escolha das atividades ao longo das férias.",
  },
  {
    id: "parentsGuide",
    title: "Guia Prático para os Pais",
    text: "Orientações simples para escolher, imprimir e utilizar os materiais.",
  },
];

export const startResources = [
  { title: "Comece aqui", text: "Uma primeira atividade e orientações para começar a usar o kit sem se perder." },
  { title: "Índice dos cadernos", text: "Lista dos 22 cadernos para encontrar uma atividade pelo tema ou módulo." },
];

export const faqs = [
  {
    question: "O Kit Férias Criativas é físico ou digital?",
    answer: "É digital. Você recebe os materiais em PDF, para imprimir em casa ou em uma gráfica de sua preferência. Não há envio físico.",
  },
  {
    question: "Como vou receber os arquivos?",
    answer: "Após a aprovação do pagamento, você recebe por e-mail as instruções para acessar a área de membros da Cakto. Lá, abre os módulos e baixa cada PDF. Confira também o spam.",
  },
  {
    question: "Preciso de impressora?",
    answer: "Para imprimir em casa, sim. Se preferir, você também pode levar os arquivos a uma gráfica.",
  },
  {
    question: "Quais materiais preciso providenciar?",
    answer: "Papel, impressão e materiais das atividades, como lápis e massinha, ficam por sua conta. Um adulto acompanha recortes e peças pequenas.",
  },
  {
    question: "Como escolho a atividade certa?",
    answer: "Pelo interesse e pelas habilidades que a criança já explora. Para quem está começando, experimente traçar uma letra ou contar figuras. Para quem já junta sílabas, veja as pequenas leituras. Abra as amostras antes de decidir.",
  },
  {
    question: "Preciso comprar uma assinatura?",
    answer: "Não. O pagamento é único, sem mensalidade.",
  },
  {
    question: "Quais bônus estão incluídos?",
    answer: "O Calendário de 30 Dias de Férias e o Guia Prático para os Pais. Acompanham também o guia Comece aqui e o índice dos cadernos.",
  },
  {
    question: "Como entro em contato caso tenha dúvidas?",
    answer: "Use o contato do produtor informado no e-mail de confirmação da compra. Tenha em mãos o e-mail usado no pedido.",
  },
  {
    question: "Como peço reembolso nos primeiros 7 dias?",
    answer: "O art. 49 do Código de Defesa do Consumidor prevê 7 dias para desistir da compra online, a contar da assinatura ou do recebimento do produto ou serviço. Para pedir pela Cakto, use o e-mail da compra e selecione o pedido no portal de reembolsos.",
  },
];
