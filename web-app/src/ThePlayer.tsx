import { useEffect, useMemo, useState } from 'react';
import { useParams } from 'react-router-dom';
import { Player, Result, supabase } from './supabase';
import { Table, TableData, Title } from '@mantine/core';

const ThePlayer = () => {
  const { id } = useParams();
  const [games, setGames] = useState<Result[]>([]);
  const [players, setPlayers] = useState<Player[]>([]);

  const tableData: TableData = useMemo(() => {
    const playersMap = new Map(players.map(p => [p.tag_id, p.nickname]));
    const body = games.map(g => {
      const tap1 = playersMap.get(g.tap1) || 'Not found';
      const tap2 = playersMap.get(g.tap2) || 'Not found';
      const tbp1 = playersMap.get(g.tbp1) || 'Not found';
      const tbp2 = playersMap.get(g.tbp2) || 'Not found';
      return [g.created_at, `${tap1} & ${tap2}`, `${g.score_a}:${g.score_b}`, `${tbp1} & ${tbp2}`];
    });
    const head = ['Date', 'Team A', 'Result', 'Team B'];
    return {head, body};
  }, [games, players]);

  useEffect(() => {
    const fetchPlayers = async () => {
      const { data, error } = await supabase
        .from('results').select('*').or(`tap1.eq.${id}, tap2.eq.${id}, tbp1.eq.${id}, tbp2.eq.${id}`).order('created_at');
      if (error) {
        throw error;
      }
      setGames(data);
    };
    void fetchPlayers();
  }, []);

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

  return <div>
    <Title order={1} mb={12}>Games</Title>
    <Table striped highlightOnHover withTableBorder data={tableData} />
  </div>;
};

export default ThePlayer;