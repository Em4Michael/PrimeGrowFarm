const express = require('express');
const mongoose = require('mongoose');
const WebSocket = require('ws');
//require('dotenv').config(); // Load environment variables from .env file

const app = express();
const server = app.listen(3001, () => {
  console.log('Server is running on port 3001');
});

mongoose.connect('mongodb+srv://dbUser:dbUser@cluster0.2ryarbp.mongodb.net/shopper?retryWrites=true&w=majority', {
  useNewUrlParser: true,
  useUnifiedTopology: true
});

const sensorDataSchema = new mongoose.Schema({
  temperature: Number,
  humidity: Number,
  moisture: Number,
  sunlight: Number,
  rainfall: Number
});

const SensorData = mongoose.model('SensorData', sensorDataSchema);

const wss = new WebSocket.Server({ server });

wss.on('connection', (socket) => {
  console.log('WebSocket connection established');

  socket.on('message', (message) => {
    // Convert the received message to a string
    const messageString = message.toString();
  
    if (messageString.startsWith('toggle:')) {
      // Handle toggle messages
      const pinName = messageString.substring(7); // Extract pinName from the message
      // Perform the toggle logic here
  
      // Broadcast the toggle message to all clients
      wss.clients.forEach((client) => {
        if (client.readyState === WebSocket.OPEN) {
          client.send(`toggle:${pinName}`);
        }
      });
    } else {
      try {
        // Parse the message as JSON (assuming it's sensor data)
        const sensorData = JSON.parse(messageString);
        const newSensorData = new SensorData(sensorData);
        newSensorData.save();
        console.log('Sensor data received:', sensorData);
  
        // Broadcast sensor data to all clients
        wss.clients.forEach((client) => {
          if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify(sensorData));
          }
        });
      } catch (error) {
        console.error('Error saving sensor data:', error);
      }
    }
  });
  
  
});

// Handle button toggles using routes


app.use(express.static('public'));
