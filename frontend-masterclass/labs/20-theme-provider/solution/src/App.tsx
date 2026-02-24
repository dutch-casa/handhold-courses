import { ThemeProvider, useTheme } from "./ThemeProvider";
import "./styles.css";

function ThemeToggle() {
  const { theme, toggleTheme } = useTheme();

  return (
    <button onClick={toggleTheme}>
      {theme === "dark" ? "Switch to Light" : "Switch to Dark"}
    </button>
  );
}

function ThemedCard({ children }: { readonly children: React.ReactNode }) {
  const { theme } = useTheme();

  return (
    <div className={`card card-${theme}`}>
      {children}
    </div>
  );
}

export default function App() {
  return (
    <ThemeProvider>
      <main>
        <h1>Theme Provider</h1>
        <ThemeToggle />
        <ThemedCard>
          <h2>Themed Content</h2>
          <p>
            This card reads the current theme from context and applies styles accordingly.
            Toggle the theme above to see it change. The preference persists in localStorage.
          </p>
        </ThemedCard>
      </main>
    </ThemeProvider>
  );
}
