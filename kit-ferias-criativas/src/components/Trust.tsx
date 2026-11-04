import { SectionHeading } from "./SectionHeading";
export function Trust() {
  const items = [
    ["22 cadernos organizados", "Seis módulos com letras, números, traçados, leituras e jogos. Você escolhe por onde começar."],
    ["Uma atividade por vez", "Baixe o caderno e selecione as páginas no leitor de PDF. A impressão pode ser feita em casa ou numa gráfica."],
    ["Um começo guiado", "O guia Comece aqui acompanha orientações e uma primeira atividade para imprimir."],
    ["Dois bônus incluídos", "Calendário de 30 dias e guia para os pais para ajudar na escolha, com flexibilidade para a sua rotina."],
  ];
  return <section className="bg-primary-50 py-16 sm:py-20">
    <div className="mx-auto max-w-6xl px-4 sm:px-6">
      <SectionHeading eyebrow="Veja antes de escolher" title="Você sabe o que vai receber" description="As capas e as folhas desta página são dos cadernos que compõem o kit." />
      <div className="mt-10 grid gap-5 sm:grid-cols-2 lg:grid-cols-4">{items.map(([title, body]) => <article key={title} className="rounded-3xl bg-white p-6 ring-1 ring-primary-100"><h3 className="text-lg font-bold text-primary-700">{title}</h3><p className="mt-3 text-ink-700">{body}</p></article>)}</div>
      <div className="mt-8 rounded-3xl bg-white p-6 sm:p-8"><h3 className="text-xl font-bold text-ink-900">Combina com a sua família?</h3><p className="mt-3 text-ink-700">Para responsáveis que querem propostas em papel e podem acompanhar a criança. Escolha pelo interesse e pelas habilidades que ela já explora. A impressão e os materiais, como lápis e massinha, ficam por sua conta. Um adulto acompanha recortes e peças pequenas.</p><a className="mt-4 inline-block font-bold text-primary-700 underline" href="#amostras">Abra as amostras e veja os enunciados</a></div>
    </div>
  </section>;
}
