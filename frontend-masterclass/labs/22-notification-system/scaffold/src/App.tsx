import { Notification } from "./Notification";
import "./notifications.css";

export default function App() {
  return (
    <main>
      <h1>Notifications</h1>
      <Notification message="Hello world" onDismiss={() => {}} />
    </main>
  );
}
