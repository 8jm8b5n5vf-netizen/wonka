type SectionHeadingProps = {
  eyebrow?: string;
  title: string;
  description?: string;
  align?: "left" | "center";
};

export function SectionHeading({ eyebrow, title, description, align = "center" }: SectionHeadingProps) {
  const alignment = align === "center" ? "mx-auto text-center" : "text-left";

  return (
    <div className={`max-w-2xl ${alignment}`}>
      {eyebrow && (
        <p className="mb-3 text-sm font-bold uppercase tracking-wider text-primary-600">{eyebrow}</p>
      )}
      <h2 className="text-3xl font-extrabold text-ink-900 sm:text-4xl">{title}</h2>
      {description && <p className="mt-4 text-lg text-ink-700">{description}</p>}
    </div>
  );
}
