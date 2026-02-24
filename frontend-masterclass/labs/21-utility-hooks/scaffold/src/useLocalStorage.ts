import { useState } from "react";

// TODO: Implement useLocalStorage<T>(key, initialValue)
// - Returns [T, setter] — same shape as useState
// - Lazy initializer: read localStorage, JSON.parse, fallback to initialValue
// - Setter: JSON.stringify and write to localStorage, then update state
// - Handle JSON parse errors gracefully (return initialValue)

export function useLocalStorage<T>(
  _key: string,
  initialValue: T,
): [T, (value: T | ((prev: T) => T)) => void] {
  const [stored] = useState(initialValue);
  return [stored, () => {}];
}
