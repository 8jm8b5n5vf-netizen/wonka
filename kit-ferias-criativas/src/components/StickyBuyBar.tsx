import { useEffect, useState } from "react";
import { siteConfig } from "../config/site";
import { formatPrice } from "../lib/format";
import { CheckoutButton } from "./CheckoutButton";

const BLOCKING_SECTION_IDS = ["oferta"];

export function StickyBuyBar() {
  const [pastHero, setPastHero] = useState(false);
  const [blockingVisible, setBlockingVisible] = useState(false);
  const [dialogOpen, setDialogOpen] = useState(false);

  useEffect(() => {
    const hero = document.getElementById("compra-topo");
    if (!hero) return;

    const heroObserver = new IntersectionObserver(([entry]) => setPastHero(!entry.isIntersecting));
    heroObserver.observe(hero);

    const visibility = new Map<Element, boolean>();
    const blockingObserver = new IntersectionObserver((entries) => {
      entries.forEach((entry) => visibility.set(entry.target, entry.isIntersecting));
      setBlockingVisible([...visibility.values()].some(Boolean));
    });
    BLOCKING_SECTION_IDS.forEach((id) => {
      const el = document.getElementById(id);
      if (el) blockingObserver.observe(el);
    });

    const openDialog = () => setDialogOpen(true);
    const closeDialog = () => setDialogOpen(false);
    window.addEventListener("open-privacy", openDialog);
    window.addEventListener("ad-consent-closed", closeDialog);

    return () => {
      heroObserver.disconnect();
      blockingObserver.disconnect();
      window.removeEventListener("open-privacy", openDialog);
      window.removeEventListener("ad-consent-closed", closeDialog);
    };
  }, []);

  if (!pastHero || blockingVisible || dialogOpen) return null;

  const { value, regularValue, locale, currency } = siteConfig.price;
  const price = formatPrice(value, locale, currency);
  const regularPrice = formatPrice(regularValue, locale, currency);

  return (
    <div
      className="fixed inset-x-0 bottom-0 z-40 border-t border-ink-200 bg-white px-4 pt-3 shadow-[0_-8px_24px_rgba(27,38,49,0.08)] md:hidden"
      style={{ paddingBottom: "calc(env(safe-area-inset-bottom) + 0.75rem)" }}
    >
      <div className="mx-auto flex max-w-md flex-col gap-2">
        <p className="text-center text-sm font-semibold text-ink-700">
          <span className="mr-1.5 text-ink-500 line-through">{regularPrice}</span>
          <span className="text-ink-900">{price}</span> · pagamento único
        </p>
        <CheckoutButton label="Comprar agora" size="lg" className="w-full" />
      </div>
    </div>
  );
}
