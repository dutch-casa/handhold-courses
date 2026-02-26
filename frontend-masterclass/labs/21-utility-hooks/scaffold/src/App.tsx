import { useState } from "react";
import { useLocalStorage } from "./useLocalStorage";
import { useMediaQuery } from "./useMediaQuery";
import { useDebounce } from "./useDebounce";

export default function App() {
  const [name, setName] = useLocalStorage("name", "");
  const isDark = useMediaQuery("(prefers-color-scheme: dark)");
  const [search, setSearch] = useState("");
  const debouncedSearch = useDebounce(search, 300);

  return (
    <main>
      <h1>Hook Demo</h1>
      <label>
        Name (persisted):
        <input value={name} onChange={(e) => setName(e.target.value)} />
      </label>
      <p>Color scheme: {isDark ? "dark" : "light"}</p>
      <label>
        Search (debounced):
        <input value={search} onChange={(e) => setSearch(e.target.value)} />
      </label>
      <p>Debounced: {debouncedSearch}</p>
    </main>
  );
}
