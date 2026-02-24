import { createContext, use, useState } from "react";

// TODO: Define AccordionContext interface
// - readonly openId: string | null
// - readonly toggle: (id: string) => void

// TODO: Create AccordionCtx with createContext<AccordionContext | null>(null)

// TODO: Create useAccordion hook that reads context with use()
// Throw an error if context is null (used outside Accordion)

// TODO: Create Accordion component (the provider)
// - useState for openId (string | null, starts null)
// - toggle function: setOpenId(prev => prev === id ? null : id)
// - Wrap children in <AccordionCtx value={{ openId, toggle }}>

// TODO: Create Item component
// - Just wraps children in a <div className="accordion-item">

// TODO: Create Trigger component
// - Props: { id: string; children: React.ReactNode }
// - Reads context with useAccordion()
// - <button onClick={() => toggle(id)}>{children}</button>

// TODO: Create Content component
// - Props: { id: string; children: React.ReactNode }
// - Reads context, returns null if openId !== id
// - Otherwise renders children in a <div className="accordion-content">

// TODO: Attach parts to Accordion namespace
// Accordion.Item = Item;
// Accordion.Trigger = Trigger;
// Accordion.Content = Content;

export function Accordion({ children }: { readonly children: React.ReactNode }) {
  return <div>{children}</div>;
}
