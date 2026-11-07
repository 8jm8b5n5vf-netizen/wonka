import { siteConfig } from "../config/site";

export function Footer() {
  return (
    <footer className="bg-ink-900 pb-40 text-white md:pb-0">
      <div className="mx-auto grid max-w-6xl gap-8 px-4 py-12 sm:px-6 md:grid-cols-3">
        <div>
          <p className="font-display text-xl font-extrabold">{siteConfig.brand}</p>
          <p className="mt-3 text-sm text-ink-200">
            {siteConfig.productName}. Materiais digitais para imprimir e compartilhar momentos em família.
          </p>
        </div>

        <div>
          <h2 className="font-bold">Atendimento ao comprador</h2>
          <p className="mt-3 text-sm text-ink-200">
            O contato do produtor está disponível no e-mail de confirmação da compra.
          </p>
          {siteConfig.contact.email && (
            <a className="mt-3 block underline" href={`mailto:${siteConfig.contact.email}`}>{siteConfig.contact.email}</a>
          )}
          {siteConfig.contact.cnpj && <p className="mt-2 text-sm">{siteConfig.contact.cnpj}</p>}
          <a className="mt-3 block text-sm underline" href={siteConfig.refundUrl} target="_blank" rel="noopener noreferrer">
            Solicitar reembolso na Cakto
          </a>
        </div>

        <nav className="flex flex-col items-start gap-3 text-sm">
          <a href="#termos" className="underline">Condições do kit</a>
          <a href="#privacidade" className="underline">Privacidade</a>
          <a href="#reembolso" className="underline">Cancelamento e reembolso</a>
          <button type="button" className="underline" onClick={() => window.dispatchEvent(new Event("open-privacy"))}>
            Preferências de anúncios
          </button>
        </nav>
      </div>

      <div className="mx-auto max-w-6xl border-t border-ink-700 px-4 py-8 text-sm text-ink-200 sm:px-6">
        <details id="termos" className="scroll-mt-24 py-4">
          <summary className="cursor-pointer font-bold text-white">Condições do kit</summary>
          <div className="mt-4 space-y-3">
            <p>O Kit Férias Criativas é um produto digital com 22 cadernos em PDF e materiais de apoio. As imagens desta página mostram exemplos dos arquivos. A compra tem pagamento único, no valor exibido nesta página.</p>
            <p>O pagamento é processado pela Cakto. Com a aprovação, as instruções de acesso são enviadas por e-mail. É necessário um dispositivo que abra PDFs. Papel, impressão e materiais de uso nas atividades são providenciados pelo comprador.</p>
            <p>Os arquivos destinam-se ao uso pessoal e familiar. O acompanhamento de um responsável permite adaptar as propostas à criança. Este material não promete resultados ou prazos de aprendizagem.</p>
            <p>Para cancelamento no prazo de sete dias, consulte a seção de cancelamento e reembolso e o portal da Cakto. O contato do produtor acompanha a confirmação do pedido.</p>
          </div>
        </details>

        <details id="privacidade" className="scroll-mt-24 border-t border-ink-700 py-4">
          <summary className="cursor-pointer font-bold text-white">Privacidade</summary>
          <div className="mt-4 space-y-3">
            <p>Esta página apresenta o kit e direciona a compra para a Cakto. Os dados do pedido e do pagamento são preenchidos no checkout, sujeito às políticas daquela plataforma. Esta página não contém formulário para recolher esses dados.</p>
            <p>Quando você abre o checkout, os parâmetros de campanha UTM presentes no endereço podem acompanhar o link para identificar a origem da visita. Sua preferência de anúncios fica salva neste navegador.</p>
            <p>Se o Pixel da Meta estiver configurado e você aceitar a medição de anúncios, ele poderá registrar a visita, a visualização do produto e o clique para iniciar a compra. Você pode recusar ou mudar a preferência pelo botão deste rodapé. A hospedagem também pode processar dados técnicos de conexão.</p>
            <p>Para questões relacionadas ao pedido ou aos dados fornecidos na compra, use o contato do produtor presente na confirmação e a política da Cakto no checkout.</p>
          </div>
        </details>

        <p className="mt-6 text-xs">© {new Date().getFullYear()} {siteConfig.brand} · {siteConfig.productName}</p>
      </div>
    </footer>
  );
}
