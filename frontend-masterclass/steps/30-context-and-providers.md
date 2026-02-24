---
title: Context and providers
---

# The prop drilling problem

{{show: drill-diagram reveal 0.3s spring}} {{show: drill-code typewriter 2s linear}} Your app has a theme. The top-level `App` component owns it. A `Button` five levels deep needs it. Every component in between must accept a `theme` prop and pass it down — even though they don't use it themselves.

{{focus: drilling}} {{zoom: 1.2x}} {{pulse: drilling}} {{annotate: drilling "Every layer relays"}} `App` passes `theme` to `Layout`. `Layout` passes it to `Main`. `Main` passes it to `Sidebar`. `Sidebar` passes it to `Button`. Four components acting as couriers. Change the prop name and you touch five files.

{{zoom: 1x}} {{focus: fragile}} {{zoom: 1.2x}} {{pan: fragile}} {{annotate: fragile "Fragile chain"}} Insert a new component in the middle? It must accept and pass the theme too. Remove a component? The chain breaks. Prop drilling couples your component hierarchy to your data flow. They should be independent.

{{pan: none}} {{zoom: 1x}} {{focus: none}} Context solves this. It lets a component deep in the tree read data from an ancestor without every intermediate component passing it through.

```code:drill-code lang=tsx
// Prop drilling: every layer must relay theme
function App() {
  const [theme, setTheme] = useState<"light" | "dark">("light");
  return <Layout theme={theme} setTheme={setTheme} />;
}

function Layout({ theme, setTheme }: LayoutProps) {
  return <Main theme={theme} setTheme={setTheme} />;
}

function Main({ theme, setTheme }: MainProps) {
  return <Sidebar theme={theme} setTheme={setTheme} />;
}

function Sidebar({ theme, setTheme }: SidebarProps) {
  return <Button theme={theme} onClick={() => setTheme("dark")} />;
}

// 4 components relay props they don't use.
// Change the prop name → touch 5 files.
---
drilling: 2-17
fragile: 7-17
```

```diagram:drill-diagram kind=graph
App [label="App\nowns theme" shape=box]
Layout [label="Layout\nrelays theme"]
Main [label="Main\nrelays theme"]
Sidebar [label="Sidebar\nrelays theme"]
Button [label="Button\nuses theme" shape=box]

App -> Layout [label="theme"]
Layout -> Main [label="theme"]
Main -> Sidebar [label="theme"]
Sidebar -> Button [label="theme"]
```

# React Context

{{clear: slide}}

{{show: context-diagram reveal 0.3s spring}} {{show: context-code typewriter 2s linear}} Context is a tunnel. A provider at the top publishes a value. Any descendant can read it directly — no matter how deep. The components in between know nothing about it.

{{focus: create}} {{zoom: 1.2x}} {{pulse: create}} {{annotate: create "Create context"}} `createContext` defines the tunnel. The type parameter says what flows through it. The argument is the default value — used when no provider exists above. For typed contexts, `null` with a runtime guard is the safe default.

{{zoom: 1x}} {{focus: provide}} {{zoom: 1.2x}} {{annotate: provide "Provide value"}} `<ThemeCtx value={...}>` publishes the value. Every component inside the provider tree can read it. Multiple providers can nest — the closest one wins.

{{zoom: 1x}} {{focus: consume}} {{zoom: 1.2x}} {{pulse: consume}} {{pan: consume}} {{annotate: consume "Consume with use()"}} `use(ThemeCtx)` reads the value. React 19 replaces `useContext` with `use`. Same behavior, cleaner name. The component re-renders when the context value changes.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:context-code lang=tsx
import { createContext, use, useState } from "react";

// 1. Create the context
interface ThemeContext {
  readonly theme: "light" | "dark";
  readonly toggleTheme: () => void;
}

const ThemeCtx = createContext<ThemeContext | null>(null);

// 2. Provider component
function ThemeProvider({ children }: { readonly children: React.ReactNode }) {
  const [theme, setTheme] = useState<"light" | "dark">("light");
  const toggleTheme = () =>
    setTheme((prev) => (prev === "light" ? "dark" : "light"));

  return (
    <ThemeCtx value={{ theme, toggleTheme }}>
      {children}
    </ThemeCtx>
  );
}

// 3. Consumer hook
function useTheme(): ThemeContext {
  const ctx = use(ThemeCtx);
  if (!ctx) throw new Error("useTheme must be inside ThemeProvider");
  return ctx;
}

// 4. Use anywhere — no prop drilling
function Button() {
  const { theme, toggleTheme } = useTheme();
  return (
    <button
      className={theme === "dark" ? "btn-dark" : "btn-light"}
      onClick={toggleTheme}
    >
      Toggle theme
    </button>
  );
}
---
create: 4-9
provide: 12-22
consume: 25-29
```

```diagram:context-diagram kind=graph
App [label="App" shape=box]
Provider [label="ThemeProvider\npublishes value" shape=box]
Layout [label="Layout\nknows nothing"]
Main [label="Main\nknows nothing"]
Sidebar [label="Sidebar\nknows nothing"]
Button [label="Button\nreads context" shape=box]

