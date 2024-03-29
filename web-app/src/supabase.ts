import { createClient } from '@supabase/supabase-js';

const supabaseUrl = import.meta.env.VITE_SUPABASE_URL;
const supabaseAnonKey = import.meta.env.VITE_SUPABASE_ANON_KEY;
export const supabase = createClient(supabaseUrl, supabaseAnonKey);

export interface Player {
  nickname: string;
  'tag_id': string;
  'created_on': string;
}

export interface Stats {
  'tag_id': string;
  'avg_received': number,
  'avg_scored': number,
  defeats: number,
  draws: number,
  played: number,
  wins: number,
}

export interface Result {
  id: string,
  'score_a': number,
  'score_b': number,
  tap1: string,
  tap2: string,
  tbp1: string,
  tbp2: string,
  'created_at': string;
}
