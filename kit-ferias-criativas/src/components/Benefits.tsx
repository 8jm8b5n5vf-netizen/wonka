import { benefits } from "../data/content";
import { SectionHeading } from "./SectionHeading";

const icons = {
  smartphone: (
    <>
      <rect x="7" y="3" width="10" height="18" rx="2" />
      <path d="M11 18h2" />
    </>
  ),
  printer: (
    <>
      <path d="M7 9V3h10v6" />
      <rect x="4" y="9" width="16" height="8" rx="2" />
      <path d="M7 14h10v7H7z" />
    </>
  ),
  book: (
    <>
      <path d="M12 6c-2-1.5-5-2-8-2v13c3 0 6 .5 8 2 2-1.5 5-2 8-2V4c-3 0-6 .5-8 2z" />
      <path d="M12 6v13" />
    </>
  ),
  calendar: (
    <>
      <rect x="3" y="5" width="18" height="16" rx="2" />
      <path d="M3 10h18M8 3v4M16 3v4" />
    </>
  ),
} as const;

export function Benefits() {
  return (
    <section className="bg-white py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="Por que escolher"
          title="Mais tempo para descansar, mais momentos de aprendizado"
        />

        <ul className="mt-12 grid gap-6 sm:grid-cols-2">
          {benefits.map((benefit) => (
            <li
              key={benefit.title}
              className="flex gap-5 rounded-3xl border border-ink-200 bg-paper p-6 transition duration-300 hover:-translate-y-1 hover:shadow-md sm:p-8"
            >
              <span className="flex h-14 w-14 shrink-0 items-center justify-center rounded-2xl bg-sky-100 text-sky-600">
                <svg
                  viewBox="0 0 24 24"
                  className="h-7 w-7"
                  fill="none"
                  stroke="currentColor"
                  strokeWidth="2"
                  strokeLinecap="round"
                  strokeLinejoin="round"
                  aria-hidden="true"
                >
                  {icons[benefit.icon]}
                </svg>
              </span>
              <div>
                <h3 className="text-lg font-bold text-ink-900">{benefit.title}</h3>
                <p className="mt-1 text-ink-700">{benefit.text}</p>
              </div>
            </li>
          ))}
        </ul>
      </div>
    </section>
  );
}
