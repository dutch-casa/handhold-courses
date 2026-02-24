import { useQuery } from "@tanstack/react-query";

interface Post {
  readonly id: number;
  readonly title: string;
  readonly body: string;
}

export function PostDetail({
  postId,
  onBack,
}: {
  readonly postId: number;
  readonly onBack: () => void;
}) {
  const { data: post, isPending, isError } = useQuery<Post>({
    queryKey: ["post", postId],
    queryFn: () =>
      fetch(`https://jsonplaceholder.typicode.com/posts/${postId}`).then((res) => res.json()),
  });

  if (isPending) return <p>Loading...</p>;
  if (isError) return <p>Error loading post</p>;

  return (
    <article>
      <button onClick={onBack}>Back</button>
      <h2>{post.title}</h2>
      <p>{post.body}</p>
    </article>
  );
}
