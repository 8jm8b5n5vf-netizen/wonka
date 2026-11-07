import { CheckoutButton } from "./CheckoutButton";
import { SectionHeading } from "./SectionHeading";

export function Guidance() {
  return (
    <section className="bg-paper py-16 sm:py-24">
      <div className="mx-auto max-w-3xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="Orientação para escolher"
          title="O material combina com a criança?"
          description="A escolha considera o interesse e as habilidades já exploradas, sem uma faixa etária rígida."
        />

        <div className="mt-10 rounded-3xl bg-white p-6 ring-1 ring-ink-200 sm:p-8">
          <ul className="space-y-4 text-ink-700">
            <li className="flex gap-3">
              <span aria-hidden="true" className="mt-1 h-2 w-2 shrink-0 rounded-full bg-primary-600" />
              <p><span className="font-semibold text-ink-900">Quem está começando:</span> experimente traçar uma letra, identificar vogais ou contar figuras.</p>
            </li>
            <li className="flex gap-3">
              <span aria-hidden="true" className="mt-1 h-2 w-2 shrink-0 rounded-full bg-primary-600" />
              <p><span className="font-semibold text-ink-900">Quem já junta sílabas:</span> veja as pequenas leituras e as atividades de palavras.</p>
            </li>
            <li className="flex gap-3">
              <span aria-hidden="true" className="mt-1 h-2 w-2 shrink-0 rounded-full bg-primary-600" />
              <p><span className="font-semibold text-ink-900">Para variar:</span> quebra-cabeças e jogos de associação dão pausas lúdicas entre as folhas de traçado.</p>
            </li>
          </ul>
          <p className="mt-6 text-sm text-ink-500">
            A impressão e os materiais, como lápis e massinha, ficam por sua conta. Um adulto acompanha recortes e peças pequenas.
          </p>
          <a className="mt-4 inline-block font-bold text-primary-700 underline" href="#amostras">Abra as amostras e veja os enunciados</a>
        </div>

        <div className="mt-8 flex justify-center">
          <CheckoutButton label="Quero meu Kit Férias Criativas" size="lg" className="w-full sm:w-auto" />
        </div>
      </div>
    </section>
  );
}
