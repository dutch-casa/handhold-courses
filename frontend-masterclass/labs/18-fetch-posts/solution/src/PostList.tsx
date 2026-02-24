import { useQuery } from "@tanstack/react-query";

interface Post {
  readonly id: number;
  readonly title: string;
  readonly body: string;
}

export function PostList({ onSelectPost }: { readonly onSelectPost: (id: number) => void }) {
  const { data: posts, isPending, isError } = useQuery<readonly Post[]>({
    queryKey: ["posts"],
    queryFn: () =>
      fetch("https://jsonplaceholder.typicode.com/posts")
        .then((res) => res.json())
        .then((all: Post[]) => all.slice(0, 10)),
  });

  if (isPending) return <p>Loading posts...</p>;
  if (isError) return <p>Error loading posts</p>;

  return (
    <ul>
      {posts.map((post) => (
        <li key={post.id} onClick={() => onSelectPost(post.id)}>
          <strong>{post.title}</strong>
        </li>
      ))}
    </ul>
  );
}
