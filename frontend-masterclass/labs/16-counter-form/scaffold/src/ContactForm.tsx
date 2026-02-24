import { useState } from "react";
import "./styles.css";

// TODO: Create a ContactForm component
// - useState for name (string)
// - useState for email (string)
// - useState for status: "idle" | "error" | "success"
//
// - <form onSubmit={handleSubmit}>
//   - <input value={name}> for name
//   - <input value={email}> for email
//   - <button type="submit">Submit</button>
//
// - handleSubmit:
//   - e.preventDefault()
//   - If name or email is empty → set status to "error"
//   - Otherwise → set status to "success"
//
// - If status is "error" → <p className="error">All fields required</p>
// - If status is "success" → <p className="success">Submitted!</p>

export function ContactForm() {
  return <form>{/* Build form here */}</form>;
}
