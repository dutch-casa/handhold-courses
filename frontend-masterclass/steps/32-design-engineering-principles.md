---
title: Design engineering principles
---

# What design engineering is

{{show: de-code typewriter 2s linear}} Design engineering lives at the intersection of design and code. It's not "make it pretty." It's engineering visual quality, interaction reliability, and inclusive access into every component. The difference between "it works" and "it feels right."

{{focus: baseline}} {{zoom: 1.2x}} {{pulse: baseline}} {{annotate: baseline "The baseline"}} A polished UI doesn't just look good. It's stable — no layout shift. It's inclusive — keyboard and screen reader accessible. It's responsive — touch-first, hover-enhanced. It's fast — no unnecessary animation or layout thrash. Polish is engineering, not decoration.

{{zoom: 1x}} {{focus: checklist}} {{zoom: 1.2x}} {{pan: checklist}} {{annotate: checklist "The checklist"}} Every component you ship should pass this: no layout shift on dynamic content. Animations respect reduced motion. Touch targets are 44px minimum. Hover effects disabled on touch. Keyboard navigation works. Icon buttons have aria labels. Inputs are 16px+ to prevent iOS zoom.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:de-code lang=tsx
// Design engineering: the checklist
// Every component should pass these before shipping

const CHECKLIST = [
  "No layout shift on dynamic content",
  "Animations have prefers-reduced-motion support",
  "Touch targets are 44px minimum",
  "Hover effects disabled on touch devices",
  "Keyboard navigation works (tab, enter, escape)",
  "Icon buttons have aria-label",
  "Inputs are 16px+ (prevents iOS zoom)",
  "No transition: all (specify exact properties)",
  "z-index uses a fixed scale",
  "Focus states are visible (focus-visible)",
] as const;

// This isn't extra work. This IS the work.
---
baseline: 4-15
checklist: 4-15
```

# No layout shift

{{clear: slide}}

{{show: shift-code typewriter 2s linear}} {{split}} {{show: shift-preview reveal 0.3s spring}} Layout shift happens when elements change size or position after the user sees them. An image loads and pushes text down. A font swaps and headings resize. A badge appears and buttons jump. Every shift erodes trust.

{{focus: problem}} {{zoom: 1.2x}} {{pulse: problem}} {{annotate: problem "Shifting content"}} A counter changes from "9" to "10" — the extra digit widens the element. Everything to the right shifts. A notification badge appears — the nav items scoot over. Dynamic content without reserved space means movement the user didn't cause.

{{zoom: 1x}} {{focus: fix}} {{zoom: 1.2x}} {{annotate: fix "Hardcoded dimensions"}} Fix it with reserved space. `min-width` on counters. `aspect-ratio` on images. `min-height` on containers that load content. The space is always there. When content fills it, nothing moves.

{{zoom: 1x}} {{focus: tabular}} {{zoom: 1.2x}} {{pulse: tabular}} {{annotate: tabular "Tabular numbers"}} `font-variant-numeric: tabular-nums` — every digit takes the same width. "111" and "999" are the same size. Counters, timers, prices — any changing number needs tabular figures or it dances on screen.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:shift-code lang=tsx
// BAD: layout shift on dynamic content
function BadCounter({ count }: { readonly count: number }) {
  return <span>{count}</span>; // width changes with digit count
}

// GOOD: reserved space, tabular numbers
function GoodCounter({ count }: { readonly count: number }) {
  return (
    <span style={{
      minWidth: "3ch",
      fontVariantNumeric: "tabular-nums",
      textAlign: "right",
      display: "inline-block",
    }}>
      {count}
    </span>
  );
}

// GOOD: image with aspect-ratio (no shift on load)
function Avatar({ src, alt }: { readonly src: string; readonly alt: string }) {
  return (
    <img
      src={src}
      alt={alt}
      width={48}
      height={48}
      style={{ aspectRatio: "1", objectFit: "cover", borderRadius: "50%" }}
    />
  );
}
---
problem: 2-4
fix: 7-17
tabular: 11
```

