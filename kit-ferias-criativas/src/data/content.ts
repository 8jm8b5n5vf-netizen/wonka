import type { siteConfig } from "../config/site";

export type BonusId = keyof typeof siteConfig.bonuses;

export const painPoints = [
  {
    title: "As férias chegam e a rotina some",
    text: "Sem escola, as horas parecem longas e as crianças logo perguntam o que fazer.",
  },
  {
    title: "Sem tempo para criar atividades",
    text: "Entre o trabalho e a casa, sobra pouco tempo para pesquisar e preparar brincadeiras educativas.",
  },
  {
    title: "A tela vira a saída mais fácil",
    text: "Celular e tablet acalmam por um momento, mas nem sempre é o que você gostaria para o seu filho.",
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
    title: "Escolha seu kit",
    text: "Faça sua compra pelo checkout da plataforma de pagamento.",
  },
  {
    title: "Acesse os materiais",
    text: "Após o pagamento aprovado, receba por e-mail as instruções para entrar na área de membros da Cakto e baixar os PDFs.",
  },
  {
    title: "Imprima e divirta-se",
    text: "Escolha atividades adequadas ao nível da criança e aproveite os momentos em casa.",
  },
];

export const categories = [
  {
    name: "Alfabeto",
    description: "Reconhecimento de letras, traçado e atividades visuais.",
    cover: "/capa-alfabeto.webp",
  },
  {
    name: "Vogais",
    description: "Identificação e exercícios com vogais.",
    cover: "/capa-vogais.webp",
  },
  {
    name: "Sílabas",
    description: "Sílabas simples e combinações mais complexas.",
    cover: "/capa-silabas.webp",
  },
  {
    name: "Números e matemática",
    description: "Contagem, números e operações introdutórias.",
    cover: "/capa-numeros.webp",
  },
  {
    name: "Coordenação motora",
    description: "Traçado e atividades para as mãos.",
    cover: "/capa-coordenacao.webp",
  },
  {
    name: "Pequenas leituras",
    description: "Textos curtos para ler conforme o nível.",
    cover: "/capa-leitura.webp",
  },
  {
    name: "Quebra-cabeças",
    description: "Atividades lúdicas de associação e raciocínio.",
    cover: "/capa-quebra-cabeca.webp",
  },
];

export const samples = [
  {
    src: "/amostra-alfabeto.webp",
    alt: "Página de exemplo da atividade de alfabeto para imprimir",
  },
  {
    src: "/amostra-vogais.webp",
    alt: "Página de exemplo da atividade de vogais para imprimir",
  },
  {
    src: "/amostra-silabas.webp",
    alt: "Página de exemplo da atividade de sílabas para imprimir",
  },
  {
    src: "/amostra-matematica.webp",
    alt: "Página de exemplo da atividade de números e matemática para imprimir",
  },
  {
    src: "/amostra-coordenacao.webp",
    alt: "Página de exemplo da atividade de coordenação motora para imprimir",
  },
  {
    src: "/amostra-quebra-cabeca.webp",
    alt: "Página de exemplo da atividade de quebra-cabeça para imprimir",
  },
];

export const bonuses: { id: BonusId; title: string; text: string }[] = [
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

export const faqs = [
  {
    question: "O Kit Férias Criativas é físico ou digital?",
    answer:
      "É digital. Você recebe os materiais em PDF, para imprimir em casa ou em uma gráfica de sua preferência.",
  },
  {
    question: "Preciso de impressora?",
    answer:
      "Para imprimir em casa, sim. Se preferir, você também pode levar os arquivos a uma gráfica.",
  },
  {
    question: "Como vou receber os arquivos?",
    answer:
      "Após a aprovação do pagamento, você recebe por e-mail as instruções para acessar a área de membros da Cakto. Lá, abre os módulos e baixa cada PDF. Confira também o spam.",
  },
  {
    question: "Para quais idades o kit é indicado?",
    answer:
      "Escolha pelo que a criança já consegue fazer. Para quem está começando, experimente traçar uma letra ou contar figuras. Para quem já junta sílabas, veja as pequenas leituras. Você pode abrir as amostras antes de decidir.",
  },
  {
    question: "Posso imprimir mais de uma atividade?",
    answer: "Você escolhe as páginas que quer imprimir para usar em casa, quando e quantas vezes precisar.",
  },
  {
    question: "Preciso comprar uma assinatura?",
    answer: "Não. O pagamento é único, sem mensalidade.",
  },
  {
    question: "Quais materiais estão incluídos?",
    answer:
      "Atividades nas categorias alfabeto, vogais, sílabas, números e matemática, coordenação motora, pequenas leituras e quebra-cabeças. Acompanha o Calendário de 30 Dias de Férias e o Guia Prático para os Pais.",
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
