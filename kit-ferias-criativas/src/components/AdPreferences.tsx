import { useEffect, useState } from "react";
import { siteConfig } from "../config/site";
import { initMetaPixel } from "../lib/metaPixel";
export function AdPreferences() {
  const [open, setOpen] = useState(false);
  useEffect(() => {
    try { setOpen(Boolean(siteConfig.metaPixelId) && !localStorage.getItem("ad-consent")); } catch {}
    const show = () => setOpen(true);
    window.addEventListener("open-privacy", show);
    return () => window.removeEventListener("open-privacy", show);
  }, []);
  function choose(accepted: boolean) {
    try { localStorage.setItem("ad-consent", accepted ? "accepted" : "declined"); } catch {}
    setOpen(false);
    window.dispatchEvent(new Event("ad-consent-closed"));
    if (accepted) initMetaPixel();
    else if (window.fbq) window.location.reload();
  }
  if (!open) return null;
  return <div role="dialog" aria-label="Preferências de anúncios" className="fixed inset-x-3 bottom-3 z-50 mx-auto max-w-2xl rounded-2xl border border-ink-200 bg-white p-5 shadow-2xl"><p className="font-bold">Medição de anúncios</p><p className="mt-2 text-sm text-ink-700">Podemos usar o Pixel da Meta para medir visitas e cliques de anúncios. Você escolhe se permite. <a href="#privacidade" className="underline">Ver privacidade</a></p><div className="mt-4 flex gap-3"><button className="rounded-full border border-primary-700 px-5 py-2 font-bold text-primary-700" onClick={() => choose(false)}>Recusar</button><button className="rounded-full bg-primary-700 px-5 py-2 font-bold text-white" onClick={() => choose(true)}>Aceitar</button></div></div>;
}
