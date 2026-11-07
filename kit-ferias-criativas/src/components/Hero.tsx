import { siteConfig } from "../config/site";
import { formatPrice } from "../lib/format";
import { getCheckoutHref } from "../lib/checkout";
import { trackMetaEvent } from "../lib/metaPixel";
import { CheckoutButton } from "./CheckoutButton";

const heroCovers = [
  { src: "/capa-vogais.webp", alt: "Capa do caderno de vogais" },
  { src: "/capa-alfabeto.webp", alt: "Capa do caderno de alfabeto" },
  { src: "/capa-numeros.webp", alt: "Capa do caderno de números" },
  { src: "/capa-silabas.webp", alt: "Capa do caderno de sílabas" },
];

export function Hero() {
  const { value, locale, currency, paymentNote } = siteConfig.price;
  const priceText = formatPrice(value, locale, currency);

  return (
    <section
      id="topo"
      className="relative overflow-hidden bg-gradient-to-b from-sun-100 via-paper to-paper pt-28 pb-16 sm:pt-36 sm:pb-24"
    >
      <div className="mx-auto grid max-w-6xl gap-6 px-4 sm:px-6 lg:grid-cols-2 lg:gap-x-16 lg:gap-y-8">
        <div className="animate-rise text-center lg:col-start-1 lg:row-start-1 lg:text-left">
          <span className="inline-block rounded-full bg-white px-4 py-1.5 text-sm font-bold text-primary-700 shadow-sm">
            {siteConfig.tagline}
          </span>
          <h1 className="mt-6 text-4xl font-extrabold text-ink-900 sm:text-5xl lg:text-6xl">
            Uma brincadeira pronta para hoje.
          </h1>
          <p className="mx-auto mt-6 max-w-xl text-lg text-ink-700 sm:text-xl lg:mx-0">
            22 cadernos em PDF para imprimir e brincar com as crianças nas férias.
          </p>
        </div>

        <div className="animate-rise relative lg:col-start-2 lg:row-span-3 lg:row-start-1 lg:self-center" style={{ animationDelay: "120ms" }}>
          <a
            href={getCheckoutHref()}
            target="_blank"
            rel="noopener noreferrer"
            aria-label="Comprar o Kit Férias Criativas na Cakto"
            onClick={() => trackMetaEvent("InitiateCheckout")}
            className="group block rounded-3xl transition duration-300 hover:-translate-y-1 focus:outline-none focus-visible:ring-4 focus-visible:ring-primary-100"
          >
            <img
              src="/composicao-hero.webp"
              width={1100}
              height={560}
              fetchPriority="high"
              alt="Capas e páginas reais dos cadernos do kit"
              className="h-auto w-full rounded-3xl shadow-lg"
            />
            <span className="absolute bottom-3 right-3 rounded-full bg-white/90 px-3 py-1 text-xs font-semibold text-ink-700 shadow-sm">
              Páginas reais do kit
            </span>
          </a>
        </div>

        <div className="animate-rise flex flex-col items-center gap-4 text-center sm:flex-row sm:justify-center lg:col-start-1 lg:row-start-2 lg:justify-start lg:text-left">
          <CheckoutButton label="Comprar agora" size="lg" className="w-full sm:w-auto" />
          <p className="text-ink-700">
            <span className="font-display text-2xl font-extrabold text-ink-900">{priceText}</span>
            <span className="ml-2 text-sm font-semibold text-primary-700">{paymentNote.toLowerCase()}</span>
          </p>
        </div>

        <div className="animate-rise lg:col-start-1 lg:row-start-3" style={{ animationDelay: "200ms" }}>
          <p className="text-center text-sm font-semibold text-ink-700 lg:text-left">Algumas das atividades</p>
          <ul className="mt-3 grid grid-cols-2 gap-3 sm:grid-cols-4">
            {heroCovers.map((cover) => (
              <li key={cover.src}>
                <img
                  src={cover.src}
                  alt={cover.alt}
                  width={1131}
                  height={1600}
                  loading="lazy"
                  decoding="async"
                  className="h-auto w-full rounded-xl border border-ink-200 bg-white shadow-sm"
                />
              </li>
            ))}
          </ul>
        </div>
      </div>
    </section>
  );
}
