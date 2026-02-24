import "./styles.css";
import { ProfileCard } from "./ProfileCard";

export default function App() {
  return (
    <main>
      <h1>Team</h1>

      <div>
        <ProfileCard
          name="Ada Lovelace"
          avatarUrl="https://i.pravatar.cc/200?img=1"
          bio="Pioneer of computing. Wrote the first algorithm intended to be processed by a machine."
          skills={["Algorithms", "Mathematics", "Vision"]}
        />
        <ProfileCard
          name="Grace Hopper"
          avatarUrl="https://i.pravatar.cc/200?img=5"
          bio="Invented the first compiler and popularized machine-independent programming languages."
          skills={["Compilers", "COBOL", "Debugging"]}
        />
      </div>
    </main>
  );
}
