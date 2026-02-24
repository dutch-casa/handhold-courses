import { createContext, use, useState } from "react";

// TODO: Define ThemeContext interface
// - readonly theme: "light" | "dark"
// - readonly toggleTheme: () => void

// TODO: Create ThemeCtx with createContext<ThemeContext | null>(null)

// TODO: Create useTheme hook
// - Reads context with use(ThemeCtx)
// - Throws if null (used outside provider)

// TODO: Create ThemeProvider component
// - Determine initial theme:
//   1. Check localStorage.getItem("theme")
//   2. If null, check window.matchMedia("(prefers-color-scheme: dark)").matches
//   3. Default to "light"
// - useState with that initial value
// - toggleTheme: switch "light" ↔ "dark", save to localStorage
// - Wrap children in <ThemeCtx value={{ theme, toggleTheme }}>

export function ThemeProvider({ children }: { readonly children: React.ReactNode }) {
  return <div>{children}</div>;
}

export function useTheme() {
  return { theme: "light" as const, toggleTheme: () => {} };
}
