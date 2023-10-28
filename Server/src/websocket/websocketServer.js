const WebSocket = require('ws');
const { saveSensorData } = require('../controllers/sensorDataController');
const { getPinState, updatePinState } = require('../controllers/pinController');

const pinStates = {};
const togglePinState = async (pinName) => {
  try {
    // Get the current state of the pin from the database
    const currentPinState = await getPinState(pinName);

    // Check if pin state was found in the database
    if (currentPinState) {
      // Toggle the pin state
      const newState = currentPinState.state === 'on' ? 'off' : 'on';

      // Update the pin state in the database and get the updated state
      const updatedPinState = await updatePinState(pinName, newState);

      // Return the resolved state value
      return updatedPinState.state;
    } else {
      // Handle the case where pin state was not found (e.g., set default state)
      console.log(`Pin state not found for ${pinName}`);
      return 'on'; // Set default state to 'off' or any desired default value
    }
  } catch (error) {
    throw error;
  }
};


const createWebSocketServer = (server) => {
    const wss = new WebSocket.Server({ server });
  
     // Send current pin states to the newly connected client
     Object.keys(pinStates).forEach((pinName) => {
      socket.send(`toggle:${pinName}:${pinStates[pinName]}`);
  });
    wss.on('connection', (socket) => {
      console.log('WebSocket connection established');
  
      socket.on('message', async (message) => {
        const messageString = message.toString();
  
        if (messageString.startsWith('toggle:')) {
          const pinName = messageString.split(':')[1];
          const newState = await togglePinState(pinName); // Toggle pin state
          const updatedPinState = await updatePinState(pinName, newState); // Update pin state in the database
  
            // Update the pin state in the pinStates object
            pinStates[pinName] = newState;
  
          // Broadcast the toggle message to all clients
          wss.clients.forEach((client) => {
            if (client !== socket && client.readyState === WebSocket.OPEN) {
              client.send(`toggle:${pinName}`);
              client.send(JSON.stringify(updatedPinState));
              client.send(`toggle:${pinName}:${newState}`);

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
  