```preview:shift-preview
<style>
  .counter-bad { font-size: 2rem; background: #fee; padding: 0.5rem 1rem; border-radius: 8px; }
  .counter-good { font-size: 2rem; background: #efe; padding: 0.5rem 1rem; border-radius: 8px; min-width: 4ch; font-variant-numeric: tabular-nums; text-align: right; display: inline-block; }
  .demo-row { display: flex; gap: 2rem; align-items: center; padding: 1rem; }
  .label { font-size: 0.875rem; color: #666; }
</style>
<div class="demo-row">
  <div>
    <div class="label">Shifts width:</div>
    <span class="counter-bad">9</span>
    <span class="counter-bad">99</span>
    <span class="counter-bad">999</span>
  </div>
  <div>
    <div class="label">Stable width:</div>
    <span class="counter-good">9</span>
    <span class="counter-good">99</span>
    <span class="counter-good">999</span>
  </div>
</div>
```

# Touch-first, hover-enhanced

{{clear: slide}}

{{show: touch-code typewriter 2s linear}} Design for fingers first. Then add hover effects for mouse users. Not the other way around. Hover doesn't exist on phones. If functionality requires hover, mobile users can't access it.

{{focus: targets}} {{zoom: 1.2x}} {{pulse: targets}} {{annotate: targets "44px minimum"}} Apple's Human Interface Guidelines: 44pt minimum tap target. That's roughly 44px on a standard screen. An icon button that's 24px wide needs padding to reach 44px. The visual element can be small — the tap area must be large.

{{zoom: 1x}} {{focus: media}} {{zoom: 1.2x}} {{annotate: media "Hover media query"}} `@media (hover: hover)` — only apply hover styles when a pointing device can hover. Phones return false. Mice return true. Wrap all hover effects in this query. On touch devices, `:hover` states stick after tap — ugly and confusing.

{{zoom: 1x}} {{focus: pattern}} {{zoom: 1.2x}} {{pulse: pattern}} {{pan: pattern}} {{annotate: pattern "The pattern"}} Base styles work everywhere. Touch targets are always large enough. Then `@media (hover: hover)` adds hover effects. The mobile experience is clean. The desktop experience is enhanced. Nobody is left out.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:touch-code lang=css
/* Touch-first: base styles for everyone */
.icon-button {
  /* Visual size: 24px icon */
  width: 24px;
  height: 24px;

  /* Tap target: 44px minimum */
  padding: 10px;
  /* Total: 24 + 10 + 10 = 44px */

  border: none;
  background: transparent;
  cursor: pointer;
  border-radius: 8px;
}

/* Hover-enhanced: only for devices that support hover */
@media (hover: hover) {
  .icon-button:hover {
    background: rgba(0, 0, 0, 0.05);
  }
}

/* NEVER do this: hover without media query */
/* .icon-button:hover { background: ...; } */
/* On touch: hover state sticks after tap */

/* Focus for keyboard users */
.icon-button:focus-visible {
  outline: 2px solid #2563eb;
  outline-offset: 2px;
}
---
targets: 2-14
media: 17-21
pattern: 17-30
```

# Keyboard navigation

{{clear: slide}}

{{show: keyboard-code typewriter 2s linear}} If you can't Tab to it, press Enter on it, and Escape out of it, it's broken. Keyboard navigation isn't an edge case — it's how power users, screen reader users, and anyone with a broken trackpad uses your app.

{{focus: tab}} {{zoom: 1.2x}} {{pulse: tab}} {{annotate: tab "Tab order"}} Tab moves through interactive elements in DOM order. Only interactive elements should be in the tab order: buttons, links, inputs. Non-interactive elements with `tabIndex={0}` are usually wrong. If it doesn't do anything on click, it shouldn't be tabbable.

{{zoom: 1x}} {{focus: focus}} {{zoom: 1.2x}} {{annotate: focus "Focus visible"}} `:focus-visible` shows focus rings only for keyboard users, not mouse clicks. Use it instead of `:focus`. Never `outline: none` without a replacement. The focus ring is how keyboard users know where they are.

{{zoom: 1x}} {{focus: escape}} {{zoom: 1.2x}} {{pulse: escape}} {{pan: escape}} {{annotate: escape "Escape to close"}} Modals, dropdowns, popovers — Escape closes them. After closing, return focus to the element that opened them. This is "focus management." Without it, keyboard users are lost in the DOM after a dialog closes.

{{pan: none}} {{zoom: 1x}} {{focus: scroll}} {{zoom: 1.2x}} {{annotate: scroll "Scroll into view"}} When keyboard focus moves to an element off-screen, scroll it into view. `element.scrollIntoView({ block: "nearest" })` ensures the focused element is visible. Tab to something below the fold? It should scroll into view automatically.

{{zoom: 1x}} {{focus: none}}

```code:keyboard-code lang=tsx
// Keyboard navigation patterns

