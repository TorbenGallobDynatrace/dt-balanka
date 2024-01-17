import { useEffect, useMemo, useState } from 'react';
import { Player, Stats, supabase } from './supabase';
import { Table, Title } from '@mantine/core';
import { Link } from 'react-router-dom';

interface TableRowData {
  name: string;
  played: number;
  wins: number;
  defeats: number;
  scored: number;
  received: number;
  tagId: string;
}

export const Overview = () => {
  const [players, setPlayers] = useState<Player[]>([]);
  const [stats, setStats] = useState<Stats[]>([]);

  const tableData: TableRowData[] = useMemo(() => {
    const statPerPlayer = new Map(stats.map(s => [s.tag_id, s]));
    return players.map(p => {
      const s = statPerPlayer.get(p.tag_id);
      const played = s?.played || 0;
      const wins = s?.wins || 0;
      const defeats = s?.defeats || 0;
      const scored = s?.avg_scored || 0;
      const received = s?.avg_received || 0;
      return {
        tagId: p.tag_id,
        name: p.nickname,
        played,
        wins,
        defeats,
        scored,
        received};
    });
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

  return <div>
    <Title order={1} mb={12}>Overall results</Title>
    <Table>
      <Table.Thead>
        <Table.Tr>
          <Table.Th>Nickname</Table.Th>
          <Table.Th>Wins</Table.Th>
          <Table.Th>Defeats</Table.Th>
          <Table.Th>#Plays</Table.Th>
          <Table.Th>Avg. Scored</Table.Th>
          <Table.Th>Avg. Received</Table.Th>
        </Table.Tr>
      </Table.Thead>
      <Table.Tbody>{tableData.map((element) => (
        <Table.Tr key={element.tagId}>
          <Table.Td><Link to={`/player/${element.tagId}`}>{element.name}</Link></Table.Td>
          <Table.Td>{element.wins}</Table.Td>
          <Table.Td>{element.defeats}</Table.Td>
          <Table.Td>{element.played}</Table.Td>
          <Table.Td>{element.scored}</Table.Td>
          <Table.Td>{element.received}</Table.Td>
        </Table.Tr>
      ))}</Table.Tbody>
    </Table>
  </div>;
};