App -> Provider
Provider -> Layout
Layout -> Main
Main -> Sidebar
Sidebar -> Button
Provider -> Button [label="context tunnel" style=dashed]
```

# The provider pattern

{{clear: slide}}

{{show: provider-code typewriter 2s linear}} A provider owns state, exposes it through context, and hides the implementation. The consumers don't know if state comes from `useState`, Zustand, a URL, or localStorage. The provider is a wall between "how state is managed" and "how UI renders."

{{focus: interface}} {{zoom: 1.2x}} {{pulse: interface}} {{annotate: interface "Generic interface"}} Three parts: `state` (current values), `actions` (functions that change state), and `meta` (loading, errors). This structure works for any provider. It's a contract — the consumers depend on the shape, not the implementation.

{{zoom: 1x}} {{focus: impl}} {{zoom: 1.2x}} {{pan: impl}} {{annotate: impl "Implementation hidden"}} Today it's `useState`. Tomorrow you replace it with Zustand or a server query. The consumers don't change. Only the provider changes. This is the single-writer principle — one place manages state, everyone else reads it.

{{pan: none}} {{zoom: 1x}} {{focus: swap}} {{zoom: 1.2x}} {{pulse: swap}} {{annotate: swap "Swap providers"}} For testing, wrap in a mock provider. For different backends, swap providers. The UI components are decoupled from the data layer. This is how large React apps stay maintainable.

{{zoom: 1x}} {{focus: none}}

```code:provider-code lang=tsx
// The provider pattern: state + actions + meta

interface AuthState {
  readonly user: User | null;
  readonly isAuthenticated: boolean;
}

interface AuthActions {
  readonly login: (email: string, password: string) => Promise<void>;
  readonly logout: () => void;
}

interface AuthMeta {
  readonly isLoading: boolean;
  readonly error: string | null;
}

interface AuthContext {
  readonly state: AuthState;
  readonly actions: AuthActions;
  readonly meta: AuthMeta;
}

const AuthCtx = createContext<AuthContext | null>(null);

// The implementation is hidden inside the provider
function AuthProvider({ children }: { readonly children: React.ReactNode }) {
  const [user, setUser] = useState<User | null>(null);
  const [isLoading, setIsLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  const login = async (email: string, password: string) => {
    setIsLoading(true);
    setError(null);
    try {
      const res = await fetch("/api/login", {
        method: "POST",
        body: JSON.stringify({ email, password }),
      });
      const data = await res.json();
      setUser(data.user);
    } catch (e) {
      setError(e instanceof Error ? e.message : "Login failed");
    } finally {
      setIsLoading(false);
    }
  };

  const logout = () => setUser(null);

  return (
    <AuthCtx value={{
      state: { user, isAuthenticated: user !== null },
      actions: { login, logout },
      meta: { isLoading, error },
    }}>
      {children}
    </AuthCtx>
  );
}

// Swap the provider → UI stays the same
// <MockAuthProvider> for tests
// <OAuthProvider> for SSO
// <AuthProvider> for email/password
---
interface: 3-22
impl: 27-55
swap: 58-60
```

# When to use context

{{clear: slide}}

{{show: when-code typewriter 2s linear}} Context isn't always the answer. It has tradeoffs. The proximity principle: pass props when the data only travels one or two levels. Use context when data needs to skip layers.

{{focus: props}} {{zoom: 1.2x}} {{pulse: props}} {{annotate: props "Props are fine"}} Parent → child? Use props. Parent → grandchild? Usually still props. The overhead of context isn't worth it for one or two levels. Props are explicit, visible, and type-checked at every step.

{{zoom: 1x}} {{focus: context-use}} {{zoom: 1.2x}} {{annotate: context-use "Context for globals"}} Theme, auth, locale, feature flags — data that the whole app needs. These live at the root and are read dozens of levels deep. Context eliminates prop chains that touch every component.

{{zoom: 1x}} {{focus: perf}} {{zoom: 1.2x}} {{pulse: perf}} {{pan: perf}} {{annotate: perf "Performance caveat"}} Every component that reads a context re-renders when the context value changes. If the theme toggles, every component that reads theme re-renders. Solution: split contexts by update frequency. Put `theme` in one context and `user` in another.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:when-code lang=tsx
// PROPS: 1-2 levels — just pass it
function Card({ title }: { readonly title: string }) {
  return <CardHeader title={title} />;
}

// CONTEXT: app-wide globals
// Theme, auth, locale, feature flags
function App() {
  return (
    <ThemeProvider>
      <AuthProvider>
        <LocaleProvider>
          <Router />
        </LocaleProvider>
      </AuthProvider>
    </ThemeProvider>
  );
}

// SPLIT CONTEXTS: separate update frequencies
// Theme changes rarely. User data changes often.
// Don't put them in the same context.
const ThemeCtx = createContext<ThemeContext | null>(null);
const AuthCtx = createContext<AuthContext | null>(null);
const ToastCtx = createContext<ToastContext | null>(null);

// Each component subscribes to only what it reads.
// Toggle theme → only theme consumers re-render.
// Login → only auth consumers re-render.
---
props: 2-4
context-use: 8-18
perf: 22-28
```
