---
title: Motion that matters
---

# Why a motion library?

{{show: limits-code typewriter 2s linear}} {{split}} {{show: limits-preview reveal 0.3s spring}} CSS animations handle entering. An element fades in, slides up, scales from center — all straightforward. But three things CSS cannot do. It cannot animate an element leaving — the moment React unmounts a component, it's gone. It cannot animate one element morphing into another across the DOM. And it cannot do real spring physics. Framer Motion fills these gaps.

{{focus: enter}} {{zoom: 1.2x}} {{pulse: enter}} {{annotate: enter "CSS can do this"}} Entering works. `@keyframes fadeIn` plays when the element mounts. CSS handles this fine. No library needed for simple enters.

{{zoom: 1x}} {{focus: exit}} {{zoom: 1.2x}} {{annotate: exit "CSS cannot do this"}} Exiting breaks. When React removes a component from the tree, the DOM node disappears instantly. There's no opportunity for a CSS exit animation — the element is already gone. You'd need to delay unmounting, play the animation, then remove. That's what Framer Motion does automatically.

{{zoom: 1x}} {{focus: shared}} {{zoom: 1.2x}} {{pulse: shared}} {{annotate: shared "CSS cannot do this"}} Shared element transitions are impossible in CSS. A tab underline in one position needs to animate to a new position under a different tab. In CSS, they're two separate elements — no relationship. In Framer Motion, give them the same `layoutId` and the library handles the interpolation.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:limits-code lang=tsx
// What CSS CAN do: enter animation
const fadeIn = `
  @keyframes fadeIn {
    from { opacity: 0; transform: translateY(8px); }
    to   { opacity: 1; transform: translateY(0); }
  }
  .enter { animation: fadeIn 200ms ease-out; }
`;

// What CSS CANNOT do: exit animation
// React unmounts → DOM node gone → no animation
{isVisible && <div className="enter">Now you see me</div>}
// When isVisible becomes false... instant disappearance.

