import { useEffect, useState } from "react";
import { siteConfig } from "../config/site";
import { formatPrice } from "../lib/format";
import { CheckoutButton } from "./CheckoutButton";

export function StickyBuyBar() {
  const [pastHero, setPastHero] = useState(false);
  const [offerInView, setOfferInView] = useState(false);

  useEffect(() => {
    const hero = document.getElementById("topo");
    const offer = document.getElementById("oferta");
    if (!hero || !offer) return;

    const heroObserver = new IntersectionObserver(([entry]) => setPastHero(!entry.isIntersecting));
    const offerObserver = new IntersectionObserver(([entry]) => setOfferInView(entry.isIntersecting));
    heroObserver.observe(hero);
    offerObserver.observe(offer);

    return () => {
      heroObserver.disconnect();
      offerObserver.disconnect();
    };
  }, []);

  if (!pastHero || offerInView) return null;

  const price = formatPrice(siteConfig.price.total, siteConfig.price.locale, siteConfig.price.currency);

  return (
    <div
      className="fixed inset-x-0 bottom-0 z-40 border-t border-ink-200 bg-white px-4 pt-3 shadow-[0_-8px_24px_rgba(27,38,49,0.08)] md:hidden"
      style={{ paddingBottom: "calc(env(safe-area-inset-bottom) + 0.75rem)" }}
    >
      <div className="mx-auto flex max-w-md flex-col gap-2">
        <p className="text-center text-sm font-semibold text-ink-700">
          {price} no total · pagamento único
        </p>
        <CheckoutButton label="Quero meu Kit Férias Criativas" size="lg" className="w-full" />
      </div>
    </div>
  );
}
