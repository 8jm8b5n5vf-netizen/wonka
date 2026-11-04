import { steps } from "../data/content";
import { CheckoutButton } from "./CheckoutButton";
import { SectionHeading } from "./SectionHeading";

export function HowItWorks() {
  return (
    <section id="como-funciona" className="scroll-mt-20 bg-white py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="Como funciona"
          title="Do pagamento à primeira atividade"
          description="Três passos para começar a usar o kit nas férias."
        />

        <ol className="mt-12 grid gap-6 md:grid-cols-3">
          {steps.map((step, index) => (
            <li key={step.title} className="rounded-3xl bg-paper p-6 ring-1 ring-ink-200 sm:p-8">
              <span className="flex h-12 w-12 items-center justify-center rounded-2xl bg-primary-600 font-display text-xl font-bold text-white">
                {index + 1}
              </span>
              <h3 className="mt-5 text-lg font-bold text-ink-900">{step.title}</h3>
              <p className="mt-2 text-ink-700">{step.text}</p>
            </li>
          ))}
        </ol>

        <p className="mt-8 text-center text-sm text-ink-500">
          É possível imprimir em casa ou em uma gráfica. A criança pode precisar do acompanhamento de um adulto.
        </p>

        <div className="mt-10 flex justify-center">
          <CheckoutButton label="Quero meu Kit Férias Criativas" size="lg" className="w-full sm:w-auto" />
        </div>
      </div>
    </section>
  );
}
