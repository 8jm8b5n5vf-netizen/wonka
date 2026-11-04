import { useEffect } from "react";
import { Trust } from "./components/Trust";
import { AdPreferences } from "./components/AdPreferences";
import { Header } from "./components/Header";
import { Hero } from "./components/Hero";
import { DeliverySummary } from "./components/DeliverySummary";
import { PainSection } from "./components/PainSection";
import { ProductPreview } from "./components/ProductPreview";
import { Categories } from "./components/Categories";
import { HowItWorks } from "./components/HowItWorks";
import { Guidance } from "./components/Guidance";
import { Bonuses } from "./components/Bonuses";
import { Offer } from "./components/Offer";
import { CancellationContact } from "./components/CancellationContact";
import { FAQ } from "./components/FAQ";
import { FinalCta } from "./components/FinalCta";
import { Footer } from "./components/Footer";
import { StickyBuyBar } from "./components/StickyBuyBar";
import { Benefits } from "./components/Benefits";
import { initMetaPixel } from "./lib/metaPixel";

export default function App() {
  useEffect(() => {
    initMetaPixel();
  }, []);

  return (
    <>
      <Header />
      <main>
        <Hero />
        <DeliverySummary />
        <PainSection />
        <ProductPreview />
        <Benefits />
        <Categories />
        <HowItWorks />
        <Guidance />
        <Bonuses />
        <Trust />
        <Offer />
        <CancellationContact />
        <FAQ />
        <FinalCta />
      </main>
      <Footer />
      <StickyBuyBar />
      <AdPreferences />
    </>
  );
}
