import { siteConfig } from "../config/site";
import { formatPrice } from "../lib/format";
import { CheckoutButton } from "./CheckoutButton";

function HeroIllustration() {
  return (
    <svg viewBox="0 0 400 360" role="img" aria-label="Ilustração de crianças brincando com atividades" className="h-auto w-full max-w-md">
      <circle cx="320" cy="70" r="46" fill="#ffc93c" />
      <circle cx="80" cy="300" r="34" fill="#3aa8e8" opacity="0.5" />
      <rect x="40" y="60" width="150" height="190" rx="18" fill="#ffffff" stroke="#c8ece4" strokeWidth="4" />
      <rect x="62" y="88" width="106" height="12" rx="6" fill="#12a08c" />
      <rect x="62" y="116" width="82" height="12" rx="6" fill="#c8ece4" />
      <rect x="62" y="144" width="96" height="12" rx="6" fill="#c8ece4" />
      <circle cx="104" cy="206" r="26" fill="#ffe4dc" />
      <path d="M92 206 l8 8 l16 -18" stroke="#e25f3e" strokeWidth="6" fill="none" strokeLinecap="round" strokeLinejoin="round" />
      <rect x="200" y="150" width="160" height="160" rx="20" fill="#ffd55c" />
      <text x="280" y="250" textAnchor="middle" fontFamily="Baloo 2, sans-serif" fontWeight="800" fontSize="92" fill="#1b2631">
        A
      </text>
      <rect x="220" y="110" width="110" height="56" rx="16" fill="#12a08c" />
      <text x="275" y="148" textAnchor="middle" fontFamily="Baloo 2, sans-serif" fontWeight="700" fontSize="26" fill="#ffffff">
        1 2 3
      </text>
    </svg>
  );
}

export function Hero() {
  const price = formatPrice(siteConfig.price.value, siteConfig.price.locale, siteConfig.price.currency);

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
            O Kit Férias Criativas reúne atividades educativas prontas para imprimir, para a criança aprender e se divertir enquanto você descansa.
          </p>

          <div className="mt-8 flex flex-col items-center gap-4 sm:flex-row sm:justify-center lg:justify-start">
            <CheckoutButton label="Quero meu Kit Férias Criativas" size="lg" className="w-full sm:w-auto" />
            <a
              href="#conteudo"
              className="inline-flex w-full items-center justify-center rounded-full border-2 border-primary-600 px-6 py-3.5 font-display text-lg font-bold text-primary-700 transition hover:bg-primary-50 sm:w-auto"
            >
              Ver o conteúdo
            </a>
          </div>
          <p className="mt-4 text-sm text-ink-500">
            Por {price}, pagamento único. Arquivos digitais em PDF.
          </p>
        </div>

        <div className="animate-rise flex justify-center" style={{ animationDelay: "120ms" }}>
          <div className="animate-float">
            <HeroIllustration />
          </div>
        </div>
      </div>
    </section>
  );
}
