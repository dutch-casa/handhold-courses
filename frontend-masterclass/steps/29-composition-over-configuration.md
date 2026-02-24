---
title: Composition over configuration
---

# The boolean prop trap

{{show: bool-code typewriter 2s linear}} {{show: bool-diagram reveal 0.3s spring}} A component starts simple. One variant needs a border. Add `hasBorder`. Another needs an icon. Add `hasIcon`. Dark mode? `isDark`. Loading? `isLoading`. Compact layout? `isCompact`. Dismissable? `isDismissable`.

{{focus: booleans}} {{zoom: 1.2x}} {{pulse: booleans}} {{annotate: booleans "Six booleans"}} Six booleans. Two to the sixth power. Sixty-four possible states. `hasBorder + isDark + isLoading + isCompact` — is that a valid combination? What about `isDismissable + isLoading`? Nobody knows. The component handles some combinations, ignores others, and crashes on a few nobody tested.

{{zoom: 1x}} {{focus: impl}} {{zoom: 1.2x}} {{pan: impl}} {{annotate: impl "Branching explosion"}} Inside the component: `if (hasBorder && !isDark)`, `if (isCompact && hasIcon)`, `if (isLoading && !isDismissable)`. The logic is a thicket of conditionals. Every new boolean doubles the branches. Every bug report requires tracing through the combinatorial maze.

{{pan: none}} {{zoom: 1x}} {{focus: none}} This is configuration-driven design. It doesn't scale.

```code:bool-code lang=tsx
// The boolean prop trap
interface AlertProps {
  readonly message: string;
  readonly hasBorder?: boolean;
  readonly hasIcon?: boolean;
  readonly isDark?: boolean;
  readonly isLoading?: boolean;
  readonly isCompact?: boolean;
  readonly isDismissable?: boolean;
}

// 6 booleans = 64 possible states
// Most are nonsensical. Some cause bugs.

function Alert({
  message,
  hasBorder,
  hasIcon,
  isDark,
  isLoading,
  isCompact,
  isDismissable,
}: AlertProps) {
  return (
    <div
      className={`alert
        ${hasBorder ? "alert-bordered" : ""}
        ${isDark ? "alert-dark" : ""}
        ${isCompact ? "alert-compact" : ""}
      `}
    >
      {isLoading && <Spinner />}
      {hasIcon && !isLoading && <Icon />}
      <span>{message}</span>
      {isDismissable && !isLoading && <CloseButton />}
    </div>
  );
}
---
booleans: 2-10
impl: 15-33
```

```diagram:bool-diagram kind=graph
Bool1 [label="hasBorder" shape=box]
Bool2 [label="hasIcon" shape=box]
Bool3 [label="isDark" shape=box]
Bool4 [label="isLoading" shape=box]
Bool5 [label="isCompact" shape=box]
Bool6 [label="isDismissable" shape=box]
States [label="2^6 = 64 states\nMost are bugs" shape=diamond]

Bool1 -> States
Bool2 -> States
Bool3 -> States
Bool4 -> States
Bool5 -> States
Bool6 -> States
```

# Composition: the alternative

{{clear: slide}}

{{show: compose-code typewriter 2s linear}} Instead of configuring one monolithic component, compose small parts. Each part does one thing. The parent assembles them into the exact combination it needs. No boolean props. No impossible states.

{{focus: parts}} {{zoom: 1.2x}} {{pulse: parts}} {{annotate: parts "Small parts"}} `Alert.Icon`, `Alert.Body`, `Alert.Dismiss` — each is a tiny component. None of them know about each other. They just render their piece. The parent decides which parts to include.

{{zoom: 1x}} {{focus: usage}} {{zoom: 1.2x}} {{pulse: usage}} {{pan: usage}} {{annotate: usage "Explicit assembly"}} Want an alert with an icon and dismiss button? Include both. Want a plain text alert? Just `Alert.Body`. Want a loading alert? Replace the icon with a spinner. Each combination is explicit in the JSX. No guessing which booleans to set.

{{pan: none}} {{zoom: 1x}} {{focus: variant}} {{zoom: 1.2x}} {{annotate: variant "Explicit variants"}} Instead of `isDark`, create `<DarkAlert>`. Instead of `isCompact`, create `<CompactAlert>`. Each variant is its own component with its own logic. No branching inside a shared component. No impossible state combinations.

{{zoom: 1x}} {{focus: none}}

```code:compose-code lang=tsx
// Composition: small parts, explicit assembly

function AlertIcon({ icon }: { readonly icon: React.ReactNode }) {
  return <div className="alert-icon">{icon}</div>;
}

function AlertBody({ children }: { readonly children: React.ReactNode }) {
  return <div className="alert-body">{children}</div>;
}

function AlertDismiss({ onDismiss }: { readonly onDismiss: () => void }) {
  return <button className="alert-dismiss" onClick={onDismiss}>×</button>;
}

function Alert({ children }: { readonly children: React.ReactNode }) {
  return <div className="alert">{children}</div>;
}

// Usage: explicit, no booleans
function WarningWithDismiss() {
  return (
    <Alert>
      <AlertIcon icon={<WarningIcon />} />
      <AlertBody>Something went wrong.</AlertBody>
      <AlertDismiss onDismiss={() => console.log("dismissed")} />
    </Alert>
  );
}

// Plain text — no icon, no dismiss
function SimpleAlert() {
  return (
    <Alert>
      <AlertBody>Just a message.</AlertBody>
    </Alert>
  );
}

// Loading — spinner replaces icon
function LoadingAlert() {
  return (
    <Alert>
      <AlertIcon icon={<Spinner />} />
      <AlertBody>Loading data...</AlertBody>
    </Alert>
  );
}
---
parts: 3-17
usage: 21-28
variant: 31-46
```

