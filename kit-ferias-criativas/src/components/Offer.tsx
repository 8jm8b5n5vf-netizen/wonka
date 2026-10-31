import { siteConfig } from "../config/site";
import { formatPrice } from "../lib/format";
import { CheckoutButton } from "./CheckoutButton";

const included = [
  "Materiais educativos em PDF, prontos para imprimir",
  "Categorias organizadas: alfabeto, vogais, sílabas, números, coordenação motora, leitura e quebra-cabeças",
  "Atividades de diferentes níveis",
  "Pagamento único, sem assinatura",
];

export function Offer() {
  const price = formatPrice(siteConfig.price.value, siteConfig.price.locale, siteConfig.price.currency);
  const [whole, cents] = price.replace("R$", "").trim().split(",");

  return (
    <section id="oferta" className="scroll-mt-20 bg-gradient-to-b from-paper to-primary-50 py-16 sm:py-24">
      <div className="mx-auto max-w-3xl px-4 sm:px-6">
        <div className="rounded-[2rem] bg-white p-6 text-center shadow-xl ring-1 ring-ink-200 sm:p-12">
          <p className="text-sm font-bold uppercase tracking-wider text-primary-600">Oferta</p>
          <h2 className="mt-3 text-3xl font-extrabold text-ink-900 sm:text-4xl">Seu {siteConfig.productName}</h2>
          <p className="mx-auto mt-3 max-w-md text-ink-700">
            Uma coleção de atividades educativas digitais para imprimir e aproveitar durante as férias.
          </p>

          <div className="mt-8 flex items-end justify-center gap-1 font-display text-ink-900">
            <span className="mb-3 text-2xl font-bold text-ink-700">R$</span>
            <span className="text-7xl font-extrabold leading-none">{whole}</span>
            <span className="mb-2 text-3xl font-bold leading-none">,{cents}</span>
          </div>
          <p className="mt-3 font-semibold text-ink-700">{siteConfig.price.paymentNote} · Produto digital</p>

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
          <p className="mx-auto mt-6 max-w-md text-left text-sm text-ink-500">
            Os bônus planejados serão liberados quando estiverem prontos e não estão incluídos neste preço.
          </p>

          <div className="mt-10">
            <CheckoutButton label="Quero meu Kit Férias Criativas" size="lg" className="w-full sm:w-auto" />
          </div>
          <p className="mt-4 text-sm text-ink-500">Você será direcionado ao checkout para finalizar a compra.</p>
        </div>
      </div>
    </section>
  );
}
