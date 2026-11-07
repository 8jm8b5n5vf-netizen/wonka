/**
 * Configuração central da landing page.
 * Edite aqui o preço, o nome do produto, o link do checkout da Cakto,
 * o contato e o ID do Meta Pixel. Deixe vazio o que ainda não existir.
 */
export const siteConfig = {
  productName: "Kit Férias Criativas",
  tagline: "Atividades educativas para imprimir",

  price: {
    value: 29.9,
    regularValue: 49.9,
    currency: "BRL",
    locale: "pt-BR",
    paymentNote: "Pagamento único",
  },

  /** Cole aqui o link de checkout da Cakto. Enquanto estiver vazio, os botões ficam desativados. */
  checkoutUrl: "https://pay.cakto.com.br/32t33vz_1269587",

  /** ID do Meta Pixel. Preencha somente quando existir. */
  metaPixelId: "",

  /** Bônus exibidos na página. Mude enabled para false para ocultar um bônus. */
  bonuses: {
    calendar30Days: { enabled: true },
    parentsGuide: { enabled: true },
  },

  brand: "Mundo das Atividades",

  /** Portal de reembolsos da Cakto (direito de arrependimento em 7 dias). */
  refundUrl: "https://reembolso.cakto.com.br/",

  contact: {
    email: "",
    whatsapp: "",
    companyName: "Mundo das Atividades",
    cnpj: "",
  },

  legal: {
    termsUrl: "#termos",
    privacyUrl: "#privacidade",
  },
};
