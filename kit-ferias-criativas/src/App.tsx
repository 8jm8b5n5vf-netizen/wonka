import { useEffect } from "react";
import { Header } from "./components/Header";
import { Hero } from "./components/Hero";
import { PainSection } from "./components/PainSection";
import { Benefits } from "./components/Benefits";
import { HowItWorks } from "./components/HowItWorks";
import { Categories } from "./components/Categories";
import { Bonuses } from "./components/Bonuses";
import { ProductPreview } from "./components/ProductPreview";
import { Offer } from "./components/Offer";
import { FAQ } from "./components/FAQ";
import { FinalCta } from "./components/FinalCta";
import { Footer } from "./components/Footer";
import { StickyBuyBar } from "./components/StickyBuyBar";
import { initMetaPixel, trackMetaEvent } from "./lib/metaPixel";

export default function App() {
  useEffect(() => {
    initMetaPixel();
    trackMetaEvent("ViewContent");
  }, []);

  return (
    <>
      <Header />
      <main>
        <Hero />
        <PainSection />
        <Benefits />
        <HowItWorks />
        <Categories />
        <Bonuses />
        <ProductPreview />
        <Offer />
        <FAQ />
        <FinalCta />
      </main>
      <Footer />
      <StickyBuyBar />
    </>
  );
}
