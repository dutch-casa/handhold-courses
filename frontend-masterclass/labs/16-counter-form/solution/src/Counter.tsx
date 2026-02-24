import { useState } from "react";
import "./styles.css";

const MAX_CHARS = 100;

export function Counter() {
  const [state, setState] = useState({ count: 0, text: "" });
  const { count, text } = state;

  return (
    <div>
      <section>
        <h2>Counter</h2>
        <p>{count}</p>

        <div>
          <button onClick={() => setCount((prev) => prev - 1)}>-1</button>
          <button onClick={() => setCount(0)}>Reset</button>
          <button onClick={() => setCount((prev) => prev + 1)}>+1</button>
        </div>
      </section>

      <section>
        <h2>Character Counter</h2>

        <input
          type="text"
          value={text}
          onChange={(e) => setState((prev) => ({ ...prev, text: e.target.value }))}
          maxLength={MAX_CHARS}
          placeholder="Start typing..."
        />

        <p>
          {text.length} / {MAX_CHARS} characters
        </p>
      </section>
    </div>
  );

  function setCount(updater: number | ((prev: number) => number)) {
    setState((prev) => ({
      ...prev,
      count: typeof updater === "function" ? updater(prev.count) : updater,
    }));
  }
}
