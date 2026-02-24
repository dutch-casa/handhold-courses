import { createContext, use, useState } from "react";

interface ThemeContext {
  readonly theme: "light" | "dark";
  readonly toggleTheme: () => void;
}

const ThemeCtx = createContext<ThemeContext | null>(null);

export function useTheme(): ThemeContext {
  const ctx = use(ThemeCtx);
  if (ctx === null) throw new Error("useTheme must be used within a <ThemeProvider>");
  return ctx;
}

function getInitialTheme(): "light" | "dark" {
  const stored = localStorage.getItem("theme");
  if (stored === "light" || stored === "dark") return stored;
  if (window.matchMedia("(prefers-color-scheme: dark)").matches) return "dark";
  return "light";
}

export function ThemeProvider({ children }: { readonly children: React.ReactNode }) {
  const [theme, setTheme] = useState<"light" | "dark">(getInitialTheme);

  const toggleTheme = () => {
    setTheme((prev) => {
      const next = prev === "light" ? "dark" : "light";
      localStorage.setItem("theme", next);
      return next;
    });
  };

  return <ThemeCtx value={{ theme, toggleTheme }}>{children}</ThemeCtx>;
}
