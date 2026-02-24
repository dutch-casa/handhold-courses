import { createContext, use, useState } from "react";

interface AccordionContext {
  readonly openId: string | null;
  readonly toggle: (id: string) => void;
}

const AccordionCtx = createContext<AccordionContext | null>(null);

function useAccordion(): AccordionContext {
  const ctx = use(AccordionCtx);
  if (ctx === null) throw new Error("Accordion compound components must be used within <Accordion>");
  return ctx;
}

function Item({ children }: { readonly children: React.ReactNode }) {
  return <div className="accordion-item">{children}</div>;
}

function Trigger({ id, children }: { readonly id: string; readonly children: React.ReactNode }) {
  const { openId, toggle } = useAccordion();
  const isOpen = openId === id;

  return (
    <button onClick={() => toggle(id)}>
      {children}
      <span style={{ transform: isOpen ? "rotate(180deg)" : "rotate(0deg)" }}>&#x25BE;</span>
    </button>
  );
}

function Content({ id, children }: { readonly id: string; readonly children: React.ReactNode }) {
  const { openId } = useAccordion();
  if (openId !== id) return null;

  return <div className="accordion-content">{children}</div>;
}

export function Accordion({ children }: { readonly children: React.ReactNode }) {
  const [openId, setOpenId] = useState<string | null>(null);

  const toggle = (id: string) => setOpenId((prev) => (prev === id ? null : id));

  return (
    <AccordionCtx value={{ openId, toggle }}>
      <div>{children}</div>
    </AccordionCtx>
  );
}

Accordion.Item = Item;
Accordion.Trigger = Trigger;
Accordion.Content = Content;
