# Tailwind gallery

Rebuild the photo gallery using only Tailwind utility classes. No custom CSS.

## Tasks

- [ ] Make the gallery a responsive grid: 1 column on mobile, 2 on `md:`, 3 on `lg:`
- [ ] Add `gap-4` between grid items
- [ ] Add hover effects to cards: scale up slightly and add shadow on hover
- [ ] Add dark mode support: dark background, light text
- [ ] Use proper spacing: `p-4` on cards, `p-6` on the page body

## Hints

Responsive grid:

```html
<div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
```

Hover with transition:

```html
<div class="hover:scale-105 hover:shadow-lg transition duration-200">
```

Dark mode:

```html
<div class="bg-white dark:bg-gray-900 text-gray-900 dark:text-gray-100">
```

All styling should be done with Tailwind classes in the HTML. The `styles.css` file should only contain `@import "tailwindcss";`.

Use `Run Tests` in the testing panel to check your progress.
