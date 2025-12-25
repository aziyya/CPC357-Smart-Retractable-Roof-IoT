import React, { useState, useEffect } from 'react';
import { db } from './firebase';
import { ref, onValue } from 'firebase/database';
import {
  ResponsiveContainer,
  AreaChart,
  Area,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip
} from 'recharts';
import './App.css';

function App() {
  const [lightValue, setLightValue] = useState(0);
  const [isRaining, setIsRaining] = useState(false);
  const [roofStatus, setRoofStatus] = useState("Closed");
  const [ledStatus, setLedStatus] = useState(false);
  const [history, setHistory] = useState([]);

  useEffect(() => {
    const stadiumRef = ref(db, 'stadium');
    const unsubscribe = onValue(stadiumRef, (snapshot) => {
      const data = snapshot.val();
      if (data) {
        const newLight = data.light || 0;

        setLightValue(newLight);
        setIsRaining(data.isRaining || false);
        setRoofStatus(data.roofStatus || "Closed");
        setLedStatus(data.ledStatus || false);

        setHistory(prev => {
          const entry = {
            time: new Date().toLocaleTimeString([], {
              hour: '2-digit',
              minute: '2-digit'
            }),
            light: newLight
          };
          const updated = [...prev, entry];
          return updated.length > 15 ? updated.slice(1) : updated;
        });
      }
    });

    return () => unsubscribe();
  }, []);

  return (
    <div className="App">
      <header className="App-header">
        <h1>🏒 Smart Retractable Roof Court Monitoring System</h1>
        <p className="subtitle">
          Real-time environmental & infrastructure monitoring
        </p>
      </header>

      <div className="dashboard-grid">

        {/* Roof */}
        <div className="card">
          <h3>Roof System</h3>
          <div className={`value ${roofStatus === 'Open' ? 'status-open' : 'status-closed'}`}>
            {roofStatus}
          </div>
          <span className="status-label">Actuator: Operational</span>
        </div>

        {/* Weather */}
        <div className="card">
          <h3>Weather Sensor</h3>
          <div className={`value ${isRaining ? 'status-raining' : 'status-clear'}`}>
            {isRaining ? 'RAIN' : 'CLEAR'}
          </div>
          <span className="status-label">
            {isRaining ? 'Auto-safety engaged' : 'Normal monitoring'}
          </span>
        </div>

        {/* Lighting */}
        <div className="card">
          <h3>Lighting System</h3>
          <div className="value" style={{ color: ledStatus ? '#f9a825' : '#9e9e9e' }}>
            {ledStatus ? 'ON' : 'OFF'}
          </div>
          <span className="status-label">
            Power Line: {ledStatus ? 'Active' : 'Idle'}
          </span>
        </div>

        {/* Graph */}
        <div className="card graph-card">
          <div className="graph-header">
            <h3>Ambient Light Intensity</h3>
            <span className="graph-subtitle">Last 15 readings</span>
          </div>

          <div className="chart-container">
            <ResponsiveContainer width="100%" height={320}>
              <AreaChart data={history}>
                <defs>
                  <linearGradient id="lightGradient" x1="0" y1="0" x2="0" y2="1">
                    <stop offset="5%" stopColor="#1a237e" stopOpacity={0.9} />
                    <stop offset="95%" stopColor="#1a237e" stopOpacity={0} />
                  </linearGradient>
                </defs>

                <CartesianGrid strokeDasharray="3 3" vertical={false} />
                <XAxis dataKey="time" />
                <YAxis domain={[0, 1024]} />
                <Tooltip />
                <Area
                  type="monotone"
                  dataKey="light"
                  stroke="#1a237e"
                  strokeWidth={3}
                  fill="url(#lightGradient)"
                />
              </AreaChart>
            </ResponsiveContainer>
          </div>

          <div className="current-stats">
            Current: <strong>{lightValue} lux</strong> &nbsp;|&nbsp;
            Threshold: <strong>800</strong>
          </div>
        </div>

      </div>
    </div>
  );
}

export default App;
