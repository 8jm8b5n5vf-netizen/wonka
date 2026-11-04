import { siteConfig } from "../config/site";
import { CheckoutButton } from "./CheckoutButton";

export function FinalCta() {
  return (
    <section className="bg-white py-16 sm:py-20">
      <div className="mx-auto flex max-w-3xl flex-col items-center px-4 text-center sm:px-6">
        <h2 className="text-3xl font-extrabold text-ink-900 sm:text-4xl">
          Escolha a primeira atividade das férias
        </h2>
        <p className="mt-4 text-lg text-ink-700">
          {siteConfig.price.paymentNote}, com 22 cadernos em PDF para imprimir e aproveitar em casa.
        </p>
        <div className="mt-8 w-full sm:w-auto">
          <CheckoutButton label="Quero meu Kit Férias Criativas" size="lg" className="w-full sm:w-auto" />
        </div>
      </div>
    </section>
  );
}
