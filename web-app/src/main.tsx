import React from 'react';
import ReactDOM from 'react-dom/client';
import App from './App.tsx';
import './index.css';
import { MantineProvider } from '@mantine/core';
import { DynatraceTheme } from './theme.ts';

ReactDOM.createRoot(document.getElementById('root')!).render(
  <React.StrictMode>
    <MantineProvider theme={DynatraceTheme}>
      <App />
    </MantineProvider>
  </React.StrictMode>
);
