import { categories } from "../data/content";
import { SectionHeading } from "./SectionHeading";

export function Categories() {
  return (
    <section id="conteudo" className="scroll-mt-20 bg-white py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="O que está incluso"
          title="Atividades por categoria para diferentes momentos"
          description="Escolha a categoria certa para o momento e para a idade de cada criança."
        />

        <ul className="mt-12 grid grid-cols-2 gap-4 sm:grid-cols-3 sm:gap-6 lg:grid-cols-4">
          {categories.map((category) => (
            <li
              key={category.name}
              className="flex flex-col overflow-hidden rounded-3xl border border-ink-200 bg-paper transition duration-300 hover:-translate-y-1 hover:border-primary-500 hover:shadow-md"
            >
              <img
                src={category.cover}
                alt={`Capa do módulo ${category.name}`}
                width={1131}
                height={1600}
                loading="lazy"
                decoding="async"
                className="h-auto w-full"
              />
              <div className="p-4 sm:p-5">
                <h3 className="text-base font-bold text-ink-900 sm:text-lg">{category.name}</h3>
                <p className="mt-1 text-sm text-ink-700">{category.description}</p>
              </div>
            </li>
          ))}
        </ul>
      </div>
    </section>
  );
}