// Focus management: return focus after dialog close
function Dialog({ isOpen, onClose, trigger }: DialogProps) {
  const dialogRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    if (isOpen) {
      dialogRef.current?.focus();
    } else {
      trigger.current?.focus(); // return focus to trigger
    }
  }, [isOpen, trigger]);

  if (!isOpen) return null;

  return (
    <div
      ref={dialogRef}
      role="dialog"
      aria-modal="true"
      tabIndex={-1}
      onKeyDown={(e) => {
        if (e.key === "Escape") onClose();
      }}
    >
      {/* dialog content */}
      <button onClick={onClose}>Close</button>
    </div>
  );
}

// Scroll into view on keyboard navigation
function ListItem({ isFocused, children }: ListItemProps) {
  const ref = useRef<HTMLLIElement>(null);

  useEffect(() => {
    if (isFocused) {
      ref.current?.scrollIntoView({ block: "nearest" });
    }
  }, [isFocused]);

  return <li ref={ref} tabIndex={isFocused ? 0 : -1}>{children}</li>;
}
---
tab: 3-13
focus: 18-27
escape: 23-25
scroll: 33-41
```

# Accessibility by default

{{clear: slide}}

{{show: a11y-code typewriter 2s linear}} Accessibility isn't a feature you add later. It's a baseline you start with. Semantic HTML, ARIA attributes where needed, and respecting user preferences. Every component, every time.

{{focus: aria}} {{zoom: 1.2x}} {{pulse: aria}} {{annotate: aria "ARIA labels"}} Every icon button needs an `aria-label`. A magnifying glass icon is meaningless to a screen reader without `aria-label="Search"`. Every decorative image needs `alt=""` (empty, not missing). Every form input needs an associated label.

{{zoom: 1x}} {{focus: reduced}} {{zoom: 1.2x}} {{annotate: reduced "Reduced motion"}} `prefers-reduced-motion: reduce` — some users get motion sick. Respect it. Animations become instant transitions. Parallax effects stop. Carousels pause. This isn't optional. Every animation needs a reduced-motion fallback.

{{zoom: 1x}} {{focus: live}} {{zoom: 1.2x}} {{pulse: live}} {{pan: live}} {{annotate: live "Live regions"}} `aria-live="polite"` announces dynamic changes to screen readers. A toast notification, a form error, a loading complete message — sighted users see it, screen reader users hear it. Without `aria-live`, dynamic updates are invisible to assistive tech.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:a11y-code lang=tsx
// Icon button: always needs aria-label
function IconButton({
  icon,
  label,
  onClick,
}: {
  readonly icon: React.ReactNode;
  readonly label: string;
  readonly onClick: () => void;
}) {
  return (
    <button onClick={onClick} aria-label={label}>
      {icon}
    </button>
  );
}

// Reduced motion: every animation needs this
const fadeIn = `
  @keyframes fadeIn {
    from { opacity: 0; transform: translateY(8px); }
    to   { opacity: 1; transform: translateY(0); }
  }

  @media (prefers-reduced-motion: reduce) {
    * { animation-duration: 0.01ms !important; }
  }
