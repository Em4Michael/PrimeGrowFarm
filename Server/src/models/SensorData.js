const mongoose = require('mongoose');

const sensorDataSchema = new mongoose.Schema({
  temperature: Number,
  humidity: Number,
  moisture: Number,
  sunlight: Number,
  rainfall: Number,
  timestamp: {
    type: Date,
    default: Date.now
  }
});

const SensorData = mongoose.model('SensorData', sensorDataSchema);

module.exports = SensorData;
