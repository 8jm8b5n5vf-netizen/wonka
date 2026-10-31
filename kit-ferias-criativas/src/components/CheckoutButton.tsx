import { siteConfig } from "../config/site";
import { trackMetaEvent } from "../lib/metaPixel";

type CheckoutButtonProps = {
  label: string;
  size?: "md" | "lg";
  className?: string;
};

const baseClasses =
  "inline-flex items-center justify-center rounded-full font-display font-bold transition duration-200 focus:outline-none focus-visible:ring-4 focus-visible:ring-primary-100";

const sizeClasses = {
  md: "px-5 py-2.5 text-base",
  lg: "px-8 py-4 text-lg sm:text-xl",
};

const enabledClasses =
  "bg-sun-500 text-ink-900 shadow-lg shadow-sun-500/30 hover:-translate-y-0.5 hover:bg-sun-300 hover:shadow-xl active:translate-y-0";

const disabledClasses = "cursor-not-allowed bg-ink-200 text-ink-500";

export function CheckoutButton({ label, size = "md", className = "" }: CheckoutButtonProps) {
  const hasCheckout = siteConfig.checkoutUrl.trim() !== "";
  const classes = [
    baseClasses,
    sizeClasses[size],
    hasCheckout ? enabledClasses : disabledClasses,
    className,
  ].join(" ");

  if (!hasCheckout) {
    return (
      <button
        type="button"
        disabled
        aria-disabled="true"
        title="Link de compra ainda não configurado em src/config/site.ts"
        className={classes}
      >
        {label}
      </button>
    );
  }

  return (
    <a
      href={siteConfig.checkoutUrl}
      target="_blank"
      rel="noopener noreferrer"
      onClick={() => trackMetaEvent("InitiateCheckout")}
      className={classes}
    >
      {label}
    </a>
  );
}
