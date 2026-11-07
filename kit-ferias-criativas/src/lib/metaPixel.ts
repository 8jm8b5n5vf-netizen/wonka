import { siteConfig } from "../config/site";
type Fbq = ((...args: unknown[]) => void) & { queue?: unknown[][]; loaded?: boolean; version?: string; callMethod?: (...args: unknown[]) => void; push?: Fbq };
declare global { interface Window { fbq?: Fbq; _fbq?: Fbq; } }
export function hasAdConsent(): boolean {
  try { return localStorage.getItem("ad-consent") === "accepted"; } catch { return false; }
}
export function initMetaPixel(): void {
  const id = siteConfig.metaPixelId.trim();
  if (!id || !hasAdConsent() || window.fbq) return;
  const fbq: Fbq = (...args) => { if (fbq.callMethod) fbq.callMethod(...args); else fbq.queue?.push(args); };
  fbq.queue = []; fbq.loaded = true; fbq.version = "2.0"; fbq.push = fbq;
  window.fbq = fbq; window._fbq = fbq;
  const script = document.createElement("script");
  script.async = true; script.src = "https://connect.facebook.net/en_US/fbevents.js";
  document.head.appendChild(script);
  fbq("init", id); fbq("track", "PageView"); fbq("track", "ViewContent");
}
export function trackMetaEvent(name: string): void {
  if (siteConfig.metaPixelId.trim() && hasAdConsent()) window.fbq?.("track", name);
}
