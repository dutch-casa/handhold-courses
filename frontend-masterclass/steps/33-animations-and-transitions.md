---
title: Animations and transitions
---

# When to animate

{{show: when-diagram reveal 0.3s spring}} {{show: when-code typewriter 2s linear}} Not everything should animate. The decision tree is short. Will the user see this a hundred times a day? Don't animate it. Is it a response to user action? Animate with ease-out, 150-250ms. Is it a page transition? 300-400ms max.

{{focus: product}} {{zoom: 1.2x}} {{pulse: product}} {{annotate: product "Product UI: fast"}} Product UI should be fast and purposeful. A sidebar toggle? Animate once, then the user will toggle it fifty times a day — speed wins. A delete confirmation? Maybe a subtle fade. A frequently-used dropdown? Instant. Speed over delight in product UI.

{{zoom: 1x}} {{focus: marketing}} {{zoom: 1.2x}} {{annotate: marketing "Marketing: elaborate"}} Marketing pages, onboarding, first impressions — these can be more elaborate. Scroll-triggered reveals. Staggered entrances. A hero animation on load. The user sees it once. Make it count.

{{zoom: 1x}} {{focus: threshold}} {{zoom: 1.2x}} {{pulse: threshold}} {{pan: threshold}} {{annotate: threshold "The 400ms rule"}} 400ms is the ceiling for UI animation. Beyond that, the interface feels sluggish. 100-200ms for micro-interactions (hover, press). 200-300ms for enter/exit. 300-400ms for page transitions. Anything longer is a movie, not an interface.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:when-code lang=css
/* Micro-interaction: 100-200ms */
.button {
  transition: transform 150ms ease-out, box-shadow 150ms ease-out;
}
.button:active {
  transform: scale(0.97);
}

/* Enter/exit: 200-300ms */
.toast-enter {
  animation: slideIn 250ms ease-out;
}
.toast-exit {
  animation: slideOut 200ms ease-in;
}

/* Page transition: 300-400ms max */
.page-enter {
  animation: fadeIn 350ms ease-out;
}

/* NEVER longer than 400ms for UI */
---
product: 2-7
marketing: 10-16
threshold: 18-21
```

```diagram:when-diagram kind=graph
Question [label="Will user see this\n100+ times daily?" shape=diamond]
DontAnimate [label="Don't animate" shape=box]
UserAction [label="Is it user-initiated?" shape=diamond]
EaseOut [label="Ease-out\n150-250ms" shape=box]
PageTrans [label="Page transition?" shape=diamond]
Longer [label="300-400ms max" shape=box]
Default [label="Ease-out\n200ms default" shape=box]

Question -> DontAnimate [label="yes"]
Question -> UserAction [label="no"]
UserAction -> EaseOut [label="yes"]
UserAction -> PageTrans [label="no"]
PageTrans -> Longer [label="yes"]
PageTrans -> Default [label="no"]
```

# Easing functions

{{clear: slide}}

{{show: easing-code typewriter 2s linear}} {{split}} {{show: easing-preview reveal 0.3s spring}} Easing controls how an animation accelerates and decelerates. Linear feels robotic. Ease-out starts fast and slows down — like a ball rolling to a stop. The easing you pick tells the user what kind of motion this is.

{{focus: ease-out}} {{zoom: 1.2x}} {{pulse: ease-out}} {{annotate: ease-out "Enter: ease-out"}} Elements entering the screen use ease-out. They arrive with energy and settle into place. The deceleration feels natural — like something landing. `cubic-bezier(0, 0, 0.2, 1)` is a good custom ease-out.

{{zoom: 1x}} {{focus: ease-in}} {{zoom: 1.2x}} {{annotate: ease-in "Exit: ease-in"}} Elements leaving use ease-in. They start slow and accelerate away — gathering speed as they depart. The user's eye catches the beginning, then it's gone. Exit animations should be slightly faster than enter animations.

{{zoom: 1x}} {{focus: ease-in-out}} {{zoom: 1.2x}} {{pulse: ease-in-out}} {{annotate: ease-in-out "Move: ease-in-out"}} Elements moving on screen use ease-in-out. Accelerate, cruise, decelerate. Think of a car changing lanes — it doesn't teleport to the new position.

{{zoom: 1x}} {{focus: spring}} {{zoom: 1.2x}} {{annotate: spring "Playful: spring"}} Springs overshoot and bounce back. Playful, energetic. Use sparingly — a toggle switch, a modal entrance. Not for every animation. `cubic-bezier(0.34, 1.56, 0.64, 1)` overshoots slightly.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:easing-code lang=css
/* ENTER: ease-out (arrives with energy, settles) */
.enter {
  animation: slideIn 250ms ease-out;
}

/* EXIT: ease-in (gathers speed, departs) */
.exit {
  animation: slideOut 200ms ease-in;
}

/* MOVE: ease-in-out (accelerate, cruise, decelerate) */
.move {
  transition: transform 300ms ease-in-out;
}

/* SPRING: overshoot and settle */
.spring {
  transition: transform 400ms cubic-bezier(0.34, 1.56, 0.64, 1);
}

/* Custom curves for more control */
:root {
  --ease-out-custom: cubic-bezier(0, 0, 0.2, 1);
  --ease-in-custom: cubic-bezier(0.4, 0, 1, 1);
  --ease-spring: cubic-bezier(0.34, 1.56, 0.64, 1);
}
---
ease-out: 2-4
ease-in: 7-9
ease-in-out: 12-14
spring: 17-19
```

