import { siteConfig } from "../config/site";
import { formatPrice } from "../lib/format";
import { CheckoutButton } from "./CheckoutButton";

export function Hero() {
  const price = formatPrice(siteConfig.price.total, siteConfig.price.locale, siteConfig.price.currency);

  return (
    <section id="topo" className="relative overflow-hidden bg-gradient-to-b from-sun-100 via-paper to-paper pt-28 pb-16 sm:pt-36 sm:pb-24">
      <div className="mx-auto grid max-w-6xl items-center gap-12 px-4 sm:px-6 lg:grid-cols-2 lg:gap-16">
        <div className="animate-rise text-center lg:text-left">
          <span className="inline-block rounded-full bg-white px-4 py-1.5 text-sm font-bold text-primary-700 shadow-sm">
            {siteConfig.tagline}
          </span>
          <h1 className="mt-6 text-4xl font-extrabold text-ink-900 sm:text-5xl lg:text-6xl">
            Férias mais divertidas, com menos tempo de tela!
          </h1>
          <p className="mx-auto mt-6 max-w-xl text-lg text-ink-700 sm:text-xl lg:mx-0">
            Escolha uma atividade de letras, números ou jogos, imprima uma folha e convide a criança para brincar. O kit reúne 22 cadernos em PDF organizados em seis módulos.
          </p>

          <div className="mt-8 inline-flex items-baseline gap-3 rounded-2xl bg-white px-5 py-3 text-left shadow-sm ring-1 ring-ink-200">
            <span className="font-display text-3xl font-extrabold text-ink-900">{price}</span>
            <span className="text-sm font-bold text-primary-700">pagamento único</span>
          </div>

          <div className="mt-6 flex flex-col items-center gap-4 sm:flex-row sm:justify-center lg:justify-start">
            <CheckoutButton label="Quero meu Kit Férias Criativas" size="lg" className="w-full sm:w-auto" />
            <a
              href="#amostras"
              className="inline-flex w-full items-center justify-center rounded-full border-2 border-primary-600 px-6 py-3.5 font-display text-lg font-bold text-primary-700 transition hover:bg-primary-50 sm:w-auto"
            >
              Ver páginas do kit
            </a>
          </div>
          <p className="mt-4 text-sm text-ink-500">Material digital em PDF · Pronto para imprimir</p>
        </div>

        <div className="animate-rise flex justify-center" style={{ animationDelay: "120ms" }}>
          <div className="animate-float">
            <img src="/composicao-hero.webp" width="1100" height="560" alt="Capas dos cadernos de vogais, alfabeto e números" className="h-auto w-full rounded-3xl" />
          </div>
        </div>
      </div>
    </section>
  );
}
