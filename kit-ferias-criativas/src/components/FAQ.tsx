import { faqs } from "../data/content";
import { SectionHeading } from "./SectionHeading";

export function FAQ() {
  return (
    <section id="perguntas" className="scroll-mt-20 bg-white py-16 sm:py-24">
      <div className="mx-auto max-w-3xl px-4 sm:px-6">
        <SectionHeading title="Perguntas frequentes" description="Tire suas dúvidas antes de comprar." />

        <div className="mt-12 divide-y divide-ink-200 rounded-3xl border border-ink-200 bg-paper">
          {faqs.map((faq) => (
            <details id={faq.question.startsWith("Como peço reembolso") ? "reembolso" : undefined} key={faq.question} className="group px-6 py-5 [&_summary::-webkit-details-marker]:hidden">
              <summary className="flex cursor-pointer list-none items-center justify-between gap-4 text-left text-lg font-bold text-ink-900">
                {faq.question}
                <span
                  aria-hidden="true"
                  className="flex h-8 w-8 shrink-0 items-center justify-center rounded-full bg-primary-100 text-primary-700 transition-transform duration-300 group-open:rotate-45"
                >
                  +
                </span>
              </summary>
              <p className="mt-3 pr-12 text-ink-700">{faq.answer} {faq.question.startsWith("Como peço reembolso") && <a className="font-semibold underline" href="https://reembolso.cakto.com.br/" target="_blank" rel="noopener noreferrer">Abrir portal da Cakto</a>}</p>
            </details>
          ))}
        </div>
      </div>
    </section>
  );
}
