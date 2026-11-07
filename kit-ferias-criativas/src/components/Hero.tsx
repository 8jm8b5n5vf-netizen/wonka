import { siteConfig } from "../config/site";
import { formatPrice } from "../lib/format";
import { getCheckoutHref } from "../lib/checkout";
import { trackMetaEvent } from "../lib/metaPixel";
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
            Uma brincadeira pronta para hoje.
          </h1>
          <p className="mx-auto mt-6 max-w-xl text-lg text-ink-700 sm:text-xl lg:mx-0">
            22 cadernos em PDF para imprimir e brincar com as crianças nas férias.
          </p>

          <div className="mt-8 flex flex-col items-center gap-4 sm:flex-row sm:justify-center lg:justify-start">
            <CheckoutButton label="Quero meu kit de férias" size="lg" className="w-full sm:w-auto" />
            <p className="text-center text-ink-700 sm:text-left">
              <span className="font-display text-2xl font-extrabold text-ink-900">{priceText}</span>
              <span className="ml-2 text-sm font-semibold text-primary-700">{paymentNote.toLowerCase()}</span>
            </p>
          </div>
          <p className="mt-4 text-sm text-ink-500">Material digital em PDF · Pronto para imprimir</p>
        </div>

        <div className="animate-rise flex justify-center" style={{ animationDelay: "120ms" }}>
          <a
            href={getCheckoutHref()}
            target="_blank"
            rel="noopener noreferrer"
            aria-label="Comprar o Kit Férias Criativas na Cakto"
            onClick={() => trackMetaEvent("InitiateCheckout")}
            className="block w-full max-w-xl rounded-3xl transition duration-300 hover:-translate-y-1 focus:outline-none focus-visible:ring-4 focus-visible:ring-primary-100"
          >
            <img
              src="/composicao-hero.webp"
              width={1100}
              height={560}
              fetchPriority="high"
              alt="Capas e páginas reais dos cadernos do kit"
              className="h-auto w-full rounded-3xl shadow-lg"
            />
          </a>
        </div>
      </div>
    </section>
  );
}
