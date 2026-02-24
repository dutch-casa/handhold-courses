import { useState } from "react";
import { QueryClient, QueryClientProvider } from "@tanstack/react-query";
import { PostList } from "./PostList";
import { PostDetail } from "./PostDetail";
import "./styles.css";

const queryClient = new QueryClient({
  defaultOptions: {
    queries: {
      staleTime: 60_000,
    },
  },
});

export default function App() {
  const [selectedPostId, setSelectedPostId] = useState<number | null>(null);

  return (
    <QueryClientProvider client={queryClient}>
      <main>
        <h1>Posts</h1>
        {selectedPostId === null ? (
          <PostList onSelectPost={setSelectedPostId} />
        ) : (
          <PostDetail postId={selectedPostId} onBack={() => setSelectedPostId(null)} />
        )}
      </main>
    </QueryClientProvider>
  );
}
