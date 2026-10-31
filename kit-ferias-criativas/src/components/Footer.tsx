import { siteConfig } from "../config/site";

const contactItems = [
  { label: "E-mail", value: siteConfig.contact.email, href: siteConfig.contact.email ? `mailto:${siteConfig.contact.email}` : "" },
  { label: "WhatsApp", value: siteConfig.contact.whatsapp, href: "" },
  { label: "Empresa", value: siteConfig.contact.companyName, href: "" },
  { label: "CNPJ", value: siteConfig.contact.cnpj, href: "" },
];

export function Footer() {
  return (
    <footer className="bg-ink-900 pb-24 text-white md:pb-0">
      <div className="mx-auto grid max-w-6xl gap-10 px-4 py-12 sm:px-6 md:grid-cols-3">
        <div>
          <p className="font-display text-2xl font-extrabold">{siteConfig.productName}</p>
          <p className="mt-3 text-sm text-ink-200">Atividades educativas em PDF para imprimir e brincar nas férias.</p>
        </div>

        <div>
          <h2 className="font-display text-lg font-bold">Contato</h2>
          <ul className="mt-3 space-y-2 text-sm text-ink-200">
            {contactItems.map((item) => (
              <li key={item.label}>
                {item.value ? (
                  item.href ? (
                    <a href={item.href} className="underline-offset-4 hover:text-sun-300 hover:underline">
                      {item.value}
                    </a>
                  ) : (
                    <span>
                      {item.label}: {item.value}
                    </span>
                  )
                ) : (
                  <span>
                    {item.label}: em breve
                  </span>
                )}
              </li>
            ))}
          </ul>
        </div>

        <div>
          <h2 className="font-display text-lg font-bold">Legal</h2>
          <ul className="mt-3 space-y-2 text-sm text-ink-200">
            <li>
              <a href={siteConfig.legal.termsUrl} className="underline-offset-4 hover:text-sun-300 hover:underline">
                Termos de uso
              </a>
            </li>
            <li>
              <a href={siteConfig.legal.privacyUrl} className="underline-offset-4 hover:text-sun-300 hover:underline">
                Política de privacidade
              </a>
            </li>
          </ul>
        </div>
      </div>

      <div id="termos" className="scroll-mt-20 border-t border-ink-700">
        <div className="mx-auto max-w-6xl space-y-6 px-4 py-10 text-sm text-ink-200 sm:px-6">
          <div id="privacidade" className="scroll-mt-20">
            <h2 className="font-display text-lg font-bold text-white">Política de privacidade</h2>
            <p className="mt-2">Texto em revisão. Será publicado antes do lançamento.</p>
          </div>
          <div>
            <h2 className="font-display text-lg font-bold text-white">Termos de uso</h2>
            <p className="mt-2">Texto em revisão. Será publicado antes do lançamento.</p>
          </div>
          <p className="pt-4 text-xs text-ink-200/70">
            © {new Date().getFullYear()} {siteConfig.productName}. Todos os direitos reservados.
          </p>
        </div>
      </div>
    </footer>
  );
}
