import { useState } from "react";
import "./styles.css";
// TODO: Import QueryClient, QueryClientProvider from "@tanstack/react-query"
// TODO: Import PostList and PostDetail

// TODO: Create a QueryClient outside the component
// Set defaultOptions.queries.staleTime to 60_000

// TODO: Wrap everything in <QueryClientProvider client={queryClient}>
// Manage selectedPostId state (number | null)
// When null → show <PostList onSelectPost={setSelectedPostId} />
// When set  → show <PostDetail postId={selectedPostId} onBack={() => setSelectedPostId(null)} />

export default function App() {
  const [selectedPostId, setSelectedPostId] = useState<number | null>(null);

  return (
    <main>
      <h1>Posts</h1>
      {/* Render PostList or PostDetail based on selectedPostId */}
    </main>
  );
}