`;

// Live region: announce dynamic changes
function Toast({ message }: { readonly message: string }) {
  return (
    <div role="alert" aria-live="polite">
      {message}
    </div>
  );
}

// Error announcement
function FormError({ error }: { readonly error: string | null }) {
  if (!error) return null;
  return (
    <p role="alert" aria-live="assertive" className="error">
      {error}
    </p>
  );
}
---
aria: 2-15
reduced: 19-27
live: 30-44
```

# The z-index scale

{{clear: slide}}

{{show: zindex-code typewriter 2s linear}} `z-index: 9999` is not a strategy. It's a band-aid that escalates into an arms race. `z-index: 99999`. `z-index: 2147483647`. The solution is a fixed scale and stacking context isolation.

{{focus: scale}} {{zoom: 1.2x}} {{pulse: scale}} {{annotate: scale "Fixed scale"}} Define named layers with fixed values. Dropdowns at 100. Modals at 200. Toasts at 300. Every component uses the scale, never a random number. Conflicts become impossible because every layer has an assigned altitude.

{{zoom: 1x}} {{focus: isolate}} {{zoom: 1.2x}} {{annotate: isolate "Isolation"}} `isolation: isolate` creates a new stacking context. A component's internal z-index values can't leak out and compete with the rest of the page. The component is self-contained. Internal layering stays internal.

{{zoom: 1x}} {{focus: none}}

```code:zindex-code lang=css
/* Fixed z-index scale — use CSS custom properties */
:root {
  --z-dropdown: 100;
  --z-sticky: 200;
  --z-modal-backdrop: 300;
  --z-modal: 400;
  --z-toast: 500;
}

/* Use the scale */
.dropdown { z-index: var(--z-dropdown); }
.modal-backdrop { z-index: var(--z-modal-backdrop); }
.modal { z-index: var(--z-modal); }
.toast { z-index: var(--z-toast); }

/* Isolation: contain z-index within a component */
.card {
  isolation: isolate;
  /* Internal z-index stays inside this stacking context */
}

.card-overlay {
  z-index: 1; /* Only competes within .card, not the page */
}

/* NEVER do this */
/* .my-thing { z-index: 9999; } */
/* .my-other-thing { z-index: 99999; } */
/* It never ends. Use the scale. */
---
scale: 2-8
isolate: 17-24
```

# Spacing, shadows, and consistency

{{clear: slide}}

{{show: system-code typewriter 2s linear}} {{split}} {{show: system-preview reveal 0.3s spring}} Polish comes from consistency. When every spacing value, shadow, and border radius follows a shared scale, the result feels intentional — even if the user can't articulate why. Random values produce random-feeling UIs. Constrained values produce calm ones.

{{focus: spacing}} {{zoom: 1.2x}} {{pulse: spacing}} {{annotate: spacing "Consistent spacing"}} Pick a base unit and stick to it. Multiples of 4 work well: 4, 8, 12, 16, 24, 32, 48, 64. Define them as custom properties and use nothing else. When every gap, padding, and margin comes from the same scale, elements align to an invisible grid. The eye picks up on that alignment even when the brain doesn't.

{{zoom: 1x}} {{focus: shadow-borders}} {{zoom: 1.2x}} {{annotate: shadow-borders "Shadow borders"}} Use `box-shadow` instead of `border` for element boundaries. A shadow border like `box-shadow: 0 0 0 1px rgba(0,0,0,0.08)` blends with any background. Normal borders sit on top and look harsh. On retina screens, you can use 0.5px for hairline precision. This is how polished interfaces achieve that "it just looks right" effect.

{{zoom: 1x}} {{focus: layered}} {{zoom: 1.2x}} {{pulse: layered}} {{annotate: layered "Layered shadows"}} A single `box-shadow` looks like a sticker. Layered shadows look like real depth. Two or three shadows: a tight contact shadow plus a softer ambient shadow. Each layer is a different spread and opacity. The combination mimics how light actually hits a surface. In dark mode, replace shadows with subtle light borders — shadows are invisible on dark backgrounds.

