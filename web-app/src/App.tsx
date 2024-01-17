import '@mantine/core/styles.css';
import './App.css';
import { AppShell, Burger, Group, Image, Title } from '@mantine/core';
import { useDisclosure } from '@mantine/hooks';
import { supabase } from './supabase';
import { useEffect, useState } from 'react';

interface Player {
  nickname: string;
}

function App () {
  const [opened, { toggle }] = useDisclosure();
  const [players, setPlayers] = useState<Player[]>([]);


  useEffect(() => {
    const fetchPlayers = async () => {
      const { data, error } = await supabase
        .from('players')
        .select('*');
      if(error) {
        throw error;
      }
      setPlayers(data);
    };
    void fetchPlayers();  
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
        <Burger
          opened={opened}
          onClick={toggle}
          hiddenFrom="sm"
          size="sm"
        />
        <Group justify='flex-start' p={8} gap={16}>
          <Image src="dynatrace.svg" height={48} />
          <Title size={18} order={1}>Dynatrace Balanka</Title>
        </Group>
      </AppShell.Header>

      <AppShell.Navbar p="md">
        {'Navbar'}
      </AppShell.Navbar>

      <AppShell.Main>
        <p>Hello</p>
        {players.map(p => <p key={p.nickname}>{p.nickname}</p>)}
      </AppShell.Main>
    </AppShell>
  );
}

export default App;