# Compound components

{{clear: slide}}

{{show: compound-code typewriter 2s linear}} Compound components take composition further. The parts share state through context. The parent component is a provider. The children are consumers. They work together but are composed freely.

{{focus: provider}} {{zoom: 1.2x}} {{pulse: provider}} {{annotate: provider "Context provider"}} `Accordion` creates context with the current open item and a toggle function. It doesn't render any items — it just provides the shared state. The children decide what to render.

{{zoom: 1x}} {{focus: item}} {{zoom: 1.2x}} {{annotate: item "Item reads context"}} `Accordion.Item` wraps each accordion section. `Accordion.Trigger` reads context to know if it's the open item and to toggle on click. `Accordion.Content` reads context to decide whether to render its children.

{{zoom: 1x}} {{focus: assembly}} {{zoom: 1.2x}} {{pulse: assembly}} {{pan: assembly}} {{annotate: assembly "Composed freely"}} The consumer decides the structure. Put an icon in the trigger. Add a footer in the content. Wrap items in a custom container. The compound component doesn't care — it manages state, the children manage rendering.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:compound-code lang=tsx
import { createContext, use, useState } from "react";

interface AccordionContext {
  readonly openId: string | null;
  readonly toggle: (id: string) => void;
}

const AccordionCtx = createContext<AccordionContext | null>(null);

function useAccordion() {
  const ctx = use(AccordionCtx);
  if (!ctx) throw new Error("useAccordion must be inside Accordion");
  return ctx;
}

function Accordion({ children }: { readonly children: React.ReactNode }) {
  const [openId, setOpenId] = useState<string | null>(null);
  const toggle = (id: string) =>
    setOpenId((prev) => (prev === id ? null : id));

  return (
    <AccordionCtx value={{ openId, toggle }}>
      <div className="accordion">{children}</div>
    </AccordionCtx>
  );
}

function Item({ id, children }: { readonly id: string; readonly children: React.ReactNode }) {
  return <div className="accordion-item" data-id={id}>{children}</div>;
}

function Trigger({ id, children }: { readonly id: string; readonly children: React.ReactNode }) {
  const { openId, toggle } = useAccordion();
  return (
    <button className="accordion-trigger" onClick={() => toggle(id)}>
      {children} {openId === id ? "▲" : "▼"}
    </button>
  );
}

function Content({ id, children }: { readonly id: string; readonly children: React.ReactNode }) {
  const { openId } = useAccordion();
  if (openId !== id) return null;
  return <div className="accordion-content">{children}</div>;
}

// Attach parts to namespace
Accordion.Item = Item;
Accordion.Trigger = Trigger;
Accordion.Content = Content;
---
provider: 17-27
item: 29-47
assembly: 50-52
```

# Using compound components

{{clear: slide}}

{{show: usage-code typewriter 2s linear}} The power shows at the call site. Compound components read like a table of contents. The structure is visible in the JSX. No props configure what's inside — the children do.

{{focus: basic-usage}} {{zoom: 1.2x}} {{pulse: basic-usage}} {{annotate: basic-usage "Self-documenting"}} Each `Item` is visible. Each `Trigger` and `Content` is explicit. You can see exactly what the accordion contains by reading the JSX. No boolean props hide the structure.

{{zoom: 1x}} {{focus: custom}} {{zoom: 1.2x}} {{pan: custom}} {{annotate: custom "Custom content"}} Put anything inside the content. An image. A form. Another accordion. The compound component doesn't restrict what goes inside — it only manages the open/close state.

{{pan: none}} {{zoom: 1x}} {{focus: pattern}} {{zoom: 1.2x}} {{pulse: pattern}} {{annotate: pattern "The pattern"}} Provider at the top. Parts read from context. Consumer assembles parts. This pattern works for accordions, tabs, dropdown menus, modals, tooltips — any component where parts need shared state but the layout varies.

{{zoom: 1x}} {{focus: none}}

```code:usage-code lang=tsx
// Clean, self-documenting usage
function FAQ() {
  return (
    <Accordion>
      <Accordion.Item id="what">
        <Accordion.Trigger id="what">
          What is React?
        </Accordion.Trigger>
        <Accordion.Content id="what">
          <p>A JavaScript library for building UI.</p>
        </Accordion.Content>
      </Accordion.Item>

      <Accordion.Item id="why">
        <Accordion.Trigger id="why">
          Why use TypeScript?
        </Accordion.Trigger>
        <Accordion.Content id="why">
          <p>Type safety catches bugs before runtime.</p>
          <p>IDE autocomplete makes you faster.</p>
        </Accordion.Content>
      </Accordion.Item>

      <Accordion.Item id="how">
        <Accordion.Trigger id="how">
          How does composition work?
        </Accordion.Trigger>
        <Accordion.Content id="how">
          <p>Small parts. Shared context. Free assembly.</p>
          <img src="/diagram.png" alt="Composition diagram" />
        </Accordion.Content>
      </Accordion.Item>
    </Accordion>
  );
}

// The pattern works everywhere:
// <Tabs> + Tabs.List + Tabs.Tab + Tabs.Panel
// <Dialog> + Dialog.Trigger + Dialog.Content
// <Menu> + Menu.Button + Menu.Items + Menu.Item
---
basic-usage: 3-33
custom: 28-30
pattern: 37-39
```
