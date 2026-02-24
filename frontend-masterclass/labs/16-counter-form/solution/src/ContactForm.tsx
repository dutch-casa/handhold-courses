import { useState } from "react";
import "./styles.css";

type FormStatus = "idle" | "error" | "success";

export function ContactForm() {
  const [name, setName] = useState("");
  const [email, setEmail] = useState("");
  const [status, setStatus] = useState<FormStatus>("idle");

  function handleSubmit(e: React.FormEvent) {
    e.preventDefault();

    if (!name.trim() || !email.trim()) {
      setStatus("error");
      return;
    }

    setStatus("success");
  }

  return (
    <form onSubmit={handleSubmit}>
      <h2>Contact</h2>

      <div>
        <label htmlFor="name">Name</label>
        <input
          id="name"
          type="text"
          value={name}
          onChange={(e) => setName(e.target.value)}
          placeholder="Your name"
        />
      </div>

      <div>
        <label htmlFor="email">Email</label>
        <input
          id="email"
          type="email"
          value={email}
          onChange={(e) => setEmail(e.target.value)}
          placeholder="you@example.com"
        />
      </div>

      <button type="submit">Submit</button>

      {status === "error" && (
        <p className="error">All fields required</p>
      )}
      {status === "success" && (
        <p className="success">Submitted!</p>
      )}
    </form>
  );
}
