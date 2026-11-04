import { siteConfig } from "../config/site";
import { resources, startResources } from "../data/content";
import { SectionHeading } from "./SectionHeading";

export function Bonuses() {
  const visible = resources.filter((r) => siteConfig.bonuses[r.id].enabled);

  return (
    <section className="bg-white py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="Para acompanhar as férias"
          title="Recursos de apoio incluídos no kit"
          description="Dois bônus e dois recursos de orientação para facilitar o primeiro acesso e a escolha das atividades."
        />

        <ul className="mt-12 grid gap-6 sm:grid-cols-2 lg:grid-cols-4">
          {visible.map((bonus) => (
            <li key={bonus.id} className="rounded-3xl border-2 border-sun-500 bg-sun-100/60 p-6">
              <span className="inline-block rounded-full bg-white px-3 py-1 text-xs font-bold uppercase tracking-wide text-warning-600">
                Bônus incluído
              </span>
              <h3 className="mt-4 text-lg font-bold text-ink-900">{bonus.title}</h3>
              <p className="mt-2 text-sm text-ink-700">{bonus.text}</p>
            </li>
          ))}
          {startResources.map((resource) => (
            <li key={resource.title} className="rounded-3xl border border-ink-200 bg-paper p-6">
              <span className="inline-block rounded-full bg-primary-100 px-3 py-1 text-xs font-bold uppercase tracking-wide text-primary-700">
                Recurso de orientação
              </span>
              <h3 className="mt-4 text-lg font-bold text-ink-900">{resource.title}</h3>
              <p className="mt-2 text-sm text-ink-700">{resource.text}</p>
            </li>
          ))}
        </ul>
      </div>
    </section>
  );
}
