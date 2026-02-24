# Dashboard layout

Build a dashboard layout using Tailwind 4 features: @theme for custom tokens, flex/grid for layout, responsive breakpoints, and dark mode.

## Tasks

- [ ] Add `@theme` block in styles.css with at least one custom color (`--color-brand-*`)
- [ ] Make the sidebar a flex column with navigation links
- [ ] Make the main content area a responsive grid of cards (1→2→3 columns)
- [ ] Hide the sidebar on mobile (`hidden md:flex`), show a header on mobile only (`md:hidden`)
- [ ] Add dark mode support to the entire layout
- [ ] Add hover state to cards (shadow or scale)

## Hints

Define custom tokens in your CSS:

```css
@import "tailwindcss";

@theme {
  --color-brand-500: oklch(0.62 0.19 250);
  --color-brand-600: oklch(0.55 0.19 250);
}
```

Sidebar layout with responsive collapse:

```html
<div class="flex min-h-screen">
  <aside class="hidden md:flex md:w-64 flex-col bg-gray-900">
    <!-- Sidebar nav -->
  </aside>
  <main class="flex-1 p-6">
    <!-- Content -->
  </main>
</div>
```

Mobile header (visible only on small screens):

```html
<header class="md:hidden flex items-center justify-between p-4 bg-gray-900">
  <span class="text-white font-bold">Dashboard</span>
</header>
```

Use `Run Tests` in the testing panel to check your progress.
