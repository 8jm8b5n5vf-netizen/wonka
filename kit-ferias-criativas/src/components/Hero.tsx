import { siteConfig } from "../config/site";
import { formatPrice } from "../lib/format";
import { CheckoutButton } from "./CheckoutButton";

export function Hero() {
  const { value, locale, currency, paymentNote } = siteConfig.price;
  const priceText = formatPrice(value, locale, currency);

  return (
    <section id="topo" className="relative overflow-hidden bg-gradient-to-b from-sun-100 via-paper to-paper pt-28 pb-16 sm:pt-36 sm:pb-24">
      <div className="mx-auto grid max-w-6xl items-center gap-10 px-4 sm:px-6 lg:grid-cols-2 lg:gap-16">
        <div className="animate-rise text-center lg:text-left">
          <span className="inline-block rounded-full bg-white px-4 py-1.5 text-sm font-bold text-primary-700 shadow-sm">
            {siteConfig.tagline}
          </span>
          <h1 className="mt-6 text-4xl font-extrabold text-ink-900 sm:text-5xl lg:text-6xl">
            Férias mais divertidas, com menos tempo de tela!
          </h1>
          <p className="mx-auto mt-6 max-w-xl text-lg text-ink-700 sm:text-xl lg:mx-0">
            Escolha uma atividade, imprima uma folha e acompanhe a criança. O kit reúne 22 cadernos em PDF organizados em seis módulos.
          </p>

          <div className="mt-8 inline-flex flex-col items-center gap-1 rounded-2xl bg-white px-6 py-4 text-center shadow-sm ring-1 ring-ink-200 lg:items-start lg:text-left">
            <div className="flex items-baseline gap-2">
              <span className="font-display text-3xl font-extrabold text-ink-900 sm:text-4xl">{priceText}</span>
              <span className="text-sm font-bold text-primary-700">{paymentNote.toLowerCase()}</span>
            </div>
            <p className="text-sm text-ink-500">Material digital em PDF, sem mensalidade</p>
          </div>

          <div className="mt-6 flex flex-col items-center gap-4 sm:flex-row sm:justify-center lg:justify-start">
            <CheckoutButton label="Quero meu Kit Férias Criativas" size="lg" className="w-full sm:w-auto" />
            <a
              href="#amostras"
              className="inline-flex w-full items-center justify-center rounded-full border-2 border-primary-600 px-6 py-3.5 font-display text-lg font-bold text-primary-700 transition hover:bg-primary-50 sm:w-auto"
            >
              Ver atividades por dentro
            </a>
          </div>
          <p className="mt-4 text-sm text-ink-500">Material digital em PDF · Pronto para imprimir</p>
        </div>

        <div className="animate-rise flex justify-center" style={{ animationDelay: "120ms" }}>
          <img
            src="/composicao-hero.webp"
            width={1100}
            height={560}
            alt="Capas e páginas reais dos cadernos de vogais, alfabeto e números do kit"
            className="h-auto w-full max-w-xl rounded-3xl shadow-lg"
          />
        </div>
      </div>
    </section>
  );
}
