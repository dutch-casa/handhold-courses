# Frontend Masterclass: Zero to React God

## Course Philosophy

Build-Up arc: simple to complex, one layer at a time. Every lesson opens with a
problem the learner can feel. Every concept is taught from first principles.
Explanations are visual, dense with choreography, and paced like a 3blue1brown video.

Labs follow every lesson cluster. They're byte-sized (5-15 min), with crystal-clear
instructions and TAP tests that validate exactly what was taught.

---

## Module 1: HTML Foundations

The web starts here. What is a browser actually doing? What is a document?

### Lesson 1: "What is the web?"
- **Steps**: The request (browser → server → response), HTML as a document, the browser as a rendering engine, your first tags, nesting and tree structure
- **Aha**: A webpage is just a text file. The browser reads it top to bottom and draws what it describes.
- **Viz**: diagram (client → server → response), code (raw HTML), preview (rendered result), split (code + preview)
- **Scenes**: ~7 steps, heavy use of diagram + split(code, preview)

### Lesson 2: "Building blocks"
- **Steps**: Elements and attributes, headings and paragraphs, links and images, lists, tables, the anatomy of a tag
- **Aha**: Every HTML element is a box. Tags describe what kind of box. Attributes configure it.
- **Viz**: code blocks with region walkthroughs, preview showing each element rendered
- **Scenes**: ~8 steps, split(code, preview) throughout

### Lab 1: "Your first page"
- Build a personal bio page with heading, paragraph, image, list of interests, link
- 5 tests: page has h1, has img with alt, has ul with 3+ items, has a link, has a paragraph
- Scaffold: empty index.html with doctype and head

### Lesson 3: "Semantic HTML"
- **Steps**: Why semantics matter (accessibility, SEO, machines reading your page), header/nav/main/section/article/aside/footer, when to use which, the outline algorithm
- **Aha**: Semantic tags don't change how a page looks. They change what a page means. Screen readers, search engines, and future-you all depend on meaning.
- **Viz**: diagram (page outline as tree), code (semantic vs div soup), split(code, preview)
- **Scenes**: ~6 steps

### Lesson 4: "Forms and inputs"
- **Steps**: The form element, text inputs, labels and accessibility, checkboxes/radios/selects, textarea, buttons (submit vs button), form submission (what actually happens)
- **Aha**: A form is a contract between the user and the server. Labels aren't decoration — they're the accessibility bridge. Without a label, a screen reader user can't fill out your form.
- **Viz**: code + preview(interactive form), diagram(form submission flow)
- **Scenes**: ~7 steps

### Lab 2: "Build a contact form"
- Build a form with name, email, message textarea, subject dropdown, submit button
- All inputs must have labels, email input must use type="email"
- 6 tests: form exists, has labeled name input, has email type input, has textarea, has select, has submit button

### Lesson 5: "The DOM"
- **Steps**: What the DOM actually is (not HTML — a live tree), how the browser builds it (parsing → tree → render tree → paint), nodes and relationships (parent/child/sibling), the document object, inspecting with DevTools
- **Aha**: HTML is the blueprint. The DOM is the building. When you change the DOM, you change the building — the blueprint stays the same. JavaScript talks to the building, not the blueprint.
- **Viz**: diagram(HTML text → parser → DOM tree → render tree → pixels), data(tree: DOM node hierarchy), split(HTML code, tree visualization)
- **Scenes**: ~7 steps

### Lab 3: "Explore the DOM"
- Given an HTML page, write JS to: find an element by ID, count all paragraphs, read the text of the first heading, add a class to an element
- 4 tests: getElementById returns element, querySelectorAll('p').length correct, heading text matches, element has the added class

---

## Module 2: CSS Foundations

How does styling actually work? Not "add this class" — the actual mechanics.

