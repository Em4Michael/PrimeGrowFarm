const WebSocket = require('ws');
const { saveSensorData } = require('../controllers/sensorDataController');

const createWebSocketServer = (server) => {
    const wss = new WebSocket.Server({ server });
  
    wss.on('connection', (socket) => {
      console.log('WebSocket connection established');
  
      socket.on('message', async (message) => {
        const messageString = message.toString();
  
        if (messageString.startsWith('toggle:')) {
          const pinName = messageString.substring(7);
          // Handle toggle logic here (similar to your previous implementation)
          // ...
  
          // Broadcast the toggle message to all clients
          wss.clients.forEach((client) => {
            if (client.readyState === WebSocket.OPEN) {
              client.send(`toggle:${pinName}`);
            }
          });
        } else {
          try {
            const sensorData = JSON.parse(messageString);
            const savedData = await saveSensorData(sensorData);
            // ...
  
            // Broadcast sensor data to all clients
            wss.clients.forEach((client) => {
              if (client.readyState === WebSocket.OPEN) {
                client.send(JSON.stringify(sensorData));
              }
            });
          } catch (error) {
            console.error('Error handling WebSocket message:', error);
          }
        }
      });
    });
  };
  
  module.exports = createWebSocketServer;
  