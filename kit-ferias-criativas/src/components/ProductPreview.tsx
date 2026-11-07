import { useCallback, useEffect, useRef, useState } from "react";
import { samples } from "../data/content";
import { SectionHeading } from "./SectionHeading";

export function ProductPreview() {
  const [openIndex, setOpenIndex] = useState<number | null>(null);
  const triggerRefs = useRef<(HTMLButtonElement | null)[]>([]);
  const closeButtonRef = useRef<HTMLButtonElement>(null);

  const close = useCallback(() => setOpenIndex(null), []);

  useEffect(() => {
    if (openIndex === null) return;

    closeButtonRef.current?.focus();
    document.body.style.overflow = "hidden";

    const onKey = (e: KeyboardEvent) => {
      if (e.key === "Escape") close();
      if (e.key === "Tab") e.preventDefault();
    };
    document.addEventListener("keydown", onKey);

    return () => {
      document.removeEventListener("keydown", onKey);
      document.body.style.overflow = "";
      triggerRefs.current[openIndex]?.focus();
    };
  }, [openIndex, close]);

  const activeSample = openIndex !== null ? samples[openIndex] : null;

  return (
    <section id="amostras" className="scroll-mt-20 bg-white py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="Demonstração real"
          title="Veja como as atividades ficam no papel"
          description="Toque ou clique numa folha para abrir a amostra ampliada e conferir os enunciados."
        />

        <ul className="mt-12 grid grid-cols-2 gap-4 sm:grid-cols-3 sm:gap-6">
          {samples.map((sample, index) => (
            <li key={sample.src}>
              <button
                ref={(el) => {
                  triggerRefs.current[index] = el;
                }}
                type="button"
                onClick={() => setOpenIndex(index)}
                aria-label={`${sample.alt}, abrir ampliada`}
                className="group block w-full rounded-2xl focus:outline-none focus-visible:ring-4 focus-visible:ring-primary-100"
              >
                <img
                  src={sample.src}
                  alt=""
                  width={1131}
                  height={1600}
                  loading={index < 2 ? "eager" : "lazy"}
                  decoding="async"
                  className="h-auto w-full rounded-2xl border border-ink-200 bg-paper shadow-sm transition duration-300 group-hover:-translate-y-1 group-hover:shadow-lg"
                />
                <span className="mt-2 block text-center text-sm font-semibold text-ink-700">{sample.caption}</span>
              </button>
            </li>
          ))}
        </ul>
      </div>

      {activeSample && (
        <div
          role="dialog"
          aria-modal="true"
          aria-label={activeSample.alt}
          onClick={close}
          className="fixed inset-0 z-[60] flex items-center justify-center bg-ink-900/80 p-4"
        >
          <div className="relative flex max-h-full flex-col items-center" onClick={(e) => e.stopPropagation()}>
            <button
              ref={closeButtonRef}
              type="button"
              onClick={close}
              aria-label="Fechar amostra"
              className="absolute -top-3 -right-3 z-10 flex h-11 w-11 items-center justify-center rounded-full bg-white text-xl text-ink-900 shadow-lg ring-2 ring-ink-200 focus:outline-none focus-visible:ring-4 focus-visible:ring-primary-100"
            >
              ✕
            </button>
            <img
              src={activeSample.src}
              alt={activeSample.alt}
              width={1131}
              height={1600}
              className="max-h-[75vh] w-auto max-w-full rounded-2xl bg-paper shadow-2xl"
            />
            <p className="mt-3 text-center text-sm font-semibold text-white">{activeSample.caption}</p>
          </div>
        </div>
      )}
    </section>
  );
}