### Lesson 6: "How CSS works"
- **Steps**: What CSS is (a set of rules), selectors and declarations, the cascade (origin, specificity, order), specificity scoring (0-0-0 system), inheritance (which properties inherit, which don't), the initial and computed values
- **Aha**: CSS isn't magic. It's an algorithm. For every property on every element, the browser runs the cascade algorithm: collect all matching rules, sort by specificity, last one wins. If nothing matches, check if the property inherits. If not, use the default. That's it.
- **Viz**: diagram(cascade algorithm flowchart), code(conflicting rules), data(specificity comparison table), split(CSS code, computed result)
- **Scenes**: ~8 steps

### Lesson 7: "The box model"
- **Steps**: Content → padding → border → margin, box-sizing (content-box vs border-box), why border-box wins, margin collapse (the weird one), display types (block vs inline vs inline-block), width and height behavior
- **Aha**: Every element on the page is a rectangle. Understanding the box model means understanding that padding is inside the border, margin is outside, and box-sizing: border-box makes width mean what you think it means.
- **Viz**: diagram(box model layers), preview(interactive box model demo), code, split
- **Scenes**: ~7 steps

### Lab 4: "Box model exercises"
- Given styled boxes, fix: a box that's wider than expected (box-sizing), collapsed margins, inline elements ignoring width
- 4 tests: box1 computed width matches target, margins don't collapse, inline-block element has width, padding doesn't expand container

### Lesson 8: "Colors and typography"
- **Steps**: Color systems (hex, rgb, hsl — and why HSL is best for humans), opacity and alpha, CSS custom properties (variables), font stacks and fallbacks, font-size units (px, rem, em, vw), line-height and letter-spacing, system font stack
- **Aha**: HSL separates what humans care about: hue (what color), saturation (how vivid), lightness (how bright). You can build an entire color palette by picking one hue and varying saturation and lightness. Custom properties let you define it once and use it everywhere.
- **Viz**: chart(HSL color wheel), preview(color palette builder), code(custom properties), split
- **Scenes**: ~7 steps

### Lesson 9: "Selectors and combinators"
- **Steps**: Type, class, ID selectors, attribute selectors, pseudo-classes (:hover, :focus, :first-child, :nth-child, :not), pseudo-elements (::before, ::after, ::placeholder), combinators (descendant, child, sibling, adjacent), grouping, the :has() selector (parent selector!)
- **Aha**: Selectors are pattern matching for the DOM tree. :has() is the game-changer — it lets you select a parent based on what's inside it. For decades, CSS could only look down the tree. :has() lets you look up.
- **Viz**: data(DOM tree with selector highlighting), code(selector examples), preview(hover/focus demos)
- **Scenes**: ~8 steps

### Lab 5: "Style the bio page"
- Take the bio page from Lab 1, add a stylesheet with: custom color scheme using HSL and custom properties, styled headings with system font stack, hover effects on links, styled list with custom markers
- 5 tests: has custom properties defined, links change on hover, headings use custom font, list has styled markers, page uses border-box

### Lesson 10: "Flexbox"
- **Steps**: The problem (centering and distributing), flex container vs flex items, main axis and cross axis, justify-content (distributing along main), align-items (aligning along cross), flex-wrap, flex-grow/shrink/basis (the flex shorthand), gap, order, common patterns (centering, nav bar, card row, holy grail)
- **Aha**: Flexbox is one-dimensional layout. Pick an axis. Items flow along it. justify-content distributes space along that axis. align-items handles the other axis. That's the whole mental model. Everything else is details.
- **Viz**: diagram(axis visualization), preview(interactive flex playground), code, split(code, preview)
- **Scenes**: ~9 steps (this is a big one)

### Lab 6: "Flexbox navigation"
- Build a responsive nav bar: logo left, links center, CTA button right, items vertically centered, wraps on mobile
- 5 tests: nav uses display:flex, items are vertically centered, logo is at start, CTA is at end, wraps when narrow

### Lesson 11: "CSS Grid"
- **Steps**: The problem (two-dimensional layout), defining rows and columns, fr units and repeat(), grid-template-areas (the named layout), placing items (grid-column, grid-row), spanning tracks, auto-fill and auto-fit (responsive without media queries!), subgrid, gap, common patterns (dashboard, magazine, photo gallery)
- **Aha**: Grid is two-dimensional layout. You define both axes at once. grid-template-areas lets you draw your layout in ASCII art — the code looks like the result. auto-fill + minmax gives you responsive layouts with zero media queries.
- **Viz**: diagram(grid lines and tracks), preview(grid template areas as ASCII → rendered), code, split(code, preview)
- **Scenes**: ~9 steps

### Lab 7: "Grid photo gallery"
- Build a photo gallery: auto-fill responsive grid, images span different numbers of columns/rows for visual interest, gap between items
- 5 tests: uses display:grid, uses auto-fill or auto-fit, has gap defined, at least one item spans 2 columns, layout is responsive without media queries

### Lesson 12: "Responsive design"
- **Steps**: The viewport meta tag, media queries (min-width, max-width, prefers-color-scheme, prefers-reduced-motion), mobile-first approach, container queries (@container — the modern way), fluid typography (clamp()), responsive images (srcset, sizes, picture element), logical properties (inline/block vs left/right)
- **Aha**: Media queries ask "how wide is the viewport?" Container queries ask "how wide is my container?" That's a fundamental shift. Components become self-contained — they adapt to their own space, not the page's space. This is how component libraries work.
- **Viz**: diagram(viewport vs container), preview(responsive component), code, split(media query vs container query)
- **Scenes**: ~8 steps

### Lab 8: "Responsive gallery"
- Take the gallery from Lab 7, add: fluid typography with clamp(), container queries for card layout changes, dark mode with prefers-color-scheme, reduced motion support
- 5 tests: uses clamp() for font-size, uses @container, has prefers-color-scheme media query, has prefers-reduced-motion, cards adapt to container width

### Lesson 13: "CSS naming and architecture"
- **Steps**: Why naming matters (at scale, CSS becomes a mess), BEM (Block Element Modifier), the naming collision problem, utility-first philosophy (why Tailwind exists), CSS Modules and scoping, the progression: no system → BEM → utilities → scoped styles
- **Aha**: At scale, CSS has a global namespace problem. Every classname is global. BEM solves it with convention. Utility-first solves it by making every class do one thing. CSS Modules solve it with tooling. Each approach trades flexibility for safety.
- **Viz**: code(BEM examples, utility examples), diagram(collision visualization), split(BEM vs utility)
- **Scenes**: ~6 steps

### Lesson 14: "Modern CSS"
- **Steps**: CSS Nesting (native, no preprocessor), CSS Layers (@layer — controlling cascade without specificity wars), the :has() selector deep dive, color-mix() and relative colors, light-dark() function, the popover API + CSS, view transitions, scroll-driven animations
- **Aha**: Native CSS nesting means you don't need Sass for the most common Sass feature. @layer lets you say "these rules always lose to those rules" regardless of specificity. The cascade becomes something you control, not something that controls you.
- **Viz**: code(nesting, layers, :has()), preview(popover demo, view transition), split
- **Scenes**: ~8 steps

### Lab 9: "Modern CSS refactor"
- Take a given stylesheet using BEM and flat selectors, refactor to use: native nesting, @layer for organization, :has() to replace JS-dependent parent styling, custom properties with light-dark()
- 5 tests: uses CSS nesting, uses @layer, uses :has(), uses light-dark() or color-mix(), no BEM double-underscores remain

---

## Module 3: Tailwind CSS

The utility-first framework that changed how we write CSS.

### Lesson 15: "Why Tailwind?"
- **Steps**: The problem with traditional CSS at scale (dead code, naming, specificity), the utility-first philosophy (constraint-based design), how Tailwind works (scanning classes → generating CSS), the design system encoded in code (spacing scale, color palette), responsive prefixes, state variants (hover:, focus:, dark:)
- **Aha**: Tailwind isn't "inline styles with extra steps." It's a design system as a utility library. The spacing scale (4, 8, 12, 16...) prevents random values. The color palette prevents hex-code chaos. Constraints breed consistency.
- **Viz**: split(traditional CSS vs Tailwind), code, preview, diagram(build pipeline)
- **Scenes**: ~7 steps

### Lesson 16: "Tailwind core"
- **Steps**: Spacing and sizing (p-4, m-2, w-full, h-screen), colors (bg-blue-500, text-gray-900), typography (text-lg, font-bold, leading-relaxed), layout (flex, grid, gap), borders and rounded corners, shadows and opacity, responsive design (sm:, md:, lg:), dark mode
- **Aha**: You already know CSS. Tailwind is a shorthand that enforces a scale. p-4 = padding: 1rem. text-blue-500 = color: a specific shade you didn't have to pick. The class names are the documentation.
- **Viz**: code(Tailwind classes), preview(rendered result), split(code, preview), data(spacing scale table)
- **Scenes**: ~8 steps

### Lab 10: "Tailwind gallery"
- Rebuild the photo gallery from Lab 7 using only Tailwind classes. Responsive grid, hover effects, dark mode support, proper spacing
- 5 tests: no custom CSS file (only Tailwind), grid is responsive, has hover effects, has dark mode classes, images have proper spacing

### Lesson 17: "Tailwind 4"
- **Steps**: CSS-first configuration (no tailwind.config.js), the @theme directive, @utility for custom utilities, container queries (@container), automatic content detection, CSS variables as the source of truth, composing with @apply (and when not to), the oxide engine
- **Aha**: Tailwind 4 made the leap from JS config to CSS-native config. Your design tokens live in CSS. @theme defines your system. @utility extends it. The config file is gone — everything is CSS.
- **Viz**: split(v3 config vs v4 @theme), code, preview
- **Scenes**: ~7 steps

### Lab 11: "Dashboard layout"
- Build a dashboard layout with Tailwind 4: sidebar nav, main content area, header, card grid, responsive (sidebar collapses on mobile), dark mode
- 6 tests: uses @theme for custom values, sidebar uses flex layout, main content uses grid, responsive breakpoint collapses sidebar, dark mode works, cards have hover state

---

## Module 4: JavaScript and the DOM

Make pages interactive. Understand what's actually happening when code runs.

### Lesson 18: "JavaScript fundamentals"
- **Steps**: Variables (let, const — never var), primitive types (string, number, boolean, null, undefined), arrays and objects, functions (declaration, arrow, parameters, return), template literals, destructuring, spread/rest, array methods (map, filter, find, reduce), control flow (if/else, ternary, for...of)
- **Aha**: JavaScript has seven primitive types and one structural type (objects). Everything else — arrays, functions, dates, regex — is an object. When you understand that functions are objects you can pass around, you understand JavaScript.
- **Viz**: data(type tree), code(examples), diagram(value vs reference)
- **Scenes**: ~9 steps

### Lesson 19: "The DOM and JavaScript"
- **Steps**: document.querySelector / querySelectorAll, reading element properties (textContent, innerHTML, classList, dataset), modifying the DOM (createElement, appendChild, remove), event listeners (addEventListener, event object, event delegation, bubbling and capturing), forms and input values, the event loop (microtask vs macrotask)
- **Aha**: addEventListener is the bridge between "stuff on screen" and "code that runs." When you click a button, the browser creates an event object, walks the DOM tree (capturing phase), triggers the handler (target phase), then walks back up (bubbling). Understanding this path is understanding interactivity.
- **Viz**: diagram(event flow: capture → target → bubble), code, preview(interactive DOM demo), split
- **Scenes**: ~8 steps

### Lab 12: "Interactive todo list"
- Build a todo list in vanilla JS: add items, mark complete (toggle class), delete items, count remaining
- Scaffold: HTML structure provided, empty app.js
- 6 tests: can add item, item appears in list, can toggle complete, can delete item, count updates, empty input doesn't add item

### Lesson 20: "Async JavaScript"
- **Steps**: Why async matters (network requests block), callbacks (the old way), promises (then/catch/finally), async/await (the modern way), fetch API, error handling (try/catch), Promise.all for parallel requests, JSON parsing
- **Aha**: A promise is a receipt. You ask for something, you get a receipt immediately. When the thing arrives, the receipt resolves. async/await lets you write code that reads top-to-bottom even though the operations happen asynchronously. It's synchronous-looking code with asynchronous behavior.
- **Viz**: diagram(sync vs async timeline), code, data(promise state machine: pending → fulfilled/rejected)
- **Scenes**: ~7 steps

### Lab 13: "Fetch and display"
- Fetch data from a provided JSON endpoint, render a list of users with name and email, handle loading state, handle error state
- 5 tests: fetches data on load, renders user list, shows loading state initially, handles fetch error gracefully, each user shows name and email

---

## Module 5: TypeScript

Types aren't annotations. They're guarantees.

### Lesson 21: "Why TypeScript?"
- **Steps**: The problem (runtime errors from wrong types), what TypeScript actually is (a compiler, not a runtime), type inference (you barely have to write types), the compiler as documentation, IDE superpowers (autocomplete, refactoring, go-to-definition)
- **Aha**: TypeScript doesn't add types to JavaScript. It reveals the types that were always there. Every JavaScript value already has a type — TypeScript just makes them visible and checked before your code runs.
- **Viz**: split(JS with runtime error, TS catching it at compile time), code, diagram(compiler pipeline)
- **Scenes**: ~6 steps

### Lesson 22: "TypeScript fundamentals"
- **Steps**: Primitive types, type annotations vs inference, arrays and tuples, objects and interfaces, union types (string | number), literal types ("success" | "error"), optional properties, type narrowing (typeof, in, discriminated unions), the unknown type (not any!)
- **Aha**: Union types and narrowing are the backbone of TypeScript. A value is string | number. After an if-typeof check, the compiler knows which one it is. The code and the types agree. This is called narrowing, and it's how TypeScript stays out of your way.
- **Viz**: code(narrowing examples), diagram(type narrowing flowchart), split(before narrowing, after narrowing)
- **Scenes**: ~8 steps

### Lesson 23: "TypeScript for real"
- **Steps**: Generics (type parameters), mapped types (Record, Partial, Required, Pick, Omit), discriminated unions (tagged unions with a kind field), template literal types, const assertions, satisfies operator, type meta-programming (deriving types from types), the "make invalid states unrepresentable" pattern
- **Aha**: Discriminated unions with a kind field let you model state machines in the type system. A Loading state has no data. A Success state has data. An Error state has an error. The compiler forces you to handle every case. Impossible states become unrepresentable.
- **Viz**: code(discriminated union, mapped types), diagram(state machine from types), data(type narrowing tree)
- **Scenes**: ~9 steps

### Lab 14: "Type a real API"
- Given raw JSON responses from an API, write TypeScript types that: model the response as discriminated unions, use generics for pagination wrapper, use mapped types to create partial update types, make invalid states unrepresentable
- 6 tests: types compile, discriminated union narrows correctly, generic pagination type works, partial update type allows subset, readonly prevents mutation, invalid state is a compile error

---

## Module 6: React Fundamentals

Components are functions. State is data. The UI is a function of state.

### Lesson 24: "Your dev environment"
- **Steps**: What Node.js is (V8 outside the browser), npm (package registry + CLI), package.json anatomy (dependencies, devDependencies, scripts), npm install and the lockfile, Vite (dev server + bundler + HMR), scaffolding a React+TS project with `npm create vite@latest`, `npm run dev` workflow
- **Aha**: The browser doesn't understand .tsx files. Vite compiles your code, serves it to the browser, and refreshes instantly when you save. One command scaffolds a complete project. Two more and you're running it.
- **Viz**: diagram(Node.js runtime), code(package.json), diagram(Vite pipeline), code(scaffold commands)
- **Scenes**: ~5 steps
- **Note**: All React labs from this point forward include Vite. Students run `npm run dev` to preview their work.

### Lesson 25: "What is React?"

- **Steps**: The problem (imperative DOM is painful at scale), declarative UI (describe what, not how), components as functions, JSX (it's just function calls), the virtual DOM (diff and patch), React's mental model (UI = f(state))
- **Aha**: A React component is a function that takes data and returns a description of UI. When data changes, React calls the function again, compares the old description to the new one, and only updates what's different. That's the whole idea.
- **Viz**: diagram(imperative vs declarative), split(vanilla JS vs React), code(JSX → createElement), diagram(virtual DOM diff)
- **Scenes**: ~7 steps

### Lesson 25: "Components, props, and JSX"
- **Steps**: Function components (the only kind that matters), returning JSX, props (data flowing down), destructuring props, TypeScript + props (interface), children prop, composition (nesting components), key prop and lists, fragments
- **Aha**: Props flow down. Always. A parent passes data to a child through props. The child never reaches up to grab data from the parent. This one-way flow makes React predictable — you can always trace where data comes from.
- **Viz**: diagram(component tree with prop arrows), code(component examples), preview(rendered components), split
- **Scenes**: ~8 steps

### Lab 15: "Profile card"
- Build a ProfileCard component that takes name, avatar URL, bio, and list of skills as props. Render it with TypeScript types.
- 5 tests: component renders name, renders avatar with alt text, renders bio, renders all skills as list items, props are typed with interface

### Lesson 26: "State and events"
- **Steps**: useState (declaring state), updating state (setter function), state is immutable (new values, not mutations), event handlers (onClick, onChange, onSubmit), controlled inputs (value + onChange), derived state (computing values from state), batched updates
- **Aha**: State is a snapshot. When you call setState, you're not changing the current value — you're requesting a new render with a new value. The old render still sees the old value. This is why React is predictable: each render is a snapshot of state at that point in time.
- **Viz**: diagram(render cycle: state → render → DOM → event → setState → render), code, preview(interactive counter), split
- **Scenes**: ~8 steps

### Lab 16: "Counter and form"
- Build: a counter with increment/decrement/reset, a controlled text input that shows character count, a form with name+email that validates on submit
- 6 tests: counter increments, counter decrements, counter resets, input shows char count, form validates empty fields, form shows success on valid submit

### Lesson 27: "Lists and conditional rendering"
- **Steps**: Rendering arrays with map(), the key prop (why it matters, what to use), conditional rendering (ternary, &&, early return), conditional styles, filtering and sorting in the render, the "lifting state up" pattern (shared state between siblings)
- **Aha**: The key prop isn't just "something React wants." It's how React knows which item is which between renders. Without a stable key, React can't tell if an item was added, removed, or moved — so it destroys and recreates everything. Keys are identity.
- **Viz**: data(virtual DOM diff with keys vs without), code, preview(filtered list), split
- **Scenes**: ~7 steps

### Lab 17: "Filterable list"
- Build a searchable, filterable list of items. Text filter, category dropdown filter, sort by name or date. All filters work together.
- 5 tests: renders all items initially, text filter narrows results, category filter works, sort changes order, combined filters work

### Lesson 28: "Data fetching the right way"
- **Steps**: Why NOT useEffect for fetching (race conditions, no caching, no dedup, cleanup nightmare), React Query / TanStack Query (the right tool), useQuery (fetch + cache + loading + error in one hook), query keys (cache identity), stale time and refetching, mutations (useMutation), optimistic updates
- **Aha**: useEffect for data fetching is an anti-pattern. It gives you loading and error state, but no caching, no deduplication, no background refetch, no retry, no optimistic updates. React Query gives you all of that in one hook. The question isn't "should I use it?" — it's "why would you not?"
- **Viz**: split(useEffect mess vs React Query clean), diagram(React Query cache flow), code, preview
- **Scenes**: ~8 steps

### Lab 18: "Fetch and display posts"
- Use React Query to: fetch a list of posts, show loading skeleton, show error state, click a post to fetch its details, back button returns to list. Cache should prevent re-fetching.
- 6 tests: shows loading initially, renders post list, clicking post shows detail, back button works, second visit to same post is instant (cached), error state renders on failure

---

## Module 7: React Composition Patterns

Stop passing booleans. Start composing components.

### Lesson 29: "Composition over configuration"
- **Steps**: The boolean prop trap (6 booleans = 64 states), compound components (Accordion.Item, Accordion.Trigger, Accordion.Content), children as composition, why this is better (each variant is explicit, no impossible states), the provider pattern for shared state
- **Aha**: Six booleans. Two to the sixth power. Sixty-four possible states. Most of them are nonsensical. Compound components eliminate the combinatorial explosion by making each variant an explicit composition of parts. No booleans. No impossible states. Just Lego bricks.
- **Viz**: code(boolean hell), diagram(state explosion), code(compound solution), split(before/after)
- **Scenes**: ~8 steps

### Lab 19: "Compound accordion"
- Build a compound Accordion component: Accordion, Accordion.Item, Accordion.Trigger, Accordion.Content. Only one item open at a time. Animated open/close.
- 6 tests: renders multiple items, clicking trigger opens content, clicking open trigger closes it, only one item open at a time, content animates, component uses compound pattern (no boolean props)

### Lesson 30: "Context and providers"
- **Steps**: The prop drilling problem, React Context (createContext, Provider, useContext → use() in React 19), the provider pattern (state + actions + meta), decoupling implementation (the provider hides HOW state is managed), when to use context vs props (proximity principle), context performance (avoiding unnecessary re-renders)
- **Aha**: A provider component owns the state. The parts read from context. Swap the provider, keep the parts. The provider is a wall between "how state is managed" and "how UI renders." Neither side knows about the other's internals. That's composition.
- **Viz**: diagram(prop drilling vs context), code(provider pattern), split(coupled vs decoupled)
- **Scenes**: ~7 steps

### Lab 20: "Theme provider"
- Build a theme provider: ThemeProvider wraps the app, useTheme hook reads current theme and toggle function, components style themselves based on theme, supports system preference detection
- 5 tests: theme context provides current theme, toggle switches themes, components read theme, system preference is respected, theme persists (localStorage)

### Lesson 31: "Custom hooks"
- **Steps**: What a custom hook is (just a function that uses hooks), extracting shared logic, rules of hooks (why they exist), useLocalStorage, useMediaQuery, useDebounce, composing hooks (hooks that use other hooks), testing hooks
- **Aha**: A custom hook is a function with superpowers. It can use state, effects, context — anything a component can. But it returns data, not UI. This is how you share stateful logic between components without changing the component tree.
- **Viz**: code(hook extraction from component), split(component with duplicated logic, after hook extraction), diagram(hook composition)
- **Scenes**: ~7 steps

### Lab 21: "Utility hooks"
- Build three hooks: useLocalStorage (get/set with JSON serialization, type-safe), useMediaQuery (returns boolean, SSR-safe), useDebounce (delays a value update). Use them in a demo component.
- 6 tests: useLocalStorage reads initial value, useLocalStorage persists on update, useMediaQuery returns boolean, useMediaQuery updates on change, useDebounce delays value, demo component uses all three

---

## Module 8: Design Engineering

The difference between "it works" and "it's polished."

### Lesson 32: "Design engineering principles"
- **Steps**: What design engineering is (the intersection of design and code), no layout shift (hardcoded dimensions, tabular-nums), touch-first design (44px targets, no hover-only), keyboard navigation (tab order, focus-visible, scroll-into-view), accessibility by default (aria labels, reduced motion, screen reader testing), the z-index scale (isolation: isolate)
- **Aha**: A polished UI doesn't just look good. It's stable (no layout shift), inclusive (keyboard and screen reader), responsive (touch-first, hover-enhanced), and fast (no unnecessary animation). Polish is not decoration — it's engineering.
- **Viz**: preview(layout shift demo), preview(touch target demo), code(accessibility patterns), diagram(z-index layers)
- **Scenes**: ~8 steps

### Lesson 33: "Animations and transitions"
- **Steps**: CSS transitions (property, duration, easing), when to animate (and when not to), easing functions (ease-out for enter, ease-in-out for move, spring for playful), transform and opacity (the only properties to animate), enter/exit animations (no height/width animation), keyframes for complex sequences, prefers-reduced-motion (non-negotiable), the 400ms rule
- **Aha**: Animate transform and opacity. Nothing else. Width, height, top, left — these trigger layout recalculation on every frame. Transform and opacity are composited on the GPU. The difference isn't subtle — it's the difference between 60fps and 15fps.
- **Viz**: preview(smooth vs janky animation), split(layout animation vs transform animation), code(transition patterns), diagram(render pipeline: layout → paint → composite)
- **Scenes**: ~8 steps

### Lab 22: "Notification system"
- Build an animated notification component: enters from top-right with slide + fade, auto-dismisses after 5s, manual dismiss with animation, stacks multiple notifications, respects reduced motion
- 6 tests: notification appears on trigger, animates in, auto-dismisses, manual dismiss works, multiple stack, reduced motion disables animation

### Lesson 34: "Forms that work"
- **Steps**: Input sizing (16px minimum to prevent iOS zoom), label association (for + id, or wrapping), error states (aria-invalid, aria-describedby, inline messages), submit with Enter and Cmd+Enter, loading states (disable button, show spinner), validation patterns (on blur, on submit, never on every keystroke), accessible error announcements (aria-live)
- **Aha**: A form that doesn't zoom on iOS, doesn't trap keyboard users, announces errors to screen readers, and validates at the right moment — that's not extra work. That's the baseline. Anything less is a broken form.
- **Viz**: preview(form with all states), code(accessible form pattern), split(broken form vs correct form), diagram(validation flow)
- **Scenes**: ~7 steps

### Lab 23: "Accessible form"
- Build a registration form: name, email, password with strength meter, confirm password. Validate on blur, announce errors, prevent iOS zoom, submit with keyboard, show loading state
- 7 tests: inputs are 16px+, labels are associated, errors use aria-invalid, error messages use aria-describedby, form submits with Enter, password strength updates, loading state disables form

### Lesson 35: "Performance patterns"
- **Steps**: Why performance matters (user perception thresholds), memoization (React.memo, useMemo, useCallback — and when NOT to), lazy loading (React.lazy + Suspense), code splitting (dynamic imports), virtualization (rendering only visible items), optimistic updates (update UI before server confirms), the measuring mindset (React DevTools profiler, Lighthouse)
- **Aha**: The fastest code is code that doesn't run. Virtualization renders 20 items instead of 10,000. Lazy loading loads the code when it's needed, not at startup. Memoization skips re-rendering when props haven't changed. The common thread: do less work.
- **Viz**: diagram(rendering 10k items vs virtualized), code(memo patterns), split(before/after profiler), chart(performance metrics)
- **Scenes**: ~8 steps

### Lesson 36: "The polished UI"
- **Steps**: Shadows (layered, not single box-shadow), gradients (subtle, purposeful), micro-interactions (button press, input focus, toggle switch), dark mode (not just inverted colors — different shadow/border treatment), scrollbar styling (only in small containers), font rendering (antialiased, optical sizing), the "feel" of great UI (everything has feedback, nothing is dead)
- **Aha**: The difference between "fine" and "great" is 50 details, each taking 5 minutes. Layered shadows that look like real depth. A button that presses in 2px on click. An input that glows on focus. Dark mode where shadows become glows. None of these are hard. They're just intentional.
- **Viz**: preview(polished component gallery), split(flat vs polished), code(shadow layers, micro-interactions)
- **Scenes**: ~7 steps

### Lab 24: "Polished dashboard card"
- Build a polished dashboard card: layered shadow, hover lift, dark mode, loading skeleton, micro-interactions, proper typography
- 6 tests: card has multiple box-shadows, hover increases shadow, dark mode changes shadows to borders/glows, loading skeleton pulses, typography uses proper scale, press state provides feedback

### Lab 25: "Capstone: Task manager"
- Build a complete task manager: list tasks (React Query fetch), add task (mutation + optimistic update), complete/uncomplete (toggle), delete with confirmation, filter by status, animated transitions, dark mode, keyboard accessible, mobile responsive
- Scaffold: API server provided, empty React app with React Query configured
- 10 tests: renders task list, can add task, can complete task, can delete task, filter works, optimistic update shows immediately, dark mode works, keyboard navigation works, mobile layout works, animations respect reduced motion

---

## Summary

| Module | Lessons | Labs | Focus |
|--------|---------|------|-------|
| 1. HTML Foundations | 5 | 3 | Tags, semantics, forms, DOM |
| 2. CSS Foundations | 9 | 6 | Cascade, box model, flex, grid, responsive, modern |
| 3. Tailwind CSS | 3 | 2 | Utility-first, core, v4 |
| 4. JavaScript & DOM | 3 | 2 | Fundamentals, events, async |
| 5. TypeScript | 3 | 1 | Types, narrowing, advanced |
| 6. React Fundamentals | 5 | 4 | Components, state, data fetching |
| 7. React Composition | 3 | 3 | Compound components, context, hooks |
| 8. Design Engineering | 5 | 4 | Polish, animation, a11y, performance |
| **Total** | **36** | **25** | |

61 total steps (lessons + labs). A complete journey from "what is a tag" to "polished React application."
