import { useEffect, useState, useCallback } from "react";
import "./notifications.css";

interface NotificationItem {
  readonly id: number;
  readonly message: string;
}

export function Notification({ message, onDismiss, duration = 5000 }: {
  readonly message: string;
  readonly onDismiss: () => void;
  readonly duration?: number;
}) {
  useEffect(() => {
    const timer = setTimeout(onDismiss, duration);
    return () => clearTimeout(timer);
  }, [onDismiss, duration]);

  return (
    <div role="alert" aria-live="polite" className="notification notification-enter">
      <span className="notification-message">{message}</span>
      <button
        className="notification-close"
        onClick={onDismiss}
        aria-label="Dismiss"
      >
        &times;
      </button>
    </div>
  );
}

export function NotificationStack() {
  const [notifications, setNotifications] = useState<readonly NotificationItem[]>([]);

  const addNotification = useCallback((message: string) => {
    const item: NotificationItem = { id: Date.now(), message };
    setNotifications((prev) => [...prev, item]);
  }, []);

  const removeNotification = useCallback((id: number) => {
    setNotifications((prev) => prev.filter((n) => n.id !== id));
  }, []);

  return (
    <>
      <button
        className="trigger-button"
        onClick={() => addNotification(`Notification at ${new Date().toLocaleTimeString()}`)}
      >
        Show Notification
      </button>

      <div className="notification-stack" aria-label="Notifications">
        {notifications.map((n) => (
          <Notification
            key={n.id}
            message={n.message}
            onDismiss={() => removeNotification(n.id)}
          />
        ))}
      </div>
    </>
  );
}
