import { siteConfig } from "../config/site";

type Fbq = ((...args: unknown[]) => void) & { queue?: unknown[][]; loaded?: boolean; version?: string };

declare global {
  interface Window {
    fbq?: Fbq;
  }
}

const pixelId = siteConfig.metaPixelId.trim();

export function initMetaPixel(): void {
  if (!pixelId || typeof window === "undefined" || window.fbq) return;

  const queue: unknown[][] = [];
  const fbq: Fbq = (...args: unknown[]) => {
    queue.push(args);
  };
  fbq.queue = queue;
  fbq.loaded = true;
  fbq.version = "2.0";
  window.fbq = fbq;

  const script = document.createElement("script");
  script.async = true;
  script.src = "https://connect.facebook.net/en_US/fbevents.js";
  document.head.appendChild(script);

  fbq("init", pixelId);
  fbq("track", "PageView");
}

export function trackMetaEvent(eventName: string): void {
  if (!pixelId || typeof window === "undefined" || !window.fbq) return;
  window.fbq("track", eventName);
}
