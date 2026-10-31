import { steps } from "../data/content";
import { SectionHeading } from "./SectionHeading";

export function HowItWorks() {
  return (
    <section id="como-funciona" className="scroll-mt-20 bg-paper py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="Como funciona"
          title="Do pagamento à primeira atividade em poucos passos"
          description="Simples para você, divertido para a criança."
        />

        <ol className="mt-12 grid gap-6 sm:grid-cols-2 lg:grid-cols-4">
          {steps.map((step, index) => (
            <li key={step.title} className="relative rounded-3xl bg-white p-6 shadow-sm ring-1 ring-ink-200">
              <span className="flex h-12 w-12 items-center justify-center rounded-2xl bg-primary-600 font-display text-xl font-bold text-white">
                {index + 1}
              </span>
              <h3 className="mt-5 text-lg font-bold text-ink-900">{step.title}</h3>
              <p className="mt-2 text-ink-700">{step.text}</p>
            </li>
          ))}
        </ol>
      </div>
    </section>
  );
}
