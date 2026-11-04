import { siteConfig } from "../config/site";
import { bonuses } from "../data/content";
import { SectionHeading } from "./SectionHeading";

export function Bonuses() {
  const visibleBonuses = bonuses.filter((bonus) => siteConfig.bonuses[bonus.id].enabled);

  if (visibleBonuses.length === 0) return null;

  return (
    <section className="bg-paper py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="Para acompanhar as férias"
          title="Dois bônus incluídos no kit"
          description="Escolha a próxima atividade no calendário e adapte cada proposta ao ritmo da criança."
        />

        <ul className="mt-12 grid gap-6 md:grid-cols-2">
          {visibleBonuses.map((bonus) => (
            <li key={bonus.id} className="rounded-3xl border-2 border-dashed border-sun-500 bg-sun-100/60 p-6 sm:p-8">
              <span className="inline-block rounded-full bg-white px-3 py-1 text-xs font-bold uppercase tracking-wide text-warning-600">
                Sem custo adicional
              </span>
              <h3 className="mt-4 text-xl font-bold text-ink-900">{bonus.title}</h3>
              <p className="mt-2 text-ink-700">{bonus.text}</p>
            </li>
          ))}
        </ul>
      </div>
    </section>
  );
}