```preview:easing-preview
<style>
  .ease-demo { display: flex; flex-direction: column; gap: 1rem; padding: 1rem; }
  .ease-track { height: 40px; background: #f1f5f9; border-radius: 8px; position: relative; overflow: hidden; }
  .ease-ball { width: 40px; height: 40px; border-radius: 50%; position: absolute; left: 0; }
  .ease-label { font-size: 0.75rem; color: #64748b; margin-bottom: 0.25rem; }
  .ease-track:hover .ease-ball { left: calc(100% - 40px); }
  .b-out { background: #3b82f6; transition: left 500ms ease-out; }
  .b-in { background: #ef4444; transition: left 500ms ease-in; }
  .b-inout { background: #22c55e; transition: left 500ms ease-in-out; }
  .b-spring { background: #a855f7; transition: left 500ms cubic-bezier(0.34, 1.56, 0.64, 1); }
</style>
<div class="ease-demo">
  <div><div class="ease-label">ease-out (enter)</div><div class="ease-track"><div class="ease-ball b-out"></div></div></div>
  <div><div class="ease-label">ease-in (exit)</div><div class="ease-track"><div class="ease-ball b-in"></div></div></div>
  <div><div class="ease-label">ease-in-out (move)</div><div class="ease-track"><div class="ease-ball b-inout"></div></div></div>
  <div><div class="ease-label">spring (playful)</div><div class="ease-track"><div class="ease-ball b-spring"></div></div></div>
  <p style="font-size: 0.75rem; color: #94a3b8;">Hover each track to see the easing</p>
</div>
```

# Transform and opacity only

{{clear: slide}}

{{show: perf-code typewriter 2s linear}} Animate `transform` and `opacity`. Nothing else. Width, height, top, left, margin, padding — all of these trigger layout recalculation on every frame. The browser recomputes the entire page layout sixty times per second. That's where jank comes from.

{{focus: layout}} {{zoom: 1.2x}} {{pulse: layout}} {{annotate: layout "Layout triggers"}} `width`, `height`, `top`, `left`, `margin`, `padding` — change any of these and the browser runs layout, paint, and composite. Three phases. For every frame. On the main thread. Any JavaScript running at the same time competes for the same thread.

{{zoom: 1x}} {{focus: composite}} {{zoom: 1.2x}} {{annotate: composite "Composite only"}} `transform` and `opacity` skip layout and paint. The browser promotes the element to its own GPU layer and animates it on the compositor thread. The main thread is free. JavaScript keeps running. Smooth 60fps.

{{zoom: 1x}} {{focus: translate}} {{zoom: 1.2x}} {{pulse: translate}} {{pan: translate}} {{annotate: translate "Use translate, not top/left"}} Move something? `transform: translateX(100px)`, not `left: 100px`. Scale something? `transform: scale(0.95)`, not `width: 95%`. Fade something? `opacity: 0`, not `visibility: hidden` with a transition.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:perf-code lang=css
/* BAD: animating layout properties */
.bad-slide {
  transition: left 300ms ease-out; /* triggers layout every frame */
  position: absolute;
  left: 0;
}
.bad-slide.open {
  left: 250px; /* 60 layout recalculations per second */
}

/* GOOD: animating transform (GPU composited) */
.good-slide {
  transition: transform 300ms ease-out; /* compositor thread */
  transform: translateX(0);
}
.good-slide.open {
  transform: translateX(250px); /* smooth 60fps */
}

/* BAD: animating height */
.bad-expand {
  transition: height 300ms ease-out; /* layout thrash */
  height: 0;
  overflow: hidden;
}
.bad-expand.open {
  height: 200px;
}

