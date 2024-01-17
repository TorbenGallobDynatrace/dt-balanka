import { useEffect, useState } from 'react';
import { Player, supabase } from './supabase';
import { Link } from 'react-router-dom';

export const PlayerView = () => {
  const [players, setPlayers] = useState<Player[]>([]);

  useEffect(() => {
    const fetchPlayers = async () => {
      const { data, error } = await supabase
        .from('players')
        .select('*').order('nickname', { ascending: true });
      if (error) {
        throw error;
      }
      setPlayers(data);
    };
    void fetchPlayers();
  }, []);

  return <div>
    <ul>
      {players.map(p => <li key={p.tag_id}><Link  to={{pathname: `/player/${p.tag_id}`}}>{p.nickname}</Link></li>)}
    </ul>
  </div>;
};