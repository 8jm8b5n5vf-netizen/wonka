import { categories } from "../data/content";
import { SectionHeading } from "./SectionHeading";

const symbolColors = [
  "bg-sun-100 text-ink-900",
  "bg-coral-100 text-coral-600",
  "bg-sky-100 text-sky-600",
  "bg-primary-50 text-primary-700",
  "bg-sun-100 text-ink-900",
  "bg-sky-100 text-sky-600",
  "bg-coral-100 text-coral-600",
];

export function Categories() {
  return (
    <section id="conteudo" className="scroll-mt-20 bg-white py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="O que está incluso"
          title="Atividades por categoria para diferentes momentos"
          description="Escolha a categoria certa para o momento e para a idade de cada criança."
        />

        <ul className="mt-12 grid grid-cols-1 gap-4 sm:grid-cols-2 lg:grid-cols-4">
          {categories.map((category, index) => (
            <li
              key={category.name}
              className="flex items-start gap-4 rounded-3xl border border-ink-200 bg-paper p-5 transition duration-300 hover:-translate-y-1 hover:border-primary-500 hover:shadow-md"
            >
              <span
                className={`flex h-14 min-w-14 items-center justify-center rounded-2xl px-3 font-display text-lg font-extrabold ${symbolColors[index]}`}
                aria-hidden="true"
              >
                {category.symbol}
              </span>
              <div>
                <h3 className="text-lg font-bold text-ink-900">{category.name}</h3>
                <p className="mt-1 text-sm text-ink-700">{category.description}</p>
              </div>
            </li>
          ))}
        </ul>
      </div>
    </section>
  );
}
