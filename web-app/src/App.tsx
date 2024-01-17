import '@mantine/core/styles.css';
import './App.css';
import { AppShell, Burger, Group, Image, Title } from '@mantine/core';
import { useDisclosure } from '@mantine/hooks';

function App () {
  const [opened, { toggle }] = useDisclosure();

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

      </AppShell.Navbar>

      <AppShell.Main>Main</AppShell.Main>
    </AppShell>
  );
}

export default App;
