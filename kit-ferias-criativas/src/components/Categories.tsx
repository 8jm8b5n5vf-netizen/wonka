import { modules } from "../data/content";
import { SectionHeading } from "./SectionHeading";

export function Categories() {
  return (
    <section id="conteudo" className="scroll-mt-20 bg-paper py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="Conteúdo organizado"
          title="Seis módulos para diferentes momentos das férias"
          description="Cada módulo reúne cadernos por tema. Escolha por onde começar conforme o interesse da criança."
        />

        <ul className="mt-12 grid gap-6 sm:grid-cols-2 lg:grid-cols-3">
          {modules.map((module) => (
            <li
              key={module.name}
              className="flex flex-col rounded-3xl border border-ink-200 bg-white p-5 transition duration-300 hover:-translate-y-1 hover:border-primary-500 hover:shadow-md"
            >
              <div className="flex justify-center gap-3">
                {module.covers.map((cover) => (
                  <img
                    key={cover}
                    src={cover}
                    alt={`Capa do módulo ${module.name}`}
                    width={1131}
                    height={1600}
                    loading="lazy"
                    decoding="async"
                    className={`h-auto min-w-0 rounded-xl border border-ink-200 ${
                      module.covers.length > 1 ? "w-1/2" : "w-2/3"
                    }`}
                  />
                ))}
              </div>
              <h3 className="mt-4 text-lg font-bold text-ink-900">{module.name}</h3>
              <p className="mt-1 text-sm text-ink-700">{module.description}</p>
              <p className="mt-3 text-sm font-semibold text-primary-700">{module.example}</p>
            </li>
          ))}
        </ul>
      </div>
    </section>
  );
}
