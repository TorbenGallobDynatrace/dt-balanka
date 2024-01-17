import { Route, RouterProvider, createBrowserRouter, createRoutesFromElements } from 'react-router-dom';
import { Layout } from './Layout';
import { Overview } from './Overview';
import { PlayerView } from './PlayerView';
import './App.css';
import '@mantine/core/styles.css';
import ThePlayer from './ThePlayer';

const router = createBrowserRouter(
  createRoutesFromElements(
    <Route path="/" element={<Layout />}>
      <Route index element={<Overview />} />
      <Route path="player"  >
        <Route index element={<PlayerView />} />
        <Route path=":id" element={<ThePlayer />} />
      </Route>
    </Route>
  )
);

const App = () => {
  return (
    <>
      <RouterProvider router={router}/>
    </>
  );
};

export default App;
  