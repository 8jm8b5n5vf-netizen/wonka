import { SectionHeading } from "./SectionHeading";

type PreviewSlot = {
  label: string;
  imageUrl?: string;
};

const previewSlots: PreviewSlot[] = [
  { label: "Prévia do PDF: Alfabeto" },
  { label: "Prévia do PDF: Números" },
  { label: "Prévia do PDF: Quebra-cabeças" },
];

export function ProductPreview() {
  return (
    <section className="bg-white py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="Prévia"
          title="Veja como as atividades ficam no papel"
          description="Algumas páginas do material, para você conhecer o formato antes de comprar."
        />

        <ul className="mt-12 grid gap-6 sm:grid-cols-3">
          {previewSlots.map((slot) => (
            <li key={slot.label}>
              <figure className="group">
                <div className="flex aspect-[3/4] items-center justify-center overflow-hidden rounded-3xl border-2 border-dashed border-primary-500/40 bg-primary-50 transition duration-300 group-hover:-translate-y-1 group-hover:shadow-lg">
                  {slot.imageUrl ? (
                    <img src={slot.imageUrl} alt={slot.label} loading="lazy" className="h-full w-full object-cover" />
                  ) : (
                    <span className="px-6 text-center text-sm font-semibold text-primary-700">
                      Espaço para a imagem real do PDF
                    </span>
                  )}
                </div>
                <figcaption className="mt-3 text-center text-sm font-semibold text-ink-700">{slot.label}</figcaption>
              </figure>
            </li>
          ))}
        </ul>
      </div>
    </section>
  );
}
