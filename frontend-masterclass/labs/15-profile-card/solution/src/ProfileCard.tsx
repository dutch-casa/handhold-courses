export interface ProfileCardProps {
  readonly name: string;
  readonly avatarUrl: string;
  readonly bio: string;
  readonly skills: readonly string[];
}

export function ProfileCard({ name, avatarUrl, bio, skills }: ProfileCardProps) {
  return (
    <article>
      <div className="avatar">
        <img src={avatarUrl} alt={name} />
      </div>

      <div>
        <h2>{name}</h2>
        <p>{bio}</p>

        <ul>
          {skills.map((skill) => (
            <li key={skill}>{skill}</li>
          ))}
        </ul>
      </div>
    </article>
  );
}
