# Project memory

## Product intent & audience

- Sell a one-time R$ 29,90 PDF activity kit via external Cakto checkout.
- No own payment implementation; checkout URL injected from config.
- Target: Brazilian parents wanting screen-free home activities during school holidays.

## Brand personality & voice

- Child-friendly, modern, warm, trustworthy.
- Persuasive, natural Brazilian Portuguese copy.
- Joyful but not cluttered; confident without fabricated social proof.

## Visual design & UX direction

- Light background, cheerful colors, legible fonts, discrete illustrations.
- Mobile-first, fully responsive, clean reusable React components.
- Sections: header (name + buy button) → hero → pain → how it works → categories → bonuses (marked in preparation) → product preview (image placeholders) → offer → FAQ → footer (contact, terms, privacy).
- Buy buttons disabled until Cakto URL configured; all links/buttons functional or clearly prepared for config.

## Decisions, constraints & rationale

- Tech: React + TypeScript + Tailwind CSS, in `kit-ferias-criativas/` subfolder (root holds unrelated C++ audio plugin).
- Central config at `src/config/site.ts` for: product name, price, Cakto checkout URL, Meta Pixel ID, contact data.
- Meta Pixel prepared without inventing ID.
- No fabricated testimonials, reviews, certifications, discounts, countdowns, guarantees, or inflated activity counts.
- No fictitious product images; placeholders for real PDF previews.
- Build passes type-check and compile; visual review pending by user.

## Open questions & assumptions

- Cakto checkout URL not yet provided.
- Final age range per activity still unconfirmed; current copy says recommended age will be stated per material.
- FAQ delivery method is generic; must be adjusted to real file delivery flow.
- Legal texts (terms, privacy) are placeholders pending review.
- Contact details and Meta Pixel ID pending from user.