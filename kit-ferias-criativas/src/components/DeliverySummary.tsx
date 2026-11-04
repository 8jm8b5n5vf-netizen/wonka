import { deliveryFacts } from "../data/content";

export function DeliverySummary() {
  return (
    <section aria-label="Resumo da entrega" className="bg-paper py-10 sm:py-14">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <ul className="grid gap-4 sm:grid-cols-3">
          {deliveryFacts.map((fact) => (
            <li key={fact.title} className="rounded-2xl bg-white p-5 text-center ring-1 ring-ink-200 sm:p-6">
              <p className="font-display text-base font-extrabold text-primary-700 sm:text-lg">{fact.title}</p>
              <p className="mt-1 text-sm text-ink-700">{fact.text}</p>
            </li>
          ))}
        </ul>
      </div>
    </section>
  );
}
