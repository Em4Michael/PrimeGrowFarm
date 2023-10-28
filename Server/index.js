const express = require('express');
const mongoose = require('mongoose');
const createWebSocketServer = require('./src/websocket/websocketServer');
const sensorDataRoutes = require('./src/routes/sensorDataRoutes');
const pinRoutes = require('./src/routes/pinRoutes');

const cors = require('cors');

require('dotenv').config();
console.log(process.env.MONGODB_URI);

const app = express();
const server = app.listen(3001, () => {
  console.log('Server is running on port 3001');
});

mongoose.connect(process.env.MONGODB_URI, {
  useNewUrlParser: true,
  useUnifiedTopology: true
})
  .then(() => {
    console.log('MongoDB connected successfully');
  })
  .catch((err) => {
    console.error('MongoDB connection error:', err);
  });

app.use(express.json());
app.use(cors());
// WebSocket server
createWebSocketServer(server);

// Routes
app.use('/api', pinRoutes);
app.use('/api', sensorDataRoutes);
app.use(express.static('public'));
