import { samples } from "../data/content";
import { SectionHeading } from "./SectionHeading";

export function ProductPreview() {
  return (
    <section id="amostras" className="scroll-mt-20 bg-white py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="Prévia"
          title="Veja como as atividades ficam no papel"
          description="Toque numa folha para abrir a amostra em tamanho maior."
        />

        <ul className="mt-12 grid grid-cols-2 gap-4 sm:grid-cols-3 sm:gap-6 lg:grid-cols-6">
          {samples.map((sample) => (
            <li key={sample.src} className="group">
              <a href={sample.src} target="_blank" rel="noopener noreferrer" aria-label={sample.alt + " (abrir em nova aba)"}>
              <img
                src={sample.src}
                alt={sample.alt}
                width={1131}
                height={1600}
                loading="lazy"
                decoding="async"
                className="h-auto w-full rounded-2xl border border-ink-200 bg-paper shadow-sm transition duration-300 group-hover:-translate-y-1 group-hover:shadow-lg"
              />
              </a>
            </li>
          ))}
        </ul>
      </div>
    </section>
  );
}
