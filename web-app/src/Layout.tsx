import { AppShell, Burger, Group, Title } from '@mantine/core';
import { useDisclosure } from '@mantine/hooks';
import { Link, Outlet } from 'react-router-dom';
import Logo from './dynatrace.svg?react';

export const Layout = () => {
  const [opened, { toggle }] = useDisclosure();
  return  (
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
        <Link to={'/'}>Home</Link>
        <Link to={'/player'}>Player</Link>
      </AppShell.Navbar>

      <AppShell.Main>
        <Outlet />
      </AppShell.Main>
    </AppShell>
  );
};