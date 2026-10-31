import { useEffect } from "react";
import { Header } from "./components/Header";
import { Hero } from "./components/Hero";
import { PainSection } from "./components/PainSection";
import { HowItWorks } from "./components/HowItWorks";
import { Categories } from "./components/Categories";
import { Bonuses } from "./components/Bonuses";
import { ProductPreview } from "./components/ProductPreview";
import { Offer } from "./components/Offer";
import { FAQ } from "./components/FAQ";
import { Footer } from "./components/Footer";
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
        <PainSection />
        <HowItWorks />
        <Categories />
        <Bonuses />
        <ProductPreview />
        <Offer />
        <FAQ />
      </main>
      <Footer />
    </>
  );
}
