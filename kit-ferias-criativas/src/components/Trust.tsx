import { SectionHeading } from "./SectionHeading";

export function Trust() {
  return (
    <section className="bg-primary-50 py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="Credibilidade"
          title="Você sabe o que vai receber"
          description="As capas e as folhas desta página são dos cadernos reais que compõem o kit."
        />

        <div className="mt-12 grid gap-5 sm:grid-cols-2 lg:grid-cols-4">
          {[
            ["22 cadernos reais", "Seis módulos com letras, números, traçados, leituras e jogos. As amostras mostram páginas dos arquivos."],
            ["Entrega explicada", "Após o pagamento aprovado, o e-mail leva à área de membros da Cakto para baixar os PDFs."],
            ["Começo guiado", "O guia Comece aqui e o índice ajudam a encontrar a primeira atividade sem se perder."],
            ["Compra transparente", "R$ 29,90 em pagamento único, sem assinatura nem mensalidade."],
          ].map(([title, body]) => (
            <article key={title} className="rounded-3xl bg-white p-6 ring-1 ring-primary-100">
              <h3 className="text-base font-bold text-primary-700 sm:text-lg">{title}</h3>
              <p className="mt-3 text-sm text-ink-700">{body}</p>
            </article>
          ))}
        </div>

        <p className="mx-auto mt-8 max-w-2xl text-center text-sm text-ink-700">
          Depoimentos e avaliações serão exibidos somente quando houver relatos reais e autorização. Não criamos avaliações, estrelas ou números de clientes.
        </p>
      </div>
    </section>
  );
}