{{zoom: 1x}} {{focus: radius}} {{zoom: 1.2x}} {{annotate: radius "Radius hierarchy"}} Border radius follows a hierarchy. Cards and containers get the largest radius. Buttons and inputs inside them get less. Badges and pills get full rounding. Inner elements always use a smaller radius than their parent — never the same or larger. Define three or four sizes as custom properties and use nothing else.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:system-code lang=css
/* --- Consistent scales --- */

:root {
  /* Spacing: pick a unit, use multiples */
  --space-1: 4px;
  --space-2: 8px;
  --space-3: 12px;
  --space-4: 16px;
  --space-6: 24px;
  --space-8: 32px;
  --space-12: 48px;
  --space-16: 64px;

  /* Shadows: shadow borders > real borders */
  --border-shadow: 0 0 0 1px rgba(0, 0, 0, 0.08);

  /* Layered shadows: contact + ambient */
  --shadow-sm:
    0 0 0 1px rgba(0, 0, 0, 0.03),
    0 1px 2px rgba(0, 0, 0, 0.06);
  --shadow-md:
    0 0 0 1px rgba(0, 0, 0, 0.03),
    0 1px 2px rgba(0, 0, 0, 0.06),
    0 4px 8px -2px rgba(0, 0, 0, 0.06);
  --shadow-lg:
    0 0 0 1px rgba(0, 0, 0, 0.03),
    0 2px 4px rgba(0, 0, 0, 0.06),
    0 8px 16px -2px rgba(0, 0, 0, 0.08);

  /* Border radius: container > child */
  --radius-sm: 6px;   /* buttons, inputs */
  --radius-md: 10px;  /* cards, panels */
  --radius-lg: 14px;  /* modals, dialogs */
  --radius-full: 9999px; /* pills, avatars */
}

/* Usage */
.card {
  padding: var(--space-6);
  border-radius: var(--radius-md);
  box-shadow: var(--shadow-md);
}

.card button {
  padding: var(--space-2) var(--space-4);
  border-radius: var(--radius-sm); /* smaller than parent */
}
---
spacing: 3-13
shadow-borders: 16
layered: 19-29
radius: 32-36
```

```preview:system-preview
<style>
  .demo { display: flex; gap: 24px; padding: 24px; flex-wrap: wrap; align-items: flex-start; }
  .shadow-col { display: flex; flex-direction: column; gap: 12px; }
  .label { font-size: 10px; color: #64748b; font-family: monospace; margin-bottom: 4px; }
  .card-demo { width: 160px; height: 56px; background: #fff; display: flex; align-items: center; justify-content: center; font-size: 11px; color: #64748b; }
  .border-real { border: 1px solid #d1d5db; border-radius: 10px; }
  .border-shadow { box-shadow: 0 0 0 1px rgba(0,0,0,0.08); border-radius: 10px; }
  .sm { box-shadow: 0 0 0 1px rgba(0,0,0,0.03), 0 1px 2px rgba(0,0,0,0.06); border-radius: 10px; }
  .md { box-shadow: 0 0 0 1px rgba(0,0,0,0.03), 0 1px 2px rgba(0,0,0,0.06), 0 4px 8px -2px rgba(0,0,0,0.06); border-radius: 10px; }
  .lg { box-shadow: 0 0 0 1px rgba(0,0,0,0.03), 0 2px 4px rgba(0,0,0,0.06), 0 8px 16px -2px rgba(0,0,0,0.08); border-radius: 10px; }
</style>
<div class="demo">
  <div class="shadow-col">
    <div class="label">border vs shadow border</div>
    <div class="card-demo border-real">border: 1px</div>
    <div class="card-demo border-shadow">shadow border</div>
  </div>
  <div class="shadow-col">
    <div class="label">layered shadows</div>
    <div class="card-demo sm">sm</div>
    <div class="card-demo md">md</div>
    <div class="card-demo lg">lg</div>
  </div>
</div>
```
