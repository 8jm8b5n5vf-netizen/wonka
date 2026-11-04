import { useEffect, useState } from "react";
import { siteConfig } from "../config/site";
import { CheckoutButton } from "./CheckoutButton";

const navLinks = [
  { href: "#como-funciona", label: "Como funciona" },
  { href: "#conteudo", label: "Conteúdo" },
  { href: "#amostras", label: "Amostras" },
  { href: "#oferta", label: "Oferta" },
  { href: "#perguntas", label: "Perguntas" },
];

export function Header() {
  const [scrolled, setScrolled] = useState(false);

  useEffect(() => {
    const onScroll = () => setScrolled(window.scrollY > 16);
    onScroll();
    window.addEventListener("scroll", onScroll, { passive: true });
    return () => window.removeEventListener("scroll", onScroll);
  }, []);

  return (
    <header
      className={`fixed inset-x-0 top-0 z-50 transition-all duration-300 ${
        scrolled ? "border-b border-ink-200 bg-white/95 shadow-sm backdrop-blur" : "bg-paper/80 backdrop-blur"
      }`}
    >
      <div className="mx-auto flex h-16 max-w-6xl items-center justify-between gap-3 px-4 sm:px-6">
        <a href="#topo" className="min-w-0 leading-tight">
          <span className="block font-display text-sm font-bold text-primary-600 sm:text-base">{siteConfig.brand}</span>
          <span className="block font-display text-base font-extrabold text-ink-900 sm:text-xl">{siteConfig.productName}</span>
        </a>

        <nav aria-label="Principal" className="hidden items-center gap-6 lg:flex">
          {navLinks.map((link) => (
            <a
              key={link.href}
              href={link.href}
              className="text-sm font-semibold text-ink-700 transition-colors hover:text-primary-600"
            >
              {link.label}
            </a>
          ))}
        </nav>

        <CheckoutButton label="Quero meu kit" size="md" className="shrink-0 whitespace-nowrap px-4 py-2 text-sm sm:px-5 sm:text-base" />
      </div>
    </header>
  );
}
