import { useEffect, useMemo, useState } from 'react';
import { Player, Stats, supabase } from './supabase';
import { Table, TableData } from '@mantine/core';

export const Overview = () => {
  const [players, setPlayers] = useState<Player[]>([]);
  const [stats, setStats] = useState<Stats[]>([]);


  const tableData: TableData = useMemo(() => {
    const statPerPlayer = new Map(stats.map(s => [s.tag_id, s]));
    const body = players.map(p => {
      const s = statPerPlayer.get(p.tag_id);
      const played = s?.played || 0;
      const wins = s?.wins || 0;
      const defeats = s?.defeats || 0;
      const avgScored = s?.avg_scored || 0;
      const avgReceived = s?.avg_received || 0;
      return [p.nickname, played, wins, defeats, avgScored, avgReceived];
    });
    const head = ['Nickname', 'Played', 'Wins', 'Defeats', 'Avg. Scored', 'Avg. Received'];
    return { head, body };
  }, [players, stats]);

  useEffect(() => {
    const fetchPlayers = async () => {
      const { data, error } = await supabase
        .from('players')
        .select('*').order('created_on', { ascending: false });
      if (error) {
        throw error;
      }
      setPlayers(data);
    };
    void fetchPlayers();
  }, []);

  useEffect(() => {
    const fetchStats = async () => {
      const { data, error } = await supabase
        .from('stats')
        .select('*');
      if (error) {
        throw error;
      }
      setStats(data);
    };
    void fetchStats();
  }, []);

  return <div><Table striped highlightOnHover withTableBorder data={tableData} /></div>;
};