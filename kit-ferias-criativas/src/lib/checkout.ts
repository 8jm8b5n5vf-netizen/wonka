import { siteConfig } from "../config/site";

export function getCheckoutHref(): string {
  const base = siteConfig.checkoutUrl.trim();
  if (!base || typeof window === "undefined") return base;

  try {
    const url = new URL(base);
    new URLSearchParams(window.location.search).forEach((value, key) => {
      if (key.startsWith("utm_") && !url.searchParams.has(key)) url.searchParams.set(key, value);
    });
    return url.toString();
  } catch {
    return base;
  }
}
