import { siteConfig } from "../config/site";
import { SectionHeading } from "./SectionHeading";

export function CancellationContact() {
  const { contact } = siteConfig;

  return (
    <section id="reembolso" className="scroll-mt-20 bg-white py-16 sm:py-24">
      <div className="mx-auto max-w-3xl px-4 sm:px-6">
        <SectionHeading
          eyebrow="Cancelamento e atendimento"
          title="Direito de arrependimento em 7 dias"
        />

        <div className="mt-10 space-y-6">
          <div className="rounded-3xl bg-paper p-6 ring-1 ring-ink-200 sm:p-8">
            <h3 className="text-lg font-bold text-ink-900">Como pedir reembolso</h3>
            <p className="mt-3 text-ink-700">
              O art. 49 do Código de Defesa do Consumidor prevê 7 dias para desistir da compra online, a contar da assinatura ou do recebimento do produto ou serviço. Use o e-mail da compra e selecione o pedido no portal de reembolsos da Cakto. O estorno depende da processadora do pagamento e pode levar alguns dias úteis.
            </p>
            <a
              href={siteConfig.refundUrl}
              target="_blank"
              rel="noopener noreferrer"
              className="mt-4 inline-block rounded-full bg-primary-700 px-5 py-2.5 font-bold text-white transition hover:bg-primary-600"
            >
              Abrir portal de reembolsos da Cakto
            </a>
          </div>

          <div className="rounded-3xl bg-paper p-6 ring-1 ring-ink-200 sm:p-8">
            <h3 className="text-lg font-bold text-ink-900">Atendimento</h3>
            <p className="mt-3 text-ink-700">
              Responsável pela venda: <span className="font-semibold text-ink-900">{contact.companyName}</span>.
            </p>
            {contact.email ? (
              <p className="mt-2 text-ink-700">
                E-mail:{" "}
                <a href={`mailto:${contact.email}`} className="font-semibold text-primary-700 underline">
                  {contact.email}
                </a>
              </p>
            ) : (
              <p className="mt-2 text-sm text-ink-500">
                O contato do produtor é enviado no e-mail de confirmação da compra. E-mail de atendimento, CNPJ e endereço serão incluídos antes da publicação.
              </p>
            )}
            {contact.cnpj && <p className="mt-1 text-sm text-ink-700">CNPJ: {contact.cnpj}</p>}
          </div>
        </div>
      </div>
    </section>
  );
}
