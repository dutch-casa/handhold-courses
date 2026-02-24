import { useState, useEffect } from "react";

// TODO: Implement useDebounce<T>(value, delay)
// - Returns T — the debounced value
// - useState initialized with value
// - useEffect: setTimeout to update debounced value after delay ms
// - Cleanup: clearTimeout (cancels pending update if value changes again)
// - Dependencies: [value, delay]

export function useDebounce<T>(value: T, _delay: number): T {
  const [debounced] = useState(value);
  return debounced;
}
