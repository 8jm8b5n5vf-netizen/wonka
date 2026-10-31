import { painPoints } from "../data/content";
import { SectionHeading } from "./SectionHeading";

export function PainSection() {
  return (
    <section className="bg-white py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="Você se identifica?"
          title="Férias longas pedem ideias para casa"
          description="Quando a rotina muda, a criança fica agitada e o responsável precisa de opções práticas que caibam no dia a dia."
        />

        <div className="mt-12 grid gap-6 md:grid-cols-3">
          {painPoints.map((item, index) => (
            <article
              key={item.title}
              className="rounded-3xl border border-ink-200 bg-paper p-6 transition duration-300 hover:-translate-y-1 hover:shadow-lg sm:p-8"
            >
              <span className="flex h-11 w-11 items-center justify-center rounded-full bg-coral-100 font-display text-lg font-bold text-coral-600">
                {index + 1}
              </span>
              <h3 className="mt-5 text-xl font-bold text-ink-900">{item.title}</h3>
              <p className="mt-3 text-ink-700">{item.text}</p>
            </article>
          ))}
        </div>
      </div>
    </section>
  );
}
