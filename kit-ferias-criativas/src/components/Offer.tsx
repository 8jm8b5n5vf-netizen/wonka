import { siteConfig } from "../config/site";
import { formatPrice } from "../lib/format";
import { CheckoutButton } from "./CheckoutButton";

export function Offer() {
  const { value, locale, currency, paymentNote } = siteConfig.price;
  const priceText = formatPrice(value, locale, currency);
  const [whole, cents] = priceText.replace("R$", "").trim().split(",");

  const included = [
    "22 cadernos em PDF, organizados em seis módulos",
    "Calendário de 30 Dias de Férias e Guia Prático para os Pais",
    "Guia Comece aqui e índice dos cadernos",
    "Pagamento único, sem assinatura",
  ];

  return (
    <section id="oferta" className="scroll-mt-20 bg-gradient-to-b from-paper to-primary-50 py-16 sm:py-24">
      <div className="mx-auto max-w-3xl px-4 sm:px-6">
        <div className="rounded-[2rem] bg-white p-6 text-center shadow-xl ring-1 ring-ink-200 sm:p-12">
          <p className="text-sm font-bold uppercase tracking-wider text-primary-600">Oferta</p>
          <h2 className="mt-3 text-3xl font-extrabold text-ink-900 sm:text-4xl">Seu {siteConfig.productName}</h2>
          <p className="mx-auto mt-3 max-w-md text-ink-700">
            22 cadernos em PDF, dois bônus e recursos de orientação. Material digital para imprimir e aproveitar nas férias.
          </p>

          <img
            src="/composicao-oferta.webp"
            alt="Capas reais dos cadernos de sílabas, leitura e quebra-cabeças"
            width={1100}
            height={560}
            loading="lazy"
            className="mx-auto mt-6 h-auto w-64 rounded-xl"
          />

          <div className="mt-8 flex items-end justify-center gap-1 font-display text-ink-900">
            <span className="mb-3 text-2xl font-bold text-ink-700">R$</span>
            <span className="text-7xl font-extrabold leading-none">{whole}</span>
            <span className="mb-2 text-3xl font-bold leading-none">,{cents}</span>
          </div>
          <p className="mt-2 font-semibold text-ink-700">{paymentNote} · Produto digital</p>

          <ul className="mx-auto mt-8 max-w-md space-y-3 text-left">
            {included.map((item) => (
              <li key={item} className="flex items-start gap-3 text-ink-700">
                <span
                  aria-hidden="true"
                  className="mt-0.5 flex h-6 w-6 shrink-0 items-center justify-center rounded-full bg-primary-100 text-sm font-bold text-primary-700"
                >
                  ✓
                </span>
                <span>{item}</span>
              </li>
            ))}
          </ul>

          <div className="mt-10">
            <CheckoutButton label="Comprar agora" size="lg" className="w-full sm:w-auto" />
          </div>
          <p className="mt-4 text-sm text-ink-500">
            A compra é concluída na Cakto. Direito de arrependimento em 7 dias.{" "}
            <a href="#reembolso" className="font-semibold underline">Como pedir reembolso</a>.
          </p>
        </div>
      </div>
    </section>
  );
}