/* GOOD: animating with transform + opacity */
.good-expand {
  transition: opacity 200ms ease-out, transform 200ms ease-out;
  opacity: 0;
  transform: scaleY(0);
  transform-origin: top;
}
.good-expand.open {
  opacity: 1;
  transform: scaleY(1);
}
---
layout: 2-9
composite: 12-18
translate: 12-18
```

# Enter and exit animations

{{clear: slide}}

{{show: enter-code typewriter 2s linear}} Enter and exit need different treatment. Enter animations add an element to the DOM and reveal it. Exit animations hide an element and then remove it. The challenge: the element must stay in the DOM until the exit animation completes.

{{focus: enter}} {{zoom: 1.2x}} {{pulse: enter}} {{annotate: enter "Enter: appear"}} Mount the element with a starting state (opacity: 0, translateY(8px)). Animate to the final state. CSS `@keyframes` or `animation` properties handle this cleanly. The element appears with motion.

{{zoom: 1x}} {{focus: exit}} {{zoom: 1.2x}} {{annotate: exit "Exit: the hard part"}} Exit is harder. You want to animate out, then remove from the DOM. But React removes the element immediately when it unmounts. Solution: delay unmounting until the animation finishes. An `onAnimationEnd` callback or a library like Framer Motion handles this.

{{zoom: 1x}} {{focus: transition}} {{zoom: 1.2x}} {{pulse: transition}} {{pan: transition}} {{annotate: transition "Transition pattern"}} `transition: all` is never the answer. Specify exact properties: `transition: opacity 200ms ease-out, transform 200ms ease-out`. `all` transitions properties you didn't intend to animate — colors, borders, backgrounds — causing performance hits and visual glitches.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:enter-code lang=css
/* Enter animation */
@keyframes fadeSlideIn {
  from {
    opacity: 0;
    transform: translateY(8px);
  }
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

.notification-enter {
  animation: fadeSlideIn 250ms ease-out forwards;
}

/* Exit animation */
@keyframes fadeSlideOut {
  from {
    opacity: 1;
    transform: translateY(0);
  }
  to {
    opacity: 0;
    transform: translateY(-8px);
  }
}

.notification-exit {
  animation: fadeSlideOut 200ms ease-in forwards;
}

/* NEVER: transition: all */
.bad { transition: all 200ms ease; }

/* ALWAYS: specify exact properties */
.good {
  transition:
    opacity 200ms ease-out,
    transform 200ms ease-out;
}

/* Reduced motion: mandatory */
@media (prefers-reduced-motion: reduce) {
  .notification-enter,
  .notification-exit {
    animation-duration: 0.01ms;
  }
}
---
enter: 2-15
exit: 18-30
transition: 33-40
```

# Prefers-reduced-motion

{{clear: slide}}

{{show: motion-code typewriter 2s linear}} This is non-negotiable. Some users experience vertigo, nausea, or seizures from motion. `prefers-reduced-motion: reduce` is their signal. Every animation you write needs a reduced-motion fallback. No exceptions.

{{focus: query}} {{zoom: 1.2x}} {{pulse: query}} {{annotate: query "The media query"}} `@media (prefers-reduced-motion: reduce)` matches when the user has enabled reduced motion in their OS settings. Inside this query, disable or drastically shorten all animations and transitions.

{{zoom: 1x}} {{focus: global}} {{zoom: 1.2x}} {{annotate: global "Global reset"}} A global reset is the safety net: `*, *::before, *::after { animation-duration: 0.01ms !important; transition-duration: 0.01ms !important; }`. This catches every animation. Individual overrides can refine specific transitions.

{{zoom: 1x}} {{focus: react-hook}} {{zoom: 1.2x}} {{pulse: react-hook}} {{pan: react-hook}} {{annotate: react-hook "React hook"}} `useMediaQuery("(prefers-reduced-motion: reduce)")` — use this in components that conditionally animate. Skip the animation entirely, or swap a slide for a fade. The user's preference drives the decision.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:motion-code lang=css
/* Global safety net */
@media (prefers-reduced-motion: reduce) {
  *,
  *::before,
  *::after {
    animation-duration: 0.01ms !important;
    animation-iteration-count: 1 !important;
    transition-duration: 0.01ms !important;
    scroll-behavior: auto !important;
  }
}

/* Per-component override */
.hero-animation {
  animation: heroEntrance 800ms ease-out;
}

@media (prefers-reduced-motion: reduce) {
  .hero-animation {
    animation: none;
    opacity: 1; /* skip animation, show final state */
  }
}

/* Parallax: completely disabled */
.parallax {
  transform: translateY(calc(var(--scroll) * 0.3));
}

@media (prefers-reduced-motion: reduce) {
  .parallax {
    transform: none; /* no parallax */
  }
}
---
query: 2-11
global: 2-11
react-hook: 14-23
```