// What CSS CANNOT do: shared element transition
// Tab underline needs to glide between positions
// Two separate DOM elements, no CSS relationship
<div className="tab active">Home</div>
<div className="underline" /> {/* How does this animate? */}
---
enter: 2-7
exit: 10-12
shared: 15-18
```

```preview:limits-preview
<style>
  .demo-col { display: flex; flex-direction: column; gap: 16px; padding: 16px; font-family: system-ui; }
  .tag { font-size: 11px; font-weight: 600; padding: 2px 8px; border-radius: 4px; display: inline-block; width: fit-content; }
  .tag-yes { background: #d1fae5; color: #166534; }
  .tag-no { background: #fee2e2; color: #991b1b; }
  .row { font-size: 13px; color: #374151; }
</style>
<div class="demo-col">
  <div class="row"><span class="tag tag-yes">CSS</span> Enter animations</div>
  <div class="row"><span class="tag tag-no">CSS</span> Exit animations (unmount)</div>
  <div class="row"><span class="tag tag-no">CSS</span> Shared element transitions</div>
  <div class="row"><span class="tag tag-no">CSS</span> Real spring physics</div>
</div>
```

# motion.div — the building block

{{clear: slide}}

{{show: motion-code typewriter 2s linear}} `motion.div` is a drop-in replacement for `div`. It accepts every prop a regular `div` does, plus animation props: `initial`, `animate`, and `exit`. The component renders a normal DOM element. No wrapper divs, no extra nodes. Replace `div` with `motion.div`, add the props, and the element animates.

{{focus: basic}} {{zoom: 1.2x}} {{pulse: basic}} {{annotate: basic "Drop-in replacement"}} `initial` is where the element starts. `animate` is where it goes. Framer Motion interpolates between them. The element fades in, slides up, or scales — whatever properties you set. Only `transform` and `opacity` here. Same performance rule as CSS: skip layout, paint on the GPU.

{{zoom: 1x}} {{focus: transition}} {{zoom: 1.2x}} {{annotate: transition "Transition config"}} The `transition` prop controls timing. Duration, easing, delay. Same concepts as CSS transitions, but expressed as an object. `ease` maps to a cubic-bezier under the hood. For most UI, `ease-out` with 200 milliseconds. Same rules from the design engineering lessons — nothing changes just because the syntax is different.

{{zoom: 1x}} {{focus: whileHover}} {{zoom: 1.2x}} {{pulse: whileHover}} {{annotate: whileHover "Gesture props"}} `whileHover` and `whileTap` animate on gestures. The element returns to its `animate` state when the gesture ends. `whileTap: {{ "{{ scale: 0.97 }}" }}` gives that button press feedback we built with CSS `:active` — but now it's interruptible. Release mid-press and the element springs back smoothly.

{{zoom: 1x}} {{focus: none}}

```code:motion-code lang=tsx
import { motion } from "framer-motion";

// Basic: initial → animate
function FadeIn({ children }: { readonly children: React.ReactNode }) {
  return (
    <motion.div
      initial={{ opacity: 0, y: 8 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.2, ease: "easeOut" }}
    >
      {children}
    </motion.div>
  );
}

// Gesture props: whileHover, whileTap
function Button({ children, onClick }: {
  readonly children: React.ReactNode;
  readonly onClick: () => void;
}) {
  return (
    <motion.button
      onClick={onClick}
      whileHover={{ scale: 1.02 }}
      whileTap={{ scale: 0.97 }}
      transition={{ duration: 0.15, ease: "easeOut" }}
    >
      {children}
    </motion.button>
  );
}
---
basic: 4-13
transition: 9
whileHover: 17-28
```

# AnimatePresence — exit animations

{{clear: slide}}

{{show: presence-code typewriter 2s linear}} The exit problem solved. Wrap conditional renders in `<AnimatePresence>`. Add an `exit` prop to the `motion` component. When the component unmounts, Framer Motion intercepts the removal, plays the exit animation, then removes the DOM node. The child needs a `key` prop so Framer Motion can track which element is leaving.

{{focus: problem}} {{zoom: 1.2x}} {{pulse: problem}} {{annotate: problem "The problem"}} Without `AnimatePresence`, React removes the element from the DOM the instant the condition becomes false. There's no hook, no delay, no chance to animate. The element just vanishes. This is why exit animations are impossible with CSS alone in React.

{{zoom: 1x}} {{focus: solution}} {{zoom: 1.2x}} {{annotate: solution "The solution"}} `AnimatePresence` watches its children. When a child disappears, it keeps the child in the DOM, plays its `exit` animation, then removes it. The `mode` prop controls sequencing: `"sync"` plays enter and exit simultaneously, `"wait"` finishes the exit before starting the enter, `"popLayout"` pops the exiting element out of flow so surrounding elements reflow immediately.

{{zoom: 1x}} {{focus: notification}} {{zoom: 1.2x}} {{pulse: notification}} {{annotate: notification "Real example"}} A notification enters from the right, sits for a few seconds, then slides out to the right. Without `AnimatePresence`, dismissing it means instant disappearance. With it, the notification slides out gracefully and the space collapses. That's the difference between "works" and "feels right."

{{zoom: 1x}} {{focus: none}}

```code:presence-code lang=tsx
import { motion, AnimatePresence } from "framer-motion";

// Without AnimatePresence: instant disappearance
// {isVisible && <div>Poof</div>}

// With AnimatePresence: animated exit
function Notification({
  message,
  isVisible,
}: {
  readonly message: string;
  readonly isVisible: boolean;
}) {
  return (
    <AnimatePresence>
      {isVisible && (
        <motion.div
          key="notification"
          role="alert"
          initial={{ opacity: 0, x: 40 }}
          animate={{ opacity: 1, x: 0 }}
          exit={{ opacity: 0, x: 40 }}
          transition={{ duration: 0.2, ease: "easeOut" }}
        >
          {message}
        </motion.div>
      )}
    </AnimatePresence>
  );
}

// mode="wait": exit finishes before enter starts
// Useful for page transitions, tab content swaps
<AnimatePresence mode="wait">
  <motion.div key={activeTab} ... />
</AnimatePresence>
---
problem: 3-4
solution: 7-27
notification: 14-25
```

# layoutId — shared element transitions

{{clear: slide}}

{{show: layout-code typewriter 2s linear}} {{split}} {{show: layout-preview reveal 0.3s spring}} The technique behind tab underlines on sites like Vercel, Linear, and Raycast. Give two elements the same `layoutId`. When one unmounts and the other mounts, Framer Motion animates between their positions and sizes. The elements don't need to be related in the DOM. They just need the same ID.

{{focus: concept}} {{zoom: 1.2x}} {{pulse: concept}} {{annotate: concept "Same layoutId"}} A tab underline sits below "Home." Click "About." The underline under "Home" and the underline under "About" share `layoutId="underline"`. Framer Motion sees the old one disappearing and the new one appearing, measures both positions, and animates the transition. To the user, a single underline glides between tabs.

{{zoom: 1x}} {{focus: hover}} {{zoom: 1.2x}} {{annotate: hover "Hover highlight"}} Same pattern works for hover highlights. Map over nav items. The hovered item renders a `motion.div` with `layoutId="hoverBg"`. When hover moves to a different item, the background slides smoothly to the new position. One line of code creates an effect that would take complex positioning math in CSS.

{{zoom: 1x}} {{focus: spring}} {{zoom: 1.2x}} {{pulse: spring}} {{annotate: spring "Spring transition"}} Layout animations look best with springs. `type: "spring", bounce: 0, duration: 0.6` gives a smooth glide with no overshoot. A small bounce like `0.15` adds personality. Zero bounce for professional UI, small bounce for playful UI. Never high bounce — it looks like a mistake, not an intention.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:layout-code lang=tsx
import { motion } from "framer-motion";

// Tab bar with animated underline
function TabBar({
  tabs,
  activeTab,
  onSelect,
}: {
  readonly tabs: readonly string[];
  readonly activeTab: string;
  readonly onSelect: (tab: string) => void;
}) {
  return (
    <div style={{ display: "flex", gap: 4 }}>
      {tabs.map((tab) => (
        <button key={tab} onClick={() => onSelect(tab)}>
          {tab}
          {activeTab === tab && (
            <motion.div
              layoutId="underline"
              style={{
                height: 2,
                background: "#3b82f6",
                borderRadius: 1,
              }}
              transition={{
                type: "spring",
                bounce: 0,
                duration: 0.6,
              }}
            />
          )}
        </button>
      ))}
    </div>
  );
}

// Hover highlight with layoutId
function NavItem({ isHovered }: { readonly isHovered: boolean }) {
  return (
    <div style={{ position: "relative" }}>
      {isHovered && (
        <motion.div
          layoutId="hoverBg"
          style={{
            position: "absolute",
            inset: 0,
            background: "rgba(0,0,0,0.05)",
            borderRadius: 8,
          }}
          transition={{ duration: 0.2 }}
        />
      )}
    </div>
  );
}
---
concept: 4-33
hover: 36-51
spring: 27-31
```

```preview:layout-preview
<style>
  .tabs { display: flex; gap: 4px; padding: 16px; font-family: system-ui; }
  .tab { padding: 8px 16px; border: none; background: transparent; cursor: pointer; font-size: 14px; position: relative; color: #374151; }
  .tab.active { color: #3b82f6; font-weight: 500; }
  .tab.active::after { content: ''; position: absolute; bottom: 0; left: 8px; right: 8px; height: 2px; background: #3b82f6; border-radius: 1px; }
  .hint { font-size: 11px; color: #9ca3af; padding: 0 16px; }
</style>
<div>
  <div class="tabs">
    <button class="tab active">Home</button>
    <button class="tab">About</button>
    <button class="tab">Contact</button>
  </div>
  <p class="hint">With layoutId, the underline glides between tabs</p>
</div>
```

# Spring physics

{{clear: slide}}

{{show: spring-code typewriter 2s linear}} Springs feel physical. Easings feel mathematical. A spring-based animation has no fixed duration — it runs until the simulated spring reaches rest. This makes springs ideal for gestures, drag interactions, and anything a user might interrupt mid-animation. The spring maintains its velocity when redirected; a CSS easing restarts from zero.

{{focus: config}} {{zoom: 1.2x}} {{pulse: config}} {{annotate: config "Spring config"}} Two ways to configure springs. The cleaner approach: `duration` and `bounce`. Duration is a guide, not a hard limit — the spring may overshoot and settle after the specified time. Bounce controls overshoot: `0` means no bounce, `0.25` is subtle, `0.5` is playful. The physics approach: `mass`, `stiffness`, `damping`. More control, harder to tune. Start with duration and bounce.

{{zoom: 1x}} {{focus: when}} {{zoom: 1.2x}} {{annotate: when "When to use springs"}} Use springs for drag interactions, `layoutId` transitions, and elements that should feel alive. Use CSS-style easings for micro-interactions like button presses, hover effects, and color changes. Springs add character. Easings add precision. Most UI should use easings. Springs are for the moments that need to feel physical.

{{zoom: 1x}} {{focus: bounce}} {{zoom: 1.2x}} {{pulse: bounce}} {{annotate: bounce "Bounce guidelines"}} Avoid bounce in most product UI. It draws attention, which is good for onboarding and bad for daily tools. Use `bounce: 0` for professional interfaces. `bounce: 0.15` for playful contexts. Never above `0.3` — it looks like a bug, not a feature. Drag-to-dismiss is the exception; bounce communicates the elastic boundary.

{{zoom: 1x}} {{focus: none}}

```code:spring-code lang=tsx
import { motion } from "framer-motion";

// Spring with duration + bounce (recommended)
<motion.div
  animate={{ x: 0 }}
  transition={{
    type: "spring",
    duration: 0.6,
    bounce: 0,
  }}
/>

// Spring with physics params (more control)
<motion.div
  animate={{ scale: 1 }}
  transition={{
    type: "spring",
    mass: 1,
    stiffness: 200,
    damping: 20,
  }}
/>

// When to use springs vs easings:
//
// Springs:
//   - layoutId transitions
//   - Drag interactions
//   - Elements that can be interrupted
//
// Easings:
//   - Button press (100ms ease-out)
//   - Hover effects (150ms ease)
//   - Color transitions (150ms ease)
//   - Tooltips, dropdowns (200ms ease-out)
---
config: 3-11
when: 23-32
bounce: 7-9
```

# useScroll and useTransform

{{clear: slide}}

{{show: scroll-code typewriter 2s linear}} Scroll-driven animations without scroll event listeners. `useScroll()` returns motion values that update as the user scrolls. `useTransform()` maps one range to another. Combine them: as scroll goes from 0 to 100 pixels, shrink a logo from scale 1 to 0.8. No event handlers, no `requestAnimationFrame`, no state updates. The animation runs outside React's render cycle — pure GPU.

{{focus: scroll}} {{zoom: 1.2x}} {{pulse: scroll}} {{annotate: scroll "Scroll motion values"}} `useScroll()` gives you `scrollY`, `scrollX`, `scrollYProgress`, and `scrollXProgress`. The progress values go from 0 to 1 — useful for tracking how far through a page the user has scrolled. The pixel values give you absolute position.

{{zoom: 1x}} {{focus: transform}} {{zoom: 1.2x}} {{annotate: transform "Mapping ranges"}} `useTransform(scrollY, [0, 50], [1, 0.8])` — when scrollY is 0, output 1. When scrollY is 50, output 0.8. Everything in between is interpolated. Pass the result to a motion component's `style` prop. The logo shrinks as you scroll. The nav fades in. A hero image parallaxes. All declarative, all performant.

{{zoom: 1x}} {{focus: reduced}} {{zoom: 1.2x}} {{pulse: reduced}} {{annotate: reduced "Respect reduced motion"}} Scroll animations need `useReducedMotion()` too. If the user prefers reduced motion, pass `0` instead of the motion value. The element stays static. No parallax, no scroll-driven transforms. The content is still there — it just doesn't move.

{{zoom: 1x}} {{focus: none}}

```code:scroll-code lang=tsx
import {
  motion,
  useScroll,
  useTransform,
  useReducedMotion,
} from "framer-motion";

// Scroll-driven nav: logo shrinks, nav slides down
function ShrinkingNav() {
  const shouldReduceMotion = useReducedMotion();
  const { scrollY } = useScroll();

  const logoScale = useTransform(scrollY, [0, 50], [1, 0.8]);
  const navY = useTransform(scrollY, [0, 50], [0, -8]);

  return (
    <nav>
      <motion.img
        src="/logo.svg"
        alt="Logo"
        style={{
          scale: shouldReduceMotion ? 1 : logoScale,
        }}
      />
      <motion.div
        style={{
          y: shouldReduceMotion ? 0 : navY,
        }}
      >
        {/* nav links */}
      </motion.div>
    </nav>
  );
}
---
scroll: 10-12
transform: 14-15
reduced: 22, 28
```

# Reduced motion — still non-negotiable

{{clear: slide}}

{{show: reduced-code typewriter 2s linear}} Same rule as CSS animations: every Framer Motion animation needs a reduced motion fallback. The library provides `useReducedMotion()` — a hook that reads the user's `prefers-reduced-motion` system setting. If true, skip the animation entirely or replace it with an opacity-only transition.

{{focus: hook}} {{zoom: 1.2x}} {{pulse: hook}} {{annotate: hook "The hook"}} `useReducedMotion()` returns a boolean. When true, set `initial` to `false` — this tells Framer Motion to skip the initial animation and render the final state immediately. Or conditionally swap the animation: transforms become opacity changes, slides become fades, springs become instant transitions.

{{zoom: 1x}} {{focus: pattern}} {{zoom: 1.2x}} {{annotate: pattern "The pattern"}} The pattern from Emil: don't just reduce motion — replace it. iOS doesn't remove all animations for reduced motion users. It replaces slides with crossfades. Follow that lead. Replace `x` and `y` transforms with `opacity`. The user still sees a transition. It just doesn't involve spatial movement.

{{zoom: 1x}} {{focus: none}}

```code:reduced-code lang=tsx
import { motion, useReducedMotion } from "framer-motion";

// Pattern: replace transforms with opacity
function SlideIn({ children }: { readonly children: React.ReactNode }) {
  const shouldReduceMotion = useReducedMotion();

  return (
    <motion.div
      initial={
        shouldReduceMotion
          ? { opacity: 0 }
          : { opacity: 0, x: -20 }
      }
      animate={{ opacity: 1, x: 0 }}
      exit={
        shouldReduceMotion
          ? { opacity: 0 }
          : { opacity: 0, x: 20 }
      }
      transition={{ duration: 0.2, ease: "easeOut" }}
    >
      {children}
    </motion.div>
  );
}
---
hook: 5
pattern: 9-19
```

# MotionConfig — compound motion

{{clear: slide}}

{{show: config-code typewriter 2s linear}} Wrap a subtree in `<MotionConfig transition={{...}}>` and every `motion` component inside inherits that transition. Same composition pattern as context providers. Define the animation personality once at the top. Every component below uses it without specifying its own transition prop. Override locally when needed.

{{focus: provider}} {{zoom: 1.2x}} {{pulse: provider}} {{annotate: provider "Composition"}} This is the provider pattern applied to motion. A tab component wraps its children in `MotionConfig` with a spring transition. The tab indicator, the content panel, the hover highlight — they all animate with the same spring. Change the config in one place, every animation updates. No prop drilling, no duplication.

{{zoom: 1x}} {{focus: override}} {{zoom: 1.2x}} {{annotate: override "Local overrides"}} Any `motion` component can override the inherited transition with its own `transition` prop. The config is a default, not a constraint. A button inside the tab component might want a faster, non-spring transition for its press feedback. It specifies its own, and the config doesn't interfere.

{{zoom: 1x}} {{focus: reduced}} {{zoom: 1.2x}} {{pulse: reduced}} {{annotate: reduced "Reduced motion config"}} Combine `MotionConfig` with `useReducedMotion()` to disable all animations in a subtree with a single check. If the user prefers reduced motion, set `transition={{ duration: 0 }}` on the config. Every child renders its final state instantly. One line handles the entire component tree.

{{zoom: 1x}} {{focus: none}}

```code:config-code lang=tsx
import { motion, MotionConfig, useReducedMotion } from "framer-motion";

// MotionConfig: shared transition for a subtree
function AnimatedTabs({ children }: { readonly children: React.ReactNode }) {
  const shouldReduceMotion = useReducedMotion();

  return (
    <MotionConfig
      transition={
        shouldReduceMotion
          ? { duration: 0 }
          : { type: "spring", bounce: 0, duration: 0.6 }
      }
    >
      {children}
    </MotionConfig>
  );
}

// Children inherit the spring transition
// No need to repeat transition on every motion element
function TabIndicator() {
  return (
    <motion.div
      layoutId="indicator"
      style={{ height: 2, background: "#3b82f6" }}
    />
    // transition comes from MotionConfig above
  );
}

// Local override: button wants faster feedback
function TabButton({ children, onClick }: {
  readonly children: React.ReactNode;
  readonly onClick: () => void;
}) {
  return (
    <motion.button
      onClick={onClick}
      whileTap={{ scale: 0.97 }}
      transition={{ duration: 0.1, ease: "easeOut" }}
    >
      {children}
    </motion.button>
  );
}
---
provider: 4-18
override: 33-42
reduced: 10-13
```
