import { useEffect } from "react";
import "./notifications.css";

// TODO: Create Notification component
// Props:
//   readonly message: string
//   readonly onDismiss: () => void
//   readonly duration?: number (default 5000)
//
// - Wrap in <div role="alert" aria-live="polite" className="notification notification-enter">
// - Show message text
// - Close button with aria-label="Dismiss" that calls onDismiss
// - useEffect: setTimeout(onDismiss, duration) with clearTimeout cleanup

// TODO: Create NotificationStack component
// - useState for notifications array: { id: number; message: string }[]
// - addNotification function that appends with Date.now() id
// - removeNotification function that filters by id
// - Position: fixed top-right container
// - Render each notification with <Notification key={id} ... />
// - Include a button to trigger test notifications

export function Notification({ message, onDismiss, duration = 5000 }: {
  readonly message: string;
  readonly onDismiss: () => void;
  readonly duration?: number;
}) {
  return (
    <div className="notification">
      <span>{message}</span>
      <button onClick={onDismiss}>×</button>
    </div>
  );
}

export function NotificationStack() {
  return <div>{/* Build notification stack here */}</div>;
}
