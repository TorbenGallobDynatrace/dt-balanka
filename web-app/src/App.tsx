import { AppShell, Burger, Group, Title } from '@mantine/core';
import '@mantine/core/styles.css';
import { useDisclosure } from '@mantine/hooks';
import { useEffect, useMemo, useState } from 'react';
import  Logo from '../public/dynatrace.svg?react';
import './App.css';
import { Player, Stats, supabase } from './supabase';
import { Table, TableData } from '@mantine/core';

function App () {
  const [opened, { toggle }] = useDisclosure();
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
    return {head, body};
  }, [players, stats]);

  useEffect(() => {
    const fetchPlayers = async () => {
      const { data, error } = await supabase
        .from('players')
        .select('*').order('created_on', {ascending: false});
      if(error) {
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
      if(error) {
        throw error;
      }
      setStats(data);
    };
    void fetchStats();  
  }, []);

  return (
    <AppShell
      header={{ height: 64 }}
      navbar={{
        width: 300,
        breakpoint: 'sm',
        collapsed: { mobile: !opened }
      }}
      padding="md"
    >
      <AppShell.Header>
        <Group px={16} h={'100%'} justify='flex-start' gap={16}>
          <Burger
            opened={opened}
            onClick={toggle}
            hiddenFrom="sm"
            size="sm"
          />
          <Logo height={'32px'} width={'32px'} />
          <Title size={18} order={1}>Dynatrace Balanka</Title>
        </Group>
      </AppShell.Header>

      <AppShell.Navbar p="md">
        {'Navbar'}
      </AppShell.Navbar>

      <AppShell.Main>
        <Table striped highlightOnHover withTableBorder data={tableData} />
      </AppShell.Main>
    </AppShell>
  );
}

export default App;
