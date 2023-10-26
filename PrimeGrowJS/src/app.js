const express = require('express');
const mongoose = require('mongoose');
const createWebSocketServer = require('./websocket/websocketServer');
const sensorDataRoutes = require('./routes/sensorDataRoutes');

require('dotenv').config();
console.log(process.env.MONGODB_URI);

const app = express();
const server = app.listen(3001, () => {
  console.log('Server is running on port 3001');
});

mongoose.connect(process.env.MONGODB_URI, {
  useNewUrlParser: true,
  useUnifiedTopology: true
});

app.use(express.json());

// WebSocket server
createWebSocketServer(server);

// Routes
app.use('/api', sensorDataRoutes);
app.use(express.static('public'));
