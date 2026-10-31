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
    text: "Receba acesso aos arquivos digitais conforme as instruções enviadas após a confirmação da compra.",
  },
  {
    title: "Imprima e divirta-se",
    text: "Escolha atividades adequadas ao nível da criança e aproveite os momentos em casa.",
  },
];

export const categories = [
  { name: "Alfabeto", symbol: "Aa", description: "Reconhecimento de letras, traçado e atividades visuais." },
  { name: "Vogais", symbol: "AEIOU", description: "Identificação e exercícios com vogais." },
  { name: "Sílabas", symbol: "ba", description: "Sílabas simples e combinações mais complexas." },
  { name: "Números e matemática", symbol: "123", description: "Contagem, números e operações introdutórias." },
  { name: "Coordenação motora", symbol: "✎", description: "Traçado e atividades para as mãos." },
  { name: "Pequenas leituras", symbol: "Lê", description: "Textos curtos para ler conforme o nível." },
  { name: "Quebra-cabeças", symbol: "◩", description: "Atividades lúdicas de associação e raciocínio." },
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
      "Após a confirmação do pagamento, os arquivos são disponibilizados conforme as instruções da plataforma de pagamento.",
  },
  {
    question: "Para quais idades o kit é indicado?",
    answer:
      "As atividades são organizadas por fase de desenvolvimento, para você escolher as mais adequadas a cada criança. A faixa etária recomendada será informada em cada material.",
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
      "Atividades nas categorias alfabeto, vogais, sílabas, números e matemática, coordenação motora, pequenas leituras e quebra-cabeças. Os bônus planejados serão liberados quando estiverem concluídos.",
  },
  {
    question: "Como entro em contato caso tenha dúvidas?",
    answer: "Use os dados de contato indicados no rodapé da página.",
  },
  {
    question: "Quais são as condições de cancelamento?",
    answer: "As condições de compra e cancelamento estão descritas nos termos de uso, disponíveis no rodapé.",
  },
];
