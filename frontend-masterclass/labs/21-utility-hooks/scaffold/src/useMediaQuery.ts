import { useState, useEffect } from "react";

// TODO: Implement useMediaQuery(query)
// - Returns boolean
// - useState initialized from window.matchMedia(query).matches
// - useEffect: create MediaQueryList, listen for "change" events
// - Cleanup: removeEventListener on unmount
// - Re-subscribe if query changes (query in dependency array)

export function useMediaQuery(_query: string): boolean {
  const [matches] = useState(false);
  return matches;
}
